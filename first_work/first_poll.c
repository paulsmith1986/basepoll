#include "first_poll.h"
#include "fd_list.h"
#include "so_decode.h"
#include "so_encode.h"
//关闭socket list
first_poll_struct_t *CLOSE_FD_LIST = NULL;
//fpm进程类型
int FIRST_FPM_TYPE = 0;

//存放普通的socket连接(不受first_poll管理)
int normal_socket_list[ MAX_NORMAL_FD ];
/**
 * 初始化poll
 */
void first_create_poll()
{
	MAIN_POOL_FD = epoll_create1( EPOLL_CLOEXEC );
	if ( MAIN_POOL_FD < 0 )
	{
		fprintf( stderr, "Can not create epoll\n" );
		abort();
	}
	first_poll_init_list();
}

//创建倒计时
PHP_FUNCTION ( first_timer_fd )
{
	check_epoll_init();
	int timerfd = timerfd_create( CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC );
	if ( timerfd < 0 )
	{
		zend_error( E_WARNING, "Can not create timerfd()" );
	}
	first_poll_add( timerfd, FD_TYPE_TIMER );
	RETURN_LONG( timerfd );
}

//创建事件类fd
PHP_FUNCTION ( first_event_fd )
{
	check_epoll_init();
	int evtfd = eventfd( 0, EFD_NONBLOCK | EFD_CLOEXEC );
	if ( evtfd < 0 )
	{
		zend_error( E_WARNING, "Can not create eventfd()" );
	}
	first_poll_add( evtfd, FD_TYPE_EVENT );
	RETURN_LONG( evtfd );
}

//设置倒计时
PHP_FUNCTION ( first_set_timeout )
{
	long timer_fd;
	long micro_sec;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll", &timer_fd, &micro_sec ) == FAILURE )
	{
		return;
	}
	struct itimerspec new_value;
	struct itimerspec old_value;
	bzero( &new_value, sizeof new_value );
	bzero( &old_value, sizeof old_value );
	struct timespec ts;
	ts.tv_sec = micro_sec / 1000;
	ts.tv_nsec = ( micro_sec % 1000 ) * 1000 * 1000;
	new_value.it_value = ts;
	int ret = timerfd_settime( timer_fd, 0, &new_value, &old_value );
	if ( ret )
	{
		zend_error( E_WARNING, "Can not set_timeout" );
	}
}

//设置信号
PHP_FUNCTION ( first_signal_fd )
{
	zval *mask_signal;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "a", &mask_signal ) == FAILURE )
	{
		return;
	}
	check_epoll_init();
	int key_len;
	ulong index_key;
	zval **z_item;
	HashTable *mask_hash = Z_ARRVAL_P( mask_signal );
    sigset_t mask;
    sigemptyset( &mask );
	for( zend_hash_internal_pointer_reset( mask_hash ); zend_hash_get_current_data( mask_hash, (void**) &z_item ) == SUCCESS; zend_hash_move_forward( mask_hash ) )
	{
		convert_to_long( *z_item );
		int m_signal = Z_LVAL_PP( z_item );
		//过滤掉不能识别的信号
		if ( m_signal > 0 && m_signal <= 32 )
		{
			sigaddset( &mask, m_signal );
		}
	}
	if ( 0 != sigprocmask( SIG_BLOCK, &mask, NULL ) )
	{
		zend_error( E_ERROR, "Can not sigprocmask()\n" );
	}
	int sfd = signalfd( -1, &mask, 0 );
	if ( -1 == sfd )
	{
		zend_error( E_ERROR, "Can not signalfd()\n" );
	}
	first_poll_add( sfd, FD_TYPE_SIGNAL );
	RETURN_LONG( sfd );
}

//创建socket_fd
PHP_FUNCTION ( first_socket_fd )
{
	char *host;
	int host_len;
	long port;
	zend_bool is_poll = 1;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl|b", &host, &host_len, &port, &is_poll ) == FAILURE )
	{
		return;
	}
	int fd = first_socket_connect( host, port );
	if ( fd < 0 )
	{
		zend_error( E_ERROR, "Can not connect server %s: %d\n", host, port );
	}
	//加入first_poll
	if ( is_poll )
	{
		check_epoll_init();
		first_poll_add( fd, FD_TYPE_SOCKET );
	}
	//加入normal_socket
	else
	{
		add_normal_socket( fd );
	}
	RETURN_LONG( fd );
}

//创建socket_fd
PHP_FUNCTION ( first_host )
{
	char *bind_ip;
	int bind_ip_len;
	long sock_port;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl", &bind_ip, &bind_ip_len, &sock_port ) == FAILURE )
	{
		return;
	}
	check_epoll_init();
	int MAIN_SOCKET_FD = 0;
	if ( ( MAIN_SOCKET_FD = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0)
	{
		zend_error( E_ERROR, "Can not create listen fd\n" );
	}
	//设置SO_REUSEADDR选项(服务器快速重起)
	int bReuseaddr = 1;
	setsockopt( MAIN_SOCKET_FD, SOL_SOCKET, SO_REUSEADDR, &bReuseaddr, sizeof( int ) );

	//发送缓冲区
	int nSendBuf = FIRST_SEND_CACHE;
	setsockopt( MAIN_SOCKET_FD, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBuf, sizeof( int ) );

	//接收缓冲区
	int nRevBuf = FIRST_SEND_CACHE;
	setsockopt( MAIN_SOCKET_FD, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRevBuf, sizeof( int ) );

	//把socket设置为非阻塞方式
	set_non_block( MAIN_SOCKET_FD );
	struct sockaddr_in serveraddr;

	memset( &serveraddr, 0, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	inet_aton( bind_ip, &( serveraddr.sin_addr ) );
	serveraddr.sin_port = htons( sock_port );
	if ( bind( MAIN_SOCKET_FD, ( struct sockaddr * ) &serveraddr, sizeof( serveraddr ) ) < 0 )
	{
		zend_error( E_ERROR, "Can not bind host:%s port:%d\n", bind_ip, sock_port );
	}
	if ( listen( MAIN_SOCKET_FD, 512 ) < 0 )
	{
		zend_error( E_ERROR, "Can not listen\n" );
	}
	first_poll_add( MAIN_SOCKET_FD, FD_TYPE_LISTEN );
}

//发送数据包
PHP_FUNCTION ( first_send_pack )
{
	long fd;
	long proto_id;
	zval *data = NULL;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll|a", &fd, &proto_id, &data ) == FAILURE )
	{
		return;
	}
	zend_bool is_poll = 1;
	first_poll_struct_t *fd_info = find_fd_info( fd );
	if ( NULL == fd_info )
	{
		if ( is_normal_socket( (int)fd ) )
		{
			is_poll = 0;
		}
		else
		{
			zend_error( E_WARNING, "Can not find fdstruct, fd:%d\n", fd );
			RETURN_FALSE;
		}
	}
	HashTable *hash_arr = NULL;
	if ( NULL != data )
	{
		hash_arr = Z_ARRVAL_P( data );
	}
	char RESULT_POOL_[ PACK_POOL_SIZE ];
	protocol_result_t pack_data_result;
	memset( &pack_data_result, 0, sizeof( protocol_result_t ) );
	pack_data_result.max_pos = PACK_POOL_SIZE;
	pack_data_result.str = RESULT_POOL_;
	php_pack_protocol_data( proto_id, hash_arr, pack_data_result );
	if ( pack_data_result.error_code )
	{
		ZVAL_FALSE( return_value );
	}
	else
	{
		if ( is_poll )
		{
			send_protocol_pack( fd_info, pack_data_result );
		}
		else
		{
			normal_send_data( fd, pack_data_result.str, pack_data_result.pos );
		}
		ZVAL_TRUE( return_value );
	}
	try_free_result_pack( pack_data_result );
}

//发送数据包
PHP_FUNCTION ( first_send_data )
{
	long fd;
	char *send_data;
	int data_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ls", &fd, &send_data, &data_len ) == FAILURE )
	{
		return;
	}
	zend_bool is_poll = 1;
	first_poll_struct_t *fd_info = find_fd_info( fd );
	if ( NULL == fd_info )
	{
		if ( is_normal_socket( (int)fd ) )
		{
			is_poll = 0;
		}
		else
		{
			zend_error( E_WARNING, "Can not find fdstruct, fd:%d\n", fd );
			RETURN_FALSE;
		}
	}
	else
	{
		if ( is_poll )
		{
			first_send_data( fd_info, send_data, data_len );
		}
		else
		{
			normal_send_data( fd, send_data, data_len );
		}
		ZVAL_TRUE( return_value );
	}
}

//设置进程类型
PHP_FUNCTION( first_set_fpm_type )
{
	long fpm_type;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &fpm_type ) == FAILURE )
	{
		return;
	}
	FIRST_FPM_TYPE = fpm_type;
	RETURN_TRUE;
}

//等待事件发生
PHP_FUNCTION ( first_poll )
{
	long max_time = MAX_LOOP_TIMEOUT;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "|l", &max_time ) == FAILURE )
	{
		return;
	}
	struct epoll_event listen_ev, events[ FIRST_POLL_MAX_EVENT ];
	int event_num = epoll_wait( MAIN_POOL_FD, events, FIRST_POLL_MAX_EVENT, max_time );
	if ( event_num > 0 )
	{
		array_init( return_value );
		int i;
		for ( i = 0; i < event_num; ++i )
		{
			zval *tmp_result;
			MAKE_STD_ZVAL( tmp_result );
			array_init( tmp_result );
			first_poll_struct_t *fd_struct = (first_poll_struct_t*)events[ i ].data.ptr;
			fd_struct->is_return = 1;
			int event_type = 0;
			switch ( fd_struct->fd_type )
			{
				case FD_TYPE_LISTEN:
				{
					struct sockaddr_in clientaddr;
					socklen_t clilen = sizeof( clientaddr );
					int connfd = accept( fd_struct->fd, (struct sockaddr *) &clientaddr, &clilen );
					if ( connfd < 0 )
					{
						fprintf( stderr, "Can not accept!\n" );
					}
					else
					{
						//设置为非阻塞
						set_non_block( connfd );
						first_poll_add( connfd, FD_TYPE_SOCKET );
						event_type = FIRST_NEW_CONNECTION;
					}
				}
				break;
				case FD_TYPE_SOCKET:		//socket事件响应
				{
					if ( events[ i ].events & EPOLLRDHUP )
					{
						first_close_fd( fd_struct );
						event_type = FIRST_SOCKET_CLOSE;
					}
					else
					{
						//响应IN事件
						if ( events[ i ].events & EPOLLIN )
						{
							//主处理函数
							read_socket_data( fd_struct, tmp_result );
							event_type = FIRST_SOCKET_DATA;
						}
						//响应OUT事件
						if ( events[ i ].events & EPOLLOUT )
						{
							first_on_socket_write( fd_struct );
							fd_struct->is_return = 0;
							event_type = FIRST_SOCKET_WRITE;
						}
					}
				}
				break;
				case FD_TYPE_EVENT:		//唤醒事件
					//poll_handler_->on_event_fd( fd_struct );
					event_type = FIRST_EVENT_WAKEUP;
				break;
				case FD_TYPE_TIMER:		//倒计时事件
				{
					uint64_t howmany;
					int re = read( fd_struct->fd, &howmany, sizeof( howmany ) );
					if ( re != sizeof( howmany ) )
					{
						fd_struct->is_return = 0;
					}
					event_type = FIRST_TIME_UP;
				}
				break;
				case FD_TYPE_SIGNAL:	//信号事件
				{
					struct signalfd_siginfo fdsi;
					int re = read( fd_struct->fd, &fdsi, sizeof( struct signalfd_siginfo ) );
					if ( re != sizeof( struct signalfd_siginfo ) )
					{
						fd_struct->is_return = 0;
					}
					else
					{
						add_assoc_long( tmp_result, "signal", fdsi.ssi_signo );
					}
					event_type = FIRST_SIGNAL;
				}
				break;
				default:
					fprintf( stderr, "Unkown fd type: %d\n", fd_struct->fd_type );
				break;
			}
			if ( fd_struct->is_return )
			{
				add_assoc_long( tmp_result, "fd", fd_struct->fd );
				add_assoc_long( tmp_result, "event_type", event_type );
				add_next_index_zval( return_value, tmp_result );
			}
			else
			{
				zval_dtor( tmp_result );
			}
		}
		if ( NULL != CLOSE_FD_LIST )
		{
			check_fd_close_list();
		}
	}
	else
	{
		RETURN_NULL();
	}
}

//关闭socket
PHP_FUNCTION ( first_close_fd )
{
	long fd;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &fd ) == FAILURE )
	{
		return;
	}
	zend_bool is_poll = 1;
	first_poll_struct_t *fd_info = find_fd_info( fd );
	if ( NULL == fd_info )
	{
		if ( is_normal_socket( (int)fd ) )
		{
			is_poll = 0;
		}
		else
		{
			zend_error( E_WARNING, "Can not find fdstruct, fd:%d\n", fd );
			RETURN_FALSE;
		}
	}
	if ( is_poll )
	{
		first_close_fd( fd_info );
	}
	else
	{
		close( fd );
		remove_normal_socket( fd );
	}
	RETURN_TRUE;
}

//获取进程id
PHP_FUNCTION( first_getpid )
{
	long pid = getpid();
	RETURN_LONG( pid );
}
//守护进程的方式运行
PHP_FUNCTION( first_fork )
{
	pid_t id = fork();
	if ( -1 == id )
	{
		zend_error( E_ERROR, "Can not fork()\n" );
	}
	RETURN_LONG( (long)id );
}
//setsid
PHP_FUNCTION( first_setsid )
{
	RETURN_LONG( (long)setsid() );
}

//给某个进程发送信号
PHP_FUNCTION( first_kill )
{
	long pid, sig = SIGTERM;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &pid, &sig ) == FAILURE )
	{
		return;
	}
	if ( kill( pid, sig ) < 0 )
	{
		RETURN_FALSE;
  	}
	RETURN_TRUE;
}

//协议解包
PHP_FUNCTION( first_proxy_unpack )
{
	char *str;
	int str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == FAILURE )
	{
		return;
	}
	if ( str_len < sizeof( packet_head_t ) )
	{
		php_error( E_WARNING, "Unpack string is bad!\n" );
		RETURN_NULL();
	}
	protocol_packet_t tmp_proxy_pack;
	tmp_proxy_pack.pos = sizeof( packet_head_t );
	tmp_proxy_pack.data = str;
	tmp_proxy_pack.max_pos = str_len;

	protocol_result_t read_result_pool;
	memset( &read_result_pool, 0, sizeof( protocol_result_t ) );
	char TMP_POOL[ PROTO_SIZE_SO_FPM_PROXY ];
	read_result_pool.max_pos = sizeof( TMP_POOL );
	read_result_pool.str = TMP_POOL;
	proto_so_fpm_proxy_t *proxy_pack = read_so_fpm_proxy( &tmp_proxy_pack, &read_result_pool );
	if ( read_result_pool.error_code )
	{
		ZVAL_NULL( return_value );
	}
	else
	{
		array_init( return_value );
		packet_head_t *pack_head = ( packet_head_t* )proxy_pack->data->bytes;
		add_assoc_long( return_value, "PROXY_PACK_ID", pack_head->pack_id );
		add_assoc_long( return_value, "role_id", proxy_pack->role_id );
		add_assoc_long( return_value, "session_id", proxy_pack->session_id );
		add_assoc_stringl( return_value, "proto_data", proxy_pack->data->bytes, proxy_pack->data->len, 1 );
	}
	try_free_result_pack( read_result_pool );
}

/**
 * 将一个fd加入到epoll
 */
void first_poll_add( int fd, first_poll_type fd_type )
{
	first_poll_struct_t *fd_info = ( first_poll_struct_t* )malloc( sizeof( first_poll_struct_t ) );
	memset( fd_info, 0, sizeof( first_poll_struct_t ) );
	fd_info->fd = fd;
	fd_info->fd_type = fd_type;
	first_poll_list_add( fd_info );
	first_update_event( fd_info, EPOLL_CTL_ADD, READ_EVENT );
}

/**
 * 更新一个fd的事件
 */
void first_update_event( first_poll_struct_t *fd_info, int op, int new_event )
{
	struct epoll_event ep_ev = {0, {0}};
	ep_ev.data.ptr = fd_info;
	ep_ev.events = new_event;
	epoll_ctl( MAIN_POOL_FD, op, fd_info->fd, &ep_ev );
}

/**
 * 创建连接过程
 */
int first_socket_connect( const char *host, int port )
{
	int socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
	struct sockaddr_in sin;
	memset( &sin, 0, sizeof( struct sockaddr_in ) );
	sin.sin_family = AF_INET;
	sin.sin_port   = htons( port );
	struct in_addr tmp;
	inet_aton( host, &tmp );
	sin.sin_addr.s_addr = tmp.s_addr;
	struct timeval timeo = {3, 0};
	socklen_t len = sizeof( timeo );
	setsockopt( socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len );
	setsockopt( socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeo, len );
	//发送缓冲区
    int nSendBuf = NET_SEND_CACHE;
    setsockopt( socket_fd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBuf, sizeof( int ) );
	if ( connect( socket_fd, ( struct sockaddr * )&sin, sizeof( struct sockaddr_in ) ) < 0 )
	{
		return -1;
	}
	//设置非阻塞
	set_non_block( socket_fd );
	return socket_fd;
}

/**
 * 设置为非阻塞状态函数
 */
void set_non_block( const int sock )
{
	int opts = fcntl( sock, F_GETFL );
	opts = opts | O_NONBLOCK;
	fcntl( sock, F_SETFL, opts );
}

/**
 * 发送数据
 */
void first_send_data( first_poll_struct_t *fd_info, void *send_re, uint32_t total_send )
{
	//之前还有一些没发出去
	if ( fd_info->un_send_len > 0 )
	{
		char *un_send_char = (char *)realloc( fd_info->un_send, total_send + fd_info->un_send_len );
		memcpy( un_send_char + fd_info->un_send_len, send_re, total_send );
		fd_info->un_send_len += total_send;
		fd_info->un_send = un_send_char;
	}
	else
	{
		int send_num = send( fd_info->fd, send_re, total_send, MSG_NOSIGNAL );
		if ( send_num < 0 )
		{
			if ( errno != EAGAIN )
			{
				return;
			}
			send_num = 0;
		}
		if ( (uint32_t)send_num != total_send )
		{
			fd_info->un_send_len = total_send - send_num;
			char *un_send_char = (char *)malloc( fd_info->un_send_len );
			memcpy( un_send_char, (char*)send_re + send_num, fd_info->un_send_len );
			fd_info->un_send = un_send_char;
			//注册OUT事件
			first_update_event( fd_info, EPOLL_CTL_MOD, WRITE_EVENT );
		}
	}
}

/**
 * 写事件
 */
void first_on_socket_write( first_poll_struct_t *fd_info )
{
	if ( 0 == fd_info->un_send_len || NULL == fd_info->un_send )
	{
		return;
	}
	int send_num = send( fd_info->fd, fd_info->un_send, fd_info->un_send_len, MSG_NOSIGNAL );
	if ( send_num < 0 )
	{
		if ( errno != EAGAIN )
		{
			first_close_fd( fd_info );
			return;
		}
		send_num = 0;
	}
	if ( send_num != fd_info->un_send_len )
	{
		int new_un_len = fd_info->un_send_len - send_num;
		char *un_send_char = (char *)malloc( new_un_len );
		memcpy( un_send_char, fd_info->un_send + send_num, new_un_len );
		free( fd_info->un_send );
		fd_info->un_send = un_send_char;
		fd_info->un_send_len = new_un_len;
	}
	else
	{
		fd_info->un_send_len = 0;
		free( fd_info->un_send );
		fd_info->un_send = NULL;
		first_update_event( fd_info, EPOLL_CTL_MOD, READ_EVENT );
	}
}

/**
 * 关闭连接
 */
void first_close_fd( first_poll_struct_t *fd_info )
{
	first_update_event( fd_info, EPOLL_CTL_DEL, READ_EVENT|WRITE_EVENT );
	if ( NULL == CLOSE_FD_LIST )
	{
		CLOSE_FD_LIST = fd_info;
	}
	else
	{
		first_poll_struct_t *tmp = CLOSE_FD_LIST;
		while ( NULL != tmp )
		{
			//已经在关闭列表里了
			if ( tmp->fd == fd_info->fd )
			{
				return;
			}
			tmp = tmp->next;
		}
		fd_info->next = CLOSE_FD_LIST;
		CLOSE_FD_LIST = fd_info;
	}
}

/**
 * 读数据包
 */
void read_socket_data( first_poll_struct_t *fd_info, zval *tmp_result )
{
	int fd = fd_info->fd;
	//栈内数据包
	char STACK_READ_PACKET_DATA[ BIG_PACKET ];
	protocol_packet_t stack_read_packet;
	stack_read_packet.data = STACK_READ_PACKET_DATA;
	stack_read_packet.pos = 0;
	stack_read_packet.max_pos = sizeof( packet_head_t );
	stack_read_packet.pool_size = BIG_PACKET;
	stack_read_packet.is_resize = 0;

	//需要读取的量
	uint32_t need_read = stack_read_packet.max_pos;

	//总共读取的量
	int total_read_bytes = 0;
	protocol_packet_t *read_packet = NULL;

	//有上一次未接收完的数据
	if ( NULL != fd_info->un_read_pack )
	{
		read_packet = fd_info->un_read_pack;
		need_read = read_packet->max_pos - read_packet->pos;
	}
	else
	{
		read_packet = &stack_read_packet;
	}

	while ( 1 )
	{
		int ret = read( fd, read_packet->data + read_packet->pos, need_read );
		if ( 0 == ret )
		{
			//收到响应事件，没有数据可读
			if ( 0 == total_read_bytes )
			{
				first_close_fd( fd_info );
			}
			fd_info->is_return = 0;
			break;
		}
		else if ( ret < 0 )
		{
			//下一次继续读
			if ( errno == EAGAIN  )
			{
				//保存本次读取记录
				if ( NULL == fd_info->un_read_pack )
				{
					//如果已经是一个新生成的packet,直接将这个packet保存
					if ( read_packet != &stack_read_packet )
					{
						fd_info->un_read_pack = read_packet;
					}
					else
					{
						//重新生成一个数据包
						protocol_packet_t *un_read = new_proto_packet( read_packet->max_pos );
						un_read->pos = read_packet->pos;
						un_read->max_pos = read_packet->max_pos;
						memcpy( un_read->data, read_packet->data, read_packet->pos );
						fd_info->un_read_pack = un_read;
					}
				}
			}
			fd_info->is_return = 0;
			break;
		}
		else
		{
			read_packet->pos += ret;
			total_read_bytes += ret;
			//刚好读完指定需要的数据长度
			if ( ret == (int)need_read )
			{
				//刚好读完头信息
				if ( read_packet->pos == sizeof( packet_head_t ) )
				{
					packet_head_t *pack_head = ( packet_head_t* )&read_packet->data[ 0 ];
					need_read = pack_head->size;
					uint32_t new_size = need_read + sizeof( packet_head_t );
					//如果剩余的空间不足
					if ( read_packet->pool_size < new_size )
					{
						//超过最大支持的数据包
						if ( new_size > MAX_READ_PACK_SIZE )
						{
							fd_info->is_return = 0;
							first_close_fd( fd_info );
							return;
						}
						else
						{
							protocol_packet_resize( read_packet, new_size );
						}
					}
					read_packet->max_pos = new_size;
				}
				//不用读数据 或者 数据读完
				if( 0 == need_read || read_packet->pos > sizeof( packet_head_t ) )
				{
					packet_head_t *pack_head = ( packet_head_t* )read_packet->data;
					add_assoc_long( tmp_result, "pack_id", pack_head->pack_id );
					add_assoc_stringl( tmp_result, "proto_data", read_packet->data, read_packet->pos, 1 );
					uint32_t hash_id = 0;
					//转发请求. 找出role_id 为 hash_id
					if ( FIRST_FPM_MAIN == FIRST_FPM_TYPE && 60000 == pack_head->pack_id )
					{
						hash_id = read_proxy_hash_id( read_packet );
					}
					add_assoc_long( tmp_result, "hash_id", hash_id );
					//如果不是栈内内存
					if ( read_packet != &stack_read_packet )
					{
						delete_protocol_packet( read_packet );
					}
					//是否重新分配内存
					else if ( read_packet->is_resize )
					{
						free( read_packet->data );
					}
					break;
				}
			}
			else
			{
				need_read -= ret;
			}
		}
	}
}

/**
 * 获取数据包里的role_id为hash_id
 */
uint32_t read_proxy_hash_id( protocol_packet_t *data_pack )
{
	uint32_t hash_id;
	memcpy( &hash_id, &data_pack->data[ sizeof( packet_head_t ) ], sizeof( uint32_t ) );
	return hash_id;
}

/**
 * 闭关真正fd
 */
void check_fd_close_list()
{
	first_poll_struct_t *tmp_fd = CLOSE_FD_LIST;
	while ( NULL != tmp_fd )
	{
		close( tmp_fd->fd );
		//未发送的消息
		if ( 0 != tmp_fd->un_send_len && NULL != tmp_fd->un_send )
		{
			free( tmp_fd->un_send );
		}
		if ( NULL != tmp_fd->un_read_pack )
		{
			delete_protocol_packet( tmp_fd->un_read_pack );
		}
		//从fd_list中移出
		first_pool_list_remove( tmp_fd->fd );
		tmp_fd = tmp_fd->next;
	}
	CLOSE_FD_LIST = NULL;
}

/**
 * 创建协议数据包
 */
protocol_packet_t *new_proto_packet( uint32_t data_len )
{
	protocol_packet_t *tmp_pack = ( protocol_packet_t* )malloc( sizeof( protocol_packet_t ) );
	tmp_pack->pool_size = data_len;
	tmp_pack->data = ( char* )malloc( data_len );
	tmp_pack->pos = 0;
	tmp_pack->is_resize = 0;
	return tmp_pack;
}

/**
 * 加入普通socket
 */
void add_normal_socket( int fd )
{
	int i;
	for ( i = 0; i < MAX_NORMAL_FD; ++i )
	{
		if ( -1 == normal_socket_list[ i ] )
		{
			normal_socket_list[ i ] = fd;
			return;
		}
	}
	zend_error( E_WARNING, "Normal socket list is full!\n" );
}

/**
 * 移除普通socket
 */
void remove_normal_socket( int fd )
{
	int i;
	for ( i = 0; i < MAX_NORMAL_FD; ++i )
	{
		if ( fd == normal_socket_list[ i ] )
		{
			normal_socket_list[ i ] = -1;
			return;
		}
	}
}

/**
 * 判断是否是普通的fd
 */
int is_normal_socket( int fd )
{
	int i;
	for ( i = 0; i < MAX_NORMAL_FD; ++i )
	{
		if ( fd == normal_socket_list[ i ] )
		{
			return 1;
		}
	}
	return 0;
}

/**
 * 初始化普通socket连接列表
 */
void init_normal_socket_list()
{
	int i;
	for ( i = 0; i < MAX_NORMAL_FD; ++i )
	{
		normal_socket_list[ i ] = -1;
	}
}

/**
 * 送数据包
 */
void normal_send_data( int fd, char *send_data, uint32_t total_len )
{
	uint32_t total_send = 0;
	while ( total_len > 0 )
	{
		int ret = send( fd, send_data + total_send, total_len, 0 );
		if ( ret < 0 )
		{
			if ( errno != EAGAIN )
			{
				close( fd );
				break;
			}
			ret = 0;
		}
		total_send += ret;
		total_len -= ret;
	}
}
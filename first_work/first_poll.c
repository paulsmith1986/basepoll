#include "first_poll.h"

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
	memset( FIRST_POOL_FD_LIST, 0, sizeof( FIRST_POOL_FD_LIST ) );
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

//等待事件发生
PHP_FUNCTION ( first_socket_fd )
{
	char *host;
	int host_len;
	long port;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl", &host, &host_len, &port ) == FAILURE )
	{
		return;
	}
	check_epoll_init();
	int fd = first_socket_connect( host, port );
	if ( fd < 0 )
	{
		zend_error( E_ERROR, "Can not join server %s: %d\n", host, port );
	}
	first_poll_add( fd, FD_TYPE_SOCKET );
	RETURN_LONG( fd );
}

//等待事件发生
PHP_FUNCTION ( first_poll )
{
	struct epoll_event listen_ev, events[ FIRST_POLL_MAX_EVENT ];
	int event_num = epoll_wait( MAIN_POOL_FD, events, FIRST_POLL_MAX_EVENT, MAX_LOOP_TIMEOUT );
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
			add_assoc_long( tmp_result, "fd", fd_struct->fd );
			int event_type;
			switch ( fd_struct->fd_type )
			{
				case FD_TYPE_SOCKET:		//socket事件响应
				{
					if ( events[ i ].events & EPOLLRDHUP )
					{
						first_close_fd( fd_struct );
						event_type = 0;
					}
					else
					{
						//响应IN事件
						if ( events[ i ].events & EPOLLIN )
						{
							//主处理函数
							read_socket_data( fd_struct, tmp_result );
							event_type = 1;
						}
						//响应OUT事件
						if ( events[ i ].events & EPOLLOUT )
						{
							first_on_socket_write( fd_struct );
							event_type = 2;
						}
					}
					break;
				}
				case FD_TYPE_EVENT:		//唤醒事件
					//poll_handler_->on_event_fd( fd_struct );
					event_type = 3;
				break;
				case FD_TYPE_TIMER:		//倒计时事件
					//poll_handler_->on_timer_fd( fd_struct );
					event_type = 4;
				break;
				case FD_TYPE_SIGNAL:	//信号事件
					//poll_handler_->on_signal_fd( fd_struct );
					event_type = 5;
				break;
				default:
					fprintf( stderr, "Unkown fd type.\n" );
				break;
			}
			add_assoc_long( tmp_result, "event_type", event_type );
			add_next_index_zval( return_value, tmp_result );
		}
		if ( NULL != CLOSE_FD_LIST )
		{
			check_fd_close_list();
			CLOSE_FD_LIST = NULL;
		}
	}
	else
	{
		RETURN_NULL();
	}
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
	int index = fd % FIRST_POLL_MAX_EVENT;
	if ( NULL == FIRST_POOL_FD_LIST[ index ] )
	{
		FIRST_POOL_FD_LIST[ index ] = fd_info;
	}
	else
	{
		fd_info->next = FIRST_POOL_FD_LIST[ index ];
		FIRST_POOL_FD_LIST[ index ] = fd_info;
	}
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
	close( fd_info->fd );
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
				break ;
			}
			else if ( errno == EINTR )
			{
				continue;
			}
			else		//不知道什么错误
			{
				break;
			}
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
						if ( new_size > MAX_READ_DATA )
						{
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
					read_packet->pos = sizeof( packet_head_t );
					first_on_socket_read( read_packet, tmp_result );
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
 * 数据到达
 */
void first_on_socket_read( protocol_packet_t *data_pack, zval *tmp_result )
{
	packet_head_t *pack_head = ( packet_head_t* )data_pack->data;
	php_unpack_protocol_data( pack_head->pack_id, &data_pack->data[ sizeof( packet_head_t ) ], tmp_result );
}

/**
 * 闭关真正fd
 */
void check_fd_close_list()
{
	first_poll_struct_t *tmp_fd = CLOSE_FD_LIST;
	while ( NULL != tmp_fd )
	{
		//未
		if ( 0 != tmp_fd->un_send_len && NULL != tmp_fd->un_send )
		{
			free( tmp_fd->un_send );
		}
		if ( NULL != tmp_fd->un_read_pack )
		{
			delete_protocol_packet( tmp_fd->un_read_pack );
		}
		tmp_fd = tmp_fd->next;
	}
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
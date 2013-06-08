#ifndef FIRST_IM_H
#include "first_im.h"
#include "firstnet.h"
#endif

first_net_socket *IM_FD_HEAD[ MAX_SERVER_NUM ];
void init_first_im()
{
	memset( IM_FD_HEAD, 0, sizeof( IM_FD_HEAD ) );
}

//php验证im key
int im_admin_login( int fd, char *join_str )
{
	int join_len = strlen( join_str );
	proto_so_php_join_t join_dat;
	join_dat.join_str = join_str;
	encode_so_php_join( send_pack, &join_dat );
	protocol_send_pack( fd, &send_pack );
	if ( send_pack.error_code )
	{
		return -1;		//消息发不出去
	}
	read_and_decode_so_php_join_re( fd, join_re );
	if ( is_decode_error() || 0 != join_re->result )
	{
		return -1;
	}
	return fd;
}

//连接im服务器
PHP_FUNCTION ( first_im_connect )
{
	long server_id;
	char *host;
	long port;
	char *join_str;
	int host_len, str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "lsls", &server_id, &host, &host_len, &port, &join_str, &str_len ) == FAILURE )
	{
		return;
	}
	int re = -1;
	int fd = first_net_connect( server_id, host, port, IM_FD_HEAD );
	if ( fd >= 0 && im_admin_login( fd, join_str ) >= 0 )
	{
		first_net_socket *p = find_first_net_server_fd( server_id, IM_FD_HEAD );
		re = p->array_index;
	}
	else
	{
		php_error( E_WARNING, "Can not join im server" );
	}
	#ifdef FIRST_DEBUG
	php_printf( "PHP进程加入服务器:%s, port:%d\n", host, port );
	#endif
	RETURN_LONG( re );
}

//检测和聊天服务器是否还连着
PHP_FUNCTION ( first_im_ping )
{
	long server_id;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &server_id ) == FAILURE )
	{
		return;
	}
	int re_value = first_net_ping( server_id, IM_FD_HEAD );
	RETURN_LONG( re_value );
}

//生成频道ID
PHP_FUNCTION ( first_im_channle )
{
	long ch_type, scene_id, sub_scene;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "lll", &ch_type, &scene_id, &sub_scene ) == FAILURE )
	{
		return;
	}
	CHANNEL_ID ch;
	ch.ch_type = ch_type;
	ch.scene_id = scene_id;
	ch.sub_id = sub_scene;
	ch.no_use = 0;
	RETURN_LONG( ch.channel_id );
}
//从IM服务器获取数据
PHP_FUNCTION ( first_im_get )
{
	long do_type, array_index;
	char *ext_data;
	int ext_data_len = 0;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll|s", &array_index, &do_type, &ext_data, &ext_data_len ) == FAILURE )
	{
		return;
	}
	first_net_socket *p = get_first_net_server_fd( array_index, IM_FD_HEAD );
	if ( NULL == p )
	{
		RETURN_NULL();
	}
	/*
	if ( 0 == ext_data_len )
	{
		first_net_send_data( p->socket_fd, do_type, NULL, 0 );
	}
	else
	{
		first_net_send_data( p->socket_fd, do_type, ext_data, ext_data_len );
	}
	make_result_array( IM_GET_POOL );
	first_net_get_data( p->socket_fd, &result_array );
	if ( 0 == result_array.max_pos )
	{
		php_error( E_WARNING, "im_get 获取数据出错" );
		unset_result( result_array );
		RETURN_NULL();
	}

	switch ( result_array.pack_type )
	{
		case 10011:			//在线人数
		{
			int online_num;
			online_num = byte_array_read_int( &result_array );
			unset_result( result_array );
			RETURN_LONG( online_num );
			break;
		}
		case 11000:			//开始一场新战斗
		{
			uint32_t fight_id;
			fight_id = byte_array_read_uint( &result_array );
			unset_result( result_array );
			RETURN_LONG( fight_id );
			break;
		}
	}*/
}

//打包战斗数据
PHP_FUNCTION ( first_pack_fight )
{
	zval *z_fight_apc;
	zval *z_attaci_side;
	zval *z_defence_side;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "aaa", &z_fight_apc, &z_attaci_side, &z_defence_side ) == FAILURE )
	{
		return;
	}
	/*char pack_data_pool[ NET_SEND_CACHE ];
	int pack_len = 0;
	char *pack_str = pack_data_pool;
	fight_apc_pack fight_apc;
	HashTable *arr_hash = Z_ARRVAL_P( z_fight_apc );
	fight_apc.max_fight_time = read_int_from_array( arr_hash, php_key_arg( "max_fight_time" ) );
	fight_apc.attack_member_num = read_int_from_array( arr_hash, php_key_arg( "attack_member_num" ) );
	fight_apc.defence_member_num = read_int_from_array( arr_hash, php_key_arg( "defence_member_num" ) );
	append_to_pack( pack_str, &pack_len, &fight_apc, sizeof( fight_apc_pack ) );
	pack_fight_member( z_attaci_side, pack_str, &pack_len );
	pack_fight_member( z_defence_side, pack_str, &pack_len );
	RETURN_STRINGL( pack_str, pack_len, 1 );
	*/
}

//发送消息
PHP_FUNCTION ( first_im_send )
{
	long fd;
	char *im_data;
	int data_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ls", &fd, &im_data, &data_len ) == FAILURE )
	{
		return;
	}
	int ret = send( fd, im_data, data_len, 0 );
	if ( ret < 0 )
	{
		ret = 0;
		if ( errno == EPIPE )
		{
			ret = -1;
		}
	}
	RETURN_LONG( ret );
}

//推送协议消息
PHP_FUNCTION ( first_im_push )
{
	long array_index;
	long protocol_id;
	zval *z_push_data = NULL;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll|a", &array_index, &protocol_id, &z_push_data ) == FAILURE )
	{
		return;
	}
	first_net_socket *p = get_first_net_server_fd( array_index, IM_FD_HEAD );
	if ( NULL == p )
	{
		RETURN_NULL();
	}
	HashTable *arr_hash = NULL;
	if ( NULL != z_push_data )
	{
		arr_hash = Z_ARRVAL_P( z_push_data );
	}
	php_send_im_pack( p->socket_fd, protocol_id, arr_hash );
	if ( php_send_im_is_error() )
	{
		RETURN_FALSE;
	}
	else
	{
		RETURN_TRUE;
	}
}

//打包二进制数据
PHP_FUNCTION ( first_im_pack )
{
	long pack_type;
	zval *pack_data;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l|z", &pack_type, &pack_data ) == FAILURE )
	{
		return;
	}
	/*
	packHeadSt pack_head;
	pack_head.packtype = pack_type;
	char pack_data_pool[ NET_SEND_CACHE ];
	char *pack_pool = pack_data_pool;
	int pack_len = 0;
	int head_len = sizeof( packHeadSt );
	switch ( pack_type )
	{
		case PHP_CONNECT:		//加入服务器
		{
			char *join_str = Z_STRVAL_P( pack_data );
			pack_len = strlen( join_str );
			memcpy( pack_pool + head_len, join_str, pack_len );
			break;
		}
		case PHP_IM_SYNC:		//同步
		break;
	}
	pack_head.size = pack_len;
	memcpy( pack_pool, &pack_head, head_len );
	RETURN_STRINGL( pack_pool, head_len + pack_len, 1 );
	*/
}

//创建epoll
PHP_FUNCTION ( first_create_poll )
{
	int re = epoll_create1( EPOLL_CLOEXEC );
	if ( re < 0 )
	{
		zend_error( E_ERROR, "Can not create epoll" );
	}
	RETURN_LONG( re );
}

//创建倒计时
PHP_FUNCTION ( first_timer_fd )
{
	int timerfd = timerfd_create( CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC );
	if ( timerfd < 0 )
	{
		zend_error( E_WARNING, "Can not create timerfd()" );
	}
	RETURN_LONG( timerfd );
}

//创建事件类fd
PHP_FUNCTION ( first_event_fd )
{
	int evtfd = eventfd( 0, EFD_NONBLOCK | EFD_CLOEXEC );
	if ( evtfd < 0 )
	{
		zend_error( E_WARNING, "Can not create eventfd()" );
	}
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

//等待事件发生
PHP_FUNCTION ( first_poll )
{
	long timeout;
	long epoll_fd;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll", &epoll_fd, &timeout ) == FAILURE )
	{
		return;
	}
	struct epoll_event listen_ev, events[ FIRST_POLL_MAX_EVENT ];
	int event_num = epoll_wait( epoll_fd, events, FIRST_POLL_MAX_EVENT, timeout );
	array_init( return_value );
	if ( event_num > 0 )
	{
		int i = 0;
		for ( i = 0; i < event_num; ++i )
		{
			add_index_long( return_value, events[ i ].data.fd, events[ i ].events );
		}
	}
}

//控制 fd
PHP_FUNCTION ( first_poll_ctl )
{
	long epoll_fd;
	long fd;
	long ctl_type;
	zend_bool eve_type;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "lllb", &epoll_fd, &fd, &ctl_type, &eve_type ) == FAILURE )
	{
		return;
	}
	struct epoll_event ep_ev = {0, {0}};
	ep_ev.data.fd = fd;
	int op;
	switch ( ctl_type )
	{
		case 2:			//更新
			op = EPOLL_CTL_MOD;
		break;
		case 3:			//删除
			op = EPOLL_CTL_DEL;
		break;
		default:
			op = EPOLL_CTL_ADD;
		break;
	}
	ep_ev.events = eve_type ? EPOLLIN : EPOLLOUT;
	int re = epoll_ctl( epoll_fd, op, fd, &ep_ev );
	RETURN_LONG( re );
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
	RETURN_LONG( sfd );
}

//读数据
PHP_FUNCTION ( first_im_read )
{
	long fd;
	char *unread_str;
	int unread_len = 0;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l|s", &fd, &unread_str, &unread_len ) == FAILURE )
	{
		return;
	}
	/*
	char READ_POOL_BUF[ NET_SEND_CACHE ];
	char *read_pool = READ_POOL_BUF;
	int current_buf_size = NET_SEND_CACHE;
	int total_read = 0;
	int read_pos = 0;
	int max_read_len = NET_SEND_CACHE;
	//之前有没有处理完的数据
	if ( unread_len > 0 )
	{
		memcpy( read_pool, unread_str, unread_len );
		read_pos = unread_len;
		max_read_len -= unread_len;
	}
	int continue_read = 1;
	while ( continue_read )
	{
		int read_re = read( fd, read_pool + read_pos, max_read_len );
		if ( read_re <= 0 )
		{
			//未读到任何数据
			if ( 0 == total_read )
			{
				RETURN_NULL();
			}
			break;
		}
		total_read += read_re;
		read_pos += read_re;
		max_read_len -= unread_len;
		//数据没有读完
		if ( read_re == max_read_len )
		{
			current_buf_size += NET_SEND_CACHE;
			char *new_pool = (char *)pemalloc( current_buf_size, 1 );
			memcpy( new_pool, read_pool, read_pos );
			if ( read_pool != READ_POOL_BUF )
			{
				pefree( read_pool, 1 );
			}
			max_read_len = NET_SEND_CACHE;
		}
		else
		{
			break;
		}
	}
	array_init( return_value );
	continue_read = 1;
	int read_data_type = 0;		//读数据类型: 0:头数据  1:包据
	int need_read = sizeof( packHeadSt );
	packHeadSt *pack_head;
	first_net_byte_array read_array;
	read_array.data = read_pool;
	read_array.read_pos = 0;
	read_array.max_pos = read_pos;
	while ( read_array.read_pos < read_pos )
	{
		//数据不够 把数据存起来
		if ( read_array.read_pos + need_read > read_pos )
		{
			//头数据读完，数据体不够，头数据要重新读
			if ( 1 == read_data_type )
			{
				read_array.read_pos -= sizeof( packHeadSt );
			}
			//把未读完的数据存在unread key里
			add_assoc_stringl ( return_value, "unread", &read_pool[ read_array.read_pos ], read_pos - read_array.read_pos, 1 );
			break;
		}
		if ( 0 == read_data_type )
		{
			pack_head = ( packHeadSt* )&read_pool[ read_array.read_pos ];
			read_array.read_pos += need_read;
			//没有数据体
			if ( 0 == pack_head->size )
			{
				zval *tmp_re;
				MAKE_STD_ZVAL( tmp_re );
				array_init( tmp_re );
				add_next_index_long( tmp_re, pack_head->packtype );
				add_next_index_zval( return_value, tmp_re );
			}
			else
			{
				read_data_type = 1;
				need_read = pack_head->size;
			}
		}
		else
		{
			parse_im_data( return_value, &read_array, pack_head );
			read_data_type = 0;
			need_read = sizeof( packHeadSt );
		}
	}
	if ( read_pool != READ_POOL_BUF )
	{
		pefree( read_pool, 1 );
	}*/
}

//关闭fd
PHP_FUNCTION ( first_im_close )
{
	long fd;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &fd ) == FAILURE )
	{
		return;
	}
	close( fd );
}

//与im互动的socket
PHP_FUNCTION ( first_im_socket )
{
	char *host;
	long port;
	int host_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl", &host, &host_len, &port ) == FAILURE )
	{
		return;
	}
	int fd = first_net_connect_do( host, port );
	if ( fd < 0 )
	{
		zend_error( E_WARNING, "Can not connect server %s:%d", host, port );
	}
	else
	{
		//设置非阻塞
		int opts;
		opts = fcntl( fd, F_GETFL );
		opts = opts | O_NONBLOCK;
		fcntl( fd, F_SETFL, opts );
	}
	RETURN_LONG( fd );
}

/*用户登陆/登出数据
void parse_login_out_data( first_net_byte_array *result, zval *data_pack )
{
	zval *tmp_result;
	MAKE_STD_ZVAL( tmp_result );
	array_init( tmp_result );
	char log_num = byte_array_read_char( result );
	if ( log_num > 0 )
	{
		int i;
		zval *login_arr;
		MAKE_STD_ZVAL( login_arr );
		array_init( login_arr );
		for ( i = 0; i < log_num; ++i )
		{
			login_pack_t *tmp_login = ( login_pack_t* )byte_array_read( result, sizeof( login_pack_t ) );
			if ( NULL != tmp_login )
			{
				zval *login_item;
				MAKE_STD_ZVAL( login_item );
				array_init( login_item );
				add_assoc_long( login_item, "role_id", tmp_login->role_id );
				add_assoc_long( login_item, "login_time", tmp_login->login_time );
				add_next_index_zval( login_arr, login_item );
			}

		}
		add_assoc_zval( tmp_result, "login", login_arr );
	}
	log_num = byte_array_read_char( result );
	if ( log_num > 0 )
	{
		int i;
		zval *logout_arr;
		MAKE_STD_ZVAL( logout_arr );
		array_init( logout_arr );
		for ( i = 0; i < log_num; ++i )
		{
			logout_pack_t *tmp_logout = ( logout_pack_t* )byte_array_read( result, sizeof( logout_pack_t ) );
			if ( NULL != tmp_logout )
			{
				zval *logout_item;
				MAKE_STD_ZVAL( logout_item );
				array_init( logout_item );
				add_assoc_long( logout_item, "role_id", tmp_logout->role_id );
				add_assoc_long( logout_item, "x", tmp_logout->x );
				add_assoc_long( logout_item, "y", tmp_logout->y );
				add_assoc_long( logout_item, "login_time", tmp_logout->login_time );
				add_assoc_long( logout_item, "logout_time", tmp_logout->logout_time );
				add_next_index_zval( logout_arr, logout_item );
			}
		}
		add_assoc_zval( tmp_result, "logout", logout_arr );
	}
	add_next_index_zval( data_pack, tmp_result );
}
*/
/*打包战斗成员数据
void pack_fight_member( zval *member_arr, char *pack_str, int *beg_pos )
{
	char *key;
	int key_len;
	ulong index_key;
	zval **z_item;
	HashPosition pointer;
	HashTable *member_hash = Z_ARRVAL_P( member_arr );
	fight_member_pack fight_member;
	//初使化进攻方的数据
	for( zend_hash_internal_pointer_reset_ex( member_hash, &pointer ); zend_hash_get_current_data_ex( member_hash, (void**) &z_item, &pointer ) == SUCCESS; zend_hash_move_forward_ex( member_hash, &pointer) )
	{
		zend_hash_get_current_key_ex( member_hash, &key, &key_len, &index_key, 0, &pointer );
		if ( index_key > MAX_CELL_ID || index_key < 0 )
		{
			zend_error( E_ERROR, "Fight data error. Max cell id:%d, current cell id:%u", MAX_CELL_ID, index_key );
		}
		fight_member.cell_id = index_key;
		HashTable *arr_hash = Z_ARRVAL_PP( z_item );
		zval **data;
		//member_id
		get_fight_member_data_int( fight_member.member_id, arr_hash, 0, data );
		//member_type
		get_fight_member_data_int( fight_member.member_type, arr_hash, 1, data );
		//level
		get_fight_member_data_int( fight_member.level, arr_hash, 2, data );
		//life_max
		get_fight_member_data_int( fight_member.life_max, arr_hash, 3, data );
		//min_attack
		get_fight_member_data_int( fight_member.min_attack, arr_hash, 4, data );
		//max_attack
		get_fight_member_data_int( fight_member.max_attack, arr_hash, 5, data );
		//defence
		get_fight_member_data_int( fight_member.defence, arr_hash, 6, data );
		append_to_pack( pack_str, beg_pos, &fight_member, sizeof( fight_member_pack ) );
	}
}
*/
/*打包数据
void append_to_pack( char *pack_str, int *beg_pos, void *data, int pack_size )
{
	if ( *beg_pos + pack_size > NET_SEND_CACHE )
	{
		zend_error( E_ERROR, "Pack out of range\n" );
	}
	memcpy( pack_str + (*beg_pos), data, pack_size );
	(*beg_pos) += pack_size;
}
*/

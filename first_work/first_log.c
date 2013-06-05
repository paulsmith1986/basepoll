#ifndef YILE_LOG_H
#include "yile_log.h"
#endif
yile_net_socket *LOG_FD_HEAD[ MAX_SERVER_NUM ];
void init_yile_log()
{
	memset( LOG_FD_HEAD, 0, sizeof( LOG_FD_HEAD ) );
}
PHP_FUNCTION ( yile_log_send )
{
	long log_handle;
	char *log_data;
	int log_len;
	long log_type = 2;
	long ext_len = 0;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ls|ll", &log_handle, &log_data, &log_len, &log_type, &ext_len ) == FAILURE )
	{
		return;
	}
	yile_net_socket *p = get_yile_net_server_fd( log_handle, LOG_FD_HEAD );
	if ( NULL == p ) //没有找到
	{
		RETURN_FALSE;
	}
	/*
	else
	{
		if ( 2 == log_type )		//通过网络的日志
		{
			yile_net_send_data( p->socket_fd, log_type, log_data, log_len + 1 );
		}
		else
		{
			ext_log_type tmp_ext;
			tmp_ext.ext_file_len = ext_len;
			tmp_ext.msg_len = log_len - ext_len - 1;
			yile_net_send_ext_data( p->socket_fd, log_type, &tmp_ext, sizeof( ext_log_type ), log_data, log_len + 1 );
		}
	}
	*/
}

//连接im服务器
PHP_FUNCTION ( yile_log_connect )
{
	long server_id;
	char *host;
	long port;
	int host_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "lsl", &server_id, &host, &host_len, &port ) == FAILURE )
	{
		return;
	}
	int re = -1;
	/*

	if ( yile_net_connect( server_id, host, port, LOG_FD_HEAD ) >= 0 )
	{
		yile_net_socket *p = find_yile_net_server_fd( server_id, LOG_FD_HEAD );
		re = p->array_index;
	}
	*/
	RETURN_LONG( re );
}

//检测和聊天服务器是否还连着
PHP_FUNCTION ( yile_log_ping )
{
	long server_id;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &server_id ) == FAILURE )
	{
		return;
	}
	int re_value = yile_net_ping( server_id, LOG_FD_HEAD );
	RETURN_LONG( re_value );
}
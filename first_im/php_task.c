#include "task.h"

/**
 * pack_id: 20007 将数据转发给某个用户
 */
void request_role_proxy_re( fd_struct_t *fd_info, proto_role_proxy_re_t *req_pack )
{
	fd_struct_t *aim_info = role_fd_info( req_pack->role_id );
	if ( NULL != aim_info )
	{
		fd_info->poller->send_data( aim_info, req_pack->data->bytes, req_pack->data->len );
	}
}

/**
 * pack_id: 20006 将数据转发给某个fd
 */
void request_fd_proxy_re( fd_struct_t *fd_info, proto_fd_proxy_re_t *req_pack )
{
	if ( IM_PROXY_OBJECT.check_anonymity_fd( req_pack->fd, req_pack->fd_id ) )
	{
		FirstPoller *main_poller = fd_info->poller;
		fd_struct_t *aim_info = main_poller->find_fd( req_pack->fd );
		if ( NULL != aim_info )
		{
			main_poller->send_data( aim_info, req_pack->data->bytes, req_pack->data->len );
		}
	}
}

/**
 * pack_id: 20001 php进程加入server
 */
void request_php_join_server( fd_struct_t *fd_info, proto_php_join_server_t *req_pack )
{
	if ( 0 != strncmp( SUPER_KEY, req_pack->join_key, strlen( SUPER_KEY ) ) )
	{
		close_fd_info( fd_info );
		return;
	}
	fd_info->socket_type = req_pack->socket_type;
	if ( SOCKET_TYPE_PHP_FPM == fd_info->socket_type )
	{
		IM_PROXY_OBJECT.add( fd_info );
	}
}

/**
 * pack_id: 20002 ping包
 */
void request_php_ping( fd_struct_t *fd_info )
{

}
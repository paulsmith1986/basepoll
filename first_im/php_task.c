#include "task.h"

/**
 * pack_id: 20007 将数据转发给某个用户
 */
void request_role_proxy_re( fd_struct_t *fd_info, proto_role_proxy_re_t *req_pack )
{

}

/**
 * pack_id: 20006 将数据转发给某个fd
 */
void request_fd_proxy_re( fd_struct_t *fd_info, proto_fd_proxy_re_t *req_pack )
{
	printf( "req_pack->fd:%d req_pack->fd_id:%d\n", req_pack->fd, req_pack->fd_id );
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
#include "task.h"

/**
 * pack_id: 20007 服务器给单个用户推数据
 */
void request_so_push_role_data( fd_struct_t *fd_info, proto_so_push_role_data_t *req_pack )
{
	fd_struct_t *role_fd = role_fd_info( req_pack->role_id );
	if ( role_fd )
	{
		MAIN_POLLER.send_data( role_fd, req_pack->data->bytes, req_pack->data->len );
	}
}

/**
 * pack_id: 20008 服务器给某个频道推数据
 */
void request_so_push_channel_data( fd_struct_t *fd_info, proto_so_push_channel_data_t *req_pack )
{

}

/**
 * pack_id: 20009 服务器给所有人推数据
 */
void request_so_push_world_data( fd_struct_t *fd_info, proto_so_push_world_data_t *req_pack )
{

}

/**
 * pack_id: 20010 服务器给一些人推数据
 */
void request_so_push_role_list_data( fd_struct_t *fd_info, proto_so_push_role_list_data_t *req_pack )
{

}

/**
 * pack_id: 20000 连接im服务器
 */
void request_so_php_join( fd_struct_t *fd_info, proto_so_php_join_t *req_pack )
{
	if ( 0 != strncmp( SUPER_KEY, req_pack->join_str, strlen( SUPER_KEY ) ) )
	{
		MAIN_POLLER.close_fd( fd_info );
		return;
	}
	if ( SOCKET_TYPE_FPM_MAIN == fd_info->socket_type )
	{
		IM_PROXY_OBJECT.add( fd_info );
	}
	proto_so_php_join_re_t out_data;
	out_data.result = 0;
	encode_so_php_join_re( result_pack, &out_data );
	send_protocol( fd_info, result_pack );
}

/**
 * pack_id: 21000 用户进入游戏
 */
void request_so_role_enter( fd_struct_t *fd_info, proto_so_role_enter_t *req_pack )
{

}

/**
 * pack_id: 21002 加入聊天频道
 */
void request_role_join_channel( fd_struct_t *fd_info, proto_role_join_channel_t *req_pack )
{

}

/**
 * pack_id: 21003 加入场景
 */
void request_role_join_scene( fd_struct_t *fd_info, proto_role_join_scene_t *req_pack )
{

}

/**
 * pack_id: 20002 ping包
 */
void request_php_ping( fd_struct_t *fd_info )
{

}
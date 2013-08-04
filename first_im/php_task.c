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
	OUT_LOG << "socket_type:" << req_pack->socket_type << fin;
	fd_info->socket_type = req_pack->socket_type;
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
	fd_struct_t *join_role_fd_info = find_fd( req_pack->session_id );
	if ( NULL == join_role_fd_info )
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "Can not find role_session_fd_info! session_id:" << req_pack->session_id << fin;
		#endif
		return;
	}
	if ( NULL != join_role_fd_info->ext_data || SOCKET_TYPE_UNKOWN != join_role_fd_info->socket_type )
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "Session is in game now!" << fin;
		#endif
		return;
	}
	proto_im_join_server_re_t join_out_data;
	join_out_data.result = 0;
	join_out_data.server_time = time( 0 );
	//判断 玩家是否已经在线
	fd_struct_t *check_role_fd_info = role_fd_info( req_pack->role_id );
	if ( NULL != check_role_fd_info )
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "role:" << req_pack->role_id << " 已经在游戏中 kick flag:" << req_pack->flag << fin;
		#endif
		if ( 0 == req_pack->flag )
		{
			join_out_data.result = 1;
			encode_im_join_server_re( send_pack, &join_out_data );
			main_poller_send_pack( join_role_fd_info, send_pack );
			return;
		}
		else
		{
			#ifdef FIRST_DEBUG
			OUT_LOG << "role:" << req_pack->role_id << " 被挤下线" << fin;
			#endif
			/*
			encode_im_role_conflic( role_conflic_re );
			main_poller_send_pack( check_role_fd_info, role_conflic_re );
			*/
			if ( NULL != check_role_fd_info->ext_data )
			{
				role_struct_t *tmp_info = static_cast<role_struct_t*>( check_role_fd_info->ext_data );
				delete_role_struct( tmp_info );
				check_role_fd_info->ext_data = NULL;
			}
			close_fd_info( check_role_fd_info );
		}
	}
	role_struct_t *new_role = new_role_struct();
	new_role->role_id = req_pack->role_id;
	new_role->login_time = join_out_data.server_time;
	new_role->model_id = req_pack->swf_id;
	memcpy( &new_role->role_name, &req_pack->role_name, sizeof( req_pack->role_name ) );
	new_role->job_id = req_pack->job_id;
	ROLE_LIST[ req_pack->role_id ] = join_role_fd_info;
	//PHP_IM_SYNC_OBJ.role_login( req_pack->role_id );
	#ifdef FIRST_DEBUG
	OUT_LOG << "加入用户 role_id:" << req_pack->role_id << " role_name:" << req_pack->role_name << fin;
	#endif
	join_role_fd_info->ext_data = new_role;
	encode_im_join_server_re( send_pack, &join_out_data );
	main_poller_send_pack( join_role_fd_info, send_pack );

	proto_main_init_t init_pack;
	init_pack.role_id = req_pack->role_id;
	encode_main_init( result_pack, &init_pack );
	IM_PROXY_OBJECT.proxy( &result_pack, 0 );
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
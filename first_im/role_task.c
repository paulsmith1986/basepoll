#include "task.h"
/**
 * pack_id: 10000 加入服务器
 */
void request_im_join_server( fd_struct_t *fd_info, proto_im_join_server_t *req_pack )
{
	printf( "key:%s\n", req_pack->join_key );
	char join_md5_str[ MD5_POOL_LEN ];
	snprintf( join_md5_str, MD5_POOL_LEN, "%s|%d|%d|%d|%s|%s", LOGIN_KEY, req_pack->role_id, req_pack->level, req_pack->job, req_pack->role_name, SUPER_KEY );
	int chk_re = chk_md5( join_md5_str, req_pack->join_key );
	if ( 1 != chk_re )
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "连接 " << fd_info->fd << "被踢,加入服务器key出错 加密串" << join_md5_str << fin;
		#endif
		close_fd_info( fd_info );
		return;
	}
	//判断 玩家是否已经在线
	fd_struct_t *role_fd = role_fd_info( req_pack->role_id );
	if ( NULL != role_fd )
	{
		if ( 0 == req_pack->flag )
		{
			encode_im_role_online( send_pack );
			send_protocol( fd_info, send_pack );
			return;
		}
		else
		{
			encode_im_role_conflic( role_conflic_re );
			send_protocol( role_fd, role_conflic_re );
			if ( NULL != role_fd->ext_data )
			{
				role_struct_t *tmp_info = static_cast<role_struct_t*>( role_fd->ext_data );
				delete_role_struct( tmp_info );
				role_fd->ext_data = NULL;
			}
			close_fd_info( role_fd );
		}
	}
	role_struct_t *new_role = new_role_struct();
	new_role->role_id = req_pack->role_id;
	new_role->login_time = time( 0 );
	new_role->model_id = req_pack->model_id;
	new_role->job_id = req_pack->job;
	memcpy( &new_role->role_name, &req_pack->role_name, sizeof( req_pack->role_name ) );
	new_role->level = req_pack->level;
	new_role->show_role = req_pack->show_role;
	ROLE_LIST[ req_pack->role_id ] = fd_info;
	#ifdef FIRST_DEBUG
	OUT_LOG << "加入用户:" << join_md5_str << fin;
	#endif
	fd_info->ext_data = new_role;
	proto_im_join_server_re_t join_out;
	printf( "login_time:%u\n", new_role->login_time );
	join_out.server_time = new_role->login_time;
	encode_im_join_server_re( send_pack, &join_out );
	send_protocol( fd_info, send_pack );
	printf( "Join end\n" );
}

/**
 * pack_id: 2 加入聊天频道
 */
void request_im_join_channel( fd_struct_t *fd_info, proto_im_join_channel_t *req_pack )
{
	channel_id_u to_join_ch;
	to_join_ch.ch_type = req_pack->join_ch->ch_type;
	to_join_ch.sub_id = req_pack->join_ch->sub_id;
	to_join_ch.msg_time = 0;
	to_join_ch.ch_id = req_pack->join_ch->ch_id;

	#ifdef FIRST_DEBUG
	OUT_LOG << "加入频道：" << print_ch_name( to_join_ch.channel_id ) << fin;
	#endif
	get_role_info_from_fd_info( fd_info );
	//*
	char join_md5_str[ MD5_POOL_LEN ];
	snprintf( join_md5_str, MD5_POOL_LEN, "%s|%c_%u_%d|%d", SUPER_KEY, to_join_ch.ch_type, to_join_ch.sub_id, to_join_ch.ch_id, role_info->role_id );
	int chk_re = chk_md5( join_md5_str, req_pack->join_key );
	if ( 1 != chk_re )
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "连接 " << fd_info->fd << "被踢,加入频道key出错 加密串" << join_md5_str << " join_key:" << req_pack->join_key << fin;
		#endif
		close_fd_info( fd_info );
		return;
	}
	join_list_t *tmp_join = role_info->join_list;
	while ( NULL != tmp_join )
	{
		if ( tmp_join->channel.ch_type == to_join_ch.ch_type )
		{
			if ( tmp_join->channel.channel_id == to_join_ch.channel_id )
			{
				#ifdef FIRST_DEBUG
				OUT_LOG << "已经加入相同的频道:" << print_ch_name( to_join_ch.channel_id ) << fin;
				#endif
				return;
			}
			proto_im_channel_t exit_ch_out;
			exit_ch_out.sub_id = tmp_join->channel.sub_id;
			exit_ch_out.ch_id = tmp_join->channel.ch_id;
			exit_ch_out.ch_type = tmp_join->channel.ch_type;
			proto_im_exit_channel_re_t exit_out;
			exit_out.exit_ch = &exit_ch_out;
			encode_im_exit_channel_re( exit_send_pack, &exit_out );
			send_protocol( fd_info, exit_send_pack );
			clear_join_ch_list( role_info, CLEAR_JOIN_LIST_ONE, tmp_join->channel.channel_id );
			break;
		}
		tmp_join = tmp_join->next;
	}
	//看该频道是否存在
	channel_map_t::iterator map_it = CHANNEL_LIST.find( to_join_ch.channel_id );
	channel_t *tmp_channel;
	if ( map_it != CHANNEL_LIST.end() )	//该频道已经存在了
	{
		tmp_channel = map_it->second;
	}
	//频道不存在，创建
	else
	{
		tmp_channel = new channel_t();
		pair< channel_map_t::iterator, bool > map_pair = CHANNEL_LIST.insert( make_pair( to_join_ch.channel_id, tmp_channel ) );
		if ( !map_pair.second )
		{
			return;
		}
		map_it = map_pair.first;
		#ifdef FIRST_DEBUG
		OUT_LOG << "创建频道：" << print_ch_name( to_join_ch.channel_id ) << fin;
		#endif
	}
	(*tmp_channel)[ role_info->role_id ] = fd_info;
	join_list_t *new_join = new_join_list( to_join_ch );
	new_join->next = role_info->join_list;
	role_info->join_list = new_join;
	#ifdef FIRST_DEBUG
	OUT_LOG << "频道:" << print_ch_name( to_join_ch.channel_id ) << " 人数：" << tmp_channel->size() << fin;
	#endif
	proto_im_channel_t join_ch_out;
	proto_im_join_channel_re_t join_out;
	join_ch_out.sub_id = to_join_ch.sub_id;
	join_ch_out.ch_type = to_join_ch.ch_type;
	join_ch_out.ch_id = to_join_ch.ch_id;
	join_out.msg_time = channle_msg_time( to_join_ch.ch_type );
	join_out.join_ch = &join_ch_out;
	encode_im_join_channel_re( send_pack, &join_out );
	send_protocol( fd_info, send_pack );
}

/**
 * pack_id: 10007 加入站街
 */
void request_im_enter_scene( fd_struct_t *fd_info, proto_im_enter_scene_t *req_pack )
{
	get_role_info_from_fd_info( fd_info );
	scene_pack_u join_scene_pack;
	join_scene_pack.sub_id = req_pack->scene_pack->sub_id;
	join_scene_pack.scene_id = req_pack->scene_pack->scene_id;
	if ( join_scene_pack.scene_pack_id == role_info->scene_pack )
	{
		return;
	}
	#ifdef FIRST_DEBUG
	OUT_LOG << "加入场景：" << print_scene_name( join_scene_pack.scene_pack_id ) << fin;
	#endif
	if ( role_info->scene_pack > 0 )
	{
		exit_scene( role_info->role_id, role_info->scene_pack );
	}
	//所有人都分配到0线
	join_scene_pack.line_id = 0;
	scene_t *scene_map = find_scene( join_scene_pack.scene_pack_id );

	proto_im_coord_t role_coord;
	role_coord.x = 0;
	role_coord.y = 0;
	if ( NULL == scene_map )
	{
		scene_map = new scene_t;
		SCENE_LIST[ join_scene_pack.scene_pack_id ] = scene_map;
		#ifdef FIRST_DEBUG
		OUT_LOG << "创建场景：" << print_scene_name( join_scene_pack.scene_pack_id ) << fin;
		#endif
	}
	else
	{
		//通知场景上的人，我来了
		proto_im_scene_add_role_t add_role_out;
		add_role_out.role_id = role_info->role_id;
		add_role_out.model = role_info->model_id;
		add_role_out.coord = &role_coord;
		memcpy( add_role_out.role_name, role_info->role_name, sizeof( add_role_out.role_name ) );
		encode_im_scene_add_role( send_pack, &add_role_out );
		send_scene_data( scene_map, &send_pack );
	}
	role_info->scene_pack = join_scene_pack.scene_pack_id;
	(*scene_map)[ role_info->role_id ] = fd_info;
	proto_im_enter_scene_re_t enter_out;
	enter_out.coord = &role_coord;
	enter_out.scene_pack = req_pack->scene_pack;
	encode_im_enter_scene_re( join_re_pack, &enter_out );
	send_protocol( fd_info, join_re_pack );
}

/**
 * pack_id: 10011 场景移动
 */
void request_im_scene_move( fd_struct_t *fd_info, proto_im_scene_move_t *req_pack )
{
	get_role_info_from_fd_info( fd_info );
	scene_t *scene_map = find_scene( role_info->scene_pack );
	if ( NULL == scene_map )
	{
		return;
	}
	proto_im_scene_move_re_t move_out;
	move_out.role_id = role_info->role_id;
	move_out.coord = req_pack->coord;
	encode_im_scene_move_re( send_pack, &move_out );
	send_scene_data( scene_map, &send_pack );
}
#include "task.h"

/**
 * 查找一个玩家
 */
fd_struct_t *role_fd_info( int role_id )
{
	role_list_t::iterator user_id_it;
	user_id_it = ROLE_LIST.find( role_id );
	if ( user_id_it != ROLE_LIST.end() )
	{
		return user_id_it->second;
	}
	else
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "[Role] role_id:" << role_id << " not exist!" << fin;
		#endif
		return NULL;
	}
}

/**
 * 判断md5加密是否正确
 */
int chk_md5( char *src_str, char *md5_str )
{
	MD5_CTX context;
	unsigned char digest[16];
	MD5_Init( &context );
	MD5_Update( &context, src_str, strlen( src_str ) );
	MD5_Final( digest, &context );
	static const char hexits[17] = "0123456789abcdef";
	return hexits[ digest[0] >> 4 ] == md5_str[ 0 ] &&
			hexits[ digest[0] & 0x0F ] == md5_str[ 1 ] &&
			hexits[ digest[1] >> 4 ] == md5_str[ 2 ] &&
			hexits[ digest[1] & 0x0F ] == md5_str[ 3 ] &&
			hexits[ digest[2] >> 4 ] == md5_str[ 4 ] &&
			hexits[ digest[2] & 0x0F ] == md5_str[ 5 ] &&
			hexits[ digest[3] >> 4 ] == md5_str[ 6 ] &&
			hexits[ digest[3] & 0x0F ] == md5_str[ 7 ] &&
			hexits[ digest[4] >> 4 ] == md5_str[ 8 ] &&
			hexits[ digest[4] & 0x0F ] == md5_str[ 9 ] &&
			hexits[ digest[5] >> 4 ] == md5_str[ 10 ] &&
			hexits[ digest[5] & 0x0F ] == md5_str[ 11 ] &&
			hexits[ digest[6] >> 4 ] == md5_str[ 12 ] &&
			hexits[ digest[6] & 0x0F ] == md5_str[ 13 ] &&
			hexits[ digest[7] >> 4 ] == md5_str[ 14 ] &&
			hexits[ digest[7] & 0x0F ] == md5_str[ 15 ];
}

/**
 * 新玩家信息
 */
role_struct_t *new_role_struct()
{
	role_struct_t *tmp_role = ROLE_STRUCT_POOL.get();
	memset( tmp_role, 0, sizeof( role_struct_t ) );
	return tmp_role;
}

/**
 * 回收玩家信息
 */
void delete_role_struct( role_struct_t *tmp_info )
{
	if ( NULL == tmp_info )
	{
		return;
	}
	//todo退出加入的场景
	if ( NULL != tmp_info->join_list )
	{
		clear_join_ch_list( tmp_info, CLEAR_JOIN_LIST_ALL, 0 );
	}
	role_list_t::iterator user_id_it = ROLE_LIST.find( tmp_info->role_id );
	if ( user_id_it == ROLE_LIST.end() )
	{
		return;
	}
	ROLE_LIST.erase( user_id_it );
	ROLE_STRUCT_POOL.recycle( tmp_info );
}

/**
 * 退出一个聊天频道
 */
void exit_channel( int role_id, uint64_t exit_ch_id )
{
	channel_map_t::iterator map_it = CHANNEL_LIST.find( exit_ch_id );
	if ( map_it == CHANNEL_LIST.end() )
	{
		return;
	}
	#ifdef FIRST_DEBUG
	OUT_LOG << "用户:" << role_id << " 退出频道" << print_ch_name( exit_ch_id ) << fin;
	#endif
	channel_t *tmp_channel = map_it->second;
	if ( NULL == tmp_channel )
	{
		return;
	}
	channel_t::iterator ch_it = tmp_channel->find( role_id );
	if ( ch_it != tmp_channel->end() )
	{
		tmp_channel->erase( ch_it );
	}
	#ifdef FIRST_DEBUG
	OUT_LOG << "频道:" << print_ch_name( exit_ch_id ) << " 人数: " << tmp_channel->size() << fin;
	#endif
	if ( tmp_channel->empty( ) )
	{
		delete( tmp_channel );
		tmp_channel = NULL;
		#ifdef FIRST_DEBUG
		OUT_LOG << "频道 " << print_ch_name( exit_ch_id ) << "因为没有人了，所以清除掉" << fin;
		#endif
		CHANNEL_LIST.erase( map_it );
	}
}

/**
 * 退出一个场景
 */
void exit_scene( int role_id, uint64_t scene_pack_id )
{
	#ifdef FIRST_DEBUG
	OUT_LOG << "退出场景:" << print_scene_name( scene_pack_id ) << fin;
	#endif
	scene_map_t::iterator map_it = SCENE_LIST.find( scene_pack_id );
	if ( map_it == SCENE_LIST.end() )
	{
		return;
	}
	scene_t *scene_map = map_it->second;
	scene_t::iterator s_it = scene_map->find( role_id );
	if ( s_it != scene_map->end() )
	{
		scene_map->erase( s_it );
	}
	#ifdef FIRST_DEBUG
	OUT_LOG << "场景：" << print_scene_name( scene_pack_id ) << "退出用户 还有人数:" << scene_map->size() << fin;
	#endif
	//已经没有人了
	if ( scene_map->empty() )
	{
		delete( scene_map );
		#ifdef FIRST_DEBUG
		OUT_LOG << "场景 " << print_scene_name( scene_pack_id ) << "因为没有人了，所以清除掉" << fin;
		#endif
		SCENE_LIST.erase( map_it );
	}
	else
	{
		proto_im_scene_role_quit_t out_data;
		out_data.role_id = role_id;
		encode_im_scene_role_quit( result_pack, &out_data );
		send_scene_data( scene_map, &result_pack );
	}
}

/**
 * 根据场景组合ID找到场景
 */
scene_t *find_scene( uint64_t scene_pack_id )
{
	scene_map_t::iterator map_it = SCENE_LIST.find( scene_pack_id );
	if ( map_it == SCENE_LIST.end() )
	{
		#ifdef FIRST_DEBUG
		OUT_LOG << "[Scene]场景:" << print_scene_name( scene_pack_id ) << " 不存在" << fin;
		#endif
		return NULL;
	}
	return map_it->second;
}

/**
 * 清除加入频道列表
 */
void clear_join_ch_list( role_struct_t *role_info, int clear_type, uint64_t channle_id )
{
	join_list_t *tmp_join = role_info->join_list;
	join_list_t *last_join = NULL;
	while ( NULL != tmp_join )
	{
		bool is_clear = false;
		bool is_break = false;
		switch ( clear_type )
		{
			case CLEAR_JOIN_LIST_ONE:
			{
				if ( tmp_join->channel.channel_id == channle_id )
				{
					is_clear = true;
					is_break = true;
				}
			}
			break;
			case CLEAR_JOIN_LIST_ALL:
				is_clear = true;
			break;
		}
		if ( is_clear )
		{
			join_list_t *find_one = tmp_join;
			tmp_join = tmp_join->next;
			//在表头
			if ( find_one == role_info->join_list )
			{
				role_info->join_list = find_one->next;
			}
			else
			{
				last_join->next = find_one->next;
			}
			exit_channel( role_info->role_id, find_one->channel.channel_id );
			delete_join_list( find_one );
		}
		else
		{
			last_join = tmp_join;
			tmp_join = tmp_join->next;
		}
		if ( is_break )
		{
			break;
		}
	}
}

/**
 * 新建一个加入频道列表的struct
 */
join_list_t *new_join_list( channel_id_u channel )
{
	join_list_t *tmp_join = ( join_list_t * )malloc( sizeof( join_list_t ) );
	tmp_join->channel = channel;
	return tmp_join;
}

/**
 * 清除加入频道列表
 */
void delete_join_list( join_list_t *tmp_join )
{
	free( tmp_join );
}

/**
 * 频道说话间隔
 */
char channle_msg_time( char ch_type )
{
	char re = 0;
	switch ( ch_type )
	{
		case 'S':
			re = 2;
		break;
		case 'T':
			re = 1;
		break;
	}
	return re;
}

/**
 * 给一个场景的人发消息
 */
void send_scene_data( scene_t *scene_map, protocol_result_t *re_pack )
{
	scene_t::iterator s_it;
	for ( s_it = scene_map->begin(); s_it != scene_map->end(); ++s_it )
	{
		send_protocol_p( s_it->second, re_pack );
	}
	if ( re_pack->is_resize )
	{
		free( re_pack->str );
	}
}

#ifdef FIRST_DEBUG
char CH_NAME_BUF[ 200 ];
char *print_ch_name( uint64_t ch_id )
{
	channel_id_u ch;
	ch.channel_id = ch_id;
	sprintf( CH_NAME_BUF, "%c_%u_%d", ch.ch_type, ch.sub_id, ch.ch_id );
	return CH_NAME_BUF;
}
char *print_scene_name( uint64_t scene_pack_id )
{
	scene_pack_u sc;
	sc.scene_pack_id = scene_pack_id;
	sprintf( CH_NAME_BUF, "%d_%d_%d", sc.scene_id, sc.sub_id, sc.line_id );
	return CH_NAME_BUF;
}
#endif

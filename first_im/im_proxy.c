#include "im_proxy.h"
#include "im_common.h"
/**
 * 主动转发
 */
void ImProxy::proxy( protocol_result_t *pack_data, uint32_t hash_id )
{
	fd_struct_t *fd_info = get_proxy_fd( hash_id );
	if ( NULL == fd_info )
	{
		return;
	}
	main_poller_send_pack_p( fd_info, pack_data );
}

/**
 * 被动转发
 */
void ImProxy::proxy( protocol_packet_t *pack_data, uint32_t hash_id )
{
	fd_struct_t *fd_info = get_proxy_fd( hash_id );
	if ( NULL == fd_info )
	{
		return;
	}
	MAIN_POLLER.send_data( fd_info, pack_data->data, pack_data->max_pos );
}

/**
 * 获取代理fd
 */
inline fd_struct_t* ImProxy::get_proxy_fd( uint32_t hash_id )
{
	size_t fpm_size = im_proxy_list_.size();
	OUT_ERROR << "fpm_size:" << fpm_size << fin;
	if ( 0 == fpm_size )
	{
		OUT_ERROR << "No php process join!" << fin;
		return NULL;
	}
	int index = 0;
	if ( fpm_size > 1 )
	{
		if ( 0 == hash_id )
		{
			index = mt_rand( 0, fpm_size );
		}
		else
		{
			index = hash_id % fpm_size;
		}
	}
	return im_proxy_list_[ index ];
}

/**
 * 加入php进程
 */
void ImProxy::add( fd_struct_t *fd_info )
{
	im_proxy_list_.push_back( fd_info );
}

/**
 * 是否是PHP连接
 */
void ImProxy::check_is_php_fd( fd_struct_t *fd_info )
{
	im_proxy_list_t::iterator list_it;
	for ( list_it = im_proxy_list_.begin(); list_it != im_proxy_list_.end(); ++list_it )
	{
		if ( *list_it == fd_info )
		{
			#ifdef FIRST_DEBUG
			OUT_LOG << "The fd in proxy_fd!" << fin;
			#endif
			im_proxy_list_.erase( list_it );
			break;
		}
	}
}
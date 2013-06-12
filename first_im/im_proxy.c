#include "im_proxy.h"
/**
 * 转发消息
 */
void ImProxy::proxy( protocol_result_t *pack_data )
{
	if ( 0 == im_proxy_list_.size() )
	{
		OUT_ERROR << "No php process join!" << fin;
		return;
	}
	if ( proxy_index_ >= im_proxy_list_.size() )
	{
		proxy_index_ = 0;
	}
	fd_struct_t *fd_info = im_proxy_list_[ proxy_index_ ];
	++proxy_index_;
	send_protocol_p( fd_info, pack_data );
}

/**
 * 加入php进程
 */
void ImProxy::add( fd_struct_t *fd_info )
{
	im_proxy_map_t::iterator it = im_proxy_map_.find( fd_info->fd );
	if ( it != im_proxy_map_.end() )
	{
		return;
	}
	im_proxy_map_[ fd_info->fd ] = time( 0 );
	im_proxy_list_.push_back( fd_info );
}

/**
 * 是否是PHP连接
 */
void ImProxy::check_is_php_fd( fd_struct_t *fd_info )
{
	im_proxy_map_t::iterator it = im_proxy_map_.find( fd_info->fd );
	if ( it != im_proxy_map_.end() )
	{
		OUT_ERROR << "Php process exist!" << fin;
		im_proxy_map_.erase( it );
	}
	im_proxy_list_t::iterator list_it;
	for ( list_it = im_proxy_list_.begin(); list_it != im_proxy_list_.end(); ++list_it )
	{
		if ( *list_it == fd_info )
		{
			im_proxy_list_.erase( list_it );
			break;
		}
	}
}
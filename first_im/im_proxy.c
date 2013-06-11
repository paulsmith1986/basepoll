#include "im_proxy.h"
/**
 * 转发消息
 */
void ImProxy::proxy( protocol_packet_t *pack_data )
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
	fd_info->poller->send_data( fd_info, pack_data->data, pack_data->pos );
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
}
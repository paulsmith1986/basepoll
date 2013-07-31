#include "im_proxy.h"
/**
 * 主动转发
 */
void ImProxy::proxy( protocol_result_t *pack_data )
{
	fd_struct_t *fd_info = get_proxy_fd();
	if ( NULL == fd_info )
	{
		return;
	}
	MAIN_POLLER.send_data( fd_info, pack_data->str, pack_data->pos );
}

/**
 * 被动转发
 */
void ImProxy::proxy( protocol_packet_t *pack_data )
{
	fd_struct_t *fd_info = get_proxy_fd();
	if ( NULL == fd_info )
	{
		return;
	}
	MAIN_POLLER.send_data( fd_info, pack_data->data, pack_data->max_pos );
}

/**
 * 获取代理fd
 */
inline fd_struct_t* ImProxy::get_proxy_fd()
{
	if ( 0 == im_proxy_list_.size() )
	{
		OUT_ERROR << "No php process join!" << fin;
		return NULL;
	}
	if ( proxy_index_ >= im_proxy_list_.size() )
	{
		proxy_index_ = 0;
	}
	fd_struct_t *fd_info = im_proxy_list_[ proxy_index_ ];
	++proxy_index_;
	#ifdef FIRST_DEBUG
	OUT_LOG << "Find fd:" << fd_info->fd << fin;
	#endif
	return fd_info;
}

/**
 * 加入php进程
 */
void ImProxy::add( fd_struct_t *fd_info )
{
	im_proxy_list_.push_back( fd_info );
	OUT_LOG << "sizeof list:" << im_proxy_list_.size() << fin;
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
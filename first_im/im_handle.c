#include "im_handle.h"
void ImHandle::on_socket_fd( fd_struct_t *fd_info, protocol_packet_t *read_packet )
{
	packet_head_t *pack_head = ( packet_head_t* )read_packet->data;
	printf( "Request packet, pack_id:%d\n", pack_head->pack_id );
	//小于10000的数据包是转发给php的数据包
	if ( pack_head->pack_id < 10000 )
	{
		//客户端能发送的最大数据量
		if ( read_packet->pos > CLIENT_MAX_REQEUST )
		{
			OUT_ERROR << "Too big data packet!" << fin;
			close_fd_info( fd_info );
			return;
		}
		proto_bin_t request_pack;
		request_pack.len = read_packet->max_pos;
		request_pack.bytes = read_packet->data;
		printf( "data len:%d\n", read_packet->max_pos );
		//还未登录的用户
		if ( NULL == fd_info->ext_data )
		{
			//未登录前.只能发 101 注册 或者 103 登录 包
			if ( 101 != pack_head->pack_id && 103 != pack_head->pack_id )
			{
				return;
			}
			printf( "proxy!\n" );
			proto_fd_proxy_t proxy_pack;
			proxy_pack.fd = fd_info->fd;
			proxy_pack.fd_id = add_anonymity( proxy_pack.fd );
			proxy_pack.data = &request_pack;
			encode_fd_proxy( proxy_new_pack, &proxy_pack );
			IM_PROXY_OBJECT.proxy( &proxy_new_pack );
		}
		else
		{
			role_struct_t *role_info = ( role_struct_t* )fd_info->ext_data;
			proto_role_proxy_t proxy_pack;
			proxy_pack.role_id = role_info->role_id;
			proxy_pack.data = &request_pack;
			encode_role_proxy( proxy_new_pack, &proxy_pack );
			IM_PROXY_OBJECT.proxy( &proxy_new_pack );
		}
	}
	else
	{
		//php和server交互包 20001包可以通过.其它包必须是SOCKET_TYPE_PHP才能通过
		if ( pack_head->pack_id > 20000 )
		{
			printf( "pack_id:%d\n", pack_head->pack_id );
			if ( fd_info->socket_type != SOCKET_TYPE_PHP_FPM && 20001 != pack_head->pack_id )
			{
				close_fd_info( fd_info );
				return;
			}
		}
		do_request_task( read_packet, fd_info );
	}
}

//加入匿名连接
uint16_t ImHandle::add_anonymity( int fd )
{
	anonymity_list_t::iterator it = anonymity_list_.find( fd );
	if ( it == anonymity_list_.end() )
	{
		if ( ++anonymity_index_ > 65535 )
		{
			anonymity_index_ = 0;
		}
		anonymity_list_[ fd ] = anonymity_index_;
		return anonymity_index_;
	}
	else
	{
		return it->second;
	}
}

//连接关闭回调
void ImHandle::on_close( fd_struct_t *fd_info )
{
	//匿名连接检查
	anonymity_list_t::iterator it = anonymity_list_.find( fd_info->fd );
	if ( it != anonymity_list_.end() )
	{
		anonymity_list_.erase( it );
	}
	IM_PROXY_OBJECT.check_is_php_fd( fd_info );
}

//信号捕捉到
void ImHandle::on_signal_fd( fd_struct_t *fd_info )
{
	struct signalfd_siginfo fdsi;
	int re = read( fd_info->fd, &fdsi, sizeof( struct signalfd_siginfo ) );
	if ( re != sizeof( struct signalfd_siginfo ) )
	{
		return;
	}
	printf( "Catch signal:%d\n", fdsi.ssi_signo );
	//判断信号类型
	switch ( fdsi.ssi_signo )
	{
		case SIGINT:			//ctrl+c
		case SIGQUIT:			//和SIGINT类似, 但由QUIT字符(通常是Ctrl-\)来控制
		case SIGTERM:			//kill
		case SIGHUP:			//用户session退出
			shut_down( fdsi.ssi_signo );
		break;
		case SIGUSR1:			//自定义

		break;
		case SIGPIPE:			//管道破裂 如Socket发数据时对方关闭
		break;
	}
}

void ImHandle::shut_down( int signal_no )
{
	printf( "Server while shutdown!" );
	exit( 0 );
}
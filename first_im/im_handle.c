#include "im_handle.h"
#include "task.h"
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
			MAIN_POLLER.close_fd( fd_info );
			return;
		}
		proto_bin_t request_pack;
		request_pack.len = read_packet->max_pos;
		request_pack.bytes = read_packet->data;
		int role_id = 0;
		//还未登录的用户
		if ( NULL == fd_info->ext_data )
		{
			if ( 100 != pack_head->pack_id )
			{
				#ifdef FIRST_DEBUG
				OUT_LOG << "Proxy pack need login!" << fin;
				#endif
				MAIN_POLLER.close_fd( fd_info );
				return;
			}
		}
		else
		{
			role_struct_t *role_info = ( role_struct_t* )fd_info->ext_data;
			role_id = role_info->role_id;
		}
		proto_so_fpm_proxy_t proxy_pack;
		proxy_pack.data = &request_pack;
		proxy_pack.role_id = role_id;
		#ifdef FIRST_DEBUG
		OUT_LOG << "session id:" << fd_info->session_id << "!" << fin;
		#endif
		proxy_pack.session_id = fd_info->session_id;
		encode_so_fpm_proxy( proxy_new_pack, &proxy_pack );
		IM_PROXY_OBJECT.proxy( &proxy_new_pack, role_id );
	}
	//10000 之间 20000的协议是聊天相关的
	else if ( pack_head->pack_id < 20000 )
	{
		if ( NULL == fd_info->ext_data )
		{
			#ifdef FIRST_DEBUG
			OUT_LOG << "Im pack need login!" << fin;
			#endif
			MAIN_POLLER.close_fd( fd_info );
			return;
		}
		do_request_task( read_packet, fd_info );
	}
	//20000以上 php和im交互
	else
	{
		if ( SOCKET_TYPE_UNKOWN == fd_info->socket_type && 20000 != pack_head->pack_id )
		{
			#ifdef FIRST_DEBUG
			OUT_LOG << "Php so pack need login!" << fin;
			#endif
			MAIN_POLLER.close_fd( fd_info );
			return;
		}
		do_request_task( read_packet, fd_info );
	}
}

//连接关闭回调
void ImHandle::on_close( fd_struct_t *fd_info )
{
	if ( fd_info->socket_type > 0 )
	{
		IM_PROXY_OBJECT.check_is_php_fd( fd_info );
	}
	if ( NULL != fd_info->ext_data )
	{
		role_struct_t *tmp_info = static_cast<role_struct_t*>( fd_info->ext_data );
		delete_role_struct( tmp_info );
		fd_info->ext_data = NULL;
	}
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
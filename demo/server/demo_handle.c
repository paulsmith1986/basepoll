#include "demo_handle.h"
void DemoHandle::on_socket_fd( fd_struct_t *fd_info, protocol_packet_t *read_packet )
{
	if ( !is_valid_packet( fd_info, read_packet ) )
	{
		close_fd_info( fd_info );
	}
	else
	{
		do_request_task( read_packet, fd_info );
	}
}

//连接关闭回调
void DemoHandle::on_close( fd_struct_t *fd_info )
{

}

//信号捕捉到
void DemoHandle::on_signal_fd( fd_struct_t *fd_info )
{
	struct signalfd_siginfo fdsi;
	int re = read( fd_info->fd, &fdsi, sizeof( struct signalfd_siginfo ) );
	if ( re != sizeof( struct signalfd_siginfo ) )
	{
		//OUT_ERROR << "Bad read signal size!" << fin;
		return;
	}
	printf( "Catch signal:%d\n", fdsi.ssi_signo );
	//判断信号类型
	switch ( fdsi.ssi_signo )
	{
		case SIGINT:			//ctrl+c
		case SIGQUIT:			//和SIGINT类似, 但由QUIT字符(通常是Ctrl-\)来控制
		case SIGTERM:			//kill
			shut_down( fdsi.ssi_signo );
		break;
		case SIGHUP:			//用户session退出

		break;
		case SIGUSR1:			//自定义

		break;
		case SIGPIPE:			//管道破裂 如Socket发数据时对方关闭
		break;
	}
}

void DemoHandle::shut_down( int signal_no )
{
	printf( "Server while shutdown!" );
	exit( 0 );
}

//数据包类型检查
bool DemoHandle::is_valid_packet( fd_struct_t *fd_info, protocol_packet_t *read_packet )
{
	return true;
}

//数据包长度检查
bool DemoHandle::check_pack_length( fd_struct_t *fd_info, protocol_packet_t *pack )
{
	return true;
}
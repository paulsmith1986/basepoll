#ifndef POLL_HANDLER_HEAD
#define POLL_HANDLER_HEAD
#include "first_poll.h"
#include "poller.h"
class PollHandler {
	public:
		//完整数据包到达
		virtual void on_socket_fd( fd_struct_t *fd_info, protocol_packet_t *read_packet )
		{
			printf( "Socket fd request!\n" );
		}
		//唤醒事件
		virtual void on_event_fd( fd_struct_t *fd_info )
		{
			printf( "Event fd wakeup!\n" );
		}
		//定时器通知
		virtual void on_timer_fd( fd_struct_t *fd_info )
		{
			printf( "Time up!\n" );
		}
		//信号通知
		virtual void on_signal_fd( fd_struct_t *fd_info )
		{
			printf( "Signal fd request!\n" );
		}
		//关闭连接回调
		virtual void on_close( fd_struct_t *fd_info )
		{
			printf( "Close fd!\n" );
		}
		//检查数据包长度
		virtual bool check_max_length( fd_struct_t *fd_info, protocol_packet_t *pack )
		{
			return true;
		}

};
#endif
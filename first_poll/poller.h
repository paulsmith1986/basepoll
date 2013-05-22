#ifndef FIRST_SERVER_POLLER_HEAD
#define FIRST_SERVER_POLLER_HEAD
#include <assert.h>
#include <vector>
#include <map>
#include "first_poll.h"
#include "poll_handler.h"
#include "struct_pool.h"
class FirstPoller
{
	public:
		FirstPoller( int max_event, int fd_pool_size, PollHandler *poll_handler )
		:epoll_fd_( epoll_create1( EPOLL_CLOEXEC ) ),
		fd_pool_( StructPool<fd_struct_t*>( fd_pool_size, sizeof( fd_struct_t ) ) ),
		events_( max_event ),
		fd_pool_size_( fd_pool_size ),
		poll_handler_( poll_handler )
		{
			if ( epoll_fd_ < 0 )
			{
				OUT_ERROR << "Epoll create failed" << fin;
				assert( epoll_fd_ >= 0 );
			}
		}
		~FirstPoller(){}
		//等待事件发生
		void poll( int interval );
		fd_struct_t* create_fd_struct( int fd, fdType fd_type );
		//唤醒fd
		fd_struct_t* create_event_fd();
		//时间fd
		fd_struct_t* create_timer_fd();
		//信号fd
		fd_struct_t* create_signal_fd( sigset_t &mask );
		//查看某个fd的详细信息
		fd_struct_t* find_fd( int fd );
		//更新fd的监听事件
		void update_fd_event( fd_struct_t *fd_info, int op, int new_event );
		//发送消息
		void send_data( fd_struct_t *fd_info, void *send_re, uint32_t total_send );
		//关闭fd
		void close_fd( fd_struct_t *fd_info );
		//设置fd状态
		inline void set_fd_status( fd_struct_t *fd_info, fdState s )
		{ fd_info->status = s; }
	private:
		typedef std::vector<struct epoll_event> EventList;
		typedef std::map<int, fd_struct_t*> CloseList;
		typedef std::map<int, fd_struct_t*> ActiveFdList;
		//写事件
		void fd_write_handle( fd_struct_t *fd_info );
		//新来连接
		void new_connection( fd_struct_t *fd_info );
		//读数据
		void read_socket_data( fd_struct_t *fd_info );
		int epoll_fd_;
		//fd_struct_t对象池
		StructPool<fd_struct_t*> fd_pool_;
		//等待关闭的fd
		CloseList close_list_;
		//活动状态的fd
		ActiveFdList fd_list_;
		//同时处理的事件个数
		EventList events_;
		//连接池大小
		const int fd_pool_size_;
		//回调处理类
		PollHandler *poll_handler_;
};
#endif
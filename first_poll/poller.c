#include "poller.h"
void FirstPoller::poll( int interval )
{
	int event_num = epoll_wait( epoll_fd_, &*events_.begin(), static_cast<int>( events_.size() ), interval );
	if ( event_num > 0 )
	{
		for ( int i = 0; i < event_num; ++i )
		{
			fd_struct_t *fd_struct = static_cast<fd_struct_t*>( events_[ i ].data.ptr );
			switch ( fd_struct->fd_type )
			{
				case FD_TYPE_LISTEN:		//新来连接了
					new_connection( fd_struct );
				break;
				case FD_TYPE_SOCKET:		//socket事件响应
				{
					if ( events_[ i ].events & EPOLLRDHUP )
					{
						#ifdef FIRST_NET_DEBUG
						NET_OUT_LOG << "Fd:" << fd_struct->fd << " has hang up!\n" << fin;
						#endif
						close_fd( fd_struct );
						continue;
					}
					//响应IN事件
					if ( events_[ i ].events & EPOLLIN )
					{
						//主处理函数
						read_socket_data( fd_struct );
					}
					//响应OUT事件
					if ( events_[ i ].events & EPOLLOUT )
					{
						#ifdef FIRST_NET_DEBUG
						NET_OUT_LOG << "Fd " << fd_struct->fd << " write buffer can send." << fin;
						#endif
						fd_write_handle( fd_struct );
					}
					break;
				}
				case FD_TYPE_EVENT:		//唤醒事件
					poll_handler_->on_event_fd( fd_struct );
				break;
				case FD_TYPE_TIMER:		//倒计时事件
					poll_handler_->on_timer_fd( fd_struct );
				break;
				case FD_TYPE_SIGNAL:	//信号事件
					poll_handler_->on_signal_fd( fd_struct );
				break;
				default:	//出错了
					OUT_ERROR << "Cache fd_type:" << fd_struct->fd_type << fin;
					#ifdef FIRST_NET_DEBUG
					abort();
					#endif
				break;
			}
		}
		if ( !close_list_.empty() )
		{
			CloseList::iterator it;
			for ( it = close_list_.begin(); it != close_list_.end(); ++it )
			{
				fd_struct_t *tmp_info = it->second;
				close( tmp_info->fd );
				//清除未发送完数据
				if ( 0 != tmp_info->un_send_len && NULL != tmp_info->un_send )
				{
					free( tmp_info->un_send );
					tmp_info->un_send = NULL;
					tmp_info->un_send_len = 0;
				}
				//清除未读完数据包
				if ( NULL != tmp_info->un_read_pack )
				{
					delete_protocol_packet( tmp_info->un_read_pack );
					tmp_info->un_read_pack = NULL;
				}

				if ( NULL != tmp_info->ext_data )
				{
					tmp_info->ext_data = NULL;
				}
				//清除fd_list
				ActiveFdList::iterator fd_it = fd_list_.find( tmp_info->fd );
				if ( fd_it != fd_list_.end() )
				{
					fd_list_.erase( fd_it );
				}
				fd_pool_.recycle( tmp_info );
			}
			close_list_.clear();
		}
	}
}

//新来连接
void FirstPoller::new_connection( fd_struct_t *fd_info )
{
	struct sockaddr_in clientaddr;
	socklen_t clilen = sizeof( clientaddr );
	int connfd = accept( fd_info->fd, (sockaddr *) &clientaddr, &clilen );
	if ( connfd < 0 )
	{
		OUT_ERROR << "Accept connect, but fd < 0" << fin;
	}
	else
	{
		#ifdef FIRST_NET_DEBUG
		NET_OUT_LOG << "New connection:" << connfd << fin;
		#endif
		//设置为非阻塞
		if ( !set_non_block( connfd ) )
		{
			OUT_ERROR << "New connect can not set not block, close it!" << fin;
			close( connfd );
			return;
		}
		fd_struct_t *new_fd = create_fd_struct( connfd, FD_TYPE_SOCKET );
		update_fd_event( new_fd, EPOLL_CTL_ADD, READ_EVENT );
	}
}

//获取一个统一的fd描述
fd_struct_t* FirstPoller::create_fd_struct( int fd, fdType fd_type )
{
	fd_struct_t *tmp_info = fd_pool_.get();
	memset( tmp_info, 0, sizeof( fd_struct_t ) );
	tmp_info->fd_type = fd_type;
	tmp_info->fd = fd;
	tmp_info->socket_type = 0;
	set_fd_status( tmp_info, FD_STATE_IDLE );
	fd_list_[ fd ] = tmp_info;
	return tmp_info;
}

//创建一个event_fd
fd_struct_t* FirstPoller::create_event_fd()
{
	int evtfd = eventfd( 0, EFD_NONBLOCK | EFD_CLOEXEC );
	if ( evtfd < 0 )
	{
		OUT_ERROR << "eventfd() failed!!" << fin;
		assert( evtfd >= 0 );
	}
	//printf( "new_event_fd:%d\n", evtfd );
	fd_struct_t *new_ev_fd = create_fd_struct( evtfd, FD_TYPE_EVENT );
	update_fd_event( new_ev_fd, EPOLL_CTL_ADD, READ_EVENT );
	return new_ev_fd;
}

//创建一个timer_fd
fd_struct_t* FirstPoller::create_timer_fd()
{
	int timerfd = timerfd_create( CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC );
	if ( timerfd < 0 )
	{
		OUT_ERROR << "timerfd_create() failed!!" << fin;
		assert( timerfd >= 0 );
	}
	fd_struct_t *new_t_fd = create_fd_struct( timerfd, FD_TYPE_TIMER );
	printf( "new_time_fd:%d\n", timerfd );
	update_fd_event( new_t_fd, EPOLL_CTL_ADD, READ_EVENT );
	return new_t_fd;
}

//发送数据
void FirstPoller::send_data( fd_struct_t *fd_info, void *send_re, uint32_t total_send )
{
	if ( fd_info->status == FD_STATE_CLOSE )
	{
		return;
	}
	//之前还有一些没发出去
	if ( fd_info->un_send_len > 0 )
	{
		char *un_send_char = (char *)realloc( fd_info->un_send, fd_info->un_send_len + total_send );
		memcpy( un_send_char + fd_info->un_send_len, send_re, total_send );
		fd_info->un_send_len += total_send;
		fd_info->un_send = un_send_char;
	}
	else
	{
		int send_num = send( fd_info->fd, send_re, total_send, MSG_NOSIGNAL );
		if ( send_num < 0 )
		{
			if ( errno != EAGAIN )
			{
				#ifdef FIRST_NET_DEBUG
				NET_OUT_LOG << "发送消息出错 errno:" << errno << fin;
				#endif
				return;
			}
			send_num = 0;
		}
		if ( (uint32_t)send_num != total_send )
		{
			fd_info->un_send_len = total_send - send_num;
			char *un_send_char = (char *)malloc( fd_info->un_send_len );
			memcpy( un_send_char, (char*)send_re + send_num, fd_info->un_send_len );
			fd_info->un_send = un_send_char;
			#ifdef FIRST_NET_DEBUG
			NET_OUT_LOG << "fd:" << fd_info->fd << " 有消息没有发送完 注册OUT事件" << fin;
			#endif
			//注册OUT事件
			update_fd_event( fd_info, EPOLL_CTL_MOD, WRITE_EVENT );
		}
	}
}

//查看一个fd详细信息
fd_struct_t* FirstPoller::find_fd( int fd )
{
	ActiveFdList::iterator fd_it = fd_list_.find( fd );
	if ( fd_it == fd_list_.end() )
	{
		return NULL;
	}
	else
	{
		return fd_it->second;
	}
}

//响应write事件
void FirstPoller::fd_write_handle( fd_struct_t *fd_info )
{
	if ( 0 == fd_info->un_send_len || NULL == fd_info->un_send )
	{
		return;
	}
	int send_num = send( fd_info->fd, fd_info->un_send, fd_info->un_send_len, MSG_NOSIGNAL );
	if ( send_num < 0 )
	{
		if ( errno != EAGAIN )
		{
			#ifdef FIRST_NET_DEBUG
			NET_OUT_LOG << "OUT事件里发送消息出错" << fin;
			#endif
			close_fd( fd_info );
			return;
		}
		send_num = 0;
	}
	if ( send_num != fd_info->un_send_len )
	{
		int new_un_len = fd_info->un_send_len - send_num;
		char *un_send_char = (char *)malloc( new_un_len );
		memcpy( un_send_char, fd_info->un_send + send_num, new_un_len );
		free( fd_info->un_send );
		fd_info->un_send = un_send_char;
		fd_info->un_send_len = new_un_len;
		#ifdef FIRST_NET_DEBUG
		NET_OUT_LOG << "fd:" << fd_info->fd << " 在OUT事件里还没把消息发完" << fin;
		#endif
	}
	else
	{
		fd_info->un_send_len = 0;
		free( fd_info->un_send );
		fd_info->un_send = NULL;
		update_fd_event( fd_info, EPOLL_CTL_MOD, READ_EVENT );
		#ifdef FIRST_NET_DEBUG
		NET_OUT_LOG << "fd:" << fd_info->fd << " 在OUT事件里把数据发送完毕，重新监听IN事件" << fin;
		#endif
	}
}

//创建一个signal_fd
fd_struct_t* FirstPoller::create_signal_fd( sigset_t &mask )
{
	int sfd = signalfd( -1, &mask, 0 );
	if ( -1 == sfd )
	{
		OUT_ERROR << "signalfd() failed!!" << fin;
		assert( sfd >= 0 );
	}
	fd_struct_t *new_signal_fd = create_fd_struct( sfd, FD_TYPE_SIGNAL );
	update_fd_event( new_signal_fd, EPOLL_CTL_ADD, READ_EVENT );
	return new_signal_fd;
}

//更新fd事件
void FirstPoller::update_fd_event( fd_struct_t *fd_info, int op, int new_event )
{
	struct epoll_event ep_ev = {0, {0}};
	ep_ev.data.ptr = fd_info;
	ep_ev.events = new_event;
	epoll_ctl( epoll_fd_, op, fd_info->fd, &ep_ev );
}

//读socket 数据
void FirstPoller::read_socket_data( fd_struct_t *fd_info )
{
	if ( fd_info->status == FD_STATE_CLOSE )
	{
		return;
	}
	int fd = fd_info->fd;

	//栈内数据包
	char STACK_READ_PACKET_DATA[ BIG_PACKET ];
	protocol_packet_t stack_read_packet;
	stack_read_packet.data = STACK_READ_PACKET_DATA;
	stack_read_packet.pos = 0;
	stack_read_packet.max_pos = sizeof( packet_head_t );
	stack_read_packet.pool_size = BIG_PACKET;
	stack_read_packet.is_resize = 0;

	//需要读取的量
	uint32_t need_read = stack_read_packet.max_pos;

	//总共读取的量
	int total_read_bytes = 0;
	protocol_packet_t *read_packet = NULL;

	//有上一次未接收完的数据
	if ( NULL != fd_info->un_read_pack )
	{
		read_packet = fd_info->un_read_pack;
		need_read = read_packet->max_pos - read_packet->pos;
		fd_info->un_read_pack = NULL;
	}
	else
	{
		read_packet = &stack_read_packet;
	}

	while ( true )
	{
		int ret = read( fd, read_packet->data + read_packet->pos, need_read );
		if ( 0 == ret )
		{
			//收到响应事件，没有数据可读
			if ( 0 == total_read_bytes )
			{
				#ifdef FIRST_NET_DEBUG
				NET_OUT_LOG << "连接 " << fd << " 断开（无可读数据）" << fin;
				#endif
				close_fd( fd_info );
			}
			break;
		}
		else if ( ret < 0 )
		{
			//下一次继续读
			if ( errno == EAGAIN  )
			{
				//保存本次读取记录
				if ( NULL == fd_info->un_read_pack )
				{
					//如果已经是一个新生成的packet,直接将这个packet保存
					if ( read_packet != &stack_read_packet )
					{
						fd_info->un_read_pack = read_packet;
					}
					else
					{
						//重新生成一个数据包
						protocol_packet_t *un_read = new_proto_packet( read_packet->max_pos );
						un_read->pos = read_packet->pos;
						un_read->max_pos = read_packet->max_pos;
						memcpy( un_read->data, read_packet->data, read_packet->pos );
						fd_info->un_read_pack = un_read;
					}
				}
				break ;
			}
			else if ( errno == EINTR )
			{
				#ifdef FIRST_NET_DEBUG
				NET_OUT_LOG << "go on read\n" << fin;
				#endif
				continue;
			}
			else		//不知道什么错误
			{
				#ifdef FIRST_NET_DEBUG
				NET_OUT_LOG << "Unkonwn error, errno:" << errno << fin;
				#endif
				break;
			}
		}
		else
		{
			read_packet->pos += ret;
			total_read_bytes += ret;
			//刚好读完指定需要的数据长度
			if ( ret == (int)need_read )
			{
				//刚好读完头信息
				if ( read_packet->pos == sizeof( packet_head_t ) )
				{
					packet_head_t *pack_head = ( packet_head_t* )&read_packet->data[ 0 ];
					need_read = pack_head->size;
					uint32_t new_size = need_read + sizeof( packet_head_t );
					//如果剩余的空间不足
					if ( read_packet->pool_size < new_size )
					{
						//超过最大支持的数据包
						if ( new_size > MAX_READ_PROTOCOL )
						{
							#ifdef YILE_NET_DEBUG
							NET_OUT_LOG << "Special packet!" << fin;
							#endif
							//最多接收SMALL_PACKET这么多字符串
							read_packet->pos += read( fd, read_packet->data + sizeof( packet_head_t ), SMALL_PACKET - sizeof( packet_head_t ) );
							read_packet->max_pos = read_packet->pos;
							//检查是不是可识别的字符串包
							int normal_type = parse_normal_pack( fd_info, read_packet );
							if ( 2 == normal_type )
							{
								poll_handler_->on_http( fd_info, read_packet );
							}
							//尝试内存释放
							try_delete_protocol_packet( read_packet, &stack_read_packet, fd_info );
							//不能被识别的字符串包
							if ( !normal_type )
							{
								OUT_ERROR << "fd:" << fd_info->fd << "Unkown pack!" << fin;
								close_fd( fd_info );
								return;
							}
							break;
						}
						else
						{
							protocol_packet_t *new_pack = new_proto_packet( new_size );
							//拷贝数据
							new_pack->pos = read_packet->pos;
							memcpy( new_pack->data, read_packet->data, read_packet->pos );
							//尝试内存释放
							try_delete_protocol_packet( read_packet, &stack_read_packet, fd_info );
							read_packet = new_pack;
						}
					}
					read_packet->max_pos = new_size;
				}
				//不用读数据 或者 数据读完
				if( 0 == need_read || read_packet->pos > sizeof( packet_head_t ) )
				{
					read_packet->pos = sizeof( packet_head_t );
					poll_handler_->on_socket_fd ( fd_info, read_packet );
					//尝试内存释放
					try_delete_protocol_packet( read_packet, &stack_read_packet, fd_info );
					break;
				}
			}
			else
			{
				need_read -= ret;
			}
		}
	}
}

//关闭连接
void FirstPoller::close_fd( fd_struct_t *fd_info )
{
	#ifdef FIRST_NET_DEBUG
	NET_OUT_LOG << "Close fd:" << fd_info->fd << fin;
	#endif
	set_fd_status( fd_info, FD_STATE_CLOSE );
	CloseList::iterator it = close_list_.find( fd_info->fd );
	if ( it == close_list_.end() )
	{
		close_list_[ fd_info->fd ] = fd_info;
		update_fd_event( fd_info, EPOLL_CTL_DEL, READ_EVENT|WRITE_EVENT );
		poll_handler_->on_close( fd_info );
	}
}
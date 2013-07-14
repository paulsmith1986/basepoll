#ifndef IM_HANDLE_HEAD
#define IM_HANDLE_HEAD
#include <map>
#include "first_poll.h"
#include "poller.h"
#include "im_proxy.h"
#include "poll_handler.h"
#include "task_server.h"

//im proxy
extern ImProxy IM_PROXY_OBJECT;

class ImHandle : public PollHandler
{
	public:
		ImHandle(){}
		void on_socket_fd( fd_struct_t *fd_info, protocol_packet_t *read_packet );
		void on_signal_fd( fd_struct_t *fd_info );
		void on_close( fd_struct_t *fd_info );
	private:
		void shut_down( int signal_no );
};
#endif
#ifndef DEMO_HANDLE_HEAD
#define DEMO_HANDLE_HEAD
#include "poller.h"
#include "poll_handler.h"
#include "task_server.h"

class DemoHandle : public PollHandler
{
	public:
		void on_socket_fd( fd_struct_t *fd_info, protocol_packet_t *read_packet );
		void on_signal_fd( fd_struct_t *fd_info );
		void on_close( fd_struct_t *fd_info );
		bool check_pack_length( fd_struct_t *fd_info, protocol_packet_t *pack );
	private:
		void shut_down( int signal_no );
		bool is_valid_packet( fd_struct_t *fd_info, protocol_packet_t *read_packet );
};
#endif
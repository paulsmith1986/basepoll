#ifndef IM_PROXY_H
#define IM_PROXY_H
#include <vector>
#include <map>
#include "first_poll.h"
#include "poller.h"
#include "first_protocol.h"
class ImProxy
{
	public:
		ImProxy()
		: proxy_index_( 0 )
		{}
		~ImProxy(){}
		void proxy( protocol_packet_t *pack_data );
		void add( fd_struct_t *fd_info );
	private:
		typedef std::map<int, time_t> im_proxy_map_t;
		typedef std::vector<fd_struct_t*> im_proxy_list_t;
		size_t proxy_index_;
		im_proxy_map_t im_proxy_map_;
		im_proxy_list_t im_proxy_list_;
};
#endif
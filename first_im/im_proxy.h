#ifndef IM_PROXY_H
#define IM_PROXY_H
#include "im_common.h"
class ImProxy
{
	public:
		ImProxy()
		: proxy_index_( 0 )
		{}
		~ImProxy(){
			im_proxy_list_.clear();
		}
		void proxy( protocol_result_t *pack_data );
		void proxy( protocol_packet_t *pack_data );
		void add( fd_struct_t *fd_info );
		void check_is_php_fd( fd_struct_t *fd_info );
		fd_struct_t *get_proxy_fd();
	private:
		typedef std::vector<fd_struct_t*> im_proxy_list_t;
		size_t proxy_index_;
		im_proxy_list_t im_proxy_list_;
};
#endif
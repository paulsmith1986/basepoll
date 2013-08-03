#ifndef IM_PROXY_H
#define IM_PROXY_H
#include "im_common.h"
class ImProxy
{
	public:
		ImProxy(){}
		~ImProxy(){
			im_proxy_list_.clear();
		}
		void proxy( protocol_result_t *pack_data, uint32_t hash_id );
		void proxy( protocol_packet_t *pack_data, uint32_t hash_id );
		void add( fd_struct_t *fd_info );
		void check_is_php_fd( fd_struct_t *fd_info );
	private:
		typedef std::vector<fd_struct_t*> im_proxy_list_t;
		inline fd_struct_t *get_proxy_fd( uint32_t hash_id );
		im_proxy_list_t im_proxy_list_;
};
#endif
#ifndef IM_PROXY_H
#define IM_PROXY_H
#include "im_common.h"
class ImProxy
{
	public:
		ImProxy()
		: proxy_index_( 0 ),
		anonymity_index_( 0 )
		{}
		~ImProxy(){
			im_proxy_map_.clear();
			im_proxy_list_.clear();
			im_proxy_list_.clear();
		}
		void proxy( protocol_result_t *pack_data );
		void add( fd_struct_t *fd_info );
		void check_is_php_fd( fd_struct_t *fd_info );
		bool check_anonymity_fd( int fd, int fd_id );
		uint16_t add_anonymity( int fd );
	private:
		typedef std::map<int, time_t> im_proxy_map_t;
		typedef std::vector<fd_struct_t*> im_proxy_list_t;
		//匿名连接列表
		typedef std::map<int, uint16_t>anonymity_list_t;
		size_t proxy_index_;
		uint16_t anonymity_index_;
		anonymity_list_t anonymity_list_;
		im_proxy_map_t im_proxy_map_;
		im_proxy_list_t im_proxy_list_;
};
#endif
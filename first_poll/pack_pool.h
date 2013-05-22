#ifndef DATA_PACK_POOL_H
#define DATA_PACK_POOL_H
#include <assert.h>
#include <list>
class dataPacketPool
{
	public:
		dataPacketPool( int pool_size, int size )
		:mutex_()
		{
			pool_size_ = pool_size;
			data_size_ = size;
		}
		//获取
		inline protocol_packet_t *get( )
		{
			protocol_packet_t *re;
			{
				MutexLockGuard lock( mutex_ );
				//没有元素
				if ( 0 == pool_data_.size() )
				{
					re = ( protocol_packet_t* )malloc( sizeof( protocol_packet_t ) );
					re->data = (char*)malloc( data_size_ );
					assert( NULL != re );
				}
				else
				{
					re = pool_data_.front();
					pool_data_.pop_front();
				}
			}
			return re;
		}
		inline void recycle( protocol_packet_t* tmp )
		{
			assert( NULL != tmp );
			MutexLockGuard lock( mutex_ );
			{
				if ( pool_data_.size() >= pool_size_ )
				{
					free( tmp->data );
					free( tmp );
				}
				else
				{
					pool_data_.push_back( tmp );
				}
			}
		}
		int pool_size()
		{
			MutexLockGuard lock( mutex_ );
			return pool_data_.size();
		}
	private:
		mutable MutexLock mutex_;
		int data_size_;
		uint32_t pool_size_;
		std::list<protocol_packet_t*> pool_data_;		//数据池
};
#endif
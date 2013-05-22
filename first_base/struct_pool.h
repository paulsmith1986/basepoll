#ifndef FIRST_STRUCT_POOL_H
#define FIRST_STRUCT_POOL_H
#include <assert.h>
#include <list>
template<typename T>
class StructPool
{
	public:
		StructPool( int pool_size, int each_size )
		{
			pool_data_.resize( 0 );
			pool_size_ = pool_size;
			each_size_ = each_size;
		}
		//获取
		inline T get()
		{
			T re;
			//没有元素
			if ( 0 == pool_data_.size() )
			{
				re = ( T )malloc( each_size_ );
				assert( NULL != re );
			}
			else
			{
				re = pool_data_.front();
				pool_data_.pop_front();
			}
			return re;
		}
		inline void recycle( T tmp )
		{
			if ( pool_data_.size() >= pool_size_ )
			{
				free( tmp );
			}
			else
			{
				pool_data_.push_back( tmp );
			}
		}
		int pool_size()
		{
			return pool_data_.size();
		}
	private:
		int each_size_;					//每一个的大小
		uint32_t pool_size_;			//池大小
		std::list<T> pool_data_;		//数据池
};
#endif
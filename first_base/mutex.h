#ifndef FIRST_MUTEX_LOCK_HEAD
#define FIRST_MUTEX_LOCK_HEAD
#include <pthread.h>
using namespace std;

class MutexLock
{
	public:
		MutexLock()
		{
			pthread_mutex_init( &mutex_, NULL );
		}

		~MutexLock()
		{
			pthread_mutex_destroy( &mutex_ );
		}

		void lock()  // 程序一般不主动调用
		{
			pthread_mutex_lock( &mutex_ );
		}

		void unlock()  // 程序一般不主动调用
		{
			pthread_mutex_unlock( &mutex_ );
		}

		pthread_mutex_t* getPthreadMutex()  // 仅供 Condition 调用，严禁自己调用
		{
			return &mutex_;
		}

		private:
			pthread_mutex_t mutex_;
};
class MutexLockGuard
{
	public:
		explicit MutexLockGuard( MutexLock& mutex ) : mutex_( mutex )
		{
			mutex_.lock();
		}
		 ~MutexLockGuard()
		{
			mutex_.unlock();
		}

	private:
		MutexLock &mutex_;
};

#define MutexLockGuard(x) static_assert( false, "missing mutex guard var name" )

#endif
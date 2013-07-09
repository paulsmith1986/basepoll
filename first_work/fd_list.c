#include "fd_list.h"
//fd list的大小
int FIRST_POOL_FD_SIZE = 0;
//fd list的大小
int FIRST_POOL_FD_MAX_SIZE = FIRST_POLL_INIT_SIZE;
//存放fd_list
first_poll_struct_t **FIRST_POOL_FD_LIST = NULL;
/**
 * 初始化fd_list
 */
void first_poll_init_list()
{
	FIRST_POOL_FD_LIST = first_poll_malloc_list( FIRST_POOL_FD_MAX_SIZE );
}

/**
 * 分配内存
 */
first_poll_struct_t **first_poll_malloc_list( int list_size )
{
	first_poll_struct_t **re_list = (first_poll_struct_t**)malloc( sizeof( first_poll_struct_t*) * list_size );
	memset( re_list, 0, sizeof( first_poll_struct_t*) * list_size );
	return re_list;
}

/**
 * 根据fd查找fd_struct
 */
first_poll_struct_t *find_fd_info( int fd )
{
	int index = fd % FIRST_POOL_FD_MAX_SIZE;
	if ( NULL == FIRST_POOL_FD_LIST[ index ] )
	{
		return NULL;
	}
	if ( FIRST_POOL_FD_LIST[ index ]->fd == fd )
	{
		return FIRST_POOL_FD_LIST[ index ];
	}
	else
	{
		first_poll_struct_t *tmp_info = FIRST_POOL_FD_LIST[ index ]->next;
		while ( NULL != tmp_info )
		{
			if ( tmp_info->fd == fd )
			{
				break;
			}
			tmp_info = tmp_info->next;
		}
		return tmp_info;
	}
}

/**
 * 将fd_info加入fd_list
 */
void first_poll_list_add( first_poll_struct_t *fd_info )
{
	//如果桶满，扩容
	if ( FIRST_POOL_FD_SIZE + 1 >= FIRST_POOL_FD_MAX_SIZE )
	{
		first_poll_init_resize( FIRST_POOL_FD_MAX_SIZE * 2 );
	}
	int index = fd_info->fd % FIRST_POOL_FD_MAX_SIZE;
	if ( NULL == FIRST_POOL_FD_LIST[ index ] )
	{
		FIRST_POOL_FD_LIST[ index ] = fd_info;
	}
	else
	{
		fd_info->next = FIRST_POOL_FD_LIST[ index ];
		FIRST_POOL_FD_LIST[ index ] = fd_info;
	}
	++FIRST_POOL_FD_SIZE;
}

/**
 * 从fd_list移出
 */
void first_pool_list_remove( int fd )
{
	int index = fd % FIRST_POOL_FD_MAX_SIZE;
	if ( NULL == FIRST_POOL_FD_LIST[ index ] )
	{
		return;
	}
	first_poll_struct_t *fd_info = FIRST_POOL_FD_LIST[ index ];
	if ( fd_info->fd == fd )
	{
		FIRST_POOL_FD_LIST[ index ] = fd_info->next;
		--FIRST_POOL_FD_SIZE;
	}
	else
	{
		first_poll_struct_t *next_fd = fd_info->next;
		while ( NULL != next_fd )
		{
			if ( next_fd->fd == fd )
			{
				fd_info->next = next_fd->next;
				--FIRST_POOL_FD_SIZE;
				break;
			}
			fd_info = next_fd;
			next_fd = next_fd->next;
		}
	}
}

/**
 * 重新设置list的大小
 */
void first_poll_init_resize( int new_size )
{
	first_poll_struct_t **new_list_poll = first_poll_malloc_list( new_size );
	int i;
	for ( i = 0; i < FIRST_POOL_FD_MAX_SIZE; ++i )
	{
		if ( NULL == FIRST_POOL_FD_LIST[ i ] )
		{
			continue;
		}
		first_poll_struct_t *tmp_info = FIRST_POOL_FD_LIST[ i ];
		while ( NULL != tmp_info )
		{
			first_poll_struct_t *next = tmp_info->next;
			int index = tmp_info->fd % new_size;
			if ( NULL == new_list_poll[ index ] )
			{
				new_list_poll[ index ] = tmp_info;
				tmp_info->next = NULL;
			}
			else
			{
				tmp_info->next = new_list_poll[ index ];
				new_list_poll[ index ] = tmp_info;
			}
			tmp_info = next;
		}
	}
	free( FIRST_POOL_FD_LIST );
	FIRST_POOL_FD_MAX_SIZE = new_size;
	FIRST_POOL_FD_LIST = new_list_poll;
}
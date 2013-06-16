#ifndef FIRST_FD_LIST_H
#define FIRST_FD_LIST_H
#include "first_poll.h"
//fd list的大小
extern int FIRST_POOL_FD_SIZE;
//fd list的大小
extern int FIRST_POOL_FD_MAX_SIZE;
//存放fd_list
extern first_poll_struct_t **FIRST_POOL_FD_LIST;

/**
 * 初始化fd_list
 */
void first_poll_init_list();

/**
 * 重新设置list的大小
 */
void first_poll_init_resize( int new_size );

/**
 * 根据fd查找fd_struct
 */
first_poll_struct_t *find_fd_info( int fd );

/**
 * 将fd_info加入fd_list
 */
void first_poll_list_add( first_poll_struct_t *fd_info );

/**
 * 分配内存
 */
first_poll_struct_t **first_poll_malloc_list( int list_size );

/**
 * 从fd_list移出
 */
void first_pool_list_remove( int fd );
#endif
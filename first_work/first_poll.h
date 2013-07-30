#ifndef FIRST_POOL_H
#define FIRST_POOL_H
#include <signal.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "first.h"
#include "proto_c.h"
#include "proto_size.h"
#include "first_protocol.h"
#define MAX_LOOP_TIMEOUT 16 * 1000							//事件等待时间
#define FIRST_POLL_MAX_EVENT 32								//同时事件个数
#define FIRST_POLL_INIT_SIZE 8								//初始化list大小
#define MAX_READ_DATA 1024 * 64								//缓冲区
#define NET_SEND_CACHE 1024 * 64							//缓冲区
#define BIG_PACKET 1024 * 32								//大数据包
#define READ_EVENT	EPOLLIN|EPOLLRDHUP						//读事件
#define WRITE_EVENT EPOLLOUT|EPOLLRDHUP						//写事件
#define MAX_NORMAL_FD 5										//普通socket连接个数

#define check_epoll_init() if ( -1 == MAIN_POOL_FD ) first_create_poll()

//发送协议包
#define send_protocol_pack( fd, pack ) first_send_data( fd, pack.str, pack.pos )
static int MAIN_POOL_FD = -1;
typedef enum first_poll_type first_poll_type;
enum first_poll_type {
	FD_TYPE_LISTEN,		//主监听fd
	FD_TYPE_SOCKET,		//socket连接
	FD_TYPE_EVENT,		//事件fd
	FD_TYPE_TIMER,		//时间fd
	FD_TYPE_SIGNAL		//信号fd
};

typedef struct first_poll_struct_t first_poll_struct_t;
struct first_poll_struct_t{
	int						fd;								//连接
	int						un_send_len;					//未发出消息长度(写)
	first_poll_type			fd_type;						//连接类型
	char*					un_send;						//未发出消息内容
	protocol_packet_t*		un_read_pack;					//未处理完的数据包
	int						is_return;						//是否返回到php
	first_poll_struct_t*	next;								//用于查找
};

//关闭socket list
extern first_poll_struct_t *CLOSE_FD_LIST;

//存放普通的socket连接(不受first_poll管理)
extern int normal_socket_list[];
/**
 * 初始化poll
 */
void first_create_poll();

/**
 * 将一个fd加入到epoll
 */
void first_poll_add( int fd, first_poll_type fd_type );

/**
 * 更新一个fd的事件
 */
void first_update_event( first_poll_struct_t *fd_info, int op, int new_event );

/**
 * 创建连接过程
 */
int first_socket_connect( const char *host, int port );

/**
 * 设置为非阻塞状态函数
 */
void set_non_block( const int sock );

/**
 * 关闭连接
 */
void first_close_fd( first_poll_struct_t *fd_info );

/**
 * 闭关真正fd
 */
void check_fd_close_list();

/**
 * 加入普通socket
 */
void add_normal_socket( int fd );

/**
 * 移除普通socket
 */
void remove_normal_socket( int fd );

/**
 * 判断是否是普通的fd
 */
int is_normal_socket( int fd );

/**
 * 初始化普通socket连接列表
 */
void init_normal_socket_list();

/**
 * 发送数据
 */
void first_send_data( first_poll_struct_t *fd_info, void *send_re, uint32_t total_send );

/**
 * 读数据包
 */
void read_socket_data( first_poll_struct_t *fd_info, zval *tmp_result );

/**
 * 回收数据包
 */
void delete_protocol_packet( protocol_packet_t *tmp_pack );

/**
 * 创建协议数据包
 */
protocol_packet_t *new_proto_packet( uint32_t data_len );

/**
 * 转发数据包
 */
void first_socket_proxy(  first_poll_struct_t *fd_info, protocol_packet_t *data_pack, zval *tmp_result );

/**
 * 写事件
 */
void first_on_socket_write( first_poll_struct_t *fd_info );

/**
 * 送数据包
 */
void normal_send_data( int fd, char *send_data, uint32_t total_len );

#endif
#ifndef FIRST_CLIENT_H
#define	FIRST_CLIENT_H
#include <sys/socket.h>								//scoket类
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include "first_protocol.h"
#define NET_SEND_CACHE		1024 * 64							//发送缓冲区
//读取数据出错
#define read_data_error( read_byte ) read_byte->pos = read_byte->max_pos = 0

/**
 * 连接服务器
 */
int first_net_connect( char* host, int port );

/**
 * 发送数据包( 注意：数据发送完后，如果传入包有过重新申请内存，数据会被释放 )
 */
void protocol_send_pack( int fd, protocol_result_t *send_pack );

/**
 * 接收数据
 */
void protocol_recv_pack( int fd, protocol_packet_t *read_byte_array );

#endif


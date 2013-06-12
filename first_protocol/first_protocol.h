#ifndef FIRST_PROTOCOL_H
#define	FIRST_PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_LIST_RECURSION			128					//最多printf缩进
#define MAX_READ_PACK_SIZE			1024 * 64			//最大支持的数据包大小
#define PROTOCOL_DATA_LEN			1024 * 16			//原始包大小
#define PROTO_ERROR_OVERFLOW		1					//结果溢出
#define PROTO_ERROR_SIZEERROR		2					//包大小出错
#define PROTO_ERROR_TOOBIG_DATA		3					//包数据太大
#define PROTO_ERROR_SEND_FAIL		4					//数据包发送出错
#define PROTO_READ_NET_DATA_ERROR	5					//读数据出错
#define PROTO_READ_PACK_ID_ERROR	6					//读到的数据包id不是想要的数据包id
#define PROTO_PACK_ERROR			7					//打包数据的时候出错

#define MOD_ADLER					65521				//用于adler32校验

typedef struct fd_struct_t fd_struct_t;

//协议传输字符串的长度定义
typedef	uint16_t string_len_t;
//协议传输字节流的长度定义
typedef	uint16_t bytes_len_t;

//网络传输形式内存块
typedef struct protocol_packet_t protocol_packet_t;
struct protocol_packet_t{
	uint32_t				pos;							//当前取数据的位置
	uint32_t				max_pos;						//最大可读取的位置
	uint32_t				pool_size;						//数据池空间
	int						is_resize;						//是否重新分配过内存
	char*					data;							//数据指针
};
//反解数据的内存块
typedef struct protocol_result_t protocol_result_t;
struct protocol_result_t{
	char			*str;									//结果
	uint32_t		pos;									//当前位置
	uint32_t		max_pos;								//最大位置
	int				is_resize;								//是否有改变大小
	int				error_code;								//错误类型
};

//字节流struct
typedef struct proto_bin_t proto_bin_t;
struct proto_bin_t{
	bytes_len_t		len;									//字节流长度
	char*			bytes;									//字节流
};

#pragma pack(2)
//发数据时候的数据头
typedef struct packet_head_t packet_head_t;
struct packet_head_t{
	uint32_t				size;							//数据大小
	uint16_t				pack_id;						//包类型
};
#pragma pack()

//解析协议数据时有用
#define add_data_pool_size( re_pack, data_pool, add_len )							\
	if ( data_pool->pos + add_len > data_pool->max_pos )							\
	{																				\
		data_pool->error_code = PROTO_ERROR_TOOBIG_DATA;							\
		data_pool->pos = 0;															\
	}																				\
	else																			\
	{																				\
		data_pool->pos += add_len;													\
	}


//将结果的一段拷出
#define result_copy( re_pack, des, len, result_pool )								\
	if ( re_pack->pos + len > re_pack->max_pos )									\
	{																				\
		result_pool->error_code = PROTO_ERROR_OVERFLOW;								\
		re_pack->pos += len;														\
		return NULL;																\
	}																				\
	memcpy( des, re_pack->data + re_pack->pos, len );								\
	re_pack->pos += len

//服务器端数据异常处理
#define catch_error_packet( byte_pack, fd_info, error_code )
//客户端数据异常处理
#define recv_error_packet( byte_pack, error_code )


/**
 * adler32效验
 */
uint32_t adler32( unsigned char *data, int32_t len );

/**
 * 往结果集写字节流
 */
void write_bytes( protocol_result_t *re_pack, proto_bin_t *data );

//往结果集写数字串
void write_UTF( protocol_result_t *re_pack, char *re_str );

//从结果集中读字符串
char *read_UTF( protocol_packet_t *re_pack, protocol_result_t *data_pool );

/**
 * 存结果
 * @param	result		结果包指针
 * @param	data		数据
 * @param	len			数据长度
 */
void first_result_push_data( protocol_result_t *result, void *data, int len );

/**
 * 从结果里读出一段
 */
proto_bin_t *read_bytes( protocol_packet_t *re_pack, proto_bin_t *data, protocol_result_t *data_pool );

/**
 * 从结果中读取出定长字符串
 */
char *read_fix_char( protocol_packet_t *re_pack, protocol_result_t *data_pool, string_len_t str_len );

/**
 * 将一个定长字符串压入结果
 * @param	data		PHP变量指针
 * @param	result		结果指针
 * @param	int			长度
 */
void write_fix_char( char *data, char *result, int len );

/**
 * 测试代码..打印协议数据
 */
void first_printf_tab_string( char *re_char, int rank );

/**
 * 数据包扩容
 */
void protocol_packet_resize( protocol_packet_t *tmp_pack, uint32_t new_size );

/**
 * bin转hex
 */
unsigned char *first_bin2hex( const unsigned char *bin_char, int bin_len );

/**
 * hex转bin
 */
char *first_hex2bin( const unsigned char *hex_str, int hex_len, int *new_len );

#endif	/* FIRST_PROTOCOL_H */


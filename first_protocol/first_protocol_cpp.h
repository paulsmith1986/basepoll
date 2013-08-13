#ifndef FIRST_PROTOCOL_CPP_H
#define	FIRST_PROTOCOL_CPP_H
#include <vector>
#include <string>
#include "first_protocol.h"

/**
 * 写入string
 */
void proto_write_string( protocol_result_t *result, std::string &str );

//解析协议数据时有用
#define cpp_add_pack_data_len( data_pool, add_len )									\
	if ( data_pool->pos + add_len > data_pool->max_pos )							\
	{																				\
		throw PROTO_ERROR_TOOBIG_DATA;												\
		data_pool->pos = 0;															\
	}																				\
	else																			\
	{																				\
		data_pool->pos += add_len;													\
	}


//将结果的一段拷出
#define cpp_result_copy( re_pack, des, len )										\
	if ( re_pack->pos + len > re_pack->max_pos )									\
	{																				\
		throw PROTO_ERROR_OVERFLOW;													\
		re_pack->pos += len;														\
		return;																		\
	}																				\
	memcpy( des, re_pack->data + re_pack->pos, len );								\
	re_pack->pos += len
#endif


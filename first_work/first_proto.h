#ifndef FIRST_NEW_PROTOCOL_H
#define FIRST_NEW_PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "php.h"
#include "first_protocol.h"

//读数字
#define read_int_from_hash( obj, k )															\
	if( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if( IS_LONG != Z_TYPE_PP( tmp_data ) )													\
		{																						\
			convert_to_long( *tmp_data );														\
		}																						\
		obj.k = Z_LVAL_PP( tmp_data );															\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//读数字
#define read_int_from_hash_var( obj, var, k )													\
	if( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if( IS_LONG != Z_TYPE_PP( tmp_data ) )													\
		{																						\
			convert_to_long( *tmp_data );														\
		}																						\
		var = Z_LVAL_PP( tmp_data );															\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//读数字
#define read_int_from_zval( zval, var )															\
		if( IS_LONG != Z_TYPE_PP( zval ) )														\
		{																						\
			convert_to_long( *zval );															\
		}																						\
		var = Z_LVAL_PP( zval )																	\

//读字符串
#define read_string_from_zval( zval )															\
		if( IS_STRING != Z_TYPE_PP( zval ) )													\
		{																						\
			convert_to_string( *zval );															\
		}																						\
		write_UTF( all_result, Z_STRVAL_PP( zval ) )

//读固定长度字符串
#define read_fix_string_from_zval( zval, result, len )											\
		if( IS_STRING != Z_TYPE_PP( zval ) )													\
		{																						\
			convert_to_string( *zval );															\
		}																						\
		write_fix_char( Z_STRVAL_PP( zval ), result, len )

//读字符串
#define read_string_from_hash( obj, k )															\
	if( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if( IS_STRING != Z_TYPE_PP( tmp_data ) )												\
		{																						\
			convert_to_string( *tmp_data );														\
		}																						\
		write_UTF( all_result, Z_STRVAL_PP( tmp_data ) );										\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//读定长字符
#define read_fixchar_from_hash( obj, k, result, len )											\
	if( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if( IS_STRING != Z_TYPE_PP( tmp_data ) )												\
		{																						\
			convert_to_string( *tmp_data );														\
		}																						\
		write_fix_char( Z_STRVAL_PP( tmp_data ), result, len );									\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//读字节流
#define read_bytes_from_hash( obj, k )															\
	if( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if( IS_STRING != Z_TYPE_PP( tmp_data ) )												\
		{																						\
			convert_to_string( *tmp_data );														\
		}																						\
		php_write_bytes( Z_STRVAL_PP( tmp_data ), all_result, Z_STRLEN_PP( tmp_data ) );		\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//读字节流
#define read_bytes_from_zval( zval )															\
		if( IS_STRING != Z_TYPE_PP( zval ) )													\
		{																						\
			convert_to_string( *zval );															\
		}																						\
		php_write_bytes( Z_STRVAL_PP( zval ), all_result, Z_STRLEN_PP )

//读其它struct
#define read_struct_from_hash( obj, k )															\
	if ( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if ( IS_ARRAY != Z_TYPE_PP( tmp_data ) )												\
		{																						\
			first_protocol_error( #obj, #k, 2 );												\
			all_result->error_code = PROTO_PACK_ERROR;											\
			return;																				\
		}																						\
		new_struct_hash = Z_ARRVAL_P( *tmp_data );												\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//读list
#define read_list_from_hash( obj, k )															\
	if ( SUCCESS == zend_hash_find( data_hash, #k, sizeof( #k ), ( void ** )&tmp_data ) )		\
	{																							\
		if ( IS_ARRAY != Z_TYPE_PP( tmp_data ) )												\
		{																						\
			first_protocol_error( #obj, #k, 2 );												\
			all_result->error_code = PROTO_PACK_ERROR;											\
			return;																				\
		}																						\
		new_list_hash = Z_ARRVAL_P( *tmp_data );												\
	}																							\
	else																						\
	{																							\
		first_protocol_error( #obj, #k, 1 );													\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

//判断是否是数组
#define protocol_list_is_array( z_item, list_name )												\
	if( IS_ARRAY != Z_TYPE_PP( z_item ) )														\
	{																							\
		first_protocol_error( NULL, list_name, 3 );												\
		all_result->error_code = PROTO_PACK_ERROR;												\
		return;																					\
	}

#define set_data_pointer( pack, size, pointer, type )											\
	if ( pack->pos + size > pack->max_pos )														\
	{																							\
		parse_data_error( #type );																\
	}																							\
	pointer = ( type* )&pack->data[ pack->pos ];												\
	pack->pos += size

#define php_result_copy( re_pack, des, len )													\
	if ( re_pack->pos + len > re_pack->max_pos )												\
	{																							\
		re_pack->pos = 0;																		\
		return;																					\
	}																							\
	memcpy( des, re_pack->data + re_pack->pos, len );											\
	re_pack->pos += len


//php数组for循环
#define first_loop_arr( arr, point, item ) zend_hash_internal_pointer_reset_ex( arr, &point ); zend_hash_get_current_data_ex( arr, (void**) &item, &point ) == SUCCESS; zend_hash_move_forward_ex( arr, &point )

/**
 * 解析数据时出错
 */
void parse_data_error( char *type );

/**
 * 报错
 */
void first_protocol_error( char *proto, char *key_name, int type );

/**
 * 将字节流写入result_pack
 */
void php_write_bytes( char *str, protocol_result_t *re_pack, int len );

#endif
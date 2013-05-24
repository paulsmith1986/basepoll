#include "first_protocol.h"
/**
 * adler32效验
 */
uint32_t adler32( unsigned char *data, int32_t len )
{
    uint32_t a = 1, b = 0;
    int32_t index;

    /* Process each byte of the data in order */
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    return (b << 16) | a;
}

/**
 * 存结果
 * @param	result		结果包指针
 * @param	data		数据
 * @param	len			数据长度
 */
void first_result_push_data( protocol_result_t *result, void *data, int len )
{
	if ( result->pos + len > result->max_pos )
	{
		char *new_result = ( char* )malloc( result->max_pos + PROTOCOL_DATA_LEN );
		memcpy( new_result, result->str, result->pos );
		if ( result->is_resize )
		{
			free( result->str );
		}
		result->is_resize = 1;
		result->max_pos += PROTOCOL_DATA_LEN;
		result->str = new_result;
	}
	if ( NULL != data )
	{
		memcpy( result->str + result->pos, data, len );
	}
	result->pos += len;
}

//往结果集写数字串
void write_UTF( protocol_result_t *re_pack, char *re_str )
{
	string_len_t str_len = strlen( re_str );
	first_result_push_data( re_pack, &str_len, sizeof( string_len_t ) );
	if ( str_len > 0 )
	{
		first_result_push_data( re_pack, re_str, str_len );
	}
}

/**
 * 往结果集写字节流
 */
void write_bytes( protocol_result_t *re_pack, proto_bin_t *data )
{
	first_result_push_data( re_pack, &data->len, sizeof( data->len ) );
	first_result_push_data( re_pack, data->bytes, data->len );
}

/**
 * 从结果里读出一段
 */
proto_bin_t *read_bytes( protocol_packet_t *re_pack, proto_bin_t *data, protocol_result_t *data_pool )
{
	if ( NULL == data )
	{
		data = ( proto_bin_t* )&data_pool->str[ data_pool->pos ];
		if ( data_pool->pos + sizeof( proto_bin_t ) > data_pool->max_pos )
		{
			data_pool->error_code = PROTO_ERROR_OVERFLOW;
			return NULL;
		}
		data_pool->pos += sizeof( proto_bin_t );
	}
	result_copy( re_pack, &data->len, sizeof( data->len ), data_pool );
	data->bytes = &data_pool->str[ data_pool->pos ];
	result_copy( re_pack, data_pool->str + data_pool->pos, data->len, data_pool );
	add_data_pool_size( re_pack, data_pool, data->len );
	return data;
}

/**
 * 从结果中读取出字符串
 */
char *read_UTF( protocol_packet_t *re_pack, protocol_result_t *data_pool )
{
	string_len_t str_len;
	result_copy( re_pack, &str_len, sizeof( string_len_t ), data_pool );
	char *re_char = &data_pool->str[ data_pool->pos ];
	if ( str_len > 0 )
	{
		result_copy( re_pack, &data_pool->str[ data_pool->pos ], str_len, data_pool );
	}
	add_data_pool_size( re_pack, data_pool, str_len + 1 );
	re_char[ str_len ] = '\0';
	return re_char;
}

/**
 * 从结果中读取出定长字符串
 */
char *read_fix_char( protocol_packet_t *re_pack, protocol_result_t *data_pool, string_len_t str_len )
{
	char *re_char = data_pool->str + data_pool->pos;
	add_data_pool_size( re_pack, data_pool, str_len );
	result_copy( re_pack, data_pool->str + data_pool->pos, str_len, data_pool );
	data_pool->pos += str_len;
	return re_char;
}

/**
 * 将一个定长字符串压入结果
 * @param	data		PHP变量指针
 * @param	result		结果指针
 * @param	int			长度
 */
void write_fix_char( char *data, char *result, int len )
{
	string_len_t str_len = strlen( data );
	if ( str_len >= len )
	{
		str_len = len - 1;
	}
	memcpy( result, data, str_len );
	result[ str_len ] = '\0';
}

/**
 * 测试代码..打印协议数据
 */
void first_printf_tab_string( char *re_char, int rank )
{
	if ( rank > MAX_LIST_RECURSION )
	{
		rank = 1;
	}
	int len = rank * 4;
	int i;
	for ( i = 0; i < len; i += 4 )
	{
		re_char[ i ] = ' ';
		re_char[ i + 1 ] = ' ';
		re_char[ i + 2 ] = ' ';
		re_char[ i + 3 ] = ' ';
	}
	re_char[ len ] = '\0';
}

/**
 * 数据包扩容
 */
void protocol_packet_resize( protocol_packet_t *tmp_pack, uint32_t new_size )
{
	char *new_data = ( char* )malloc( new_size );
	memcpy( new_data, tmp_pack->data, tmp_pack->pos );
	if ( tmp_pack->is_resize )
	{
		free( tmp_pack->data );
	}
	tmp_pack->is_resize = 1;
	tmp_pack->max_pos = new_size;
}

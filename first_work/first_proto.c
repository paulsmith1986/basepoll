#include "first_proto.h"
/**
 * 报错
 */
void first_protocol_error( char *proto, char *key_name, int type )
{
	switch ( type )
	{
		case 1:
			php_error( E_WARNING, "`%s` has no key: %s\n", proto, key_name );
		break;
		case 2:
			php_error( E_WARNING, "`%s` key: %s must be array!\n", proto, key_name );
		break;
		case 3:
			php_error( E_WARNING, "List: %s must be array!\n", key_name );
		break;
	}
}

/**
 * 解析数据时出错
 */
void parse_data_error( char *type )
{
	php_error( E_ERROR, "Parse data: %s error, overflow!\n", type );
}

/**
 * 将字节流写入result_pack
 */
void php_write_bytes( char *str, protocol_result_t *re_pack, int len )
{
	bytes_len_t bin_len = len;
	//溢出判断
	if ( bin_len != len )
	{
		php_error( E_WARNING, "byte数据长度超过协议最大支持量!\n" );
		bin_len = 0;
	}
	first_result_push_data( re_pack, &bin_len, sizeof( bin_len ) );
	if ( bin_len > 0 )
	{
		first_result_push_data( re_pack, str, bin_len );
	}
}
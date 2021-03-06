#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "first.h"
#include "fd_list.h"
#include "so_decode.h"
#include "so_encode.h"
static int le_first_work;

const zend_function_entry first_work_functions[] = {
	PHP_FE( first, NULL )
	PHP_FE( first_poll, NULL )
	PHP_FE( first_host, NULL )
	PHP_FE( first_timer_fd, NULL )
	PHP_FE( first_event_fd, NULL )
	PHP_FE( first_set_timeout, NULL )
	PHP_FE( first_signal_fd, NULL )
	PHP_FE( first_socket_fd, NULL )
	PHP_FE( first_send_pack, NULL )
	PHP_FE( first_send_data, NULL )
	PHP_FE( first_proxy_unpack, NULL )
	PHP_FE( first_pack, NULL )
	PHP_FE( first_unpack, NULL )
	PHP_FE( first_getpid, NULL )
	PHP_FE( first_fork, NULL )
	PHP_FE( first_setsid, NULL )
	PHP_FE( first_kill, NULL )
	PHP_FE( first_close_fd, NULL )
	PHP_FE( first_set_fpm_type, NULL )

	PHP_FE( is_binary, NULL )
	PHP_FE( str_to_array, NULL )
	{NULL, NULL, NULL}	/* Must be the last line in first_work_functions[] */
};

zend_module_entry first_work_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"first_work",
	first_work_functions,
	PHP_MINIT(first_work),
	PHP_MSHUTDOWN(first_work),
	PHP_RINIT(first_work),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(first_work),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(first_work),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_FIRST_WORK
ZEND_GET_MODULE(first_work)
#endif


PHP_MINIT_FUNCTION(first_work)
{
	first_poll_init_list();
	init_normal_socket_list();
	REGISTER_LONG_CONSTANT( "FIRST_NEW_CONNECTION", 1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SOCKET_CLOSE", 2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SOCKET_DATA", 3, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SOCKET_WRITE", 4, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_EVENT_WAKEUP", 5, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_TIME_UP", 6, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGNAL", 7, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGHUP", 1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGINT", 2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGPIPE", 13, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGTERM", 15, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGUSR1", 10, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGUSR2", 12, CONST_CS | CONST_PERSISTENT);
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(first_work)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(first_work)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(first_work)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(first_work)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "first_work support", "enabled");
	php_info_print_table_end();
}

PHP_FUNCTION(first)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "first_work", arg);
	RETURN_STRINGL(strg, len, 0);
}
/**
 * 判断是不是二进制串
 */
PHP_FUNCTION( is_binary )
{
	char *str;
	int str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == FAILURE )
	{
		return;
	}
	if ( strlen( str ) == str_len )
	{
		RETURN_FALSE;
	}
	else
	{
		RETURN_TRUE;
	}
}

/**
 * 分隔字符串
 */
PHP_FUNCTION( str_to_array )
{
	char *str;
	char *main_delim = ",";
	char *sub_delim = ":";
	int len1 = 0,len2 = 0,len3 = 0;
	int argc = ZEND_NUM_ARGS();
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s|ss", &str, &len1, &main_delim, &len2, &sub_delim, &len3 ) == FAILURE )
	{
		return;
	}
	if ( ( 0 != len2 && 1 != len2 ) || ( 0 != len3 && 1 != len3 ) )
	{
		zend_error( E_ERROR, "str_to_array 分割符参数出错" );
	}
	array_init( return_value );
	char *tmp_str;
	char tmp_value[ 101 ];
	if ( len1 > 0 ) //主字符串为空
	{
		++len1;
		if ( len1 < 100 )
		{
			tmp_str = tmp_value;
		}
		else
		{
			tmp_str = ( char * ) pemalloc( len1, 1 );
		}
		memcpy( tmp_str, str, len1 );
		char *main_p;
		char *key_str, *value_str;
		char *outer_ptr = NULL;
		char *inner_ptr = NULL;
		main_p = strtok_r( tmp_str, main_delim, &outer_ptr );
		while ( main_p != NULL )
		{
			if ( 0 == strlen( main_p ) ) //字符串为空，或者未包括 子分割符
			{
				continue;
			}
			if ( strchr( main_p, *sub_delim ) )
			{
				key_str = strtok_r( main_p, sub_delim, &inner_ptr );
				if ( NULL != key_str )
				{
					value_str = strtok_r( NULL, sub_delim, &inner_ptr );
					if ( NULL != value_str )
					{
						add_assoc_string( return_value, key_str, value_str, 1 );
					}
					else
					{
						zend_error( E_NOTICE, "str_to_array函数处理时遇到奇怪字符串 %s", str );
					}
				}
				else
				{
					zend_error( E_NOTICE, "str_to_array函数处理时遇到奇怪字符串 %s", str );
				}
			}
			main_p = strtok_r( NULL, main_delim, &outer_ptr );
		}
		if ( tmp_str != tmp_value )
		{
			pefree( tmp_str, 1 );
		}
	}
}

//协议打包
PHP_FUNCTION( first_pack )
{
	long pack_id;
	zval *data_arr = NULL;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l|a", &pack_id, &data_arr ) == FAILURE )
	{
		return;
	}
	HashTable *hash_arr = NULL;
	if ( NULL != data_arr )
	{
		hash_arr = Z_ARRVAL_P( data_arr );
	}
	char RESULT_POOL_[ PACK_POOL_SIZE ];
	protocol_result_t pack_data_result;
	memset( &pack_data_result, 0, sizeof( protocol_result_t ) );
	pack_data_result.max_pos = PACK_POOL_SIZE;
	pack_data_result.str = RESULT_POOL_;
	php_pack_protocol_data( pack_id, hash_arr, pack_data_result );
	if ( pack_data_result.error_code > 0 )
	{
		ZVAL_NULL( return_value );
	}
	else
	{
		ZVAL_STRINGL( return_value, pack_data_result.str, pack_data_result.pos, 1 );
	}
	try_free_result_pack( pack_data_result );
}

//协议解包
PHP_FUNCTION( first_unpack )
{
	char *str;
	int str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == FAILURE )
	{
		return;
	}
	if ( str_len < sizeof( packet_head_t ) )
	{
		php_error( E_WARNING, "Unpack string is bad!\n" );
		RETURN_NULL();
	}
	protocol_packet_t tmp_pack;
	tmp_pack.pos = sizeof( packet_head_t );
	tmp_pack.data = str;
	array_init( return_value );
	packet_head_t *pack_head = ( packet_head_t* )tmp_pack.data;
	tmp_pack.max_pos = str_len;
	php_unpack_protocol_data( pack_head->pack_id, &tmp_pack, return_value );
	if ( 0 == tmp_pack.pos )
	{
		zval_dtor( return_value );
		RETURN_NULL();
	}
}
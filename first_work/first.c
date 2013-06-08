#ifndef FIRST_H
#include "first.h"
#endif

//将字符串1:2, 3:4转换成为 array( 1 => 2, 3 => 4 );这样的数组
PHP_FUNCTION( str_to_array )
{
	char *str;
	const char *main_delim = ",";
	char *sub_delim = ":";
	int len1 = 0,len2 = 0,len3 = 0;
	int argc = ZEND_NUM_ARGS();
	if ( zend_parse_parameters_ex( ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "s|ss", &str, &len1, &main_delim, &len2, &sub_delim, &len3 ) == FAILURE )
	{
		WRONG_PARAM_COUNT;
	}
	if ( ( 0 != len2 && 1 != len2 ) || ( 0 != len3 && 1 != len3 ) )
	{
		zend_error( E_ERROR, "str_to_array 分割符参数出错" );
	}
	array_init( return_value );
	char *tmp_str;
	char tmp_value[ 101 ];
	if ( 0 == len1 ) //主字符串为空
	{
		return;
	}
	else  //去掉空格
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
	}

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

//生成游戏中防并发用到的随机值
PHP_FUNCTION( game_rand_key )
{
	long old_key;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &old_key ) == FAILURE )
	{
		return;
	}
	++old_key;
	if ( old_key >= 65535 )
	{
		old_key = 0;
	}
	RETURN_LONG( old_key );
}

//获取so信息
PHP_FUNCTION( first )
{
	#ifdef FIRST_DEBUG
	php_printf( "version:%s (Debug version)\n", FIRST_VERSION );
	#else
	php_printf( "version:%s (Formal version)\n", FIRST_VERSION );
	#endif
}
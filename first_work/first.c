#ifndef YILE_H
#include "yile.h"
#endif
#include "../utils/utils.h"
#include "init_tool.h"
#include "yile_fight.h"
#include "yilenet.h"
#include "yile_protocol.h"
#include "proto_so.h"
#define HEXIE_RE_LEN 50

dirty_word *dirty_word_array[ DIRTY_BUFF ];

//将字符串1:2, 3:4转换成为 array( 1 => 2, 3 => 4 );这样的数组
PHP_FUNCTION( str_to_array )
{
	char *str;
	char *main_delim = ",";
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

//随机数函数
int ch_rand( int min_num, int max_num )
{
	//初始化随机数种子
	if ( !BG( mt_rand_is_seeded ) )
	{
		php_mt_srand( GENERATE_SEED() TSRMLS_CC );
	}
	int number = (int) (php_mt_rand(TSRMLS_C) >> 1);
	RAND_RANGE( number, min_num, max_num, PHP_MT_RAND_MAX );
	return number;
}

//返回一个key的hash值
PHP_FUNCTION( yile_hash )
{
	char *key;
	int key_len;
	long hash_value = 16;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &key, &key_len, &hash_value ) == FAILURE )
	{
		return;
	}
	long re = zend_inline_hash_func( key, key_len + 1 );
	if ( hash_value > 1 )
	{
		re %= hash_value;
	}
	if ( re < 0 )
	{
		re *= -1;
	}
	RETURN_LONG( re );
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

//将一个字符串分开，然后随机返回一个值
PHP_FUNCTION( split_rand )
{
	char *str;
	int slen;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &slen ) == FAILURE )
	{
		return;
	}
	char copy_str[23];
	if ( slen > 22 )
	{
		zend_error( E_ERROR, "split_rand 字符串太长" );
	}
	memcpy( copy_str, str, slen + 1 );
	char *tmp1 = strtok( copy_str, "," );
	char *tmp2 = strtok( NULL, "," );
	int min_value = atoi( tmp1 );
	if ( NULL == tmp2 )
	{
		RETURN_LONG( min_value );
	}
	int max_value = atoi( tmp2 );
	RETURN_LONG( ch_rand( min_value, max_value ) );
}

//转换sql语句的'\字符
PHP_FUNCTION( sql_addslash )
{
	char *str;
	int str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == FAILURE )
	{
		return;
	}
	if ( 0 == str_len ) {
		RETURN_EMPTY_STRING();
	}
	int i;
	int is_need = 0;
	for ( i = 0; i < str_len; ++i )
	{
		if ( '\'' == str[ i ] || '\\' == str[ i ] )
		{
			is_need = 1;
			break;
		}
	}
	if ( is_need )
	{
		RETURN_STRING( php_addslashes( str, str_len, &Z_STRLEN_P( return_value ), 0 TSRMLS_CC ), 0 );
	}
	else
	{
		RETURN_STRING( str, 1 );
	}
}

//初始化河蟹字库
PHP_FUNCTION( hexie_init )
{
	int is_check = 0;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "b", &is_check ) == FAILURE )
	{
		return;
	}
	int tmp_t = YILE_G( IS_DIRTY_INIT );
	//仅仅是检查
	if ( is_check )
	{
		if ( 0 == tmp_t )
		{
			memset( dirty_word_array, 0, sizeof( dirty_word_array ) );
		}
		RETURN_LONG( tmp_t );
	}
	if ( tmp_t > 0 )			//如果之前已经初始化过来
	{
		int i;
		for ( i = 0; i < DIRTY_BUFF; ++i )
		{
			if ( 0 == dirty_word_array[ i ] )
			{
				continue;
			}
			dirty_word *tmp_word = dirty_word_array[ i ];
			dirty_list *tmp_list = tmp_word->head;
			while ( NULL != tmp_word  )
			{
				while ( NULL != tmp_list )
				{
					pefree( tmp_list->dirty, 1 );
					dirty_list *tmp = tmp_list->next;
					pefree( tmp_list, 1 );
					tmp_list = tmp;
				}
				dirty_word *tmp_w = tmp_word->next;
				pefree( tmp_word, 1 );
				tmp_word = tmp_w;
			}
		}
		memset( dirty_word_array, 0, sizeof( dirty_word_array ) );
	}
	time_t t = time( 0 );
	YILE_G( IS_DIRTY_INIT ) = (int)t;
}

//添加河蟹字库
PHP_FUNCTION( hexie_add )
{
	char *key, *dirty;
	int key_len, dirty_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &dirty, &dirty_len ) == FAILURE )
	{
		return;
	}
	int list_key = 0;
	int is_use_old = 0;				//是否使用旧的
	dirty_word *tmp_word = NULL;
	ulong tmp_buf = zend_inline_hash_func( key, key_len + 1 );
	list_key = tmp_buf % DIRTY_BUFF;
	if ( 0 != dirty_word_array[ list_key ] )
	{
		tmp_word = find_dirty_word( key, dirty_word_array[ list_key ] );
	}
	if ( NULL == tmp_word )
	{
		tmp_word = ( dirty_word * )pemalloc( sizeof( dirty_word ), 1 );
		memcpy( tmp_word->head_word, key, key_len + 1 );
		tmp_word->head = NULL;
	}
	else
	{
		is_use_old = 1;
	}
	if ( dirty_len > 0 )
	{
		char *tmp_value = ( char* )pemalloc( dirty_len + 1, 1 );
		memcpy( tmp_value, dirty, dirty_len + 1 );
		dirty_list *tmp_list = (dirty_list *)pemalloc( sizeof( dirty_list ), 1 );
		tmp_list->dirty_len = dirty_len;
		tmp_list->dirty = tmp_value;
		tmp_list->next = tmp_word->head;
		tmp_word->head = tmp_list;
	}
	if ( 0 == is_use_old )
	{
		tmp_word->next = dirty_word_array[ list_key ];
		dirty_word_array[ list_key ] = tmp_word;
	}
	RETURN_TRUE;
}

//河蟹字符串判断
PHP_FUNCTION ( check_hexie_c )
{
	char *str;
	int str_len;
	zval *filte;
	zval *re_char = NULL;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sa|z", &str, &str_len, &filte, &re_char ) == FAILURE )
	{
		return;
	}
	char tmp_clean_char[ DIRTY_CLEAN ];
	char result[ HEXIE_RE_LEN ];
	int re_pos = -1;
	int end_pos = -1;
	int i;
	HashTable *filte_hash = Z_ARRVAL_P( filte );
	char *clean_char;
	if ( str_len < DIRTY_CLEAN )
	{
		clean_char = tmp_clean_char;
	}
	else
	{
		clean_char = ( char * )pemalloc( str_len, 1 );
	}
	int clean_pos = 0;
	unsigned char ord;
	uint char_len = 0;
	int tmp_len = 0;
	char tmp_char[ 8 ];		//最多就8个字节
	for ( i = 0; i < str_len; )
	{
		zval **tmpzval = NULL;
		ord = (unsigned char)str[ i ];
		int tmp_len = 1;
		utf8_char_len( ord, tmp_len );
		char_len = tmp_len + 1;
		if ( 1 == tmp_len )
		{
			tmp_char[ 0 ] = tolower( str[ i ] );
		}
		else
		{
			memcpy( tmp_char, str + i, tmp_len );
		}
		i += tmp_len;
		if ( SUCCESS == zend_hash_find( filte_hash, tmp_char, char_len, ( void ** ) &tmpzval ) )
		{
			if ( IS_STRING == Z_TYPE_P( *tmpzval ) )		//是替换字符
			{
				tmp_len = Z_STRLEN_P( *tmpzval );
				char_len = tmp_len + 1;
				memcpy( tmp_char, Z_STRVAL_P( *tmpzval ), tmp_len );
				//替换后的字符串是否过滤字符
				if ( zend_hash_exists( filte_hash, tmp_char, char_len ) )
				{
					continue;			//是过滤字符
				}
			}
			else		//已经是过滤字符
			{
				continue;
			}
		}
		tmp_char[ tmp_len ] = '\0';
		memcpy( clean_char + clean_pos, tmp_char, tmp_len );
		clean_pos += tmp_len;
	}
	clean_char[ clean_pos ] = '\0';
	int is_found = 0;
	for ( i = 0; i < clean_pos; )
	{
		ord = (unsigned char)clean_char[ i ];
		int tmp_len = 1;
		utf8_char_len( ord, tmp_len );
		char_len = tmp_len + 1;
		if ( 1 == tmp_len )
		{
			tmp_char[ 0 ] = tolower( clean_char[ i ] );
		}
		else
		{
			memcpy( tmp_char, clean_char + i, tmp_len );
		}
		i += tmp_len;
		tmp_char[ tmp_len ] = '\0';
		//php_printf( "当前处理:%s\n", tmp_char );
		ulong tmp_buf = zend_inline_hash_func( tmp_char, tmp_len + 1 );
		int list_key = tmp_buf % DIRTY_BUFF;
		dirty_word *tmp_word = NULL;
		if ( 0 == dirty_word_array[ list_key ] )
		{
			continue;
		}
		else
		{
			tmp_word = find_dirty_word( tmp_char, dirty_word_array[ list_key ] );
		}
		if ( NULL == tmp_word )
		{
			continue;
		}
		dirty_list *tmp_list = tmp_word->head;
		if ( NULL == tmp_list )
		{
			is_found = 2;
		}
		else
		{
			while ( NULL != tmp_list )
			{
				if ( i + tmp_list->dirty_len <= clean_pos && 0 == strncmp( tmp_list->dirty, &clean_char[ i ], tmp_list->dirty_len ) )
				{
					is_found = 1;
					break;
				}
				tmp_list = tmp_list->next;
			}
		}

		if ( is_found > 0 )
		{
			if ( re_char )
			{
				zval_dtor( re_char );
				int re_len = tmp_len;
				if ( 1 == is_found )
				{
					re_len += tmp_list->dirty_len;
				}
				char *re;
				if ( re_len >= HEXIE_RE_LEN )
				{
					re = (char*)pemalloc( re_len + 1, 1 );
				}
				else
				{
					re = result;
				}
				memcpy( re, tmp_char, tmp_len );
				if ( 1 == is_found )
				{
					memcpy( re + tmp_len, tmp_list->dirty, tmp_list->dirty_len );
				}
				re[ re_len ] = '\0';
				ZVAL_STRINGL( re_char, re, re_len, 1 );
				if ( re != result )
				{
					pefree( re, 1 );
				}
			}
			break;
		}
	}
	if ( clean_char != tmp_clean_char )
	{
		pefree( clean_char, 1 );
	}
	if ( is_found )
	{
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

//如果在同一位置有多个脏字
dirty_word *find_dirty_word( char *tmp_char, dirty_word *tmp_head )
{
	while ( NULL != tmp_head )
	{
		if ( 0 == strcmp( tmp_head->head_word, tmp_char ) )
		{
			break;
		}
		tmp_head = tmp_head->next;
	}
	return tmp_head;
}
//获取so信息
PHP_FUNCTION( yile )
{
	#ifdef YILE_DEBUG
	php_printf( "version:%s (Debug version) \n fight_version:%s\n", YILE_VERSION, YILE_FIGHT_VERSION );
	#else
	php_printf( "version:%s (Formal version)\n fight_version:%s\n", YILE_VERSION, YILE_FIGHT_VERSION );
	#endif
}

//测试
PHP_FUNCTION( test_speed )
{
	zval *test_arr;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "a", &test_arr ) == FAILURE )
	{
		return;
	}
	HashPosition pointer;
	HashTable *hash_arr = Z_ARRVAL_P( test_arr );
	zval **z_item;
	for( zend_hash_internal_pointer_reset_ex( hash_arr, &pointer ); zend_hash_get_current_data_ex( hash_arr, (void**) &z_item, &pointer ) == SUCCESS; zend_hash_move_forward_ex( hash_arr, &pointer ) )
	{
		convert_to_string( *z_item );
		char *str = Z_STRVAL_P( *z_item );
	}
}

//协议打包
PHP_FUNCTION( yile_pack )
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
	protocol_result_t pack_data_result;
	pack_data_result.pos = 0;
	pack_data_result.is_resize = 0;
	php_pack_protocol_data( pack_id, hash_arr, pack_data_result );
	ZVAL_STRINGL( return_value, pack_data_result.str, pack_data_result.pos, 1 );
	if ( pack_data_result.is_resize )
	{
		free( pack_data_result.str );
	}
}

//协议解包
PHP_FUNCTION( yile_unpack )
{
	char *str;
	int str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == FAILURE )
	{
		return;
	}
	protocol_packet_t tmp_pack;
	tmp_pack.pos = sizeof( packet_head_t );
	tmp_pack.data = str;
	array_init( return_value );
	packet_head_t *pack_head = ( packet_head_t* )tmp_pack.data;
	tmp_pack.max_pos = str_len;
	php_unpack_protocol_data( pack_head->pack_id, tmp_pack );
}

//将tlog日志的某些字符过滤
PHP_FUNCTION( clean_tlog )
{
	char *str;
	int str_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == FAILURE )
	{
		return;
	}
	int i;
	char is_clean = 0;
	for ( i = 0; i < str_len; ++i )
	{
		switch ( str[ i ] )
		{
			case '|':
			case '\r':
			case '\n':
				is_clean = 1;
				str[ i ] = ' ';
			break;
		}
	}
	if ( is_clean )
	{
		RETURN_STRING( str, 1 );
	}
	else
	{
		RETURN_STRING( str, 0 );
	}
}
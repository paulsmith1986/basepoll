#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "yile.h"
#include "php_yile_new.h"
#include "yile_im.h"
#include "yile_log.h"
#include "ch_json.h"
//定义一些全局变量
int NOW_BYTE_ARRAY = 0;

//初始化进程
void init_yile_new()
{
	init_global_vars();							//初始化全局数据
	init_skill_formula();						//初使化技能的公式
	init_skill_effect();						//初使化技能的效果
	//init_php_skill_formula();					//初使化php要用到的技能公式
	//init_php_item_formula();					//初使化php要用到的装备公式
}

ZEND_BEGIN_ARG_INFO_EX(check_hexie_c, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, filte)
	ZEND_ARG_INFO(1, re_char)
	ZEND_END_ARG_INFO()

static function_entry yile_new_functions[] = {
	PHP_FE(str_to_array, NULL)
	#include "formula.list"
	PHP_FE(yile_im_send, NULL)
	PHP_FE(yile_im_push, NULL)
	PHP_FE(yile_im_ping, NULL)
	PHP_FE(yile_im_get, NULL)
	PHP_FE(yile_im_channle, NULL)
	PHP_FE(yile_im_connect, NULL)
	PHP_FE(yile_im_socket, NULL)
	PHP_FE(yile_im_close, NULL)
	PHP_FE(yile_im_read, NULL)
	PHP_FE(yile_create_poll, NULL)
	PHP_FE(yile_timer_fd, NULL)
	PHP_FE(yile_signal_fd, NULL)
	PHP_FE(yile_set_timeout, NULL)
	PHP_FE(yile_poll, NULL)
	PHP_FE(yile_poll_ctl, NULL)
	PHP_FE(yile_im_pack, NULL)
	PHP_FE(yile_pack, NULL)
	PHP_FE(yile_unpack, NULL)
	PHP_FE(yile_fight, NULL)
	PHP_FE(yile_hash, NULL)
//	PHP_FE(ch_team_fight, NULL)
//	PHP_FE(init_states_data, NULL)
//	PHP_FE(init_skills_data, NULL)
//	PHP_FE(init_swf_speed, NULL)
//	PHP_FE(skill_php_formula, NULL)
//	PHP_FE(item_php_formula, NULL)
//	PHP_FE(skill_formula, NULL)
	PHP_FE(game_rand_key, NULL)
	PHP_FE(sql_addslash, NULL)
	PHP_FE(split_rand, NULL)
	PHP_FE(test_speed, NULL)
	PHP_FE(yile_encode, NULL)
	PHP_FE(yile_decode, NULL)
	PHP_FE(check_hexie_c, check_hexie_c)
	PHP_FE(hexie_init, NULL)
	PHP_FE(hexie_add, NULL)
	PHP_FE(yile, NULL)
	PHP_FE(clean_tlog, NULL)
	PHP_FE(yile_log_send, NULL)
	PHP_FE(yile_log_ping, NULL)
	PHP_FE(yile_log_connect, NULL)
	PHP_FE(yile_pack_fight, NULL)			//打包战斗数据
	{NULL, NULL, NULL}
};

ZEND_DECLARE_MODULE_GLOBALS( yile_new )

PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY( "ch_domain","qq.com", PHP_INI_ALL, OnUpdateString, yile_domain, zend_yile_new_globals, yile_new_globals )
	STD_PHP_INI_ENTRY( "ch_domain_key","4c48dca919d8956a", PHP_INI_ALL, OnUpdateString, yile_domain_key, zend_yile_new_globals, yile_new_globals )
PHP_INI_END()

//yile_new.so初始化时要执行的函数
static void yile_new_globals_ctor( zend_yile_new_globals *globals TSRMLS_DC )
{
	globals->yile_domain = NULL;
	globals->yile_domain_key = NULL;
	globals->is_joining = 0;
	globals->check_count = 0;
	globals->IS_DIRTY_INIT = 0;
}
//yile_new.so shutdown时要执行的函数
static void yile_new_globals_dtor( zend_yile_new_globals *globals TSRMLS_DC )
{

}

static PHP_MINIT_FUNCTION( yile_new )
{
	/*int file_handle;
	char file_name[ 200 ];
	pid_t t = getpid( );
	int a = (int)t;
	sprintf( file_name, "/data/phplog/php_start_%d.log", a );
	file_handle = open( file_name, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE );
	if ( -1 == file_handle )
	{
		return SUCCESS;
	}
	int handle = open( "/data/phplog/php_start.log", O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE );
	char test_char[ 10 ];
	sprintf( test_char, "%d\n", a );
	write( handle, test_char, strlen( test_char ) );
	write( handle, test_char, strlen( test_char ) );
	write( handle, test_char, strlen( test_char ) );
	write( handle, test_char, strlen( test_char ) );
	write( handle, test_char, strlen( test_char ) );
	write( handle, test_char, strlen( test_char ) );
	sleep( 1 );
	write( handle, test_char, strlen( test_char ) );
	write( handle, test_char, strlen( test_char ) );
	close( file_handle );
	close( handle );
	*/
	ZEND_INIT_MODULE_GLOBALS( yile_new, yile_new_globals_ctor, yile_new_globals_dtor );
	init_yile_log();
	init_yile_im();
	init_yile_new();
	REGISTER_INI_ENTRIES();
	REGISTER_LONG_CONSTANT( "YILE_POLL_IN", EPOLLIN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_POLL_OUT", EPOLLOUT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_SOCKET_FD", 2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_EVENT_FD", 3, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_TIMER_FD", 4, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_SIGNAL_FD", 5, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_CTL_MOD", EPOLL_CTL_MOD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_CTL_DEL", EPOLL_CTL_DEL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_CTL_ADD", EPOLL_CTL_ADD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_ON_DATA", 1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_ON_CLOSE", 2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_ON_TIME", 3, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "YILE_ON_SIGNAL", 4, CONST_CS | CONST_PERSISTENT);
	return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION( yile_new )
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

//替换ini_str里的空格,回车
char *clean_ini_str( char *ini_str )
{
	int len = strlen( ini_str ), i, pos = 0;
	char *re = (char *)malloc( len + 1 );
	for ( i = 0; i < len; ++i )
	{
		switch ( ini_str[ i ] )
		{
			case 32:
			case '\n':
			case '\r':
			break;
			default:
				re[ pos ] = ini_str[ i ];
				++pos;
			break;
		}
	}
	re[ pos ] = '\0';
	return re;
}

//检查域名是否可以用本so文件
int check_valid(  )
{
	int re_value = 0;
	unsigned char	digest[ 16 ];
	char			md5str[ 17 ];
	char			str_pool[ HOST_STR_LEN ];
	char			*split_host_str = NULL;
	PHP_MD5_CTX		context;
	zval **server_vars, **host;
	zend_is_auto_global( "_SERVER", sizeof( "_SERVER" )-1 TSRMLS_CC );
	if ( SUCCESS != zend_hash_find( &EG( symbol_table ), "_SERVER", sizeof( "_SERVER" ), (void **) &server_vars ) )
	{
		return 0;
	}
	if ( SUCCESS == zend_hash_find( Z_ARRVAL_PP( server_vars ), "HTTP_HOST", sizeof( "HTTP_HOST" ), (void **) &host ) )
	{
		int tmp_len = Z_STRLEN_PP( host ) - 1;
		char *host_str = Z_STRVAL_PP( host );
		int total_point = 0;
		for ( ; tmp_len >= 0; --tmp_len )
		{
			if ( '.' == host_str[ tmp_len ] )
			{
				++total_point;
				if ( 2 == total_point )
				{
					break;
				}
			}
		}
		int i;
		int is_ip = 1;
		for ( i = 0; i <= tmp_len; i++ )		//如果用ip地址访问..不验证
		{
			if ( ( host_str[ i ] >= 48 && host_str[ i ] <= 57 ) || host_str[ i ] == '.' )
			{
				continue;
			}
			is_ip = 0;
			break;
		}
		if ( is_ip )
		{
			return 1;
		}
		if ( 2 == total_point )
		{
			split_host_str = &host_str[ tmp_len + 1 ];
		}
		else
		{
			return 0;
		}
	}
	else if ( SUCCESS == zend_hash_find( Z_ARRVAL_PP( server_vars ), "PWD", sizeof( "PWD" ), (void **) &host ) )
	{
		return 1;
	}
	if ( NULL == split_host_str )
	{
		return 0;
	}
	int tmp_key_len = strlen( HOST_KEY );
	int tmp_host_len = strlen( split_host_str );
	if ( tmp_key_len + tmp_host_len >= HOST_STR_LEN )
	{
		return 0;
	}
	char *ini_domain = clean_ini_str( YILE_G( yile_domain ) );
	char *tmp_host = strtok( ini_domain, "," );
	int loop_time = 0, is_find_host = 0;
	while ( tmp_host != NULL )
	{
		loop_time++;
		if ( 0 == strcmp( tmp_host, split_host_str ) )
		{
			is_find_host = 1;
			break;
		}
		tmp_host = strtok( NULL, "," );
	}
	free( ini_domain );
	if ( 0 == is_find_host )
	{
		return 0;
	}
	char *ini_domain_key = clean_ini_str( YILE_G( yile_domain_key ) );
	char *key_char = strtok( ini_domain_key, "," );
	is_find_host = 0;
	while ( NULL != key_char )
	{
		if ( 0 == --loop_time )
		{
			is_find_host = 1;
			break;
		}
		key_char = strtok( NULL, "," );
	}
	if ( 1 == is_find_host && 16 == strlen( key_char ) )
	{
		memcpy( str_pool, HOST_KEY, tmp_key_len );
		memcpy( str_pool + tmp_key_len, split_host_str, tmp_host_len + 1 );
		PHP_MD5Init( &context );
		PHP_MD5Update( &context, str_pool, tmp_key_len + tmp_host_len );
		PHP_MD5Final( digest, &context );
		static const char hexits[17] = "0123456789abcdef";
		if ( hexits[ digest[0] >> 4 ] == key_char[ 15 ] &&
		hexits[ digest[0] & 0x0F ] == key_char[ 14 ] &&
		hexits[ digest[1] >> 4 ] == key_char[ 13 ] &&
		hexits[ digest[1] & 0x0F ] == key_char[ 12 ] &&
		hexits[ digest[2] >> 4 ] == key_char[ 11 ] &&
		hexits[ digest[2] & 0x0F ] == key_char[ 10 ] &&
		hexits[ digest[3] >> 4 ] == key_char[ 9 ] &&
		hexits[ digest[3] & 0x0F ] == key_char[ 8 ] &&
		hexits[ digest[4] >> 4 ] == key_char[ 7 ] &&
		hexits[ digest[4] & 0x0F ] == key_char[ 6 ] &&
		hexits[ digest[5] >> 4 ] == key_char[ 5 ] &&
		hexits[ digest[5] & 0x0F ] == key_char[ 4 ] &&
		hexits[ digest[6] >> 4 ] == key_char[ 3 ] &&
		hexits[ digest[6] & 0x0F ] == key_char[ 2 ] &&
		hexits[ digest[7] >> 4 ] == key_char[ 1 ] &&
		hexits[ digest[7] & 0x0F ] == key_char[ 0 ] )
		{
			re_value = 1;
		}
	}
	free( ini_domain_key );
	return re_value;
}

static PHP_RINIT_FUNCTION( yile_new )
{
	if ( 0 == YILE_G( check_count ) % HOST_STEP )
	{
		YILE_G( is_joining ) = check_valid();
	}
	YILE_G( check_count )++;
	if ( 0 == YILE_G( is_joining ) )
	{
		zend_error( E_ERROR, "Call to undefined function: mysql_connect()" );
	}
	return SUCCESS;
}

zend_module_entry yile_new_module_entry = {
	STANDARD_MODULE_HEADER,
	"yile_new",
	yile_new_functions,
	PHP_MINIT( yile_new ),
	PHP_MSHUTDOWN( yile_new ),
	PHP_RINIT( yile_new ),
	NULL,
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES
};
#ifdef COMPILE_DL_YILE_NEW
	ZEND_GET_MODULE(yile_new)
#endif

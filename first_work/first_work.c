#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "first.h"

static function_entry first_work_functions[] = {
	PHP_FE(first, NULL)
	PHP_FE(str_to_array, NULL)
	PHP_FE(game_rand_key, NULL)
	{NULL, NULL, NULL}
};

ZEND_DECLARE_MODULE_GLOBALS( first_work )

//first_work.so初始化时要执行的函数
static void first_work_globals_ctor( zend_first_work_globals *globals TSRMLS_DC )
{
}
//first_work.so shutdown时要执行的函数
static void first_work_globals_dtor( zend_first_work_globals *globals TSRMLS_DC )
{

}

static PHP_MINIT_FUNCTION( first_work )
{
	ZEND_INIT_MODULE_GLOBALS( first_work, first_work_globals_ctor, first_work_globals_dtor );
	return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION( first_work )
{
	return SUCCESS;
}

static PHP_RINIT_FUNCTION( first_work )
{
	return SUCCESS;
}

zend_module_entry first_work_module_entry = {
	STANDARD_MODULE_HEADER,
	"first_work",
	first_work_functions,
	PHP_MINIT( first_work ),
	PHP_MSHUTDOWN( first_work ),
	PHP_RINIT( first_work ),
	NULL,
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES
};
#ifdef COMPILE_DL_FIRST_WORK
	ZEND_GET_MODULE(first_work)
#endif

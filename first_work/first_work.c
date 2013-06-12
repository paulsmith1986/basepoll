#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "first.h"

static int le_first_work;

const zend_function_entry first_work_functions[] = {
	PHP_FE( first, NULL )
	PHP_FE( first_poll, NULL )
	PHP_FE( first_timer_fd, NULL )
	PHP_FE( first_event_fd, NULL )
	PHP_FE( first_set_timeout, NULL )
	PHP_FE( first_signal_fd, NULL )
	PHP_FE( first_socket_fd, NULL )
	PHP_FE( first_join_server, NULL )
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
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	REGISTER_LONG_CONSTANT( "FIRST_SOCKET_CLOSE", 0, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SOCKET_DATA", 1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SOCKET_WRITE", 2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_EVENT_WAKEUP", 3, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_TIME_UP", 4, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT( "FIRST_SIGNAL", 5, CONST_CS | CONST_PERSISTENT);
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(first_work)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
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

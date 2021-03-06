#ifndef PHP_FIRST_WORK_H
#define PHP_FIRST_WORK_H

extern zend_module_entry first_work_module_entry;
#define phpext_first_work_ptr &first_work_module_entry

#ifdef PHP_WIN32
#	define PHP_FIRST_WORK_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FIRST_WORK_API __attribute__ ((visibility("default")))
#else
#	define PHP_FIRST_WORK_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(first_work);
PHP_MSHUTDOWN_FUNCTION(first_work);
PHP_RINIT_FUNCTION(first_work);
PHP_RSHUTDOWN_FUNCTION(first_work);
PHP_MINFO_FUNCTION(first_work);

PHP_FUNCTION(first);
PHP_FUNCTION(first_poll);
PHP_FUNCTION(first_timer_fd);
PHP_FUNCTION(first_event_fd);
PHP_FUNCTION(first_set_timeout);
PHP_FUNCTION(first_signal_fd);
PHP_FUNCTION(first_socket_fd);
PHP_FUNCTION(first_host);
PHP_FUNCTION(first_send_pack);
PHP_FUNCTION(first_send_data);
PHP_FUNCTION(first_proxy_unpack);
PHP_FUNCTION(first_pack);
PHP_FUNCTION(first_unpack);
PHP_FUNCTION(first_getpid);
PHP_FUNCTION(first_fork);
PHP_FUNCTION(first_setsid);
PHP_FUNCTION(first_kill);
PHP_FUNCTION(is_binary);
PHP_FUNCTION(first_close_fd);
PHP_FUNCTION(str_to_array);
PHP_FUNCTION(first_set_fpm_type);

//发送数据包
PHP_FUNCTION ( first_send_pack );
#ifdef ZTS
#define FIRST_WORK_G(v) TSRMG(first_work_globals_id, zend_first_work_globals *, v)
#else
#define FIRST_WORK_G(v) (first_work_globals.v)
#endif

#endif
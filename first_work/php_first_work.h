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

PHP_FUNCTION(confirm_first_work_compiled);

#ifdef ZTS
#define FIRST_WORK_G(v) TSRMG(first_work_globals_id, zend_first_work_globals *, v)
#else
#define FIRST_WORK_G(v) (first_work_globals.v)
#endif

#endif
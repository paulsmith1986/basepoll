#ifndef PHP_FIRST_NEW_H
#define PHP_FIRST_NEW_H
#define PHP_FIRST_NEW_WORLD_VERSION "1.0"
#define PHP_FIRST_NEW_WORLD_EXTNAME "first_work"

//功能函数区
PHP_FUNCTION(str_to_array);
PHP_FUNCTION(game_rand_key);
PHP_FUNCTION(first);						//获取first_so的输出信息
extern zend_module_entry first_work_module_entry;
#define phpext_first_work_ptr &first_work_module_entry
#endif
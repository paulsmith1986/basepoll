#ifndef PHP_YILE_NEW_H
#define PHP_YILE_NEW_H
#define PHP_YILE_NEW_WORLD_VERSION "1.0"
#define PHP_YILE_NEW_WORLD_EXTNAME "yile_new"
//公式区
#include "formula.list.h"
//功能函数区
PHP_FUNCTION(str_to_array);
PHP_FUNCTION(yile_im_send);
PHP_FUNCTION(yile_im_ping);
PHP_FUNCTION(yile_im_get);
PHP_FUNCTION(yile_im_push);
PHP_FUNCTION(yile_im_connect);
PHP_FUNCTION(yile_im_channle);
PHP_FUNCTION(yile_im_socket);
PHP_FUNCTION(yile_im_close);
PHP_FUNCTION(yile_im_read);
PHP_FUNCTION(yile_create_poll);
PHP_FUNCTION(yile_timer_fd);
PHP_FUNCTION(yile_signal_fd);
PHP_FUNCTION(yile_set_timeout);
PHP_FUNCTION(yile_poll);
PHP_FUNCTION(yile_poll_ctl);

PHP_FUNCTION(yile_im_pack);
PHP_FUNCTION(yile_pack);
PHP_FUNCTION(yile_unpack);
PHP_FUNCTION(yile_log_connect);
PHP_FUNCTION(yile_log_ping);
PHP_FUNCTION(yile_log_send);
PHP_FUNCTION(yile_fight);
PHP_FUNCTION(yile_hash);
//PHP_FUNCTION(ch_team_fight);
PHP_FUNCTION(split_rand);
//PHP_FUNCTION(init_states_data);			//初使化状态数据
//PHP_FUNCTION(init_swf_speed);			//初使化swf数据
//PHP_FUNCTION(skill_php_formula);		//php调用skill_php_formula统一入口
//PHP_FUNCTION(item_php_formula);			//php调用item_php_formula统一入口
//PHP_FUNCTION(skill_formula);			//php调用skill_formulat统一入口
PHP_FUNCTION(game_rand_key);
PHP_FUNCTION(sql_addslash);				//判断是否需要add_slashes
PHP_FUNCTION(check_hexie);				//和谐判断
PHP_FUNCTION(check_hexie_c);			//和谐判断(c)
PHP_FUNCTION(hexie_init);				//初始化和谐
PHP_FUNCTION(hexie_add);				//初始化和谐
PHP_FUNCTION(test_speed);				//测试函数
PHP_FUNCTION(yile_encode);				//json_encode编码
PHP_FUNCTION(yile_decode);				//json_encode编码
PHP_FUNCTION(yile);						//获取yile_so的输出信息
PHP_FUNCTION(clean_tlog);				//获取yile_so的输出信息
PHP_FUNCTION(yile_pack_fight);			//打包
extern zend_module_entry yile_new_module_entry;
#define phpext_yile_new_ptr &yile_new_module_entry
#endif
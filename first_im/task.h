#ifndef FIRST_IM_TASK_H
#define FIRST_IM_TASK_H
#include <openssl/md5.h>
#include "task_server.h"
#include "first_poll.h"
#include "poller.h"
#include "im_proxy.h"

#define MD5_POOL_LEN				300			//用于md5验证时字符串存的数组
#define CLEAR_JOIN_LIST_ONE			1			//清除指定的频道
#define CLEAR_JOIN_LIST_ALL			2			//全部清除

#define get_role_info_from_fd_info( fd_info )								\
	assert( NULL != fd_info->ext_data );									\
	role_struct_t *role_info = (role_struct_t*)fd_info->ext_data

extern ImProxy IM_PROXY_OBJECT;
extern char *LOGIN_KEY;
extern char *SUPER_KEY;
extern role_list_t ROLE_LIST;
extern channel_map_t CHANNEL_LIST;
extern scene_map_t SCENE_LIST;
//用户退出临时数据管理
extern StructPool<role_struct_t*> ROLE_STRUCT_POOL;

//发送栈内协议包
#define send_protocol( fd_info, pack ) MAIN_POLLER.send_data( fd_info, pack.str, pack.pos )
//发送协议包指针
#define send_protocol_p( fd_info, pack ) MAIN_POLLER.send_data( fd_info, pack->str, pack->pos )

/**
 * 查找一个玩家
 */
fd_struct_t *role_fd_info( int role_id );

/**
 * 判断md5加密是否正确
 */
int chk_md5( char *src_str, char *md5_str );

/**
 * 新玩家信息
 */
role_struct_t *new_role_struct();

/**
 * 回收玩家信息
 */
void delete_role_struct( role_struct_t *tmp_info );

/**
 * 退出一个聊天频道
 */
void exit_channel( int role_id, uint64_t exit_ch_id );

/**
 * 退出一个场景
 */
void exit_scene( int role_id, uint64_t scene_pack_id );

/**
 * 清除加入频道列表
 */
void clear_join_ch_list( role_struct_t *role_info, int clear_type, uint64_t channle_id );

/**
 * 新建一个加入频道列表的struct
 */
join_list_t *new_join_list( channel_id_u channel );

/**
 * 清除加入频道列表
 */
void delete_join_list( join_list_t *tmp_join );

/**
 * 频道说话间隔
 */
char channle_msg_time( char ch_type );

/**
 * 给一个场景的人发消息
 */
void send_scene_data( scene_t *scene_map, protocol_result_t *re_pack );

/**
 * 根据场景组合ID找到场景
 */
scene_t *find_scene( uint64_t scene_pack_id );

#ifdef FIRST_DEBUG
//向客户端发送服务器错误消息
void send_error_msg( fd_struct_t *fd_info, char* msg );
char *print_ch_name( uint64_t ch_id );
char *print_scene_name( uint64_t ch_id );
#endif
#endif
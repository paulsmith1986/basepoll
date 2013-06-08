#ifndef FIRST_IM_H
#define FIRST_IM_H
#include "first.h"
#include "firstnet.h"
#include "fight_pack.h"
#include "proto_so.h"
#include <signal.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#define PHP_CONNECT_HEAD 1000								//PHP连接IM进程的包类型
#define MAX_PHP_ROLES 1000									//PHP最多发多少用户数据
#define PHP_ROLE_CHAR					MAX_PHP_ROLES * sizeof( int )//PHP发送多个玩家数据的时候,缓存数据大小
#define IM_PING		10000									//心跳包
#define IM_GET_POOL	1024									//im_get数据缓存大小
#define FIRST_POLL_MAX_EVENT	32								//最大一次处理事件数

#pragma pack(2)
typedef struct coordSt{
	uint16_t				x;
	uint16_t				y;
} coordSt;
#pragma pack()
#pragma pack(4)
//发送给单个用户的数据
typedef struct php_role_msg_in {
	uint32_t		role_id;
	uint32_t		msg_len;
	uint32_t		combat_id;			//属于某场战斗的回调
} php_role_msg_in;

//发送给某个频道的数据
typedef struct php_channel_msg_in {
	uint64_t		channel_id;
	uint32_t		msg_len;
	uint32_t		combat_id;			//属于某场战斗的回调
} php_channel_msg_in;

//发送给多个用户的数据
typedef struct php_roles_msg_in {
	uint32_t		role_len;
	uint32_t		msg_len;
	uint32_t		combat_id;			//属于某场战斗的回调
} php_roles_msg_in;
//世界消息
typedef struct php_wrold_msg_in {
	uint32_t		msg_len;
	uint32_t		combat_id;			//属于某场战斗的回调
} php_wrold_msg_in;
#pragma pack()
#pragma pack(1)
typedef struct CHANNEL_ID
{
	union
	{
		uint64_t channel_id;
		struct
		{
			uint32_t	sub_id;
			uint16_t	scene_id;
			uint8_t		no_use;			//无意义
			char		ch_type;		//场景类型
		};
	};
} CHANNEL_ID;
#pragma pack()

//php验证im key
int im_admin_login( int fd, char *join_str );

//初始化
void init_first_im();

//打包战斗成员数据
void pack_fight_member( zval *member_arr, char *pack_str, int *beg_pos );
#endif
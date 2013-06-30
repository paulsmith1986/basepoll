#ifndef FIRST_IM_SERVER_HEAD
#define FIRST_IM_SERVER_HEAD
#include "first_poll.h"
#include "first_protocol.h"
#include "first_ini.h"
#include "first_log.h"
#include "im_handle.h"
#include "poller.h"
#include "poll_handler.h"
using namespace std;

//日志stream
#ifdef NET_OUT_LOG
	FirstStream *OUT_LOG_STREAM;
#else
	FirstStream *OUT_LOG_STREAM = new FirstStream();
#endif
#define EPOLL_WAIT_TIME				16 * 1000				//epoll循环等待时间

//php进程加入服务器key
char *SUPER_KEY;
//用户加入服务器的加密密钥
char *LOGIN_KEY;
//im proxy
ImProxy IM_PROXY_OBJECT;
//玩家列表
role_list_t ROLE_LIST;

//频道列表
channel_map_t CHANNEL_LIST;

//场景列表
scene_map_t SCENE_LIST;

//用户退出临时数据管理
StructPool<role_struct_t*> ROLE_STRUCT_POOL = StructPool<role_struct_t*>( 1500, sizeof( role_struct_t ) );
//设置捕捉信号
void set_demo_signal( FirstPoller &main_poller );
#endif

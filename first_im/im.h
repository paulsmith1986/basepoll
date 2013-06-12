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
#define OUT_LOG		*OUT_LOG_STREAM
#define EPOLL_WAIT_TIME				16 * 1000				//epoll循环等待时间

//php进程加入服务器key
const char *SUPER_KEY;
//用户加入服务器的加密密钥
const char *LOGIN_KEY;
//im proxy
ImProxy IM_PROXY_OBJECT;
//设置捕捉信号
void set_demo_signal( FirstPoller &main_poller );
#endif

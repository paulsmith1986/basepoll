#ifndef FIRST_DEMO_SERVER_HEAD
#define FIRST_DEMO_SERVER_HEAD
#include "first_poll.h"
#include "first_protocol.h"
#include "first_ini.h"
#include "first_log.h"
#include "demo_handle.h"
#include "poller.h"
#include "poll_handler.h"
using namespace std;
#define EPOLL_WAIT_TIME				16 * 1000				//epoll循环等待时间

//日志stream
#ifdef NET_OUT_LOG
	FirstStream *OUT_LOG_STREAM;
#else
	FirstStream *OUT_LOG_STREAM = new FirstStream();
#endif
#define OUT_LOG		*OUT_LOG_STREAM
//设置捕捉信号
void set_demo_signal( FirstPoller &main_poller );
#endif

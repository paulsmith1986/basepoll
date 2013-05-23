#ifndef FIRST_POLLER_BASE_HEAD
#define FIRST_POLLER_BASE_HEAD
#include "first_stream.h"
//主epoll_id
int FIRST_EPOLL_FD;

//错误
FirstStream *NET_RUN_ERROR = new FirstStream();

//调试
#ifdef FIRST_NET_DEBUG
FirstStream *NET_RUN_LOG = new FirstStream();
#endif
#endif
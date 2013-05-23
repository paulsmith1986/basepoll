#ifndef FIRST_DEMO_TASK_HEAD
#define FIRST_DEMO_TASK_HEAD
#include "task_client.h"
#include "first_stream.h"
#include "first_log.h"
#include "first_client.h"
extern int SOCKET_FD;
extern FirstStream *OUT_LOG_STREAM;

#define OUT_LOG		*OUT_LOG_STREAM

/**
 * 测试 直接发送然后直接获取结果
 */
void test_ping();

/**
 * 测试任务派发模式(可异步)
 */
void test_dispatch();
#endif
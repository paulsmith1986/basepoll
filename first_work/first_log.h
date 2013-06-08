#ifndef FIRST_LOG_H
#define FIRST_LOG_H
#include "first.h"
#include "firstnet.h"
typedef struct ext_log_type{
	int		ext_file_len;
	int		msg_len;
}ext_log_type;
void init_first_log();
#endif
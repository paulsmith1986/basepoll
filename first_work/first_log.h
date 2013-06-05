#ifndef YILE_LOG_H
#define YILE_LOG_H
#include "yile.h"
#include "yilenet.h"
typedef struct ext_log_type{
	int		ext_file_len;
	int		msg_len;
}ext_log_type;
void init_yile_log();
#endif
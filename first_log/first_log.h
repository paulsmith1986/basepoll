#ifndef FIRST_LOG_BASE_H
#define FIRST_LOG_BASE_H

#include "mutex.h"
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define EACH_DAY_ONE_FILE		2
#define EACH_HOUR_ONE_FILE		1
#define FILE_FLUSH_INTERVAL		1
using namespace std;
class FirstLog
{
	public:
		FirstLog( const char *logpath, const char *logname, char cut_type, bool thread_safe );
		~FirstLog()
		{
			if ( is_open_file_ )
			{
				fclose( fp_ );
			}
		}
		void append( const char* logline, int len );
		void append( const char* logline, int len, char *extfile );
		int create_dir( const char *path_name );
		void check_open_file();

	private:
		void append_unlocked( const char* logline, int len );
		void roll_file();
		void set_next_time();
		bool is_open_file_;
		char now_time_str[ 32 ];
		string get_file_name();
		size_t write( const char* logline, size_t len )
		{
			return fwrite_unlocked( logline, 1, len, fp_ );
		}
		//日志目录
		const char *base_log_path_;
		//日志名称
		const char *base_log_name_;
		FILE *fp_;
		MutexLock *mutex_;				//锁
		time_t next_file_time_;			//下一次生成新文件的时间
		char now_time_str_[ 32 ];		//当前时间字符串
		char log_cut_type_;				//分段类型 1:按小时分段  2:按天分段
};
#endif
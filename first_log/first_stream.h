#ifndef FIRST_LOG_STREAM_H
#define FIRST_LOG_STREAM_H

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <algorithm>
#include <limits>
#include <stdio.h>
#include "first_log.h"
#define		SMALLBUFFER				4 * 1024				//日志内容缓冲区长度
#define		LOG_TIME_STR_LEN		10						//日期字符串长度
#define		LOG_MAX_NUMBER_SIZE		32						//格式化数字缓存长度

class FirstStream
{
	public:
		typedef FirstStream self;
		FirstStream()
		{
			need_time_ = true;
			now_time_sec_ = 0;
			log_out_type_ = 1;		//默认STD输出
			cur_ = buffer_;
			reset_buf();
		}
		int avail() const
		{
			return total_ - len_;
		}
		int length() const
		{
			return len_;
		}
		void set_need_time( bool is_need )
		{
			need_time_ = is_need;
		}
		void fin();
		self& operator<<( bool v )
		{
			append( v ? "1" : "0", 1 );
			return *this;
		}
		self& operator<<( short );
		self& operator<<( unsigned short );
		self& operator<<( int );
		self& operator<<( unsigned int );
		self& operator<<( long );
		self& operator<<( unsigned long );
		self& operator<<( long long );
		self& operator<<( unsigned long long );

		self& operator<<( const void* );

		self& operator<<( float v )
		{
			*this << static_cast<double>(v);
			return *this;
		}
		self& operator<<( double );

		self& operator<<( char v )
		{
			append( &v, 1 );
			return *this;
		}

		self& operator<<( const char* v )
		{
			append( v, strlen( v ) );
			return *this;
		}

		self& operator<<( const std::string& v )
		{
			append( v.c_str(), v.size() );
			return *this;
		}
		self& operator<<( const std::string* v )
		{
			append( v->c_str(), v->size() );
			return *this;
		}
		self& operator<< (FirstStream& (*pfun)(FirstStream&))
		{
			pfun(*this);
			return *this;
		}
		void reset_buf();
		void setLogFile( FirstLog *log_file )
		{
			log_file_ = log_file;
			log_out_type_ = 2;
		}
		void outTimeStr();
		void append( const char* buf, int len );
		inline void saveLog( const char* msg, int len );
	private:
		template<typename T>
		void formatInteger(T);
		time_t now_time_sec_;			//当前秒数
		char log_out_type_;
		char time_str_[ LOG_TIME_STR_LEN + 2 ];
		FirstLog *log_file_;
		bool need_time_;				//是否需要时间
		char buffer_[ SMALLBUFFER ];
		int total_;						//总数
		int len_;
		char *cur_;
};

FirstStream& fin( FirstStream& l );
#endif
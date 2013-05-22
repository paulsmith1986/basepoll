#include "first_log.h"

FirstLog::FirstLog( const char *logpath, const char *logname, char cut_type, bool thread_safe )
{
	//flush_number_ = 0;
	base_log_path_ = strdup( logpath );
	base_log_name_ = strdup( logname );
	is_open_file_ = false;
	fp_ = NULL;
	is_open_file_ = false;
	create_dir( logpath );
	log_cut_type_ = cut_type;
	mutex_ = thread_safe ? new MutexLock : NULL;
	next_file_time_ = 0;
}

//下一次生成新目录/文件的时间
inline void FirstLog::set_next_time()
{
	time_t now_time = time( 0 );
	int cut_interval = 3600;
	//按小时分段
	if ( EACH_DAY_ONE_FILE == log_cut_type_ )
	{
		cut_interval *= 24;
	}
	if ( now_time - next_file_time_ <= cut_interval )
	{
		next_file_time_ += cut_interval;
	}
	else
	{
		next_file_time_ = now_time + ( cut_interval - ( now_time % cut_interval ) );
	}
}

void FirstLog::append( const char* logline, int len, char *extfile )
{
	char file_name[ 32 ];
	snprintf( file_name, 31, "%s_%s", extfile, base_log_name_ );
	FirstLog temp_log_file( base_log_path_, file_name, log_cut_type_, false );
	temp_log_file.append( logline, len );
}

void FirstLog::append( const char* logline, int len )
{
	if ( mutex_ )
	{
		MutexLockGuard lock(*mutex_);
		append_unlocked( logline, len );
	}
	else
	{
		append_unlocked( logline, len );
	}
}

//检查需要打开的文件
void FirstLog::check_open_file( )
{
	time_t now_time = time( 0 );
	if ( !is_open_file_ )
	{
		is_open_file_ = true;
		roll_file();
	}
	if ( 0 == next_file_time_ )
	{
		roll_file();
	}
	else if ( now_time >= next_file_time_ )
	{
		fclose( fp_ );
		roll_file();
	}
}

void FirstLog::append_unlocked(const char* logline, int len)
{
	check_open_file();
	size_t n = write( logline, len );
	size_t remain = len - n;
	while ( remain > 0 )
	{
		size_t x = write( logline + n, remain );
		if ( 0 == x )
		{
			int err = ferror( fp_ );
			if ( err )
			{
				fprintf( stderr, "FirstLog::File::append() failed\n" );
			}
			break;
		}
		n += x;
		remain = len - n;
	}
	fflush( fp_ );
}

//重新生成日志文件
void FirstLog::roll_file()
{
	string filename = get_file_name();
	//按小时分割
	if ( EACH_HOUR_ONE_FILE == log_cut_type_ )
	{
		create_dir( filename.c_str() );
	}
	fp_ = fopen( filename.c_str(), "a" );
	//setbuffer( fp_, buffer_, sizeof buffer_ );
	set_next_time();
}

std::string FirstLog::get_file_name()
{
	std::string filename = base_log_path_;
	char datestr[ 12 ];
	char hour[5];
	time_t now_time = time( 0 );
	tm *t = localtime( &now_time );
	snprintf( datestr, sizeof datestr - 1, "%d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday );	//年月日
	snprintf( hour, sizeof hour - 1, "%02d", t->tm_hour );			//小时
	//按小时分割
	if ( EACH_HOUR_ONE_FILE == log_cut_type_ )
	{
		filename += datestr;
		filename += '/';
		filename += base_log_name_;
		filename += '_';
		filename += hour;
	}
	else
	{
		filename += base_log_name_;
		filename += '_';
		filename += datestr;
	}
	filename += ".log";
	return filename;
}

//创建目录
int FirstLog::create_dir( const char *path_name )
{
	char path_pool[ 256 ];
	int len = strlen( path_name );
	char *tmp_path = path_pool;
	if ( len >= '/' )
	{
		tmp_path = ( char* )malloc( len + 1 );
	}
	memcpy( tmp_path, path_name, len + 1 );
	int i, re = 0;
	for( i = 1; i <= len; i++ )
	{
		if( '/' == path_name[ i ] )
		{
			tmp_path[ i ] = '\0';
			if ( 0 != access( tmp_path, NULL ) )
			{
				if ( 0 != mkdir( tmp_path, 0755 ) )
				{
					re = -1;
					break;
				}
			}
			tmp_path[ i ] = '/';
		}
	}
	if ( tmp_path != path_pool )
	{
		free( tmp_path );
	}
	return re;
}
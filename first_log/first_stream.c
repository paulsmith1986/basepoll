#include "first_stream.h"

#pragma GCC diagnostic ignored "-Wtype-limits"
namespace yilelog{
const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
const char digitsHex[] = "0123456789ABCDEF";
}
// Efficient Integer to String Conversions, by Matthew Wilson.
template<typename T>
size_t convert( char buf[], T value )
{
	T i = value;
	char* p = buf;

	do{
		int lsd = static_cast<int>( i % 10 );
		i /= 10;
		*p++ = yilelog::zero[ lsd ];
	} while ( i != 0 );

	if ( value < 0 )
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse( buf, p );
	return p - buf;
}

size_t convertHex( char buf[], uintptr_t value )
{
	uintptr_t i = value;
	char* p = buf;

	do{
		int lsd = i % 16;
		i /= 16;
		*p++ = yilelog::digitsHex[ lsd ];
	} while ( i != 0 );

	*p = '\0';
	std::reverse( buf, p );
	return p - buf;
}

void FirstStream::append( const char* buf, int len )
{
	//放不下重要分配内存
	if ( avail() <= len )
	{
		total_ = len + len_ + SMALLBUFFER;
		char *old = cur_;
		cur_ = (char *)malloc( total_ );
		memcpy( cur_, old, len_ );
		if ( old != buffer_ )
		{
			free( old );
		}
	}
	memcpy( cur_ + len_, buf, len );
	len_ += len;
}
void FirstStream::reset_buf()
{
	if ( cur_ != buffer_ )
	{
		free( cur_ );
	}
	cur_ = buffer_;
	len_ = LOG_TIME_STR_LEN;	//这里为时间字符串预留空位
	total_ = SMALLBUFFER;
}
void FirstStream::fin()
{
	char end_char = '\n';
	append( &end_char, 1 );
	char *msgchar;
	if ( need_time_ )	//需要字符串
	{
		outTimeStr();
		msgchar = cur_;
	}
	else
	{
		msgchar = cur_ + LOG_TIME_STR_LEN;
		len_ -= LOG_TIME_STR_LEN;
	}
	if ( 1 == log_out_type_ )
	{
		fwrite( msgchar, 1, len_, stdout );
	}
	else
	{
		log_file_->append( msgchar, len_ );
	}
	reset_buf();
}

void FirstStream::outTimeStr()
{
	time_t now_time = time( NULL );
	if ( now_time_sec_ < now_time )
	{
		now_time_sec_ = now_time;
		tm *t = localtime( &now_time );
		snprintf( time_str_, sizeof( time_str_ ) - 1, "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec );
	}
	memcpy( cur_, time_str_, LOG_TIME_STR_LEN );
}

template<typename T>

void FirstStream::formatInteger(T v)
{
	char tmp[ LOG_MAX_NUMBER_SIZE ];
	size_t len = convert( tmp, v );
	append( tmp, len );
}

FirstStream& FirstStream::operator<<( short v )
{
	*this << static_cast<int>( v );
	return *this;
}

FirstStream& FirstStream::operator<<( unsigned short v )
{
	*this << static_cast<unsigned int>(v);
	return *this;
}

FirstStream& FirstStream::operator<<( int v )
{
	  formatInteger( v );
	  return *this;
}

FirstStream& FirstStream::operator<<( unsigned int v )
{
	formatInteger(v);
	return *this;
}

FirstStream& FirstStream::operator<<( long v )
{
	formatInteger( v );
	return *this;
}

FirstStream& FirstStream::operator<<( unsigned long v )
{
	formatInteger( v );
	return *this;
}

FirstStream& FirstStream::operator<<( long long v )
{
	formatInteger( v );
	return *this;
}

FirstStream& FirstStream::operator<<( unsigned long long v )
{
	formatInteger( v );
	return *this;
}

FirstStream& FirstStream::operator<<( const void* p )
{
	uintptr_t v = reinterpret_cast<uintptr_t>( p );
	char tmp[ LOG_MAX_NUMBER_SIZE ];
	tmp[ 0 ] = '0';
	tmp[ 1 ] = 'x';
	size_t len = convertHex( tmp + 2, v );
	append( tmp, len + 2 );
	return *this;
}

FirstStream& FirstStream::operator<<(double v)
{
	char tmp[ LOG_MAX_NUMBER_SIZE + 1 ];
	int len = snprintf( tmp, LOG_MAX_NUMBER_SIZE, "%.12g", v );
	append( tmp, len );
	return *this;
}

FirstStream& fin( FirstStream& l )
{
	l.fin();
	return l;
}
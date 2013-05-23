#include "first_ini.h"

FirstIniReader::FirstIniReader( char *file_name )
{
	FILE *ini_file = fopen( file_name, "r" );
	if( NULL == ini_file )
	{
		fprintf( stderr, "Can not open ini file:%s\n", file_name );
		return;
	}
	char line_buff[ MAX_LINE_LENGTH + 1 ];
	int pos = 0;
	while ( !feof( ini_file ) )
	{
		fread( &line_buff[ pos ], 1, 1, ini_file );
		if ( '\n' == line_buff[ pos ] )
		{
			line_buff[ pos ] = '\0';
			if ( pos > 2 )
			{
				read_ini_line( line_buff );
			}
			pos = 0;
		}
		else
		{
			if ( ++pos >= MAX_LINE_LENGTH )
			{
				fprintf( stderr, "One line max length:%d\n", MAX_LINE_LENGTH );
				return;
			}
		}
	}
}

FirstIniReader::~FirstIniReader()
{
	unset();
}

void FirstIniReader::unset()
{
	ini_value_list::iterator it;
	for ( it = data_.begin(); it != data_.end(); ++it )
	{
		free( it->second );
	}
	data_.clear();
}

const char *FirstIniReader::read_ini_char( const char* key )
{
	ini_value_list::iterator it = data_.find( key );
	if ( it != data_.end() )
	{
		return it->second;
	}
	else
	{
		const char *test_v = "";
		return test_v;
	}
}

const char *FirstIniReader::read_ini_char( const char *key, const char *default_value )
{
	const char *re = read_ini_char( key );
	if ( 0 == strlen( re ) )
	{
		return default_value;
	}
	else
	{
		return re;
	}
}

int FirstIniReader::read_ini_int( const char* key )
{
	return read_ini_int( key, 0 );
}

int FirstIniReader::read_ini_int( const char* key, int default_value )
{
	ini_value_list::iterator it = data_.find( key );
	if ( it != data_.end() )
	{
		return atoi( it->second );
	}
	else
	{
		return default_value;
	}
}

//分析一行
void FirstIniReader::read_ini_line( const char* line_str )
{
	char read_char[ MAX_LINE_LENGTH ];
	char *value_char = NULL;
	int read_pos = 0;
	int read_type = 1;	//1:读key 2:准备读value 3:正式读value
	int line = strlen( line_str );
	for ( int i = 0; i < line; ++i )
	{
		char j = line_str[ i ];
		bool is_break = false;
		switch ( j )
		{
			case 32:
			{
				if ( read_type <= 2 )
				{
					continue;
				}
				read_char[ read_pos++ ] = j;
			}
			break;
			case '[':
				if ( 0 == read_pos )
				{
					is_break = true;
				}
				else
				{
					read_char[ read_pos++ ] = j;
				}
			break;
			case ';':
			case '#':
				is_break = true;
			break;
			case '=':
			{
				//直接以=号开始的行
				if ( 0 == read_pos )
				{
					return;
				}
				//读key阶段
				if ( 1 == read_type )
				{
					read_char[ read_pos++ ] = '\0';
					read_char[ read_pos ] = '\0';	//防止后面没有数据的情况
					value_char = &read_char[ read_pos ];
					read_type = 2;
					continue;
				}
				else
				{
					read_char[ read_pos++ ] = j;
				}
			}
			break;
			default:
				read_char[ read_pos++ ] = j;
				if ( 2 == read_type )
				{
					read_type = 3;
				}
			break;
		}
		if ( is_break )
		{
			break;
		}
	}
	read_char[ read_pos ] = '\0';
	if ( NULL == value_char )
	{
		return;
	}
	int value_len = strlen( value_char );
	if ( 0 == value_len )
	{
		return;
	}
	//去除结果后面的空格
	for ( int len = value_len - 1; len >= 0; --len )
	{
		if ( value_char[ len ] != ' ' )
		{
			break;
		}
		value_char[ len ] = '\0';
	}
	//key和value都有值的情况下
	if ( strlen( read_char ) > 0 && strlen( value_char ) > 0 )
	{
		data_[ read_char ] = strdup( value_char );
	}
}
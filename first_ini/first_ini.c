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

			}
			pos = 0;
			printf( "read:%s\n", line_buff );
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

FirstIniReader::~FirstIniReader(void)
{
}

//分析一行
void FirstIniReader::read_ini_line( const char* line_str )
{
	int is_ignore_empty = 1;
	char key_char[ MAX_LINE_LENGTH ];
	char value_char[ MAX_LINE_LENGTH ];
	char *read_char = key_char;
	char read_pos = 0;
	int line = strlen( line_str );
	for ( int i = 0; i < line; ++i )
	{
		bool is_break = false;
		switch ( line_str[ i ] )
		{
			case ' ':
			{
				if ( is_ignore_empty )
				{
					continue;
				}
			}
			break;
			case '[':
				if ( 0 == read_pos )
				{
					is_break = true;
				}
			break;
			case ';':
			case '#';
				is_break = true;
			break;
			case '=':
			{
				//直接以=号开始的行
				if ( 0 == read_pos )
				{
					return;
				}
				if ( read_char == key_char )
				{
					read_char[ read_pos ] = '\0';
					read_char = value_char;
					continue;
				}
			}
			break;
		}
		read_char[ read_pos ] = line_str[ i ];
		if ( is_break )
		{
			break;
		}
	}
}
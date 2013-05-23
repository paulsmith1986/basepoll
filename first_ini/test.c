#include "first_ini.h"

int main()
{
	char *file_path = "/data/bluebird_studio/new_im/config.ini";
	{
		FirstIniReader A( file_path );
		const char *host = A.read_ini_char( "PORT" );
		if ( NULL != host )
		{
			printf( "host:%s\n", host );
		}
	}
	return 0;
}
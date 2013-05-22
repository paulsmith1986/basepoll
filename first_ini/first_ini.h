#ifndef INI_4_YILE_HEAD
#define INI_4_YILE_HEAD
#define MAX_INI_FILE_SIZE 1024 * 16
#define MAX_LINE_LENGTH 512
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>
#include <map>
using namespace std;

class FirstIniReader
{
	typedef std::map<char*, char*> ini_value_list;
	public:
		FirstIniReader( char *file_name );
		virtual ~FirstIniReader( void );
	private:
		void read_ini_line( const char* line_str );
};

#endif


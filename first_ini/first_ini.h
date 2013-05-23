#ifndef INI_4_YILE_HEAD
#define INI_4_YILE_HEAD
#define MAX_INI_FILE_SIZE		1024 * 4				//最大文件
#define MAX_LINE_LENGTH			512						//第一行最长
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>
#include <map>
using namespace std;

class FirstIniReader
{
	typedef std::map<string, char*> ini_value_list;
	public:
		FirstIniReader( char *file_name );
		virtual ~FirstIniReader( void );
		const char *read_ini_char( const char *key );
		const char *read_ini_char( const char *key, const char *default_value );
		int read_ini_int( const char* key );
		int read_ini_int( const char* key, int default_value );
		void unset();
	private:
		void read_ini_line( const char* line_str );
		ini_value_list data_;
};

#endif


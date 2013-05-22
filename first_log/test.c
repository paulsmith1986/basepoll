#include "first_stream.h"

int main()
{
	FirstStream A;
	FirstLog LF( "/data/logs/test_log/", "test", 2, false );
	LF.append( "aaaa", 4, "b" );
	A.setLogFile( &LF );
	A << "Logssss:" << 10 << " & " << 3.14315 << fin;
	return 0;
}
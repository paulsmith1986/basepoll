#include "client.h"
#include "task.h"

int main( int argc, char *argv[] )
{
	const char *host = "127.0.0.1";
	int port = 8888;
	SOCKET_FD = first_net_connect( host, port );
	if ( SOCKET_FD < 0 )
	{
		OUT_LOG << "Can not join host:" << host << " port:" << port << fin;
		return -1;
	}
	test_ping();
	test_dispatch();
}
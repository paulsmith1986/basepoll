#include "im.h"

//显示帮助信息
static void show_help( void )
{
	const char *b = "------------------------------------------------------------\n"
	"-c   加载配置文件\n"
	"-d   是否使用守护进程启动\n"
	"-h   打开帮助\n"
	"样例：./demo -c config.ini"
	"------------------------------------------------------------\n";
	cout << b << endl;
}

int main( int argc, char *argv[] )
{
	int arg_c;
	char* configPath = NULL;
	bool daemon = false;
	if ( argc < 2 )
	{
		show_help();
		return 1;
	}

	while ( -1 != ( arg_c = getopt( argc, argv, "c:vdh" ) ) )
	{
		switch ( arg_c )
		{
			case 'c':
				configPath = argv[ 2 ];
			break;
			case 'd':
				daemon = true;
			break;
			case 'h':
				show_help();
			break;
			case 'v':
				cout << "Demo 1.0" << endl;
				return 0;
			break;
			default:
				show_help();
				return 1;
			break;
		}
	}

	if ( daemon )
	{
		daemonize();
	}
	//设置信号处理
	set_demo_signal( MAIN_POLLER );
	//读取配置
	FirstIniReader conf_ini( configPath );
	const char *bind_ip = conf_ini.read_ini_char( "host", "0.0.0.0" );
	int port = conf_ini.read_ini_int( "port", 6666 );
	SUPER_KEY = strdup( conf_ini.read_ini_char( "super_key" ) );
	LOGIN_KEY = strdup( conf_ini.read_ini_char( "login_key" ) );

	//随机数种子
	struct timeval tp_time;
	gettimeofday( &tp_time, NULL );
	srand( tp_time.tv_usec );

	int main_fd = start_net_service( bind_ip, port );
	assert( main_fd >= 0 );
	const char *err_log_path = conf_ini.read_ini_char( "log" );
	//如果设置了日志文件, 输出到文件 否则直接输出
	if ( strlen( err_log_path ) )
	{
		NET_RUN_ERROR->setLogFile( new FirstLog( err_log_path, "error", EACH_DAY_ONE_FILE, false ) );
		OUT_LOG_STREAM->setLogFile( new FirstLog( err_log_path, "log", EACH_DAY_ONE_FILE, false ) );
	}
	fd_struct_t *main_fd_struct = MAIN_POLLER.create_fd_struct( main_fd, FD_TYPE_LISTEN );
	MAIN_POLLER.update_fd_event( main_fd_struct, EPOLL_CTL_ADD, EPOLLIN );
	OUT_LOG << "Server start!" << fin;
	conf_ini.unset();
	while ( true )
	{
		MAIN_POLLER.poll( EPOLL_WAIT_TIME );
	}
}

//设置im进程的信号处理
void set_demo_signal( FirstPoller &main_poller )
{
	sigset_t mask;
	sigemptyset( &mask );
	sigaddset( &mask, SIGINT );		//ctrl+c
	sigaddset( &mask, SIGTERM );	//kil
	sigaddset( &mask, SIGPIPE );	//socket管道破裂
	sigaddset( &mask, SIGHUP );		//用户session退出
	sigaddset( &mask, SIGUSR1 );	//自定义信号
	int re = sigprocmask( SIG_BLOCK, &mask, NULL );
	if ( -1 == re )
	{
		OUT_ERROR << "sigprocmask() failed!!" << fin;
	}
	main_poller.create_signal_fd( mask );
}
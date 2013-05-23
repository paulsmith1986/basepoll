#include "first_poll.h"
#include "poller.h"
#include "poll_base.h"
#include "poll_handler.h"

int daemonize()
{
	int fd;
	// Fork a new process
	switch( fork() )
	{
		case -1 :
			return -1;
		case 0 :
			break;
		default :
			exit( 1 );
	}

	if (setsid() == -1)
	{
		return -1;
	}

	// Redirect standard IO to null device
	fd = open( "/dev/null", O_RDWR, 0 );
	if ( fd )
	{
		( void ) dup2(fd, STDIN_FILENO);
		( void ) dup2(fd, STDOUT_FILENO);
		( void ) dup2(fd, STDERR_FILENO);
		if ( fd > STDERR_FILENO )
		{
			( void ) close( fd );
		}
	}
	return 0;
}

//开始网络进程
int start_net_service( const char *bind_ip, int sock_port )
{
	int MAIN_SOCKET_FD = 0;
	if ( ( MAIN_SOCKET_FD = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0)
	{
		OUT_ERROR << "Socket create Error" << fin;
		return -1;
	}
	//设置SO_REUSEADDR选项(服务器快速重起)
	int bReuseaddr = 1;
	setsockopt( MAIN_SOCKET_FD, SOL_SOCKET, SO_REUSEADDR, &bReuseaddr, sizeof( int ) );

	//发送缓冲区
	int nSendBuf = FIRST_SEND_CACHE;
	setsockopt( MAIN_SOCKET_FD, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBuf, sizeof( int ) );

	//接收缓冲区
	int nRevBuf = FIRST_SEND_CACHE;
	setsockopt( MAIN_SOCKET_FD, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRevBuf, sizeof( int ) );

	//把socket设置为非阻塞方式
	if ( !set_non_block( MAIN_SOCKET_FD ) )
	{
		OUT_ERROR << "Main_socket_fd can not set not block, exit!" << fin;
	}
	//struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	//设置与要处理的事件相关的文件描述符
	memset( &serveraddr, 0, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	inet_aton( bind_ip, &( serveraddr.sin_addr ) );	//htons(INI_PORT);
	serveraddr.sin_port = htons( sock_port );
	if ( bind( MAIN_SOCKET_FD, ( sockaddr * ) &serveraddr, sizeof( serveraddr ) ) < 0 )
	{
		OUT_ERROR << "Bind ip:" << bind_ip << " error!" << fin;
		return -1;
	}
	if ( listen( MAIN_SOCKET_FD, MAX_EVENTS ) < 0 )
	{
		OUT_ERROR << "Listen port:" << MAIN_SOCKET_FD << " Error" << fin;
		return -1;
	}
	return MAIN_SOCKET_FD;
}

//设置为非阻塞状态函数
bool set_non_block( const int sock )
{
	int opts;
	opts = fcntl( sock, F_GETFL );
	if ( opts < 0 )
	{
		OUT_ERROR << "set_not_block error F_GETFL:fd" << sock << fin;
		return false;
	}
	opts = opts | O_NONBLOCK;
	if ( fcntl( sock, F_SETFL, opts ) < 0 )
	{
		OUT_ERROR << "set_not_block error F_SETFL:fd" << sock << fin;
		return false;
	}
	return true;
}

/**
 * 创建协议数据包
 */
protocol_packet_t *new_proto_packet( uint32_t data_len )
{
	//最大支持的数据包
	if ( data_len > MAX_READ_PROTOCOL )
	{
		return NULL;
	}
	protocol_packet_t *tmp_pack = ( protocol_packet_t* )malloc( sizeof( protocol_packet_t ) );
	tmp_pack->pool_size = data_len;
	tmp_pack->data = ( char* )malloc( data_len );
	tmp_pack->pos = 0;
	tmp_pack->is_resize = 0;
	return tmp_pack;
}

/**
 * 删除协议数据包
 */
void delete_protocol_packet( protocol_packet_t *tmp_pack )
{
	free( tmp_pack->data );
	free( tmp_pack->data );
}

//唤醒
void wakeup( int fd )
{
	uint64_t one = 1;
	write( fd, &one, sizeof( one ) );
}

//设置倒计时
void set_timeout( int timer_fd, int ms )
{
	struct itimerspec new_value;
	struct itimerspec old_value;
	bzero( &new_value, sizeof new_value );
	bzero( &old_value, sizeof old_value );
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>( ms / 1000 );
	ts.tv_nsec = static_cast<long>( ms % 1000 ) * 1000 * 1000;
	new_value.it_value = ts;
	int ret = timerfd_settime( timer_fd, 0, &new_value, &old_value );
	if ( ret )
	{
		OUT_ERROR << "timerfd_settime()" << fin;
	}
}

#include "first_client.h"
/**
 * 连接服务器
 */
int first_net_connect( const char* host, int port )
{
	int socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
	struct sockaddr_in	sin;
	memset( &sin, 0, sizeof( struct sockaddr_in ) );
	sin.sin_family = AF_INET;
	sin.sin_port   = htons( port );
	struct in_addr tmp;
	inet_aton( host, &tmp );
	sin.sin_addr.s_addr = tmp.s_addr;
	struct timeval timeo = {2, 0};
	socklen_t len = sizeof( timeo );
	setsockopt( socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len );
	setsockopt( socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeo, len );
	//发送缓冲区
    int nSendBuf = NET_SEND_CACHE;
    setsockopt( socket_fd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBuf, sizeof( int ) );
	if ( connect( socket_fd, ( struct sockaddr * )&sin, sizeof( struct sockaddr_in ) ) < 0 )
	{
		return -1;
	}
	return socket_fd;
}


/**
 * 发送数据包( 注意：数据发送完后，如果传入包有过重新申请内存，数据会被释放 )
 */
void protocol_send_pack( int fd, protocol_result_t *send_pack )
{
	uint32_t total_data = send_pack->pos;
	uint32_t total_send = 0;
	packet_head_t *pack_head = ( packet_head_t* )send_pack->str;
	while ( total_data > 0 )
	{
		int ret = send( fd, send_pack->str + total_send, total_data, 0 );
		if ( ret < 0 )
		{
			if ( errno != EAGAIN )
			{
				close( fd );
				send_pack->error_code = PROTO_ERROR_SEND_FAIL;
				break;
			}
			ret = 0;
		}
		total_send += ret;
		total_data -= ret;
	}
	//检测内存释放
	if ( send_pack->is_resize )
	{
		free( send_pack->str );
	}
}

/**
 * 接收数据
 */
void protocol_recv_pack( int fd, protocol_packet_t *read_byte_array )
{
	uint32_t need_read_len = read_byte_array->max_pos - read_byte_array->pos;
	int total_read = 0;
	while ( need_read_len > 0 )
	{
		int read_re = read( fd, read_byte_array->data + read_byte_array->pos, need_read_len );
		if ( read_re < 0 )
		{
			if ( EAGAIN == errno || EINTR == errno )
			{
				usleep( 100 );
				continue;
			}
			//其它出错
			else
			{
				read_data_error( read_byte_array );
				break;
			}
		}
		else if ( 0 == read_re )
		{
			//被关闭
			if ( 0 == total_read )
			{
				read_data_error( read_byte_array );
				close( fd );
				break;
			}
		}
		else
		{
			total_read += read_re;
			read_byte_array->pos += read_re;
			need_read_len -= read_re;
			if ( 0 == need_read_len )
			{
				if ( sizeof( packet_head_t ) == read_byte_array->pos )
				{
					packet_head_t *pack_head = ( packet_head_t* )read_byte_array->data;
					need_read_len = pack_head->size;
					uint32_t new_packet_size = need_read_len + sizeof( packet_head_t );
					//空间不够
					if ( new_packet_size > read_byte_array->pool_size )
					{
						//超过最大读取量
						if ( new_packet_size > MAX_READ_PACK_SIZE )
						{
							read_data_error( read_byte_array );
							break;
						}
						protocol_packet_resize( read_byte_array, new_packet_size );
					}
					read_byte_array->max_pos = new_packet_size;
				}
			}
		}
	}
}
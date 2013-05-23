#include "task.h"

/**
 * 测试 直接发送然后直接获取结果
 */
void test_ping()
{
	//制作发送的数据包 ping_pack
	encode_ping( ping_pack );
	//将数据包发送出去
	protocol_send_pack( SOCKET_FD, &ping_pack );
	if ( ping_pack.error_code > 0 )
	{
		OUT_LOG << "发送数据出错!" << fin;
		return;
	}
	//从socket里读数据 并 解析 为变量 ping_re
	read_and_decode_ping_re( SOCKET_FD, ping_re );
	//需要判断反解是否出错
	if ( is_decode_error() )
	{
		OUT_LOG << "解析数据出错!" << fin;
		return;
	}
	//打印结果
	print_ping_re( ping_re );
}

/**
 * 测试任务派发模式(可异步)
 */
void test_dispatch()
{
	//一次发几个包..服务器返回的数据经过统一函数处理
	//请求数据包_1
	proto_test_add_t add_arg;
	add_arg.a = 1818;
	add_arg.b = 1616;
	encode_test_add( send_pack, &add_arg );
	protocol_send_pack( SOCKET_FD, &send_pack );
	OUT_LOG << "Send pack test_add" << fin;

	//请求数据包_2
	const char *email_list[ 5 ];
	email_list[ 0 ] = "18844626@qq.com";
	email_list[ 1 ] = "bluebirdhsz@gmail.com";
	email_list[ 2 ] = "188332626@qq.com";
	email_list[ 3 ] = "188001866@qq.com";
	email_list[ 4 ] = "huangsz8@qq.com";
	proto_list_varchar_t email_data;
	email_data.len = 5;
	email_data.item = (char**)email_list;
	proto_contact_arr_arr_t contact;
	contact.QQ = (char*)"18844626";
	const char* mobile = "18018684626";
	strcpy( contact.mobile, mobile );
	contact.email = &email_data;
	proto_role_info_t role_info;
	const char* name = (char*)"文成";
	strcpy( role_info.name, name );
	role_info.age = 30;
	role_info.contact = &contact;
	//打包
	encode_role_info( role_pack, &role_info );
	protocol_send_pack( SOCKET_FD, &role_pack );

	OUT_LOG << "Send pack role_info" << fin;

	//请求数据_3
	proto_car_info_t car_a;
	proto_car_info_t car_b;
	proto_car_info_t car_c;
	car_a.car_id = 123;
	car_a.color = (char*)"red";
	car_a.owner = (char*)"Zhang sanxing";

	car_b.car_id = 456;
	car_b.color = (char*)"green";
	car_b.owner = (char*)"Wang mazi";

	car_c.car_id = 789;
	car_c.color = (char*)"White";
	car_c.owner = (char*)"Guang heshang";

	proto_car_list_t car_arg;
	car_arg.car_a = &car_a;
	car_arg.car_b = &car_b;
	car_arg.car_c = &car_c;
	encode_car_list( car_pack, &car_arg );
	protocol_send_pack( SOCKET_FD, &car_pack );

	OUT_LOG << "Send pack car" << fin;

	read_packs();
}

/**
 * 模拟客户端循环收数据过程
 */
void read_packs()
{
	while ( true )
	{
		const int buff_pool_size = 16 * 1024;
		char STACK_READ_PACKET_DATA[ buff_pool_size ];
		protocol_packet_t stack_read_packet;
		stack_read_packet.data = STACK_READ_PACKET_DATA;
		stack_read_packet.pos = 0;
		stack_read_packet.max_pos = sizeof( packet_head_t );
		stack_read_packet.pool_size = buff_pool_size;
		stack_read_packet.is_resize = 0;
		protocol_recv_pack( SOCKET_FD, &stack_read_packet );
		//当出错的时候 stack_read_packet.pos 会被置为0
		if ( 0 != stack_read_packet.pos )
		{
			stack_read_packet.pos = sizeof( packet_head_t );
			do_request_task( &stack_read_packet );
		}
		sleep( 1 );
	}
}

/**
 * 心跳
 */
void request_ping_re( proto_ping_re_t *req )
{
}

/**
 * pack_id: 2 加法返回
 */
void request_test_add_re( proto_test_add_re_t *req_pack )
{
	print_test_add_re( req_pack );
}

/**
 * pack_id: 3 测试私有struct
 */
void request_role_info_re( proto_role_info_re_t *req_pack )
{
	print_role_info_re( req_pack );
}

/**
 * pack_id: 5 请求
 */
void request_car_list_re( proto_car_list_re_t *req_pack )
{
	print_car_list_re( req_pack );
}
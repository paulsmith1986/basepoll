#include "task_server.h"
#include "first_poll.h"
#include "poller.h"
/**
 * 心跳
 */
void request_ping( fd_struct_t *fd_info )
{
	proto_ping_re_t out_pack;
	out_pack.result = 0;
	encode_ping_re( result_pack, &out_pack );
	send_protocol( fd_info, result_pack );
}

/**
 * pack_id: 1 测试加法
 */
void request_test_add( fd_struct_t *fd_info, proto_test_add_t *req_pack )
{
	print_test_add( req_pack );
	proto_test_add_re_t add_result;
	add_result.re = req_pack->a + req_pack->b;
	encode_test_add_re( send_pack, &add_result );
	send_protocol( fd_info, send_pack );
}

/**
 * pack_id: 3 测试私有struct
 */
void request_role_info( fd_struct_t *fd_info, proto_role_info_t *req_pack )
{
	print_role_info( req_pack );
	//将收到的数据直接返回
	proto_role_info_re_t re;
	strcpy( re.name, req_pack->name );
	re.age = req_pack->age;
	proto_contact_arr_t con_re;
	con_re.QQ = req_pack->contact->QQ;
	con_re.email = req_pack->contact->email;
	strcpy( con_re.mobile, req_pack->contact->mobile );
	re.contact = &con_re;
	encode_role_info_re( send_pack, &re );
	send_protocol( fd_info, send_pack );
}

/**
 * pack_id: 5 请求
 */
void request_car_list( fd_struct_t *fd_info, proto_car_list_t *req_pack )
{
	print_car_list( req_pack );
	proto_car_list_re_t re;
	re.car_a = req_pack->car_a;
	re.car_b = req_pack->car_b;
	re.car_c = req_pack->car_c;
	encode_car_list_re( send_pack, &re );
	send_protocol( fd_info, send_pack );
}
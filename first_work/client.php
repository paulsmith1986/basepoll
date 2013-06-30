<?php
require '/data/wwwroot/tool/build/proto_simulate.php';

/**
 * 测试登录
 */
function login ()
{
	$socket = socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
	socket_connect( $socket, '192.168.128.129', 9999 );
	$data = proto_simulate_game_login();
	$pack_data = proto_pack_game_login( $data, false );
	socket_write( $socket, $pack_data, strlen( $pack_data ) );
	usleep( 1000 );
}

function api_im_join_server ( $role )
{

	$str =  'aaaaaa|'. $role[ 'role_id' ] .'|'. $role[ 'level' ] .'|'. $role[ 'job' ] .'|'. $role[ 'role_name' ] .'|bbbbbb';
	echo $str, "\n";
	return substr( md5( $str ), 0, 16 );
}

/**
 * 加入聊天
 */
function join_im ()
{
	$data = proto_simulate_im_join_server();
	$data[ 'join_key' ] = api_im_join_server( $data );
	print_r( $data );
	$socket = socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
	socket_connect( $socket, '192.168.2.10', 9999 );
	$pack_data = proto_pack_im_join_server( $data, false );
	socket_write( $socket, $pack_data, strlen( $pack_data ) );
	$re = socket_read( $socket, 1000 );
	$re = substr( $re, 6 );
	$join_re = proto_unpack_im_join_server_re( $re );
	print_r( $join_re );
	sleep( 10 );
}
join_im();
<?php
require '/data/wwwroot/tool/build/so/proto_simulate.php';

/**
 * 测试登录
 */
function login ()
{
	$socket = socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
	socket_connect( $socket, '192.168.128.129', 9999 );
	$data = proto_simulate_account_login();
	var_dump( $data );
	$pack_data = proto_pack_account_login( $data, false );
	socket_write( $socket, $pack_data, strlen( $pack_data ) );
	usleep( 1000 );
}

login();
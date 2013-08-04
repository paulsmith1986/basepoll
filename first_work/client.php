<?php
require '/data/wwwroot/tool/build/proto_simulate.php';

function test_1 ()
{
	$data = array(
		'auth_key'	=> 'gqNzdHLaACmDqmFjY291bnRfaWQMqHVzZXJuYW1lqGJsdWViaXJkpHRpbWXOUf3EPqRoYXNo2gAgZDM2MzY0NmFiMjZiNTBiNjQ1OTgwMjQ4MmM3NzY3NWM=',
		'flag' => 1
	);
	$send = proto_pack_game_login( $data, false );
	$socket = socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
	socket_connect( $socket, '127.0.0.1', 9999 );
	$re = socket_write( $socket, $send, strlen( $send ) );
	while ( true )
	{
		$read = socket_read( $socket, 65536 );
		var_dump( strlen( $read ) );
	}
}
test_1();
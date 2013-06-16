<?php
function test_1 ()
{
	$fd = first_socket_fd( "192.168.128.129", 9999 );
	first_send_pack( $fd, 25003, array( 'fpm_id' => 20 ) );
}

test_1();
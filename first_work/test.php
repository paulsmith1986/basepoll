<?php
function test_1 ()
{
	$fd = first_socket_fd( "192.168.128.129", 9999 );
	first_join_server( $fd, 'bbbbbb' );
	while ( true )
	{
		$re = first_poll();
		var_dump( $re );
	}
}

test_1();
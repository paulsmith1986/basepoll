<?php
function test_1 ()
{
	$fd = first_socket_fd( "192.168.128.129", 8888 );
	while ( true )
	{
		$re = first_poll();
		var_dump( $re );
	}
}

test_1();
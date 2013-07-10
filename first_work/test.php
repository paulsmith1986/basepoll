<?php
function test_1 ()
{
	$fd = first_socket_fd( "192.168.128.129", 9999 );
	first_send_pack( $fd, 25003, array( 'fpm_id' => 20 ) );
}

function test_2 ()
{
	var_dump( first_getpid() );
}

function test_3 ()
{
	$re = first_pack_data( array( array( '101', array( 'error_no' => 10, 'error_arg' => array(), 'error_msg' => 'aaa' ) ), array( '101', array( 'error_no' => 10, 'error_arg' => array(), 'error_msg' => 'aaa' ) ),array( '101', array( 'error_no' => 10, 'error_arg' => array(), 'error_msg' => 'aaa' ) ), array( '101', array( 'error_no' => 10, 'error_arg' => array(), 'error_msg' => 'aaa' ) ), array( '101', array( 'error_no' => 10, 'error_arg' => array(), 'error_msg' => 'aaa' ) ),array( '101', array( 'error_no' => 10, 'error_arg' => array(), 'error_msg' => 'aaa' ) ) ) );
	var_dump( $re );
}
test_3();
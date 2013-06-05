<?php
/**
 * 测试PHP版本的战斗
 */
function test_php_fight( )
{

	$attack_side = array( array( 0 => make_role_fight(), 2 => make_role_fight(), 3 => make_role_fight(), 4 => make_role_fight(), 5 => make_role_fight() ) );
	$defence_side = array( array( 0 => make_role_fight(), 2 => make_role_fight(), 3 => make_role_fight(), 4 => make_role_fight(), 5 => make_role_fight() ) );
	$re = yile_fight( $attack_side, $defence_side, 500, 1 );
	return $re;
}
function make_role_fight ()
{
	$role_fight = array(
		'level'			=> 10,							//等级
		'life_max'		=> mt_rand( 10000, 20000 ),		//最大生命
		'hr'			=> mt_rand( 5, 20 ),			//命中率
		'dr'			=> mt_rand( 5, 10 ),			//躲闪率
		'ds'			=> mt_rand( 5, 20 ),			//暴击率
		'attack'		=> mt_rand( 200, 600 ),			//攻击力
		'attack_type'	=> mt_rand( 0, 1 ),				//攻击类型
		'defence'		=> mt_rand( 60, 200 ),			//防御力
		'suck'			=> mt_rand( 0, 3 ),				//吸血
		'damage_return'	=> mt_rand( 0, 2 ),				//伤害反弹
		'avoid_ds'		=> 0,							//免暴率
		'avoid_harm'	=> 0,							//免伤率
		'force'			=> mt_rand( 50, 100 ),			//武力
		'IQ'			=> mt_rand( 50, 100 ),			//智力
		'skill'			=> '',							//使用技能
		'ext_effect'	=> '',							//额外效果
		'ext_buff'		=> '',							//额外状态
		'buff_add_time'	=> '',							//状态时间加成
		'swf'			=> 0,							//模型ID
		'name'			=> 'test_'. mt_rand( 100, 1000 ),
	);
	return $role_fight;
}
function test_1 ()
{
	yile();
	$t1 = microtime( true );
	for ( $i = 0; $i < 1000; ++$i )
	{
		$re = test_php_fight();
		file_put_contents( "/data/wwwroot/new_code/so/im/fight_data/data_". $i .".dat", $re[ 'data' ] );
	}
	$t2 = microtime( true );
	echo "\n Usetime:", ( $t2 - $t1 ), "\n";
}
function test_2 ()
{
	$data_2 = array(
		'item_a'		=> 1801868,
		'item_b'		=> 4626,
		'item_c'		=> 65533,
		'item_d'		=> $data,
		//'test_f'		=> array( array( array( $data, $data, $data ), array( $data, $data ) ), array( array( $data, $data, $data ) ), array( array( $data, $data, $data ), array( $data, $data ) ) ),
		'test_f'		=> array( array( array( $data, $data, $data ), array( $data, $data ) ), array( array( $data, $data, $data ) ), array( array( $data, $data, $data ), array( $data, $data ) ) ),
	);

	$out_data = array( array( 14, $data_2 ) );
	$json = yile_encode( $out_data );
	//echo "json_len:", strlen( $json ), "\n";
	$str = yile_pack( $out_data );
}
function test_3 ()
{
	$re = yile_im_connect( 5, '192.168.128.128', 6666, 'bbbbbb' );
	var_dump( $re );
	$ping_re = yile_im_ping( 5 );
	var_dump( $ping_re );
}
function test_4 ()
{
	$str = yile_pack( 10007, array( 'role_id' => 32, 'combat_id' => 0, 'data' => 'asdfasd' ) );
	echo strlen( $str );
	$re = yile_unpack( $str );
	print_r( $re );
}
function test_5 ()
{
	$int_arr1 = array( 100, 200, 300, 400 );
	$int_arr2 = array( 20, 30, 40, 50 );
	$str_arr2 = array(
		"Huang",
		"zhang",
		"xingchao",
	);
	$str_arr1 = array(
		"file_put_contents",
		"wowowowohahaha",
		"Hello",
	);
	$data = array(
		'data_1'		=> 192,
		'data_8'		=> -128,
		'data_2'		=> "bluebird",
		'data_3'		=> $str_arr1,
		'data_4'		=> $int_arr1,
		'data_5'		=> array( $str_arr1, $str_arr2 ),
		'data_6'		=> array( $int_arr1, $int_arr2 ),
		'data_7'		=> 'The end',
	);

	$re = yile_im_connect( 5, '192.168.128.128', 6666, 'bbbbbb' );
	$send_arr = array(
		'a'				=> -500,
		'b'				=> 127,
		'c'				=> 'asdfasdfasdfasdf',
		'd'				=> $data,
		'e'				=> amf_encode( $data, 1 )
	);
	yile_im_push( $re, 10087, $send_arr );
}

function test_6 ()
{
	$yac = new Yac();
	$arr = array( 1,3,3,4,5,6 );
	$yac->set("foo", $arr );
	$re = $yac->get( 'foo' );
	var_dump( $re );
}
test_6();
#include "fight.h"

int main()
{
	global_t global_vars;
	printf( "sizeof fight_unit_t:%d\n", sizeof( fight_unit_t ) );
	srand( time( 0 ) );
	combat_info_t combat_info;
	memset( &combat_info, 0, sizeof( combat_info_t ) );
	memset( &global_vars, 0, sizeof( global_t ) );
	combat_info.fight_result = (char*)malloc( FIGHT_RESULT_CHAR );
	combat_info.result_pos_max = FIGHT_RESULT_CHAR;
	global_vars.max_ext_buff = 1;
	global_vars.max_ext_effect = 1;
	global_vars.max_fight_unit = 10;
	global_vars.max_fight_buff = 1;
	global_vars.max_use_skill = 1;
	//初始化技能公式
	init_skill_formula();
	//初始化战斗
	init_first_fight();
	int read_re = read_fight_config_dat_file( (char*)"skill.dat" );
	#ifdef FIRST_DEBUG
	printf( "读取配置文件  re:%d\n", read_re );
	#endif
	if ( read_re < 0 )
	{
		return -1;
	}
	combat_info.global_var = &global_vars;
	int i = 0;
	for ( i = 0; i < SIDE_MEMBER; ++i )
	{
		fight_unit_t *new_unit = create_fight_unit( &combat_info );
		init_member( new_unit, ATTACK_SIDE, i, &combat_info );
		join_member( new_unit, &combat_info );
	}
	for ( i = 0; i < SIDE_MEMBER; ++i )
	{
		fight_unit_t *new_unit = create_fight_unit( &combat_info );
		init_member( new_unit, DEFENCE_SIDE, i, &combat_info );
		join_member( new_unit, &combat_info );
	}
	combat_info.max_second = 500;
	while ( 0 == combat_info.is_over )
	{
		once_second( &combat_info );
	}
	FILE *re_file = fopen( "result.dat", "w" );
	fwrite( combat_info.fight_result, combat_info.result_pos, 1, re_file );
	fclose( re_file );
	printf( "战斗结束 is_over = %d\n", combat_info.is_over );
	return 0;
}

use_skill_t *make_skill( int skill_type, combat_info_t *combat_info )
{
	int vk_skill_id[ 77 ] = {1,2,3,5,6,8,11,16,26,31,35,46,51,52,53,55,56,58,61,65,75,79,83,94,99,100,101,103,104,106,109,112,114,120,121,122,125,130,134,141,144,149,150,151,153,154,156,159,164,174,178,182,191,196,197,198,199,200,202,203,204,205,206,208,209,210,211,212,213,214,216,217,219,220,222,226,227};
	int ag_skill_id[ 35 ] = {9,17,21,30,36,41,50,59,66,71,78,84,89,98,107,115,119,129,135,140,148,157,165,169,177,183,188,195,201,207,215,223,224,225,228};
	int skill_id;
	if ( 1 == skill_type )
	{
		skill_id = vk_skill_id[ first_rand( 0, 74 ) ];
	}
	else
	{
		skill_id = ag_skill_id[ first_rand( 0, 34 ) ];
	}
	skill_t *skill_info = find_skill_info( skill_id );
	use_skill_t *use_sk = create_use_skill( combat_info );
	use_sk->skill_id = skill_id;
	use_sk->skill_level = first_rand( 3, 5 );
	use_sk->add_hr = 0;
	use_sk->skill_cost = 50;
	use_sk->cd_use_time = 0;
	use_sk->skill_info = skill_info;
	return use_sk;
}

/**
 * 初始化成员
 */
void init_member( fight_unit_t *member, int side, int cell_id, combat_info_t *combat_info )
{
	memset( member, 0, sizeof( fight_unit_t ) );
	member->cell_id = cell_id;
	member->side = side;
	member->level = first_rand( 1, 100 );
	member->life_max = first_rand( 10000, 20000 );
	member->life_now = member->life_max;
	member->hit_ration = first_rand( 5, 50 );
	member->dodge_ration = first_rand( 5, 50 );
	member->ds_ration = first_rand( 5, 50 );
	member->attack_max = first_rand( 300, 500 );
	member->attack_min = first_rand( 200, 400 );
	member->ds_damage = first_rand( 5, 20 );
	member->defence = first_rand( 100, 200 );
	member->suck = first_rand( 0, 3 );
	member->damage_return = first_rand( 0, 3 );
	member->tenacity = first_rand( 5, 20 );
	member->avoid_harm = first_rand( 5, 20 );
	member->str = first_rand( 100, 300 );
	member->dex = first_rand( 100, 300 );
	member->vit = first_rand( 100, 300 );
	member->base_speed = first_rand( 10, 20 );
	member->ca_ration = first_rand( 50, 90 );
	member->vigour_skill = make_skill( 1, combat_info );
	member->anger_skill = make_skill( 2, combat_info );
	ext_buff_t *tmp_buff = create_ext_buff( combat_info );
	tmp_buff->next = NULL;
	tmp_buff->level = 2;
	tmp_buff->last_time = 3;
	tmp_buff->need_skill = 0;
	tmp_buff->buff_info = find_buff_info( 10801 );
	member->ext_object_buff = tmp_buff;
}

/**
 * 成员阵亡的回调函数
 * @param	combat_info		战斗信息
 * @param	side			立场
 * @param	cell_id			格子编号
 */
void member_dead_callback( combat_info_t *combat_info, int side, int cell_id )
{

}

/**
 * 尝试切换下一组队员
 * @param	combat_info		战斗信息
 */
void combat_try_change_team( combat_info_t *combat_info )
{

}
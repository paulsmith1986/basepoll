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
	global_vars.max_fight_unit = 1;
	global_vars.max_fight_buff = 1;
	global_vars.max_use_skill = 1;
	//初始化技能公式
	init_skill_formula();
	//初始化战斗
	init_first_fight();
	int read_re = read_fight_config_dat_file( "skill.dat" );
	#ifdef FIRST_DEBUG
	printf( "读取配置文件  re:%d\n", read_re );
	#endif
	if ( read_re < 0 )
	{
		return -1;
	}
	#ifdef FIRST_DEBUG
	read_re = debug_read_config_skill_name( "skill_name.dat" );
	printf( "读取技能的状态的名称 re:%d\n", read_re );
	if ( read_re < 0 )
	{
		return -1;
	}
	#endif

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
	combat_info.max_second = 50;

	printf( "战斗结束 is_over = %d\n", combat_info.is_over );
	return 0;
}

/**
 * 初始化成员
 */
void init_member( fight_unit_t *member, int side, int cell_id, combat_info_t *combat_info )
{
	memset( member, 0, sizeof( fight_unit_t ) );
	/*member->cell_id = cell_id;
	member->side = side;
	member->level = first_rand( 1, 100 );
	member->life_max = first_rand( 10000, 20000 );
	member->life_now = member->life_max;
	member->anger_max = first_rand( 100, 200 );
	member->hit_rating = first_rand( 5, 50 );
	member->dodge_ration = first_rand( 5, 50 );
	member->deadly_strike = first_rand( 5, 50 );
	member->attack_max = first_rand( 300, 400 );
	member->attack_min = first_rand( 200, 300 );
	member->attack_type = first_rand( 1, 2 );
	member->attack_area = first_rand( 1, 5 );
	member->ds_damage = first_rand( 5, 20 );
	member->defence = first_rand( 100, 200 );
	member->suck = first_rand( 0, 3 );
	member->damage_return = first_rand( 0, 3 );
	member->avoid_ds = first_rand( 5, 20 );
	member->avoid_harm = first_rand( 5, 20 );
	member->IQ = first_rand( 50, 100 );
	member->force = first_rand( 50, 100 );
	member->attack_num = first_rand( 1, 3 );
	ext_effect_t *eff = create_ext_effect( combat_info );
	eff->effect_id = first_rand( 1, 25 );
	eff->effect_value = first_rand( 50, 500 );
	eff->need_skill = 0;
	eff->next = NULL;
	int skill_id = 1;
	skill_t *tmp_skill_info = find_skill_info( skill_id );
	if ( NULL == tmp_skill_info )
	{
		member->anger_skill = NULL;
	}
	else
	{
		use_skill_t *tmp_use_skill = create_use_skill( combat_info );
		tmp_use_skill->skill_id = skill_id;
		tmp_use_skill->skill_level = 3;
		tmp_use_skill->add_hr = 0;
		member->skill_level = tmp_use_skill->skill_level;
		tmp_use_skill->use_anger = first_rand( 20, 60 );
		tmp_use_skill->skill_info = tmp_skill_info;
		member->anger_skill = tmp_use_skill;
	}
	member->attack_object_eff[ AIM_DIRECT ] = eff;
	*/
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
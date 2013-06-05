#include "new_fight.h"
//全局战斗数据
global_t FIGHT_GLOBAL;
/**
 * 战斗主入口
 */
PHP_FUNCTION ( yile_fight )
{
	zval *z_attack;			//传入的攻击方
	zval *z_defence;		//传入的防守方
	long fight_max_time;	//最大战斗时间
	long win_type;			//平局时胜负规则：1:防守方胜利 2:进攻方胜利 3:按受损百分比，相同比率下进攻方胜利 4：按受损百分比，相同比率下防守方胜利 5：按受损百分比，比率相同情况下平局
	if ( FAILURE == zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "aall", &z_attack, &z_defence, &fight_max_time, &win_type ) )
	{
		return;
	}
	php_combat_info_t php_combat_info;
	//战斗信息
	combat_info_t combat_info;
	memset( &combat_info, 0, sizeof( combat_info_t ) );
	//优化，使用栈内内存
	char TEMP_FIGHT_POOL[ FIGHT_RESULT_CHAR * 2 ];
	combat_info.result_pos_max = FIGHT_RESULT_CHAR * 2;
	combat_info.fight_result = TEMP_FIGHT_POOL;
	combat_info.global_var = &FIGHT_GLOBAL;
	combat_info.ext_data = &php_combat_info;
	combat_info.max_second = fight_max_time;
	php_combat_info.all_member[ 0 ] = Z_ARRVAL_P( z_attack );
	php_combat_info.all_member[ 1 ] = Z_ARRVAL_P( z_defence );
	//获取攻击方第一队战斗成员
	get_fight_round_hash( &combat_info, ATTACK_SIDE );
	//获取防守方第一队战斗成员
	get_fight_round_hash( &combat_info, DEFENCE_SIDE );
	while ( 0 == combat_info.is_over )
	{
		fight_round( &combat_info );
	}
	array_init( return_value );
	add_assoc_long( return_value, "result", combat_info.result );
	add_assoc_stringl( return_value, "data", combat_info.fight_result, combat_info.result_pos, 1 );
	if ( combat_info.is_free_result )
	{
		free( combat_info.fight_result );
	}
}

/**
 * 获取每一轮出场人员的hash_table
 * @param	combat_info		战斗信息
 * @param	side			战斗立场
 */
void get_fight_round_hash( combat_info_t *combat_info, int side )
{
	int team_id = combat_info->side_team[ side ];
	php_combat_info_t *php_combat_info = combat_info->ext_data;
	zval **team_member_zval;
	//如果存在小队成员
	if ( FAILURE != zend_hash_index_find( php_combat_info->all_member[ side ], team_id, (void**)&team_member_zval ) )
	{
		php_combat_info->team_member[ side ] = Z_ARRVAL_PP( team_member_zval );
		combat_info->side_team[ side ] = team_id + 1;
		int cell_id;
		php_printf( "Init team\n" );
		for ( cell_id = 0; cell_id < SIDE_MEMBER; ++cell_id )
		{
			php_printf( "check for pos:%d\n", cell_id );
			fight_unit_t *new_member = init_fight_member( side, cell_id, php_combat_info->team_member[ side ], combat_info );
			if ( NULL != new_member )
			{
				printf( "New member init ok\n" );
				join_member( new_member, combat_info );
				printf( "New member join fight ok\n" );
			}
		}
	}
	else
	{
		php_combat_info->team_member[ side ] = NULL;
	}
}

/**
 * 初始化战斗成员
 * @param	side			立场
 * @param	cell_id			格子
 * @param	team_arr		该队的所有成员
 * @param	combat_info		战斗信息
 */
fight_unit_t *init_fight_member( int side, int team_cell_id, HashTable *team_arr, combat_info_t *combat_info )
{
	zval **member_zval;
	//如果该位置不存在人
	if ( FAILURE ==zend_hash_index_find( team_arr, team_cell_id, (void**)&member_zval ) )
	{
		return NULL;
	}

	fight_unit_t *fight_unit = create_fight_unit( combat_info );
	/*HashTable *member_hash = Z_ARRVAL_PP( member_zval );
	//等级
	fight_unit->level = read_int_from_array( member_hash, php_key_arg( "level" ) );
	//气血上限
	fight_unit->life_max = read_int_from_array( member_hash, php_key_arg( "life_max" ) );
	fight_unit->life_now = fight_unit->life_max;
	//命中率
	fight_unit->hit_rating = read_int_from_array( member_hash, php_key_arg( "hr" ) );
	//躲闪率
	fight_unit->dodge_ration = read_int_from_array( member_hash, php_key_arg( "dr" ) );
	//暴击率
	fight_unit->deadly_strike = read_int_from_array( member_hash, php_key_arg( "ds" ) );
	//攻击力
	int attack = read_int_from_array( member_hash, php_key_arg( "attack" ) );
	fight_unit->attack_max = attack * 1.1;
	fight_unit->attack_min = attack * 0.9;
	//攻击类型
	fight_unit->attack_type = read_int_from_array( member_hash, php_key_arg( "attack_type" ) );
	//防御
	fight_unit->defence = read_int_from_array( member_hash, php_key_arg( "defence" ) );
	//吸血率
	fight_unit->suck = read_int_from_array( member_hash, php_key_arg( "suck" ) );
	//伤害反弹
	fight_unit->damage_return = read_int_from_array( member_hash, php_key_arg( "damage_return" ) );
	//免暴率
	fight_unit->avoid_ds = read_int_from_array( member_hash, php_key_arg( "avoid_ds" ) );
	//免伤率
	fight_unit->avoid_harm = read_int_from_array( member_hash, php_key_arg( "avoid_harm" ) );
	//武力
	fight_unit->force = read_int_from_array( member_hash, php_key_arg( "force" ) );
	//智力
	fight_unit->IQ = read_int_from_array( member_hash, php_key_arg( "IQ" ) );
	//使用技能字符串
	char *tmp_fight_char = read_string_from_array( member_hash, php_key_arg( "skill" ) );
	if ( NULL != tmp_fight_char )
	{
		parse_skill_use_str( fight_unit, combat_info, tmp_fight_char, strlen( tmp_fight_char ) );
	}
	//额外被动技能效果
	tmp_fight_char = read_string_from_array( member_hash, php_key_arg( "ext_effect" ) );
	if ( NULL != tmp_fight_char )
	{
		parse_ext_effect_str( fight_unit, combat_info, tmp_fight_char, strlen( tmp_fight_char ) );
	}
	//额外被动技能状态
	tmp_fight_char = read_string_from_array( member_hash, php_key_arg( "ext_buff" ) );
	if ( NULL != tmp_fight_char )
	{
		parse_ext_buff_str( fight_unit, combat_info, tmp_fight_char, strlen( tmp_fight_char ) );
	}
	//模型Id
	fight_unit->swf_id = read_int_from_array( member_hash, php_key_arg( "swf" ) );
	tmp_fight_char = read_string_from_array( member_hash, php_key_arg( "name" ) );
	//无名称 或者 名称太长
	if ( NULL == tmp_fight_char || strlen( tmp_fight_char ) >= MEMBER_NAME_LEN )
	{
		tmp_fight_char = "Anonymous";
	}
	int name_len = strlen( tmp_fight_char );
	memcpy( fight_unit->name, tmp_fight_char, name_len );
	fight_unit->name[ name_len ] = '\0';
	//立场
	fight_unit->side = side;
	//这里要取余
	fight_unit->cell_id = team_cell_id % SIDE_MEMBER;
	//这里表示在小队中的位置
	fight_unit->team_cell_id = team_cell_id;
	*/
	return fight_unit;
}

/**
 * 成员阵亡的回调函数
 * @param	combat_info		战斗信息
 * @param	side			立场
 * @param	team_cell_id	在小队中的格子编号
 */
void member_dead_callback( combat_info_t *combat_info, int side, int team_cell_id )
{
	php_combat_info_t *php_combat_info = ( php_combat_info_t* )combat_info->ext_data;
	fight_unit_t *new_member = init_fight_member( side, team_cell_id + SIDE_MEMBER, php_combat_info->team_member[ side ], combat_info );
	if ( NULL != new_member )
	{
		join_member( new_member, combat_info );
	}
}

/**
 * 尝试切换下一组队员
 * @param	combat_info		战斗信息
 */
void combat_try_change_team( combat_info_t *combat_info )
{
	int side = ( 0 == combat_info->side_num[ ATTACK_SIDE ] ) ? ATTACK_SIDE : DEFENCE_SIDE;
	get_fight_round_hash( combat_info, side );
}

/**
 * 初始化全局数据
 */
void init_global_vars()
{
	memset( &FIGHT_GLOBAL, 0, sizeof( FIGHT_GLOBAL ) );
	FIGHT_GLOBAL.max_ext_buff = 24;				//最大的ext_buff数量
	FIGHT_GLOBAL.max_ext_effect = 24;			//最大的ext_effect数量
	FIGHT_GLOBAL.max_fight_unit = 12;			//最大的fight_unit数量
	FIGHT_GLOBAL.max_fight_buff = 50;			//最大的fight_buff数量
	FIGHT_GLOBAL.max_use_skill = 12;			//当前的use_skill数量
}
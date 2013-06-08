#ifndef FIRST_FIGHT_FUNC_H
#define FIRST_FIGHT_FUNC_H
#include "php.h"
#include "standard/php_string.h"
#include "ext/standard/php_smart_str.h"
#include "first.h"
#include "first_fight.h"
#include "skill_effect.h"
#include "struct_pool.h"
#include "skill_formula.h"

typedef struct php_combat_info_t php_combat_info_t;

struct php_combat_info_t{
	HashTable				*all_member[ 2 ];			//双方所有战斗成员(PHP数据结构)
	HashTable				*team_member[ 2 ];			//当前小队的成员
};

/**
 * 获取每一轮出场人员的hash_table
 * @param	combat_info		战斗信息
 * @param	side			战斗立场
 */
void get_fight_round_hash( combat_info_t *combat_info, int side );

/**
 * 初始化战斗成员
 * @param	side			立场
 * @param	cell_id			格子
 * @param	team_arr		该队的所有成员
 * @param	combat_info		战斗信息
 */
fight_unit_t *init_fight_member( int side, int team_cell_id, HashTable *team_arr, combat_info_t *combat_info );

/**
 * 初始化全局数据
 */
void init_global_vars();
#endif
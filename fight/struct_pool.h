#ifndef FIRST_FIGHT_STRUCT_POOL_H
#define FIRST_FIGHT_STRUCT_POOL_H
#include <stdlib.h>
#include "first_fight.h"
/**
 * 新创建一个fight-buff
 * @param	combat_info		战斗信息
 */
fight_buff_t *create_fight_buff( combat_info_t *combat_info );

/**
 * 回收一个fight_buff
 * @param	tmp_buff		状态指针
 * @param	combat_info		战斗信息
 */
void destroy_fight_buff( fight_buff_t *tmp_buff, combat_info_t *combat_info );

/**
 * 新创建一个fight_unit_t
 * @param	combat_info		战斗信息
 */
fight_unit_t *create_fight_unit( combat_info_t *combat_info );

/**
 * 回收一个fight_unit_t
 * @param	tmp_unit		unit指针
 * @param	combat_info		战斗信息
 */
void destroy_fight_unit( fight_unit_t *tmp_unit, combat_info_t *combat_info );

/**
 * 新创建一个use_skill_t
 * @param	combat_info		战斗信息
 */
use_skill_t *create_use_skill( combat_info_t *combat_info );

/**
 * 回收一个use_skill_t
 * @param	tmp_skill		use_skill指针
 * @param	combat_info		战斗信息
 */
void destroy_use_skill( use_skill_t *tmp_skill, combat_info_t *combat_info );

/**
 * 创建一个被动技能效果
 * @param	combat_info		战斗信息
 */
ext_effect_t *create_ext_effect( combat_info_t *combat_info );

/**
 * 回收一个ext_effect完整链
 * @param	tmp_eff			效果指针
 * @param	combat_info		战斗信息
 */
void destroy_ext_effect_all( ext_effect_t *tmp_eff, combat_info_t *combat_info );

/**
 * 创建一个被动技能状态
 * @param	combat_info		战斗信息
 */
ext_buff_t *create_ext_buff( combat_info_t *combat_info );

/**
 * 回收一个ext_buff完整链
 * @param	tmp_eff			状态指针
 * @param	combat_info		战斗信息
 */
void destroy_ext_buff_all( ext_buff_t *tmp_buff, combat_info_t *combat_info );

/**
 * 创建流血效果
 * @param	combat_info		战斗信息
 */
life_dot_t *create_life_dot( combat_info_t *combat_info );

/**
 * 回收一个流血效果
 * @param	tmp_dit			流血效果
 * @param	combat_info		战斗信息
 */
void destroy_life_dot( life_dot_t *tmp_dot, combat_info_t *combat_info );

/**
 * 创建状态时间增加
 * @param	combat_info		战斗信息
 */
buff_addtime_t *create_buff_addtime( combat_info_t *combat_info );

/**
 * 回收状态时间增加
 * @param	tmp_dit			流血效果
 * @param	combat_info		战斗信息
 */
void destroy_buff_addtime( buff_addtime_t *tmp_dot, combat_info_t *combat_info );

#endif
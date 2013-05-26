#ifndef FIRST_FIGHT_FORMULA_H
#define FIRST_FIGHT_FORMULA_H
#include "first_fight.h"
struct fight_unit_t;

/**
 * 获取防守方对攻击方的暴击率
 */
int get_ca_rate( fight_unit_t *attack, fight_unit_t *defence );

/**
 * 获取攻击方对防守方的暴击率
 */
int get_ds_rate( fight_unit_t *attack, fight_unit_t *defence );

/**
 * 获取攻击方对防守方的命中率
 */
int get_hr_rate( fight_unit_t *attack, fight_unit_t *defence );

/**
 * 将各种值转换成率的公式
 */
int rate2ration( int rate, int level );

/**
 * 产生范围从x 到 y的随机数
 * @param	x				起始值
 * @param	y				结束值
 */
int first_rand( int x, int y );

/**
 * 暴击伤害公式
 */
int get_ds_damage( int nomal_attack, int ds_damage_add, int tenacity );

/**
 * 碾压造成的伤害
 */
int attack_press_damage( int life );
#endif
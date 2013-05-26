#ifndef YILE_FIGHT_EFFECT_H
#define YILE_FIGHT_EFFECT_H
#define SKILL_EFFECT_NUM 25					//技能效果的个数
#include "yile_fight.h"
//所有效果集合
void ( *skill_effect_arr[ SKILL_EFFECT_NUM + 1 ] )( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );

/**
 * 初始化所有效果列表
 */
void init_skill_effect();

/**
 * 0号效果（当一个效果不存在的时候，容错）
 * @param	aim_member		效果承受者
 * @param	effect_value	效果值
 * @param	effect_do_type	效果生效或者失效标志 EFFECT_ADD: 效果生效  EFFECT_CLEAN:效果失效
 * @param	effect_type		效果持续类型 EFFECT_ONCE: 单次 EFFECT_MANY: 持续效果
 * @param	combat_info		战斗信息
 */
void skill_effect_0( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );


void skill_effect_1( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );

void skill_effect_2( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );

void skill_effect_3( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );

void skill_effect_4( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_5( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_6( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_7( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_8( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_9( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_10( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_11( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_12( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_13( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_14( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_15( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_16( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_17( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_18( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_19( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_20( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_21( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_22( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_23( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_24( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
void skill_effect_25( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type, combat_info_t *combat_info );
#endif
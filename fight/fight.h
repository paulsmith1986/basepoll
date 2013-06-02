#ifndef FIRST_FIGHT_H
#define FIRST_FIGHT_H
#include "first_fight.h"
#include "skill_effect.h"
#include "struct_pool.h"
#include "skill_formula.h"

/**
 * 初始化成员
 */
void init_member( fight_unit_t *member, int side, int cell_id, combat_info_t *combat_info );
#endif
#ifndef YILE_SKILL_FORMULA_H
#define YILE_SKILL_FORMULA_H
#include <math.h>
#include "init_define.h"
#include "yile_fight.h"
void init_skill_formula();
int ( *skill_formula_arr[ SKILL_FORMULA_NUM ] )( fight_unit_t *attack );
#endif

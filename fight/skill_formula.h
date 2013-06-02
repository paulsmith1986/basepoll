#ifndef FIRST_SKILL_FORMULA_H
#define FIRST_SKILL_FORMULA_H
#include <math.h>
#include "init_define.h"
#include "first_fight.h"
void init_skill_formula();
extern int ( *skill_formula_arr[] )( fight_unit_t *attack );
#endif

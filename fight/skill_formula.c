#include "skill_formula.h"

int skill_formula_0( fight_unit_t *attack )
{
	return attack->skill_level * 2;
}

int skill_formula_1( fight_unit_t *attack )
{
	return attack->skill_level * 100;
}

int skill_formula_2( fight_unit_t *attack )
{
	return attack->skill_level * 3;
}

int skill_formula_3( fight_unit_t *attack )
{
	return attack->skill_level * 20;
}

int skill_formula_4( fight_unit_t *attack )
{
	return attack->skill_level * 2;
}

int skill_formula_5( fight_unit_t *attack )
{
	return attack->skill_level * 2;
}
/**
* 初始化技能公式
*/
void init_skill_formula()
{
	skill_formula_arr[ 0 ] = skill_formula_0;
	skill_formula_arr[ 1 ] = skill_formula_1;
	skill_formula_arr[ 2 ] = skill_formula_2;
	skill_formula_arr[ 3 ] = skill_formula_3;
	skill_formula_arr[ 4 ] = skill_formula_4;
	skill_formula_arr[ 5 ] = skill_formula_5;

}
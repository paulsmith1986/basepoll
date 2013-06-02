#include "skill_formula.h"
int ( *skill_formula_arr[ SKILL_FORMULA_NUM ] )( fight_unit_t *attack );

int skill_formula_0( fight_unit_t *attack )
{
	return 17+attack->skill_level*4;
}

int skill_formula_1( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 1 + 0.6 * ( attack->str + attack->dex );

break;

case 2:

return 4 + 0.6 * ( attack->str + attack->dex );

break;

case 3:

return 7 + 0.6 * ( attack->str + attack->dex );

break;

case 4:

return 10 + 0.6 * ( attack->str + attack->dex );

break;

case 5:

return 13 + 0.6 * ( attack->str + attack->dex );

break;

case 6:

return 16 + 0.6 * ( attack->str + attack->dex );

break;

case 7:

return 25 + 0.6 * ( attack->str + attack->dex );

break;

case 8:

return 28 + 0.6 * ( attack->str + attack->dex );

break;

case 9:

return 36 + 0.6 * ( attack->str + attack->dex );

break;

case 10:

return 48 + 0.6 * ( attack->str + attack->dex );

break;

case 11:

return 64 + 0.6 * ( attack->str + attack->dex );

break;

case 12:

return 78 + 0.6 * ( attack->str + attack->dex );

break;

case 13:

return 99 + 0.6 * ( attack->str + attack->dex );

break;

case 14:

return 128 + 0.6 * ( attack->str + attack->dex );

break;

case 15:

return 229 + 0.6 * ( attack->str + attack->dex );

break;

case 16:

return 293 + 0.6 * ( attack->str + attack->dex );

break;

case 17:

return 366 + 0.6 * ( attack->str + attack->dex );

break;

case 18:

return 459 + 0.6 * ( attack->str + attack->dex );

break;

case 19:

return 582 + 0.6 * ( attack->str + attack->dex );

break;

case 20:

return 711 + 0.6 * ( attack->str + attack->dex );

break;

case 21:

return 894 + 0.6 * ( attack->str + attack->dex );

break;

case 22:

return 1118 + 0.6 * ( attack->str + attack->dex );

break;

case 23:

return 1321 + 0.6 * ( attack->str + attack->dex );

break;

case 24:

return 1658 + 0.6 * ( attack->str + attack->dex );

break;

case 25:

return 2059 + 0.6 * ( attack->str + attack->dex );

break;

case 26:

return 2463 + 0.6 * ( attack->str + attack->dex );

break;

case 27:

return 3050 + 0.6 * ( attack->str + attack->dex );

break;

default :

return 3050 + 0.6 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_2( fight_unit_t *attack )
{
	switch(attack->skill_level){
case 1:
return 19 + 0.15 * ( attack->str + attack->dex );
break;
case 2:
return 23 + 0.15 * ( attack->str + attack->dex );
break;
case 3:
return 29 + 0.15 * ( attack->str + attack->dex );
break;
case 4:
return 36 + 0.15 * ( attack->str + attack->dex );
break;
case 5:
return 43 + 0.15 * ( attack->str + attack->dex );
break;
case 6:
return 54 + 0.15 * ( attack->str + attack->dex );
break;
case 7:
return 66 + 0.15 * ( attack->str + attack->dex );
break;
case 8:
return 81 + 0.15 * ( attack->str + attack->dex );
break;
case 9:
return 100 + 0.15 * ( attack->str + attack->dex );
break;
case 10:
return 122 + 0.15 * ( attack->str + attack->dex );
break;
case 11:
return 150 + 0.15 * ( attack->str + attack->dex );
break;
case 12:
return 185 + 0.15 * ( attack->str + attack->dex );
break;
case 13:
return 225 + 0.15 * ( attack->str + attack->dex );
break;
case 14:
return 408 + 0.15 * ( attack->str + attack->dex );
break;
case 15:
return 498 + 0.15 * ( attack->str + attack->dex );
break;
case 16:
return 607 + 0.15 * ( attack->str + attack->dex );
break;
case 17:
return 737 + 0.15 * ( attack->str + attack->dex );
break;
case 18:
return 902 + 0.15 * ( attack->str + attack->dex );
break;
case 19:
return 1139 + 0.15 * ( attack->str + attack->dex );
break;
case 20:
return 1383 + 0.15 * ( attack->str + attack->dex );
break;
case 21:
return 1678 + 0.15 * ( attack->str + attack->dex );
break;
case 22:
return 2055 + 0.15 * ( attack->str + attack->dex );
break;
case 23:
return 2486 + 0.15 * ( attack->str + attack->dex );
break;
case 24:
return 3036 + 0.15 * ( attack->str + attack->dex );
break;
case 25:
return 3683 + 0.15 * ( attack->str + attack->dex );
break;
case 26:
return 4450 + 0.15 * ( attack->str + attack->dex );
break;
default :
return 4450 + 0.15 * ( attack->str + attack->dex );
break;
}
}

int skill_formula_3( fight_unit_t *attack )
{
	switch(attack->skill_level){
case 1:
return 27 + 0.25 * ( attack->str + attack->dex );
break;
case 2:
return 34 + 0.25 * ( attack->str + attack->dex );
break;
case 3:
return 41 + 0.25 * ( attack->str + attack->dex );
break;
case 4:
return 51 + 0.25 * ( attack->str + attack->dex );
break;
case 5:
return 62 + 0.25 * ( attack->str + attack->dex );
break;
case 6:
return 76 + 0.25 * ( attack->str + attack->dex );
break;
case 7:
return 93 + 0.25 * ( attack->str + attack->dex );
break;
case 8:
return 113 + 0.25 * ( attack->str + attack->dex );
break;
case 9:
return 138 + 0.25 * ( attack->str + attack->dex );
break;
case 10:
return 168 + 0.25 * ( attack->str + attack->dex );
break;
case 11:
return 208 + 0.25 * ( attack->str + attack->dex );
break;
case 12:
return 252 + 0.25 * ( attack->str + attack->dex );
break;
case 13:
return 455 + 0.25 * ( attack->str + attack->dex );
break;
case 14:
return 554 + 0.25 * ( attack->str + attack->dex );
break;
case 15:
return 677 + 0.25 * ( attack->str + attack->dex );
break;
case 16:
return 820 + 0.25 * ( attack->str + attack->dex );
break;
case 17:
return 997 + 0.25 * ( attack->str + attack->dex );
break;
case 18:
return 1260 + 0.25 * ( attack->str + attack->dex );
break;
case 19:
return 1530 + 0.25 * ( attack->str + attack->dex );
break;
case 20:
return 1863 + 0.25 * ( attack->str + attack->dex );
break;
case 21:
return 2257 + 0.25 * ( attack->str + attack->dex );
break;
case 22:
return 2735 + 0.25 * ( attack->str + attack->dex );
break;
case 23:
return 3322 + 0.25 * ( attack->str + attack->dex );
break;
case 24:
return 4023 + 0.25 * ( attack->str + attack->dex );
break;
default :
return 4023 + 0.25 * ( attack->str + attack->dex );
break;
}
}

int skill_formula_4( fight_unit_t *attack )
{
	switch(attack->skill_level){case 1:return -17-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 2:return -21-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 3:return -25-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 4:return -31-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 5:return -39-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 6:return -48-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 7:return -59-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 8:return -72-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 9:return -89-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 10:return -164-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 11:return -202-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 12:return -250-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 13:return -307-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 14:return -397-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 15:return -490-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 16:return -604-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 17:return -752-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 18:return -928-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 19:return -1156-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;case 20:return -1426-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;default:return -1426-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;break;}
}

int skill_formula_5( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 2 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 2:

return 4 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 3:

return 12 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 4:

return 20 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 5:

return 33 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 6:

return 53 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 7:

return 84 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 8:

return 168 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 9:

return 241 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 10:

return 365 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 11:

return 502 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 12:

return 726 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 13:

return 963 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 14:

return 1378 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 15:

return 1845 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

case 16:

return 2610 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

default :

return 2610 + 0.65 * ( attack->str + attack->dex )+attack->defence*0.1;

break;

}
}

int skill_formula_6( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 3 + 1.3 * ( attack->str + attack->dex );

break;

case 2:

return 8 + 1.3 * ( attack->str + attack->dex );

break;

case 3:

return 24 + 1.3 * ( attack->str + attack->dex );

break;

case 4:

return 30 + 1.3 * ( attack->str + attack->dex );

break;

case 5:

return 44 + 1.3 * ( attack->str + attack->dex );

break;

case 6:

return 75 + 1.3 * ( attack->str + attack->dex );

break;

case 7:

return 107 + 1.3 * ( attack->str + attack->dex );

break;

case 8:

return 152 + 1.3 * ( attack->str + attack->dex );

break;

case 9:

return 315 + 1.3 * ( attack->str + attack->dex );

break;

case 10:

return 406 + 1.3 * ( attack->str + attack->dex );

break;

case 11:

return 563 + 1.3 * ( attack->str + attack->dex );

break;

case 12:

return 765 + 1.3 * ( attack->str + attack->dex );

break;

case 13:

return 1003 + 1.3 * ( attack->str + attack->dex );

break;

case 14:

return 1365 + 1.3 * ( attack->str + attack->dex );

break;

case 15:

return 1842 + 1.3 * ( attack->str + attack->dex );

break;

case 16:

return 2305 + 1.3 * ( attack->str + attack->dex );

break;

case 17:

return 3110 + 1.3 * ( attack->str + attack->dex );

break;

case 18:

return 3926 + 1.3 * ( attack->str + attack->dex );

break;

case 19:

return 5219 + 1.3 * ( attack->str + attack->dex );

break;

default :

return 5219+ 1.3 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_7( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*2.2;
}

int skill_formula_8( fight_unit_t *attack )
{
	return -60+attack->skill_level*3;
}

int skill_formula_9( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*3.9;
}

int skill_formula_10( fight_unit_t *attack )
{
	return -80+attack->skill_level;
}

int skill_formula_11( fight_unit_t *attack )
{
	return (attack->str+attack->dex)/3;
}

int skill_formula_12( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.114-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;
}

int skill_formula_13( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*6;
}

int skill_formula_14( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*2;
}

int skill_formula_15( fight_unit_t *attack )
{
	return attack->skill_level*4;
}

int skill_formula_16( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*(0.15+0.02*attack->skill_level);
}

int skill_formula_17( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*4;
}

int skill_formula_18( fight_unit_t *attack )
{
	return attack->skill_level*6+10;
}

int skill_formula_19( fight_unit_t *attack )
{
	return 60+(attack->str+attack->dex)/100;
}

int skill_formula_20( fight_unit_t *attack )
{
	switch(attack->skill_level){
case 1:
return 4 + 1.1 * ( attack->str + attack->dex );
break;
case 2:
return 7 + 1.1 * ( attack->str + attack->dex );
break;
case 3:
return 12 + 1.1 * ( attack->str + attack->dex );
break;
case 4:
return 18 + 1.1 * ( attack->str + attack->dex );
break;
case 5:
return 21 + 1.1 * ( attack->str + attack->dex );
break;
case 6:
return 35 + 1.1 * ( attack->str + attack->dex );
break;
case 7:
return 37 + 1.1 * ( attack->str + attack->dex );
break;
case 8:
return 49 + 1.1 * ( attack->str + attack->dex );
break;
case 9:
return 64 + 1.1 * ( attack->str + attack->dex );
break;
case 10:
return 75 + 1.1 * ( attack->str + attack->dex );
break;
case 11:
return 103 + 1.1 * ( attack->str + attack->dex );
break;
case 12:
return 134 + 1.1 * ( attack->str + attack->dex );
break;
case 13:
return 168 + 1.1 * ( attack->str + attack->dex );
break;
case 14:
return 287 + 1.1 * ( attack->str + attack->dex );
break;
case 15:
return 356 + 1.1 * ( attack->str + attack->dex );
break;
case 16:
return 451 + 1.1 * ( attack->str + attack->dex );
break;
case 17:
return 571 + 1.1 * ( attack->str + attack->dex );
break;
case 18:
return 636 + 1.1 * ( attack->str + attack->dex );
break;
case 19:
return 845 + 1.1 * ( attack->str + attack->dex );
break;
case 20:
return 1063 + 1.1 * ( attack->str + attack->dex );
break;
case 21:
return 1329 + 1.1 * ( attack->str + attack->dex );
break;
case 22:
return 1526 + 1.1 * ( attack->str + attack->dex );
break;
case 23:
return 1913 + 1.1 * ( attack->str + attack->dex );
break;
case 24:
return 2191 + 1.1 * ( attack->str + attack->dex );
break;
case 25:
return 2791 + 1.1 * ( attack->str + attack->dex );
break;
case 26:
return 3478 + 1.1 * ( attack->str + attack->dex );
break;
default :
return 3478 + 1.1 * ( attack->str + attack->dex );
break;
}
}

int skill_formula_21( fight_unit_t *attack )
{
	switch(attack->skill_level){case 1:return 34 + 6 * ( attack->str + attack->dex );break;case 2:return 45 + 6 * ( attack->str + attack->dex );break;case 3:return 165 + 6 * ( attack->str + attack->dex );break;case 4:return 122 + 6 * ( attack->str + attack->dex );break;case 5:return 218 + 6 * ( attack->str + attack->dex );break;case 6:return 188 + 6 * ( attack->str + attack->dex );break;case 7:return 166 + 6 * ( attack->str + attack->dex );break;case 8:return 276 + 6 * ( attack->str + attack->dex );break;case 9:return 317 + 6 * ( attack->str + attack->dex );break;case 10:return 269 + 6 * ( attack->str + attack->dex );break;case 11:return 374 + 6 * ( attack->str + attack->dex );break;case 12:return 508 + 6 * ( attack->str + attack->dex );break;case 13:return 686 + 6 * ( attack->str + attack->dex );break;case 14:return 675 + 6 * ( attack->str + attack->dex );break;case 15:return 1018 + 6 * ( attack->str + attack->dex );break;case 16:return 1102 + 6 * ( attack->str + attack->dex );break;case 17:return 1283 + 6 * ( attack->str + attack->dex );break;case 18:return 1531 + 6 * ( attack->str + attack->dex );break;case 19:return 2101 + 6 * ( attack->str + attack->dex );break;case 20:return 1936 + 6 * ( attack->str + attack->dex );break;case 21:return 3299 + 6 * ( attack->str + attack->dex );break;case 22:return 5461 + 6 * ( attack->str + attack->dex );break;case 23:return 7532 + 6 * ( attack->str + attack->dex );break;case 24:return 10856 + 6 * ( attack->str + attack->dex );break;case 25:return 14606 + 6 * ( attack->str + attack->dex );break;case 26:return 20959 + 6 * ( attack->str + attack->dex );break;case 27:return 27539 + 6 * ( attack->str + attack->dex );break;case 28:return 38374 + 6 * ( attack->str + attack->dex );break;case 29:return 51548 + 6 * ( attack->str + attack->dex );break;case 30:return 68161 + 6 * ( attack->str + attack->dex );break;default :return 68161 + 6 * ( attack->str + attack->dex );break;}
}

int skill_formula_22( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 32 + 0.15 * ( attack->str + attack->dex );

break;

case 2:

return 43 + 0.15 * ( attack->str + attack->dex );

break;

case 3:

return 59 + 0.15 * ( attack->str + attack->dex );

break;

case 4:

return 81 + 0.15 * ( attack->str + attack->dex );

break;

case 5:

return 111 + 0.15 * ( attack->str + attack->dex );

break;

case 6:

return 150 + 0.15 * ( attack->str + attack->dex );

break;

case 7:

return 204 + 0.15 * ( attack->str + attack->dex );

break;

case 8:

return 408 + 0.15 * ( attack->str + attack->dex );

break;

case 9:

return 547 + 0.15 * ( attack->str + attack->dex );

break;

case 10:

return 737 + 0.15 * ( attack->str + attack->dex );

break;

case 11:

return 1033 + 0.15 * ( attack->str + attack->dex );

break;

case 12:

return 1383 + 0.15 * ( attack->str + attack->dex );

break;

case 13:

return 1862 + 0.15 * ( attack->str + attack->dex );

break;

case 14:

return 2486 + 0.15 * ( attack->str + attack->dex );

break;

case 15:

return 3343 + 0.15 * ( attack->str + attack->dex );

break;

case 16:

return 4450 + 0.15 * ( attack->str + attack->dex );

break;

default :

return 4450 + 0.15 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_23( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 63 + 1 * ( attack->str + attack->dex );

break;

case 2:

return 81 + 1 * ( attack->str + attack->dex );

break;

case 3:

return 110 + 1 * ( attack->str + attack->dex );

break;

case 4:

return 138 + 1 * ( attack->str + attack->dex );

break;

case 5:

return 177 + 1 * ( attack->str + attack->dex );

break;

case 6:

return 234 + 1 * ( attack->str + attack->dex );

break;

case 7:

return 302 + 1 * ( attack->str + attack->dex );

break;

case 8:

return 391 + 1 * ( attack->str + attack->dex );

break;

case 9:

return 748 + 1 * ( attack->str + attack->dex );

break;

case 10:

return 945 + 1 * ( attack->str + attack->dex );

break;

case 11:

return 1220 + 1 * ( attack->str + attack->dex );

break;

case 12:

return 1563 + 1 * ( attack->str + attack->dex );

break;

case 13:

return 2052 + 1 * ( attack->str + attack->dex );

break;

case 14:

return 2639 + 1 * ( attack->str + attack->dex );

break;

case 15:

return 3385 + 1 * ( attack->str + attack->dex );

break;

case 16:

return 4265 + 1 * ( attack->str + attack->dex );

break;

case 17:

return 5479 + 1 * ( attack->str + attack->dex );

break;

case 18:

return 6921 + 1 * ( attack->str + attack->dex );

break;

case 19:

return 8847 + 1 * ( attack->str + attack->dex );

break;

default :

return 8847 + 1 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_24( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*(0.7+attack->skill_level*0.1);
}

int skill_formula_25( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*(2+attack->skill_level*0.5);
}

int skill_formula_26( fight_unit_t *attack )
{
	return (attack->str + attack->dex)*0.08;
}

int skill_formula_27( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.02-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))/2*(0.006*attack->skill_level);
}

int skill_formula_28( fight_unit_t *attack )
{
	return 50+10*attack->skill_level;
}

int skill_formula_29( fight_unit_t *attack )
{
	return pow(1.038,attack->level)*55;
}

int skill_formula_30( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.15-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.05;
}

int skill_formula_31( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.25-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.4;
}

int skill_formula_32( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*2.5-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*3;
}

int skill_formula_33( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 19 + 0.15 * ( attack->str + attack->dex );

break;

case 2:

return 23 + 0.15 * ( attack->str + attack->dex );

break;

case 3:

return 29 + 0.15 * ( attack->str + attack->dex );

break;

case 4:

return 36 + 0.15 * ( attack->str + attack->dex );

break;

case 5:

return 43 + 0.15 * ( attack->str + attack->dex );

break;

case 6:

return 54 + 0.15 * ( attack->str + attack->dex );

break;

case 7:

return 66 + 0.15 * ( attack->str + attack->dex );

break;

case 8:

return 81 + 0.15 * ( attack->str + attack->dex );

break;

case 9:

return 100 + 0.15 * ( attack->str + attack->dex );

break;

case 10:

return 122 + 0.15 * ( attack->str + attack->dex );

break;

case 11:

return 150 + 0.15 * ( attack->str + attack->dex );

break;

case 12:

return 185 + 0.15 * ( attack->str + attack->dex );

break;

case 13:

return 225 + 0.15 * ( attack->str + attack->dex );

break;

case 14:

return 408 + 0.15 * ( attack->str + attack->dex );

break;

case 15:

return 498 + 0.15 * ( attack->str + attack->dex );

break;

case 16:

return 607 + 0.15 * ( attack->str + attack->dex );

break;

case 17:

return 737 + 0.15 * ( attack->str + attack->dex );

break;

case 18:

return 902 + 0.15 * ( attack->str + attack->dex );

break;

case 19:

return 1139 + 0.15 * ( attack->str + attack->dex );

break;

case 20:

return 1383 + 0.15 * ( attack->str + attack->dex );

break;

case 21:

return 1678 + 0.15 * ( attack->str + attack->dex );

break;

case 22:

return 2055 + 0.15 * ( attack->str + attack->dex );

break;

case 23:

return 2486 + 0.15 * ( attack->str + attack->dex );

break;

case 24:

return 3036 + 0.15 * ( attack->str + attack->dex );

break;

case 25:

return 3683 + 0.15 * ( attack->str + attack->dex );

break;

case 26:

return 4450 + 0.15 * ( attack->str + attack->dex );

break;

default :

return 4450 + 0.15 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_34( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 7 + 1.1 * ( attack->str + attack->dex );

break;

case 2:

return 15 + 1.1 * ( attack->str + attack->dex );

break;

case 3:

return 22 + 1.1 * ( attack->str + attack->dex );

break;

case 4:

return 26 + 1.1 * ( attack->str + attack->dex );

break;

case 5:

return 34 + 1.1 * ( attack->str + attack->dex );

break;

case 6:

return 41 + 1.1 * ( attack->str + attack->dex );

break;

case 7:

return 55 + 1.1 * ( attack->str + attack->dex );

break;

case 8:

return 70 + 1.1 * ( attack->str + attack->dex );

break;

case 9:

return 81 + 1.1 * ( attack->str + attack->dex );

break;

case 10:

return 103 + 1.1 * ( attack->str + attack->dex );

break;

case 11:

return 139 + 1.1 * ( attack->str + attack->dex );

break;

case 12:

return 175 + 1.1 * ( attack->str + attack->dex );

break;

case 13:

return 299 + 1.1 * ( attack->str + attack->dex );

break;

case 14:

return 372 + 1.1 * ( attack->str + attack->dex );

break;

case 15:

return 484 + 1.1 * ( attack->str + attack->dex );

break;

case 16:

return 595 + 1.1 * ( attack->str + attack->dex );

break;

case 17:

return 675 + 1.1 * ( attack->str + attack->dex );

break;

case 18:

return 886 + 1.1 * ( attack->str + attack->dex );

break;

case 19:

return 1118 + 1.1 * ( attack->str + attack->dex );

break;

case 20:

return 1286 + 1.1 * ( attack->str + attack->dex );

break;

case 21:

return 1611 + 1.1 * ( attack->str + attack->dex );

break;

case 22:

return 2032 + 1.1 * ( attack->str + attack->dex );

break;

case 23:

return 2339 + 1.1 * ( attack->str + attack->dex );

break;

case 24:

return 2933 + 1.1 * ( attack->str + attack->dex );

break;

default :

return 2933 + 1.1 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_35( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 9 + 0.15 * ( attack->str + attack->dex );

break;

case 2:

return 14 + 0.15 * ( attack->str + attack->dex );

break;

case 3:

return 23 + 0.15 * ( attack->str + attack->dex );

break;

case 4:

return 34 + 0.15 * ( attack->str + attack->dex );

break;

case 5:

return 51 + 0.15 * ( attack->str + attack->dex );

break;

case 6:

return 74 + 0.15 * ( attack->str + attack->dex );

break;

case 7:

return 107 + 0.15 * ( attack->str + attack->dex );

break;

case 8:

return 222 + 0.15 * ( attack->str + attack->dex );

break;

case 9:

return 310 + 0.15 * ( attack->str + attack->dex );

break;

case 10:

return 436 + 0.15 * ( attack->str + attack->dex );

break;

case 11:

return 628 + 0.15 * ( attack->str + attack->dex );

break;

case 12:

return 867 + 0.15 * ( attack->str + attack->dex );

break;

case 13:

return 1194 + 0.15 * ( attack->str + attack->dex );

break;

case 14:

return 1636 + 0.15 * ( attack->str + attack->dex );

break;

case 15:

return 2241 + 0.15 * ( attack->str + attack->dex );

break;

case 16:

return 3048 + 0.15 * ( attack->str + attack->dex );

break;

default :

return 3048 + 0.15 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_36( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*(attack->skill_level+2.5);
}

int skill_formula_37( fight_unit_t *attack )
{
	return -10*attack->skill_level;
}

int skill_formula_38( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*(attack->skill_level+3)*0.1;
}

int skill_formula_39( fight_unit_t *attack )
{
	return -70+attack->skill_level*2;
}

int skill_formula_40( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)/2;
}

int skill_formula_41( fight_unit_t *attack )
{
	return -10-attack->skill_level*4;
}

int skill_formula_42( fight_unit_t *attack )
{
	return -65+attack->skill_level*2;
}

int skill_formula_43( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*2.8;
}

int skill_formula_44( fight_unit_t *attack )
{
	return -70+attack->skill_level*2;
}

int skill_formula_45( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*0.3;
}

int skill_formula_46( fight_unit_t *attack )
{
	return 20+attack->skill_level*4;
}

int skill_formula_47( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*0.65;
}

int skill_formula_48( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*(0.011+attack->skill_level*0.002)-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.02;
}

int skill_formula_49( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.045-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;
}

int skill_formula_50( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 5 + 0.15 * ( attack->str + attack->dex );

break;

case 2:

return 8 + 0.15 * ( attack->str + attack->dex );

break;

case 3:

return 12 + 0.15 * ( attack->str + attack->dex );

break;

case 4:

return 16 + 0.15 * ( attack->str + attack->dex );

break;

case 5:

return 22 + 0.15 * ( attack->str + attack->dex );

break;

case 6:

return 28 + 0.15 * ( attack->str + attack->dex );

break;

case 7:

return 37 + 0.15 * ( attack->str + attack->dex );

break;

case 8:

return 48 + 0.15 * ( attack->str + attack->dex );

break;

case 9:

return 62 + 0.15 * ( attack->str + attack->dex );

break;

case 10:

return 77 + 0.15 * ( attack->str + attack->dex );

break;

case 11:

return 100 + 0.15 * ( attack->str + attack->dex );

break;

case 12:

return 126 + 0.15 * ( attack->str + attack->dex );

break;

case 13:

return 234 + 0.15 * ( attack->str + attack->dex );

break;

case 14:

return 294 + 0.15 * ( attack->str + attack->dex );

break;

case 15:

return 370 + 0.15 * ( attack->str + attack->dex );

break;

case 16:

return 459 + 0.15 * ( attack->str + attack->dex );

break;

case 17:

return 572 + 0.15 * ( attack->str + attack->dex );

break;

case 18:

return 738 + 0.15 * ( attack->str + attack->dex );

break;

case 19:

return 915 + 0.15 * ( attack->str + attack->dex );

break;

case 20:

return 1135 + 0.15 * ( attack->str + attack->dex );

break;

case 21:

return 1399 + 0.15 * ( attack->str + attack->dex );

break;

case 22:

return 1723 + 0.15 * ( attack->str + attack->dex );

break;

case 23:

return 2129 + 0.15 * ( attack->str + attack->dex );

break;

case 24:

return 2615 + 0.15 * ( attack->str + attack->dex );

break;

default :

return 2615 + 0.15 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_51( fight_unit_t *attack )
{
	switch(attack->skill_level){

case 1:

return 15 + 1.1 * ( attack->str + attack->dex );

break;

case 2:

return 21 + 1.1 * ( attack->str + attack->dex );

break;

case 3:

return 34 + 1.1 * ( attack->str + attack->dex );

break;

case 4:

return 49 + 1.1 * ( attack->str + attack->dex );

break;

case 5:

return 72 + 1.1 * ( attack->str + attack->dex );

break;

case 6:

return 103 + 1.1 * ( attack->str + attack->dex );

break;

case 7:

return 152 + 1.1 * ( attack->str + attack->dex );

break;

case 8:

return 287 + 1.1 * ( attack->str + attack->dex );

break;

case 9:

return 392 + 1.1 * ( attack->str + attack->dex );

break;

case 10:

return 571 + 1.1 * ( attack->str + attack->dex );

break;

case 11:

return 753 + 1.1 * ( attack->str + attack->dex );

break;

case 12:

return 1063 + 1.1 * ( attack->str + attack->dex );

break;

case 13:

return 1357 + 1.1 * ( attack->str + attack->dex );

break;

case 14:

return 1913 + 1.1 * ( attack->str + attack->dex );

break;

case 15:

return 2482 + 1.1 * ( attack->str + attack->dex );

break;

case 16:

return 3478 + 1.1 * ( attack->str + attack->dex );

break;

default :

return 3478 + 1.1 * ( attack->str + attack->dex );

break;

}
}

int skill_formula_52( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*(0.4+0.1*attack->skill_level);
}

int skill_formula_53( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*(1+0.2*attack->skill_level);
}

int skill_formula_54( fight_unit_t *attack )
{
	return -3*attack->skill_level;
}

int skill_formula_55( fight_unit_t *attack )
{
	return -4*attack->skill_level;
}

int skill_formula_56( fight_unit_t *attack )
{
	return attack->skill_level*4;
}

int skill_formula_57( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.6-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.4;
}

int skill_formula_58( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.7-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.45;
}

int skill_formula_59( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.8-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.5;
}

int skill_formula_60( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*1.1-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.7;
}

int skill_formula_61( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*(2+attack->skill_level*0.2)-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*(1+attack->skill_level*0.2);
}

int skill_formula_62( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*1-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.6;
}

int skill_formula_63( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*1.35;
}

int skill_formula_64( fight_unit_t *attack )
{
	return attack->skill_level*2+20;
}

int skill_formula_65( fight_unit_t *attack )
{
	return attack->skill_level*3+35;
}

int skill_formula_66( fight_unit_t *attack )
{
	return ( attack->life_max * 0.5 + 1.5 * attack->life_now ) / ( attack->life_max * 2 - 1.5 * attack->life_now ) * 22;
}

int skill_formula_67( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*0.25;
}

int skill_formula_68( fight_unit_t *attack )
{
	return attack->life_now*10/100;
}

int skill_formula_69( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*0.035*attack->skill_level;
}

int skill_formula_70( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*attack->skill_level*0.4;
}

int skill_formula_71( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*25/100*attack->skill_level;
}

int skill_formula_72( fight_unit_t *attack )
{
	return -attack->skill_level*4;
}

int skill_formula_73( fight_unit_t *attack )
{
	return (attack->str+attack->dex)/30*attack->skill_level;
}

int skill_formula_74( fight_unit_t *attack )
{
	return -(attack->str + attack->dex)*(0.25*attack->skill_level);
}

int skill_formula_75( fight_unit_t *attack )
{
	return ceil((attack->str+attack->dex)*(0.05+0.01*5));
}

int skill_formula_76( fight_unit_t *attack )
{
	return 2*attack->skill_level;
}

int skill_formula_77( fight_unit_t *attack )
{
	return -ceil((attack->str+attack->dex)*0.005*attack->skill_level)-1;
}

int skill_formula_78( fight_unit_t *attack )
{
	return -attack->skill_level;
}

int skill_formula_79( fight_unit_t *attack )
{
	return ceil((attack->str+attack->dex)*0.011*attack->skill_level);
}

int skill_formula_80( fight_unit_t *attack )
{
	return -ceil((attack->str+attack->dex)*0.0025*attack->skill_level);
}

int skill_formula_81( fight_unit_t *attack )
{
	return -ceil((attack->str+attack->dex)*0.03);
}

int skill_formula_82( fight_unit_t *attack )
{
	return attack->skill_level*-15;
}

int skill_formula_83( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*attack->skill_level*0.04;
}

int skill_formula_84( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*attack->skill_level*0.009-1;
}

int skill_formula_85( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*attack->skill_level*0.3;
}

int skill_formula_86( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*(attack->skill_level*0.006)-1;
}

int skill_formula_87( fight_unit_t *attack )
{
	return -(attack->str+attack->dex)*0.01*attack->skill_level;
}

int skill_formula_88( fight_unit_t *attack )
{
	return attack->skill_level*2;
}

int skill_formula_89( fight_unit_t *attack )
{
	return attack->skill_level*3;
}

int skill_formula_90( fight_unit_t *attack )
{
	return attack->skill_level;
}

int skill_formula_91( fight_unit_t *attack )
{
	return -get_attack( attack, attack->attack_max )/5;
}

int skill_formula_92( fight_unit_t *attack )
{
	return attack->life_max/8;
}

int skill_formula_93( fight_unit_t *attack )
{
	return attack->life_max/20;
}

int skill_formula_94( fight_unit_t *attack )
{
	return attack->life_max/60;
}

int skill_formula_95( fight_unit_t *attack )
{
	return -attack->skill_level*(attack->str+attack->dex)/100-1;
}

int skill_formula_96( fight_unit_t *attack )
{
	return -attack->skill_level*(attack->str+attack->dex)/150-1;
}

int skill_formula_97( fight_unit_t *attack )
{
	return -(get_attack( attack, attack->attack_max )-500)*0.345;
}

int skill_formula_98( fight_unit_t *attack )
{
	return attack->life_now*5/100;
}

int skill_formula_99( fight_unit_t *attack )
{
	return attack->life_now;
}

int skill_formula_100( fight_unit_t *attack )
{
	return (int)(5*attack->life_now/attack->life_max)*10;
}

int skill_formula_101( fight_unit_t *attack )
{
	return attack->life_max/10;
}

int skill_formula_102( fight_unit_t *attack )
{
	return 1000 + 3500 *(int)((int)(14 - attack->life_now *10 / attack->life_max) / 10);
}

int skill_formula_103( fight_unit_t *attack )
{
	return 50+ (int)((int)(attack->life_now *10 /attack->life_max+6) / 10) * 60;
}

int skill_formula_104( fight_unit_t *attack )
{
	return (int)(attack->life_now *40/attack->life_max) -80;
}

int skill_formula_105( fight_unit_t *attack )
{
	return 50 - (int)(attack->life_now * 100/attack->life_max);
}

int skill_formula_106( fight_unit_t *attack )
{
	return ((int)(1.5-(attack->life_now/attack->life_max))*0.2+0.1)*attack->defence;
}

int skill_formula_107( fight_unit_t *attack )
{
	return attack->life_now*30/100;
}

int skill_formula_108( fight_unit_t *attack )
{
	return attack->life_now*8/100;
}

int skill_formula_109( fight_unit_t *attack )
{
	return  attack->life_now;
}

int skill_formula_110( fight_unit_t *attack )
{
	return (attack->life_max + 4 * attack->life_now ) / ( 5 *attack->life_max - 4 *  attack->life_now ) * 100;
}

int skill_formula_111( fight_unit_t *attack )
{
	return round(pow(attack->level,5.1)*7.5*pow(10,-8)+2)+0.6 * ( attack->str + attack->dex );
}

int skill_formula_112( fight_unit_t *attack )
{
	return round(pow(attack->level,5)*9*pow(10,-8)+0.65 * ( attack->str + attack->dex )+ attack->defence * 0.1);
}

int skill_formula_113( fight_unit_t *attack )
{
	return round(pow(attack->level,5.3)*4.4*pow(10,-8)+2)+0.15 * ( attack->str + attack->dex );
}

int skill_formula_114( fight_unit_t *attack )
{
	return round(pow(attack->level,5.7)*7.2*pow(10,-9)+10)+1.3 * ( attack->str + attack->dex );
}

int skill_formula_115( fight_unit_t *attack )
{
	return round(pow(1.054,attack->level)*9.25+0.25* ( attack->str + attack->dex ));
}

int skill_formula_116( fight_unit_t *attack )
{
	return round(pow(attack->level,4.45)*1.65*pow(10,-6) + 1.1 * ( attack->str + attack->dex ));
}

int skill_formula_117( fight_unit_t *attack )
{
	return round(pow(attack->level,6)*1.8*pow(10,-8.5)+attack->level*1.2+20)+6 * ( attack->str + attack->dex );
}

int skill_formula_118( fight_unit_t *attack )
{
	return -round(pow(attack->level,4.7)*1.45*pow(10,-7)+16)*1.3-(attack->str+attack->dex)*0.05-(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.03;
}

int skill_formula_119( fight_unit_t *attack )
{
	return round(pow(attack->level,5)*1.1*pow(10,-7)+6)+0.15 * ( attack->str + attack->dex );
}

int skill_formula_120( fight_unit_t *attack )
{
	return 0.05*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ));
}

int skill_formula_121( fight_unit_t *attack )
{
	return 0.2*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ));
}

int skill_formula_122( fight_unit_t *attack )
{
	return 16*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max )-1980);
}

int skill_formula_123( fight_unit_t *attack )
{
	return pow(2,(attack->life_max-attack->life_now)*0.0001)*1000;
}

int skill_formula_124( fight_unit_t *attack )
{
	return -11111+5*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ));
}

int skill_formula_125( fight_unit_t *attack )
{
	return -11111+4*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ));
}

int skill_formula_126( fight_unit_t *attack )
{
	switch(attack->skill_level){case 1:return 15;case 2:return 20;case 3:return 25;case 4:return 30; case 5:return 35;break;};
}

int skill_formula_127( fight_unit_t *attack )
{
	switch(attack->skill_level){case 1:return 40;case 2:return 45;case 3:return 50; break;};
}

int skill_formula_128( fight_unit_t *attack )
{
	return 0.3*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ));
}

int skill_formula_129( fight_unit_t *attack )
{
	return 0.4*(get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ));
}

int skill_formula_130( fight_unit_t *attack )
{
	return attack->defence*0.2-28500;
}

int skill_formula_131( fight_unit_t *attack )
{
	return attack->life_now*0.015;
}

int skill_formula_132( fight_unit_t *attack )
{
	return attack->life_now*0.1;
}

int skill_formula_133( fight_unit_t *attack )
{
	return -attack->life_now*0.1-attack->life_max*0.01;
}

int skill_formula_134( fight_unit_t *attack )
{
	return attack->life_now*40/100;
}

int skill_formula_135( fight_unit_t *attack )
{
	return 10*((get_attack( attack, attack->attack_min )+get_attack( attack, attack->attack_max ))*0.5-20000);
}

int skill_formula_136( fight_unit_t *attack )
{
	return attack->skill_level*5;
}

int skill_formula_137( fight_unit_t *attack )
{
	return attack->skill_level*2;
}

int skill_formula_138( fight_unit_t *attack )
{
	return attack->skill_level*1;
}

int skill_formula_139( fight_unit_t *attack )
{
	return attack->skill_level*3;
}

int skill_formula_140( fight_unit_t *attack )
{
	return attack->skill_level*(attack->level/3-6);
}

int skill_formula_141( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*4/100*attack->skill_level;
}

int skill_formula_142( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*5/100*attack->skill_level;
}

int skill_formula_143( fight_unit_t *attack )
{
	return attack->skill_level*4;
}

int skill_formula_144( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*8/100*attack->skill_level;
}

int skill_formula_145( fight_unit_t *attack )
{
	return (attack->str+attack->dex)/50*attack->skill_level;
}

int skill_formula_146( fight_unit_t *attack )
{
	return attack->skill_level*pow(attack->level,3)/4000;
}

int skill_formula_147( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*0.4*attack->skill_level;
}

int skill_formula_148( fight_unit_t *attack )
{
	return -attack->skill_level*5;
}

int skill_formula_149( fight_unit_t *attack )
{
	return attack->skill_level*40;
}

int skill_formula_150( fight_unit_t *attack )
{
	return (attack->str+attack->dex)/5*attack->skill_level;
}

int skill_formula_151( fight_unit_t *attack )
{
	return attack->skill_level*pow(attack->level,2)/100;
}

int skill_formula_152( fight_unit_t *attack )
{
	return attack->skill_level*10;
}

int skill_formula_153( fight_unit_t *attack )
{
	return -attack->skill_level*25;
}

int skill_formula_154( fight_unit_t *attack )
{
	return (attack->str+attack->dex)/3*attack->skill_level;
}

int skill_formula_155( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*attack->skill_level*0.1;
}

int skill_formula_156( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*attack->skill_level*0.06;
}

int skill_formula_157( fight_unit_t *attack )
{
	return (attack->str+attack->dex)*attack->skill_level*0.04;
}

int skill_formula_158( fight_unit_t *attack )
{
	return attack->skill_level*8;
}

int skill_formula_159( fight_unit_t *attack )
{
	return -attack->skill_level*10;
}

int skill_formula_160( fight_unit_t *attack )
{
	return 25*attack->skill_level;
}

int skill_formula_161( fight_unit_t *attack )
{
	return -attack->skill_level*15;
}

int skill_formula_162( fight_unit_t *attack )
{
	return attack->skill_level;
}

int skill_formula_163( fight_unit_t *attack )
{
	return ceil((pow(1.04,attack->level))*1.3*attack->skill_level);
}

int skill_formula_164( fight_unit_t *attack )
{
	return ceil((pow(1.025,attack->level))*attack->skill_level);
}

int skill_formula_165( fight_unit_t *attack )
{
	return -attack->skill_level*12;
}

int skill_formula_166( fight_unit_t *attack )
{
	return attack->skill_level*0.06*(attack->str+attack->dex);
}

int skill_formula_167( fight_unit_t *attack )
{
	switch(attack->skill_level){case 1:return 125;break;case 2:return 166;break;case 3:return 216;break;case 4:return 275;break;case 5:return 343;break;case 6:return 422;break;case 7:return 512;break;case 8:return 614;break;case 9:return 729;break;case 10:return 857;break;case 11:return 1000;break;case 12:return 1158;break;case 13:return 1331;break;case 14:return 1521;break;case 15:return 1728;break;case 16:return 1953;break;case 17:return 2197;break;case 18:return 2460;break;case 19:return 2744;break;case 20:return 3049;break;case 21:return 3375;break;case 22:return 3724;break;case 23:return 4096;break;case 24:return 4492;break;case 25:return 4913;break;case 26:return 5359;break;case 27:return 5832;break;default:return 5832;break;}
}

int skill_formula_168( fight_unit_t *attack )
{
	return attack->skill_level*(attack->str+attack->dex)/20;
}

int skill_formula_169( fight_unit_t *attack )
{
	return attack->skill_level*(attack->str+attack->dex)/30;
}

int skill_formula_170( fight_unit_t *attack )
{
	return attack->skill_level*(attack->str+attack->dex)/25;
}

int skill_formula_171( fight_unit_t *attack )
{
	return attack->skill_level*(attack->str+attack->dex)/15;
}

int skill_formula_172( fight_unit_t *attack )
{
	return 50-attack->skill_level*7;
}

int skill_formula_173( fight_unit_t *attack )
{
	return attack->skill_level*30;
}

int skill_formula_174( fight_unit_t *attack )
{
	return attack->skill_level*(attack->str+attack->dex)/40;
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
	skill_formula_arr[ 6 ] = skill_formula_6;
	skill_formula_arr[ 7 ] = skill_formula_7;
	skill_formula_arr[ 8 ] = skill_formula_8;
	skill_formula_arr[ 9 ] = skill_formula_9;
	skill_formula_arr[ 10 ] = skill_formula_10;
	skill_formula_arr[ 11 ] = skill_formula_11;
	skill_formula_arr[ 12 ] = skill_formula_12;
	skill_formula_arr[ 13 ] = skill_formula_13;
	skill_formula_arr[ 14 ] = skill_formula_14;
	skill_formula_arr[ 15 ] = skill_formula_15;
	skill_formula_arr[ 16 ] = skill_formula_16;
	skill_formula_arr[ 17 ] = skill_formula_17;
	skill_formula_arr[ 18 ] = skill_formula_18;
	skill_formula_arr[ 19 ] = skill_formula_19;
	skill_formula_arr[ 20 ] = skill_formula_20;
	skill_formula_arr[ 21 ] = skill_formula_21;
	skill_formula_arr[ 22 ] = skill_formula_22;
	skill_formula_arr[ 23 ] = skill_formula_23;
	skill_formula_arr[ 24 ] = skill_formula_24;
	skill_formula_arr[ 25 ] = skill_formula_25;
	skill_formula_arr[ 26 ] = skill_formula_26;
	skill_formula_arr[ 27 ] = skill_formula_27;
	skill_formula_arr[ 28 ] = skill_formula_28;
	skill_formula_arr[ 29 ] = skill_formula_29;
	skill_formula_arr[ 30 ] = skill_formula_30;
	skill_formula_arr[ 31 ] = skill_formula_31;
	skill_formula_arr[ 32 ] = skill_formula_32;
	skill_formula_arr[ 33 ] = skill_formula_33;
	skill_formula_arr[ 34 ] = skill_formula_34;
	skill_formula_arr[ 35 ] = skill_formula_35;
	skill_formula_arr[ 36 ] = skill_formula_36;
	skill_formula_arr[ 37 ] = skill_formula_37;
	skill_formula_arr[ 38 ] = skill_formula_38;
	skill_formula_arr[ 39 ] = skill_formula_39;
	skill_formula_arr[ 40 ] = skill_formula_40;
	skill_formula_arr[ 41 ] = skill_formula_41;
	skill_formula_arr[ 42 ] = skill_formula_42;
	skill_formula_arr[ 43 ] = skill_formula_43;
	skill_formula_arr[ 44 ] = skill_formula_44;
	skill_formula_arr[ 45 ] = skill_formula_45;
	skill_formula_arr[ 46 ] = skill_formula_46;
	skill_formula_arr[ 47 ] = skill_formula_47;
	skill_formula_arr[ 48 ] = skill_formula_48;
	skill_formula_arr[ 49 ] = skill_formula_49;
	skill_formula_arr[ 50 ] = skill_formula_50;
	skill_formula_arr[ 51 ] = skill_formula_51;
	skill_formula_arr[ 52 ] = skill_formula_52;
	skill_formula_arr[ 53 ] = skill_formula_53;
	skill_formula_arr[ 54 ] = skill_formula_54;
	skill_formula_arr[ 55 ] = skill_formula_55;
	skill_formula_arr[ 56 ] = skill_formula_56;
	skill_formula_arr[ 57 ] = skill_formula_57;
	skill_formula_arr[ 58 ] = skill_formula_58;
	skill_formula_arr[ 59 ] = skill_formula_59;
	skill_formula_arr[ 60 ] = skill_formula_60;
	skill_formula_arr[ 61 ] = skill_formula_61;
	skill_formula_arr[ 62 ] = skill_formula_62;
	skill_formula_arr[ 63 ] = skill_formula_63;
	skill_formula_arr[ 64 ] = skill_formula_64;
	skill_formula_arr[ 65 ] = skill_formula_65;
	skill_formula_arr[ 66 ] = skill_formula_66;
	skill_formula_arr[ 67 ] = skill_formula_67;
	skill_formula_arr[ 68 ] = skill_formula_68;
	skill_formula_arr[ 69 ] = skill_formula_69;
	skill_formula_arr[ 70 ] = skill_formula_70;
	skill_formula_arr[ 71 ] = skill_formula_71;
	skill_formula_arr[ 72 ] = skill_formula_72;
	skill_formula_arr[ 73 ] = skill_formula_73;
	skill_formula_arr[ 74 ] = skill_formula_74;
	skill_formula_arr[ 75 ] = skill_formula_75;
	skill_formula_arr[ 76 ] = skill_formula_76;
	skill_formula_arr[ 77 ] = skill_formula_77;
	skill_formula_arr[ 78 ] = skill_formula_78;
	skill_formula_arr[ 79 ] = skill_formula_79;
	skill_formula_arr[ 80 ] = skill_formula_80;
	skill_formula_arr[ 81 ] = skill_formula_81;
	skill_formula_arr[ 82 ] = skill_formula_82;
	skill_formula_arr[ 83 ] = skill_formula_83;
	skill_formula_arr[ 84 ] = skill_formula_84;
	skill_formula_arr[ 85 ] = skill_formula_85;
	skill_formula_arr[ 86 ] = skill_formula_86;
	skill_formula_arr[ 87 ] = skill_formula_87;
	skill_formula_arr[ 88 ] = skill_formula_88;
	skill_formula_arr[ 89 ] = skill_formula_89;
	skill_formula_arr[ 90 ] = skill_formula_90;
	skill_formula_arr[ 91 ] = skill_formula_91;
	skill_formula_arr[ 92 ] = skill_formula_92;
	skill_formula_arr[ 93 ] = skill_formula_93;
	skill_formula_arr[ 94 ] = skill_formula_94;
	skill_formula_arr[ 95 ] = skill_formula_95;
	skill_formula_arr[ 96 ] = skill_formula_96;
	skill_formula_arr[ 97 ] = skill_formula_97;
	skill_formula_arr[ 98 ] = skill_formula_98;
	skill_formula_arr[ 99 ] = skill_formula_99;
	skill_formula_arr[ 100 ] = skill_formula_100;
	skill_formula_arr[ 101 ] = skill_formula_101;
	skill_formula_arr[ 102 ] = skill_formula_102;
	skill_formula_arr[ 103 ] = skill_formula_103;
	skill_formula_arr[ 104 ] = skill_formula_104;
	skill_formula_arr[ 105 ] = skill_formula_105;
	skill_formula_arr[ 106 ] = skill_formula_106;
	skill_formula_arr[ 107 ] = skill_formula_107;
	skill_formula_arr[ 108 ] = skill_formula_108;
	skill_formula_arr[ 109 ] = skill_formula_109;
	skill_formula_arr[ 110 ] = skill_formula_110;
	skill_formula_arr[ 111 ] = skill_formula_111;
	skill_formula_arr[ 112 ] = skill_formula_112;
	skill_formula_arr[ 113 ] = skill_formula_113;
	skill_formula_arr[ 114 ] = skill_formula_114;
	skill_formula_arr[ 115 ] = skill_formula_115;
	skill_formula_arr[ 116 ] = skill_formula_116;
	skill_formula_arr[ 117 ] = skill_formula_117;
	skill_formula_arr[ 118 ] = skill_formula_118;
	skill_formula_arr[ 119 ] = skill_formula_119;
	skill_formula_arr[ 120 ] = skill_formula_120;
	skill_formula_arr[ 121 ] = skill_formula_121;
	skill_formula_arr[ 122 ] = skill_formula_122;
	skill_formula_arr[ 123 ] = skill_formula_123;
	skill_formula_arr[ 124 ] = skill_formula_124;
	skill_formula_arr[ 125 ] = skill_formula_125;
	skill_formula_arr[ 126 ] = skill_formula_126;
	skill_formula_arr[ 127 ] = skill_formula_127;
	skill_formula_arr[ 128 ] = skill_formula_128;
	skill_formula_arr[ 129 ] = skill_formula_129;
	skill_formula_arr[ 130 ] = skill_formula_130;
	skill_formula_arr[ 131 ] = skill_formula_131;
	skill_formula_arr[ 132 ] = skill_formula_132;
	skill_formula_arr[ 133 ] = skill_formula_133;
	skill_formula_arr[ 134 ] = skill_formula_134;
	skill_formula_arr[ 135 ] = skill_formula_135;
	skill_formula_arr[ 136 ] = skill_formula_136;
	skill_formula_arr[ 137 ] = skill_formula_137;
	skill_formula_arr[ 138 ] = skill_formula_138;
	skill_formula_arr[ 139 ] = skill_formula_139;
	skill_formula_arr[ 140 ] = skill_formula_140;
	skill_formula_arr[ 141 ] = skill_formula_141;
	skill_formula_arr[ 142 ] = skill_formula_142;
	skill_formula_arr[ 143 ] = skill_formula_143;
	skill_formula_arr[ 144 ] = skill_formula_144;
	skill_formula_arr[ 145 ] = skill_formula_145;
	skill_formula_arr[ 146 ] = skill_formula_146;
	skill_formula_arr[ 147 ] = skill_formula_147;
	skill_formula_arr[ 148 ] = skill_formula_148;
	skill_formula_arr[ 149 ] = skill_formula_149;
	skill_formula_arr[ 150 ] = skill_formula_150;
	skill_formula_arr[ 151 ] = skill_formula_151;
	skill_formula_arr[ 152 ] = skill_formula_152;
	skill_formula_arr[ 153 ] = skill_formula_153;
	skill_formula_arr[ 154 ] = skill_formula_154;
	skill_formula_arr[ 155 ] = skill_formula_155;
	skill_formula_arr[ 156 ] = skill_formula_156;
	skill_formula_arr[ 157 ] = skill_formula_157;
	skill_formula_arr[ 158 ] = skill_formula_158;
	skill_formula_arr[ 159 ] = skill_formula_159;
	skill_formula_arr[ 160 ] = skill_formula_160;
	skill_formula_arr[ 161 ] = skill_formula_161;
	skill_formula_arr[ 162 ] = skill_formula_162;
	skill_formula_arr[ 163 ] = skill_formula_163;
	skill_formula_arr[ 164 ] = skill_formula_164;
	skill_formula_arr[ 165 ] = skill_formula_165;
	skill_formula_arr[ 166 ] = skill_formula_166;
	skill_formula_arr[ 167 ] = skill_formula_167;
	skill_formula_arr[ 168 ] = skill_formula_168;
	skill_formula_arr[ 169 ] = skill_formula_169;
	skill_formula_arr[ 170 ] = skill_formula_170;
	skill_formula_arr[ 171 ] = skill_formula_171;
	skill_formula_arr[ 172 ] = skill_formula_172;
	skill_formula_arr[ 173 ] = skill_formula_173;
	skill_formula_arr[ 174 ] = skill_formula_174;

}
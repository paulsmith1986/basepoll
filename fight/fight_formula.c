#include "fight_formula.h"
/**
 * 获取攻击方对防守方的命中率
 */
int get_hr_rate( fight_unit_t *attack, fight_unit_t *defence )
{
	//攻击者 对 防御者 的命中率
	if ( attack->hr_cache[ defence->cell_id ] < 0 )
	{
		attack->hr_cache[ defence->cell_id ] = rate2ration( attack->hr_rate, defence->level );
	}
	//防御者 对 攻击者 的躲闪率
	if ( defence->dr_cache[ attack->cell_id ] < 0 )
	{
		defence->dr_cache[ attack->cell_id ] = rate2ration( defence->dr_rate, attack->level );
	}
	int tmp_hr = (100 + attack->hit_ration + attack->once_effect.hr + attack->hr_cache[ defence->cell_id ] ) - ( defence->dodge_ration + defence->dr_cache[ attack->cell_id ] ) ;
	if ( tmp_hr < 5 )
	{
		tmp_hr = 5;
	}
	return tmp_hr;
}

/**
 * 获取攻击方对防守方的暴击率
 */
int get_ds_rate( fight_unit_t *attack, fight_unit_t *defence )
{
	//攻击者 对 防御者 的暴击率
	if ( attack->ds_cache[ defence->cell_id ] < 0 )
	{
		attack->ds_cache[ defence->cell_id ] = rate2ration( attack->ds_rate, defence->level );
	}
	return attack->ds_ration + attack->once_effect.ds + attack->ds_cache[ defence->cell_id ];
}

/**
 * 获取防守方对攻击方的暴击率
 */
int get_ca_rate( fight_unit_t *attack, fight_unit_t *defence )
{
	//攻击者 对 防御者 的暴击率
	if ( defence->ca_cache[ attack->cell_id ] < 0 )
	{
		defence->ca_cache[ attack->cell_id ] = rate2ration( defence->ca_rate, attack->level );
	}
	return defence->ca_ration + defence->ca_cache[ attack->cell_id ];
}

/**
 * 将各种值转换成率的公式
 */
int rate2ration( int rate, int level )
{
	return ceil( (float)rate / ( (float)pow( level + 30, 3 ) * 0.0002 ) );
}


/**
 * 产生范围从x 到 y的随机数
 * @param	x				起始值
 * @param	y				结束值
 */
int first_rand( int x, int y )
{
	//如果 x 大于 y 交换值
	if ( x > y )
	{
		int z = y;
		y = x;
		x = z;
	}
	return rand() % ( y - x + 1 ) + x;
}

/**
 * 暴击伤害公式
 */
int get_ds_damage( int nomal_attack, int ds_damage_add, int tenacity )
{
	float tmp = (float)ds_damage_add / 100 + 0.5 - (float)tenacity / 100;
	if ( tmp < 0.1 )
	{
		tmp = 0.1;
	}
	return ceil( nomal_attack * ( 1 + tmp ) );
}

/**
 * 碾压造成的伤害
 */
int attack_press_damage( int life )
{
	float com = 0.05;
	return ceil( life * com );
}
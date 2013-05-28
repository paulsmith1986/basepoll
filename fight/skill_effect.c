#include "skill_effect.h"
/**
 * 0号效果（当一个效果不存在的时候，容错）
 * @param	aim_member		效果承受者
 * @param	effect_value	效果值
 * @param	effect_do_type	效果生效或者失效标志 EFFECT_ADD: 效果生效  EFFECT_CLEAN:效果失效
 * @param	effect_type		效果持续类型 EFFECT_ONCE: 单次 EFFECT_MANY: 持续效果
 */
void skill_effect_0( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
}

/**
 * 攻击力
 */
void skill_effect_1( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		aim_member->once_effect.attack += effect_value;
	}
	//持续效果
	else
	{
		aim_member->buff_attack += effect_value;
	}
}

/**
 * 攻击力增加百分比
 */
void skill_effect_2( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		aim_member->once_effect.attack_rate += effect_value;
	}
	//持续效果
	else
	{
		aim_member->buff_attack_rate += effect_value;
	}
}

/**
 * 增加防御力
 */
void skill_effect_3( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->defence += effect_value;
	}
}

/**
 * 增加防御力百分比
 */
void skill_effect_4( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->defence_rate += effect_value;
	}
}

/**
 * 命中率
 */
void skill_effect_5( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		aim_member->once_effect.hr += effect_value;
	}
	else
	{
		aim_member->hit_ration += effect_value;
	}
}

/**
 * 闪避率
 */
void skill_effect_6( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->dodge_ration += effect_value;
	}
}

/**
 * 暴击率
 */
void skill_effect_7( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		aim_member->once_effect.ds += effect_value;
	}
	else
	{
		aim_member->ds_ration += effect_value;
	}
}

/**
 * 免暴率
 */
void skill_effect_8( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		//aim_member->avoid_ds += effect_value;
	}
}

/**
 * 暴击伤害
 */
void skill_effect_9( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		aim_member->once_effect.ds_damage += effect_value;
	}
	else
	{
		aim_member->ds_damage += effect_value;
	}
}

/**
 * 伤害反弹
 */
void skill_effect_10( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->damage_return += effect_value;
	}
}

/**
 * 吸血率
 */
void skill_effect_11( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		aim_member->once_effect.suck += effect_value;
	}
	else
	{
		aim_member->suck += effect_value;
	}
}

/**
 * 怒气改变
 */
void skill_effect_12( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		change_anger_value( aim_member, effect_value );
	}
}

/**
 * 个体负面清除
 */
void skill_effect_13( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		remove_buff( aim_member, NULL, REMOVE_BUFF_BAD );
	}
}

/**
 * 个体正面清除
 */
void skill_effect_14( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		remove_buff( aim_member, NULL, REMOVE_BUFF_GOOD );
	}
}

/**
 * 个体所有状态清除
 */
void skill_effect_15( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		remove_buff( aim_member, NULL, REMOVE_BUFF_ALL );
	}
}

/**
 * 持续改变气血 dot
 */
void skill_effect_16( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->dot_life += effect_value;
	}
}

/**
 * 眩晕
 */
void skill_effect_17( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->stun = effect_value;
	}
}

/**
 * 锁穴
 */
void skill_effect_18( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->no_skill = effect_value;
	}
}

/**
 * 伤害加深
 */
void skill_effect_19( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->append_harm += effect_value;
	}
}

/**
 * 气血值改变，当下
 */
void skill_effect_20( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		change_life_value( aim_member, DAMAGE_SKILL, effect_value );
	}
}

/**
 * 气血值改变(按气血上限百分比)，当下
 */
void skill_effect_21( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		int damage = ceil( (float)( aim_member->life_max * effect_value ) / 100 );
		change_life_value( aim_member, DAMAGE_SKILL, damage );
	}
}

/**
 * 气血值改变(按当前气血值百分比)，当下
 */
void skill_effect_22( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		int damage = ceil( (float)( aim_member->life_now *  effect_value ) / 100 );
		change_life_value( aim_member, DAMAGE_SKILL, damage );
	}
}

/**
 * 镇静
 */
void skill_effect_23( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		change_anger_value( aim_member, aim_member->anger_now * -1 );
	}
}

/**
 * 武力值
 */
void skill_effect_24( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		//aim_member->once_effect.force += effect_value;
	}
	else
	{
		//aim_member->force += effect_value;
	}
}

/**
 * 智力值
 */
void skill_effect_25( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	//当下效果
	if ( EFFECT_ONCE == effect_type  )
	{
		//aim_member->once_effect.IQ += effect_value;
	}
	else
	{
		//aim_member->IQ += effect_value;
	}
}

/**
 * 初始化所有效果列表
 */
void init_skill_effect()
{
	int i;
	for ( i = 0; i <= SKILL_EFFECT_NUM; ++i )
	{
		skill_effect_arr[ i ] = skill_effect_0;
	}
	skill_effect_arr[ 1 ] = skill_effect_1;
	skill_effect_arr[ 2 ] = skill_effect_2;
	skill_effect_arr[ 3 ] = skill_effect_3;
	skill_effect_arr[ 4 ] = skill_effect_4;
	skill_effect_arr[ 5 ] = skill_effect_5;
	skill_effect_arr[ 6 ] = skill_effect_6;
	skill_effect_arr[ 7 ] = skill_effect_7;
	skill_effect_arr[ 8 ] = skill_effect_8;
	skill_effect_arr[ 9 ] = skill_effect_9;
	skill_effect_arr[ 10 ] = skill_effect_10;
	skill_effect_arr[ 11 ] = skill_effect_11;
	skill_effect_arr[ 12 ] = skill_effect_12;
	skill_effect_arr[ 13 ] = skill_effect_13;
	skill_effect_arr[ 14 ] = skill_effect_14;
	skill_effect_arr[ 15 ] = skill_effect_15;
	skill_effect_arr[ 16 ] = skill_effect_16;
	skill_effect_arr[ 17 ] = skill_effect_17;
	skill_effect_arr[ 18 ] = skill_effect_18;
	skill_effect_arr[ 19 ] = skill_effect_19;
	skill_effect_arr[ 20 ] = skill_effect_20;
	skill_effect_arr[ 21 ] = skill_effect_21;
	skill_effect_arr[ 22 ] = skill_effect_22;
	skill_effect_arr[ 23 ] = skill_effect_23;
	skill_effect_arr[ 24 ] = skill_effect_24;
	skill_effect_arr[ 25 ] = skill_effect_25;
}
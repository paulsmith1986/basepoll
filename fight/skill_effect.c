#include "skill_effect.h"

//所有效果集合
void ( *skill_effect_arr[ SKILL_EFFECT_NUM + 1 ] )( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type );

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
 * 韧性
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
		aim_member->tenacity += effect_value;
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

//反击
void skill_effect_10( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->ca_ration += effect_value;
	}
}

//碾压
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
		aim_member->once_effect.press += effect_value;
	}
	else
	{
		aim_member->press_ration += effect_value;
	}
}

/**
 * 伤害反弹
 */
void skill_effect_12( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
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
void skill_effect_13( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
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
void skill_effect_14( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->avoid_harm += effect_value;
	}
}

/**
 * 斗气上限
 */
void skill_effect_15( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->vigour_max += effect_value;
	}
}

/**
 * 受攻击时怒气增加
 */
void skill_effect_16( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//无用
}

/**
 * 个体负面清除
 */
void skill_effect_17( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		if ( NULL != aim_member->buff_list[ 0 ] )
		{
			remove_buff( aim_member, aim_member->buff_list[ 0 ], REMOVE_BUFF_BAD );
		}
		if ( NULL != aim_member->buff_list[ 1 ] )
		{
			remove_buff( aim_member, aim_member->buff_list[ 1 ], REMOVE_BUFF_BAD );
		}
	}
}

/**
 * 个体正面清除
 */
void skill_effect_18( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		if ( NULL != aim_member->buff_list[ 0 ] )
		{
			remove_buff( aim_member, aim_member->buff_list[ 0 ], REMOVE_BUFF_GOOD );
		}
		if ( NULL != aim_member->buff_list[ 1 ] )
		{
			remove_buff( aim_member, aim_member->buff_list[ 1 ], REMOVE_BUFF_GOOD );
		}
	}
}

/**
 * 个体所有状态清除
 */
void skill_effect_19( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		if ( NULL != aim_member->buff_list[ 0 ] )
		{
			remove_buff( aim_member, aim_member->buff_list[ 0 ], REMOVE_BUFF_ALL );
		}
		if ( NULL != aim_member->buff_list[ 1 ] )
		{
			remove_buff( aim_member, aim_member->buff_list[ 1 ], REMOVE_BUFF_ALL );
		}
	}
}

/**
 * 集气速度
 */
void skill_effect_20( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_MANY == effect_type )
	{
		//离下次出手的时间
		int next_attack_time = aim_member->next_round - aim_member->combat_info->second;
		if ( EFFECT_CLEAN == effect_do_type )
		{
			effect_value *= -1;
		}
		int old_speed = aim_member->base_speed;
		aim_member->base_speed -= effect_value;
		int new_time = ( next_attack_time * aim_member->base_speed ) / old_speed;
		aim_member->next_round = aim_member->combat_info->second + new_time;
	}
}

//气血dot效果
void skill_effect_21( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 移除效果
	if ( EFFECT_CLEAN == effect_do_type )
	{
		life_dot_t *dot_head = aim_member->dot_life;
		life_dot_t *last_dot = NULL;
		while ( NULL != dot_head )
		{
			if ( dot_head->value == effect_value )
			{
				if ( NULL == last_dot )
				{
					aim_member->dot_life = dot_head->next;
				}
				else
				{
					last_dot->next = dot_head->next;
				}
				destroy_life_dot( dot_head, aim_member->combat_info );
				break;
			}
			last_dot = dot_head;
			dot_head = dot_head->next;
		}
	}
	else		//增加效果
	{
		life_dot_t *tmp_dot = create_life_dot( aim_member->combat_info );
		tmp_dot->value = effect_value;
		tmp_dot->next = aim_member->dot_life;
		aim_member->dot_life = tmp_dot;
	}
}

/**
 * 负面状态无效
 */
void skill_effect_22( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		aim_member->avoid_debuff = 0;
	}
	else
	{
		skill_effect_17( aim_member, 0, 0, EFFECT_ONCE );
		aim_member->avoid_debuff = 1;
	}
}


/**
 * 眩晕
 */
void skill_effect_23( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_CLEAN == effect_do_type )
	{
		aim_member->stun = 0;
	}
	else
	{
		aim_member->stun = 1;
	}
}

/**
 * 锁穴
 */
void skill_effect_24( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_CLEAN == effect_do_type )
	{
		aim_member->no_skill = 0;
	}
	else
	{
		aim_member->no_skill = 1;
	}
}

/**
 * 伤害加深
 */
void skill_effect_25( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
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
 * 斗气消耗
 */
void skill_effect_26( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//无用
}

/**
 * 怒气消耗
 */
void skill_effect_27( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//无用
}

/**
 * 斗气消耗
 */
void skill_effect_28( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//无用
}

/**
 * 怒气消耗
 */
void skill_effect_29( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//无用
}

/**
 * 气血改变
 */
void skill_effect_30( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		//remove_buff( aim_member, NULL, REMOVE_BUFF_BAD );
	}
}

/**
 * 承受伤害加血
 */
void skill_effect_31( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//无用
}

/**
 * 镇静
 */
void skill_effect_32( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		change_anger_value( aim_member, aim_member->anger_now * -1 );
	}
}

/**
 * 怒气改变
 */
void skill_effect_33( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	//如果是效果失效 值要变成负数
	if ( EFFECT_CLEAN == effect_do_type )
	{
		effect_value *= -1;
	}
	if ( EFFECT_MANY == effect_type )
	{
		aim_member->avoid_harm += effect_value;
	}
}

/**
 * 当下碾压伤害
 */
void skill_effect_34( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		aim_member->once_effect.press_damage += effect_value;
	}
}

/**
 * 碾压无效
 */
void skill_effect_35( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_CLEAN == effect_do_type )
	{
		aim_member->avoid_press = 0;
	}
	else
	{
		aim_member->avoid_press = 1;
	}
}

/**
 * 斗气改变
 */
void skill_effect_36( fight_unit_t *aim_member, int effect_value, int effect_do_type, int effect_type )
{
	if ( EFFECT_ONCE == effect_type )
	{
		change_vigour_value( aim_member, effect_value );
	}
}

/**
 * 初始化所有效果列表
 */
void init_skill_effect()
{
	skill_effect_arr[ 0 ] = skill_effect_0;
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
	skill_effect_arr[ 26 ] = skill_effect_26;
	skill_effect_arr[ 27 ] = skill_effect_27;
	skill_effect_arr[ 28 ] = skill_effect_28;
	skill_effect_arr[ 29 ] = skill_effect_29;
	skill_effect_arr[ 30 ] = skill_effect_30;
	skill_effect_arr[ 31 ] = skill_effect_31;
	skill_effect_arr[ 32 ] = skill_effect_32;
	skill_effect_arr[ 33 ] = skill_effect_33;
	skill_effect_arr[ 34 ] = skill_effect_34;
	skill_effect_arr[ 35 ] = skill_effect_35;
	skill_effect_arr[ 36 ] = skill_effect_36;
}
#include "first_fight.h"
#include "skill_effect.h"
#include "skill_formula.h"
#include "struct_pool.h"
#include "fight_debug.h"
#include "fight_protocol.h"
/**
 * 战斗中的一秒
 * @param	combat_info		战斗信息
 */
void once_second( combat_info_t *combat_info )
{
	//战斗超最大时间，正式结束
	if ( ++combat_info->second > combat_info->max_second )
	{
		combat_info->is_over = 1;
		fight_final_end( combat_info );
		return;
	}

	//输出结果
	result_pack_sec_t pack_sec;
	pack_sec.pack_id = RESULT_NEW_SEC;
	pack_sec.sec = combat_info->second;
	push_fight_result( combat_info, &pack_sec, sizeof( result_pack_sec_t ) );

	//Dot流血效果
	fight_dot_life( combat_info->atk_member );
	fight_dot_life( combat_info->def_member );

	//出手顺序
	static const int attack_seqence[ 9 ] = { 5, 2, 8, 4, 1, 7, 3, 0, 6 };
	register int now_fight_time = combat_info->second;
	//确认出手者
	register int i;
	for ( i = 0; i < SIDE_MEMBER; ++i )
	{
		fight_unit_t *round_attack_member[ 2 ] = {NULL, NULL};
		//攻击方该位置有人
		if ( check_is_round( combat_info->atk_member, i, now_fight_time ) )
		{
			round_attack_member[ 0 ] = combat_info->atk_member[ i ];
		}
		//防守方有人
		if ( check_is_round( combat_info->def_member, i, now_fight_time ) )
		{
			round_attack_member[ 1 ] = combat_info->def_member[ i ];
		}
		//当攻击和防守方都有人时
		if ( NULL != round_attack_member[ 0 ] && NULL != round_attack_member[ 1 ] )
		{
			once_round( round_attack_member[ combat_info->now_rand_side ] );
			round_attack_member[ combat_info->now_rand_side ] = NULL;
			combat_info->now_rand_side = 1 - combat_info->now_rand_side;
		}
		//双方都没有人出手
		if ( NULL == round_attack_member[ 0 ] && NULL == round_attack_member[ 1 ] )
		{
			continue;
		}
		//攻击方有人出手
		else if ( NULL != round_attack_member[ 0 ] )
		{
			once_round( round_attack_member[ 0 ] );
		}
		else
		{
			once_round( round_attack_member[ 1 ] );
		}
	}
}

/**
 * 尝试使用某个技能
 */
int try_use_skill( fight_unit_t *attack_member, use_skill_t *use_skill )
{
	skill_t *skill_info = use_skill->skill_info;
	//被锁穴了，斗气技能不能使出来
	if ( attack_member->no_skill && 1 == skill_info->skill_type )
	{
		return 0;
	}
	//出手轮数不够，还不能使用技能
	if ( attack_member->round_num < skill_info->start_round )
	{
		return 0;
	}
	//斗气技能
	if ( 1 == skill_info->skill_type )
	{
		//技能冻结时间还未结束
		if ( attack_member->combat_info->second < use_skill->cd_use_time )
		{
			#ifdef FIRST_DEBUG
			php_printf( "怒气技能在CD中，时间到 %d 秒\n", use_skill->cd_use_time );
			#endif
			return 0;
		}
		#ifdef FIRST_DEBUG
		php_printf( " 需要消耗斗气:%d 当前斗气 %d\n", use_skill->skill_cost, attack_member->vigour_now );
		#endif
		if ( attack_member->vigour_now < use_skill->skill_cost )
		{
			return 0;
		}
	}
	//怒气技能
	else if ( 2 == skill_info->skill_type )
	{
		if ( attack_member->combat_info->second < use_skill->cd_use_time )
		{
			#ifdef FIRST_DEBUG
			php_printf( "怒气技能在CD中，时间到 %d 秒\n", use_skill->cd_use_time );
			#endif
			return 0;
		}
		#ifdef FIRST_DEBUG
		php_printf( " 需要消耗怒气:%d 当前怒气 %d\n", use_skill->skill_cost, use_skill->anger_now );
		#endif
		if ( attack_member->anger_now < use_skill->skill_cost )
		{
			return 0;
		}
	}
	//之后所有的公式都用skill_level
	attack_member->skill_level = use_skill->skill_level;
	//计算气血消耗
	int life_cost = 0;
	//写死的
	if ( 0 == skill_info->life_cost[ 0 ] )
	{
		life_cost = skill_info->life_cost[ 1 ];
	}
	//公式算出来的值
	else if ( 1 == skill_info->life_cost[ 0 ] )
	{
		life_cost = skill_formula( skill_info->life_cost[ 1 ], attack_member );
	}
	//需要消耗气血
	if ( life_cost > 0 )
	{
		if ( attack_member->life_now < life_cost )
		{
			#ifdef FIRST_DEBUG
			php_printf( " 需要消耗气血 %d 当前气血 %d", life_cost, attack_member->life_now );
			#endif
			return 0;
		}
		else
		{
			change_life_value( attack_member, DAMAGE_SKILL_COST, life_cost );
		}
	}
	//斗气值改变
	if ( 1 == skill_info->skill_type )
	{
		change_vigour_value( attack_member, use_skill->skill_cost * -1 );
	}
	//怒气值改变
	else
	{
		change_anger_value( attack_member, use_skill->skill_cost * -1 );
	}
	//确定下一次出手的时间
	if ( skill_info->skill_cd > 0 )
	{
		#ifdef FIRST_DEBUG
		php_printf( " 该技能CD时间为 %d", skill_info->skill_cd );
		#endif
		use_skill->cd_use_time = attack_member->combat_info->second + skill_info->skill_id;
	}
	return 1;
}

/**
 * 一个成员的一轮
 */
void once_round( fight_unit_t *attack_member )
{
	//下一次出手的时间
	attack_member->next_round += attack_member->base_speed;
	attack_member->round_num++;
	change_anger_value( attack_member, ANGER_ADD_NUM );
	//空技能
	skill_t vitual_skill;
	//空技能
	use_skill_t vitual_use_skill;
	//判断是否使用怒气技能
	use_skill_t *use_skill;
	if ( NULL != attack_member->anger_skill && try_use_skill( attack_member, attack_member->anger_skill ) )
	{
		use_skill = attack_member->vigour_skill;
	}
	else if ( NULL != attack_member->vigour_skill && try_use_skill( attack_member, attack_member->vigour_skill ) )
	{
		use_skill = attack_member->vigour_skill;
	}
	else
	{
		memset( &vitual_skill, 0, sizeof( skill_t ) );
		vitual_skill.attack_num = 1;
		vitual_skill.object_aim = 0;
		vitual_skill.self_aim = 0;
		vitual_skill.hit_ration[ 0 ] = 1;
		vitual_use_skill.skill_info = &vitual_skill;
		vitual_use_skill.add_hr = 0;
		vitual_use_skill.skill_level = 0;
		use_skill = &vitual_use_skill;
		use_skill = &vitual_use_skill;
	}
	//攻击
	once_attack( attack_member, use_skill, ATTACK_NORMAL );
}

/**
 * 一次攻击过程
 * @param	attack_member	战斗成员
 * @param	use_skill		使用的技能
 * @param	atta_type		攻击类型 1:正常攻击 2:反击
 */
void once_attack( fight_unit_t *attack_member, use_skill_t *use_skill, int attack_type )
{
	skill_t *skill_info = use_skill->skill_info;
	combat_info_t *combat_info = attack_member->combat_info;
	//输出结果
	result_pack_attack_t pack_attack;
	pack_attack.pack_id = RESULT_ATTACK;
	pack_attack.index_id = attack_member->index;
	pack_attack.skill_id = skill_info->skill_id;
	pack_attack.attack_area = skill_info->object_aim;
	pack_attack.attack_area = skill_info->attack_num;
	push_fight_result( combat_info, &pack_attack, sizeof( result_pack_attack_t ) );

	//目标列表
	int aim_list[ SIDE_MEMBER ];
	int aim_id;
	//己方作用人数
	int self_aim_num = 0;
	fight_unit_t **my_side_member;
	#ifdef FIRST_DEBUG
	printf( YELLOW_FONT( "\n==========" GREEN_FONT( "己方目标" ) "========== " ) );
	#endif
	//技能对自己方有效果或者状态
	if ( is_do_self_side( skill_info, attack_member ) )
	{
		#ifdef FIRST_DEBUG
		printf( " \n目标类型 " );
		#endif
		find_oppose_side( 1 - attack_member->side, attack_member->combat_info, my_side_member );
		self_aim_num = find_indirect_aim( skill_info->self_aim, attack_member->cell_id, aim_list, my_side_member );
		for ( aim_id = 0; aim_id < self_aim_num; ++aim_id )
		{
			fight_unit_t *aim_member = my_side_member[ aim_list[ aim_id ] ];
			if ( NULL == aim_member || is_dead( aim_member ) )
			{
				continue;
			}

			//被动技能的效果和状态
			pasv_skill_effect_and_buff( attack_member, aim_member, 0 == aim_id, skill_info->skill_id );

			//技能命中率 对己方的默认命中为100
			int hit_ration = 100;
			//获取对己方的命中率
			skill_self_hit_ration( attack_member, use_skill, skill_info, hit_ration );
			if ( !is_rand( hit_ration ) )
			{
				#ifdef FIRST_DEBUG
				printf( " 对自己方，合计命中率：" GREEN_FONT( " %d" ) RED_FONT( "【Miss】" ), hit_ration );
				#endif
				continue;
			}
			//技能对己方的效果
			if ( NULL != skill_info->self_effect )
			{
				direct_effect( attack_member, aim_member, skill_info->self_effect );
			}
			//技能对己方的状态
			if ( NULL != skill_info->self_buff )
			{
				skill_buff( attack_member, aim_member, skill_info->self_buff );
			}
		}
	}
	#ifdef FIRST_DEBUG
	else
	{
		printf( " 【NULL】 " );
	}
	#endif
	#ifdef FIRST_DEBUG
	printf( YELLOW_FONT( "\n==========" ) RED_FONT( "敌方目标" ) YELLOW_FONT( "========== " ) );
	#endif
	//技能对敌方有作用效果
	if ( is_live( attack_member ) && is_attack_object( skill_info, attack_member ) )
	{
		#ifdef FIRST_DEBUG
		printf( "\n 目标类型 " );
		#endif
		//对方所有成员
		fight_unit_t **aim_side_member;
		find_oppose_side( attack_member->side, attack_member->combat_info, aim_side_member );
		int direct_aim = find_direct_aim( attack_member->cell_id, aim_side_member );
		//找出本次攻击对方受影响的列表
		int object_aim_num = find_indirect_aim( skill_info->object_aim, direct_aim, aim_list, aim_side_member );
		#ifdef FIRST_DEBUG
		printf( "\n攻击次数:" RED_FONT( "%d " ), skill_info->attack_num );
		debug_attack_type( skill_info->attack_type, "\n" );
		#endif
		for ( aim_id = 0; aim_id < object_aim_num; ++aim_id )
		{
			fight_unit_t *aim_member = aim_side_member[ aim_list[ aim_id ] ];

			if ( NULL == aim_member || is_dead( aim_member ) )
			{
				continue;
			}

			//普通攻击，被攻击的人增加怒气
			if ( ATTACK_NORMAL == attack_type )
			{
				change_anger_value( aim_member, ANGER_ADD_NUM );
			}

			#ifdef FIRST_DEBUG
			printf( RED_FONT( "\n【被击】" ) "成员: %d_%d 气血:%d \n\t", aim_member->side, aim_member->cell_id, aim_member->life_now );
			#endif
			//本次攻击的命中率
			int hit_ration;
			skill_object_hit_ration( attack_member, use_skill, skill_info, hit_ration, aim_member );
			//第一次判断的命中率
			int is_first_time_hr = is_rand( hit_ration );
			int is_pasv_effect_done = 0;
			//技能效果和状态的命中判断
			if ( is_first_time_hr )
			{
				//被动技能的效果和状态
				pasv_skill_effect_and_buff( attack_member, aim_member, 0 == aim_id, skill_info->skill_id );

				//记录已经执行过技能被动效果了
				is_pasv_effect_done = 1;
				//技能对敌方的效果
				if ( NULL != skill_info->object_effect )
				{
					direct_effect( attack_member, aim_member, skill_info->object_effect );
				}
				//技能对敌方的状态
				if ( NULL != skill_info->object_buff )
				{
					skill_buff( attack_member, aim_member, skill_info->object_buff );
				}
			}
			//被闪避
			else
			{
				#ifdef FIRST_DEBUG
				printf( CYAN_FONT( "【MISS】 " ) );
				#endif
				fight_dr_pack( aim_member );
			}

			//攻击判断
			if ( skill_info->attack_num > 0 )
			{
				int attack_i;
				for ( attack_i = 0; attack_i < skill_info->attack_num; ++attack_i )
				{
					//每一次单独命中判断
					if ( ( 0 == attack_i && is_first_time_hr ) || ( 0 != attack_i && is_rand( hit_ration ) ) )
					{
						//如果该技能打多人时，攻击力有递增功能
						if ( 0 != skill_info->attack_step && 0 != attack_i )
						{
							//先加上去这个临时增加的攻击
							attack_member->once_effect.attack_rate += skill_info->attack_step;
							normal_attack( attack_member, aim_member );
							//再减掉这个临时增加的攻击
							attack_member->once_effect.attack_rate -= skill_info->attack_step;
						}
						else
						{
							normal_attack( attack_member, aim_member );
						}
						//检测被动技能效果和状态有没有执行过
						if ( !is_pasv_effect_done )
						{
							pasv_skill_effect_and_buff( attack_member, aim_member, 0 == aim_id, skill_info->skill_id );
							is_pasv_effect_done = 1;
						}
						//有可能攻击者被反弹死
						if ( is_dead( aim_member ) || is_dead( attack_member ) )
						{
							break;
						}
					}
					//继续输出闪避包
					else if ( 0 != attack_i )
					{
						#ifdef FIRST_DEBUG
						printf( CYAN_FONT( "【MISS】 " ) );
						#endif
						fight_dr_pack( aim_member );
					}
				}
				//如果是第一个受攻击者,判断反击
				if ( ATTACK_NORMAL == attack_type && 0 == aim_id )
				{
					for ( attack_i = 0; attack_i < skill_info->attack_num; ++attack_i )
					{
						if ( is_live( aim_member ) && is_live( aim_member ) && 0 == aim_member->stun )
						{
							int attack_back_ration = get_ca_rate( aim_member, attack_member );
							if ( is_rand( attack_back_ration ) )
							{
								counter_attack( attack_member, aim_member );
							}
						}
					}
				}
			}

			//攻击者自己死掉
			if ( is_dead( attack_member ) )
			{
				break;
			}
		}
	}
	#ifdef FIRST_DEBUG
	else
	{
		printf( " 【NULL】 " );
	}
	#endif
	//清除出手方当下效果
	for ( aim_id = 0; aim_id < self_aim_num; ++aim_id )
	{
		fight_unit_t *member = my_side_member[ aim_list[ aim_id ] ];
		if ( NULL != member )
		{
			memset( &member->once_effect, 0, sizeof( effect_once_t ) );
		}
	}
	if ( is_live( attack_member ) )
	{
		int next_time;
		//确定下一次出手的时间
		if ( combat_info->side_num[ attack_member->side ] <= MIN_NEXT_ATTACK_TIME )
		{
			next_time = MIN_NEXT_ATTACK_TIME;
		}
		else
		{
			next_time = combat_info->side_num[ attack_member->side ];
		}
	}
	#ifdef FIRST_DEBUG
	printf( "\n" );
	#endif
	//状态过期
	fight_buff_expire( combat_info->atk_member );
	fight_buff_expire( combat_info->def_member );
	//如果战斗结束了,判断有没有下一队成员
	if ( combat_info->is_over )
	{
		combat_try_change_team( combat_info );
		//没有下一队，战斗真的结束了
		if ( combat_info->is_over )
		{
			fight_final_end( combat_info );
		}
	}
}

/**
 * 反击攻击
 * @param	attack_member	攻击者
 * @param	aim_member		防守者
 */
void counter_attack( fight_unit_t *attack_member, fight_unit_t *aim_member )
{
	int tmp_hr = get_hr_rate( attack_member, aim_member );
	if ( is_rand( tmp_hr ) )
	{
		normal_attack( attack_member, aim_member );
		//判断给对手的状态或者效果
		if ( is_live( attack_member ) && is_live( aim_member ) )
		{

		}
	}
	else
	{
		//闪避
	}

}

/**
 * 一次出手的被动技能效果和状态
 * @param	attack_member	来源者
 * @param	aim_member		承受者
 * @param	is_direct		是否是直接目标
 * @param	skill_id		技能ID
 */
void pasv_skill_effect_and_buff( fight_unit_t *attack_member, fight_unit_t *aim_member, int is_direct, int skill_id )
{
	ext_effect_t **ext_effect;
	ext_buff_t **ext_buff;
	//根据立场判断self还是object
	if ( attack_member->side == aim_member->side )
	{
		ext_effect = attack_member->attack_self_eff;
		ext_buff = attack_member->attack_self_buff;
	}
	else
	{
		ext_effect = attack_member->attack_object_eff;
		ext_buff = attack_member->attack_object_buff;
	}
	//直接目标
	if ( is_direct )
	{
		//直接目标被动技能的效果
		if ( NULL != ext_effect[ AIM_DIRECT ] )
		{
			direct_extend_effect( aim_member, ext_effect[ AIM_DIRECT ], skill_id );
		}
		//直接目标被动技能的状态
		if ( NULL != ext_buff[ AIM_DIRECT ] )
		{
			add_extend_buff( attack_member, aim_member, ext_buff[ AIM_DIRECT ], skill_id );
		}
	}
	//间接目标
	else
	{
		//间接目标(不含直接目标)被动技能的效果
		if ( NULL != ext_effect[ AIM_EXCEPT_DIRECT ] )
		{
			direct_extend_effect( aim_member, ext_effect[ AIM_EXCEPT_DIRECT ], skill_id );
		}
		//间接目标(不含直接目标)被动技能的状态
		if ( NULL != ext_buff[ AIM_EXCEPT_DIRECT ] )
		{
			add_extend_buff( attack_member, aim_member, ext_buff[ AIM_EXCEPT_DIRECT ], skill_id );
		}
	}
	//间接目标被动技能的效果
	if ( NULL != ext_effect[ AIM_INDIRECT ] )
	{
		direct_extend_effect( aim_member, ext_effect[ AIM_INDIRECT ], skill_id );
	}
	//间接目标被动技能的状态
	if ( NULL != ext_buff[ AIM_INDIRECT ] )
	{
		add_extend_buff( attack_member, aim_member, ext_buff[ AIM_INDIRECT ], skill_id );
	}
}

/**
 * 给一个成员加状态( 技能主动状态 )
 * @param	attack_member	状态来源者
 * @param	aim_member		状态承受者
 * @param	sk_buff			技能状态信息
 */
static void skill_buff( fight_unit_t *attack_member, fight_unit_t *aim_member, sk_buff_t *sk_buff )
{
	buff_t *buff_info = sk_buff->buff_info;
	//免疫负面效果
	if ( 2 == buff_info->buff_type && aim_member->avoid_debuff )
	{
		return;
	}
	//连击技能
	if ( NULL != sk_buff->next )
	{
		//如果need_state是null了，就直接上这个状态
		while ( NULL != sk_buff && NULL != sk_buff->need_buff )
		{
			fight_buff_t *have_buff = chk_have_buff( aim_member, sk_buff->need_buff->buff_info );
			if ( NULL != have_buff )
			{
				remove_buff( aim_member, have_buff, REMOVE_BUFF_ONE );
				break;
			}
			sk_buff = sk_buff->next;
		}
		//出错的情况
		if ( NULL == sk_buff )
		{
			return;
		}
		buff_info = sk_buff->buff_info;
	}
	int last_time;
	//固定时间
	if ( 0 == sk_buff->last_time_type )
	{
		last_time = sk_buff->last_time;
	}
	//公式计算
	else
	{
		last_time = skill_formula( sk_buff->last_time, attack_member );
	}
	add_buff( attack_member, aim_member, buff_info, attack_member->skill_level, last_time );
}

/**
 * 给一个成员加状态
 * @param	attack_member	状态来源者
 * @param	aim_member		状态承受者
 * @param	buff_info		状态信息
 * @param	skill_level		技能等级
 * @param	buff_lst_time	持续时间
 */
static void add_buff( fight_unit_t *attack_member, fight_unit_t *aim_member, buff_t *buff_info, int skill_level, int buff_last_time )
{
	if ( is_dead( aim_member ) )
	{
		return;
	}
	#ifdef FIRST_DEBUG
	printf( CYAN_FONT( "\n〓〓〓〓〓〓〓〓" ) "成员 " CYAN_FONT( "%d_%d" ) " 增加 ", aim_member->side, aim_member->cell_id );
	debug_buff_type( buff_info->buff_type );
	printf( " " YELLOW_FONT( "【%s】 时间:" GREEN_FONT( "%d 秒\n" ) ), debug_buff_name( buff_info->buff_id ), buff_last_time );
	#endif

	fight_buff_t *same_buff = chk_have_buff( aim_member, buff_info );
	//已经有相同的状态了
	if ( NULL != same_buff )
	{
		//状态不可以叠加，或者 来自相同成员的，移除该状态
		if ( !buff_info->can_sup || same_buff->by_member_id == attack_member->index )
		{
			remove_buff( attack_member, same_buff, REMOVE_BUFF_ONE );
		}
	}
	combat_info_t *combat_info = attack_member->combat_info;
	int expire_type = buff_info->expire_type;
	fight_buff_t *new_buff = create_fight_buff( combat_info );
	new_buff->expire = buff_last_time + combat_info->second;
	new_buff->level = skill_level;
	new_buff->buff_info = buff_info;
	new_buff->by_member_id = attack_member->index;
	//之前没有任何状态
	if ( NULL == aim_member->buff_list[ expire_type ] )
	{
		new_buff->next = NULL;
		aim_member->buff_list[ expire_type ] = new_buff;
	}
	//按过期先后顺序插入
	else
	{
		fight_buff_t *last_buff = NULL;
		fight_buff_t *head_buff = aim_member->buff_list[ expire_type ];
		int is_insert = 0;
		while ( NULL != head_buff )
		{
			//找到位置
			if ( head_buff->expire >= new_buff->expire )
			{
				is_insert = 1;
				new_buff->next = head_buff;
				if ( NULL == last_buff )
				{
					aim_member->buff_list[ expire_type ] = new_buff;
				}
				else
				{
					last_buff->next = head_buff;
				}
				break;
			}
			else
			{
				last_buff = head_buff;
				head_buff = head_buff->next;
			}
		}
		//插入到末尾
		if ( 0 == is_insert )
		{
			last_buff->next = new_buff;
			new_buff->next = NULL;
		}
	}

	//存入结果
	result_pack_buff_add_t pack_buff;
	pack_buff.pack_id = RESULT_BUFF_ADD;
	pack_buff.index_id = aim_member->index;
	pack_buff.buff_type = buff_info->buff_type;
	pack_buff.buff_id = buff_info->buff_id;
	push_fight_result( combat_info, &pack_buff, sizeof( pack_buff ) );

	//效果生效
	buff_effect( attack_member, aim_member, buff_info->buff_effect, EFFECT_ADD, new_buff->cache_value );
}

/**
 * 被动技能状态
 * @param	attack_member	状态来源者
 * @param	aim_member		状态承受者
 * @param	buff_info		状态的头指针
 * @param	skill_id		技能等级
 */
void add_extend_buff( fight_unit_t *attack_member, fight_unit_t *aim_member, ext_buff_t *buff_head, int skill_id )
{
	//承受者死了
	if ( is_dead( aim_member ) )
	{
		return;
	}
	int old_skill_level = attack_member->skill_level;
	while ( NULL != buff_head )
	{
		if ( 0 == buff_head->need_skill || skill_id == buff_head->need_skill )
		{
			attack_member->skill_level = buff_head->level;
			add_buff( attack_member, aim_member, buff_head->buff_info, attack_member->skill_level, buff_head->last_time );
		}
		buff_head = buff_head->next;
	}
	//完事后，再次改成主动技能等级
	attack_member->skill_level = old_skill_level;
}

/**
 * 移除状态
 * @param	member			成员
 * @param	buff			状态
 * @param	remove_type		方式 REMOVE_BUFF_ONE:指定 REMOVE_BUFF_EXPIRE:过期 REMOVE_BUFF_ALL:全部 REMOVE_BUFF_BAD:负面 REMOVE_BUFF_GOOD:正面
 */
void remove_buff( fight_unit_t *member, fight_buff_t *buff, int remove_type )
{
	fight_buff_t *last_buff;
	int expire_type = buff->buff_info->expire_type;
	fight_buff_t *head_buff = member->buff_list[ expire_type ];
	while ( NULL != head_buff )
	{
		int is_remove = 0;
		int is_break = 0;
		combat_info_t *combat_info = member->combat_info;
		switch ( remove_type )
		{
			case REMOVE_BUFF_ONE:			//移除指定的buff
			{
				if ( head_buff == buff )
				{
					is_remove = 1;
					is_break = 1;
				}
				break;
			}
			case REMOVE_BUFF_EXPIRE:		//移除过期的buff
			{
				if ( head_buff->expire <= combat_info->second )
				{
					is_remove = 1;
				}
				else
				{
					is_break = 1;
				}
				break;
			}
			case REMOVE_BUFF_ALL:			//移除全部
				is_remove = 1;
			break;
			case REMOVE_BUFF_BAD:			//移除负面的状态
			{
				if ( 2 == head_buff->buff_info->buff_type )
				{
					is_remove = 1;
				}
				break;
			}
			case REMOVE_BUFF_GOOD:			//移除正面的状态
			{
				if ( 1 == head_buff->buff_info->buff_type )
				{
					is_remove = 1;
				}
				break;
			}
			default:
			break;
		}
		//需要移除
		if ( is_remove )
		{
			//在表头
			if ( head_buff == member->buff_list[ expire_type ] )
			{
				member->buff_list[ expire_type ] = head_buff->next;
			}
			//不在表头
			else
			{
				last_buff->next = head_buff->next;
			}
			fight_buff_t *tmp_del = head_buff;
			head_buff = head_buff->next;
			#ifdef FIRST_DEBUG
			printf(  GREEN_FONT( "\n〓〓〓〓〓〓〓〓" ) "成员 " CYAN_FONT( "%d_%d" ) " 移除 ", member->side, member->cell_id );
			debug_buff_type( tmp_del->buff_info->buff_type );
			printf( " " YELLOW_FONT( "【%s】" ) "，移除原因：", debug_buff_name( tmp_del->buff_info->buff_id ) );
			debug_buff_remove_reason( remove_type );
			#endif
			buff_effect( NULL, member, tmp_del->buff_info->buff_effect, EFFECT_CLEAN, tmp_del->cache_value );

			//输出结果
			result_pack_buff_del_t pack_del_buff;
			pack_del_buff.pack_id = RESULT_BUFF_DEL;
			pack_del_buff.index_id = member->index;
			pack_del_buff.buff_id = tmp_del->buff_info->buff_id;
			push_fight_result( combat_info, &pack_del_buff, sizeof( result_pack_buff_del_t ) );

			destroy_fight_buff( tmp_del, combat_info );
		}
		else
		{
			last_buff = head_buff;
			head_buff = head_buff->next;
		}
		if ( is_break )
		{
			break;
		}
	}
}

/**
 * 清除所有的状态( 不做战斗属性计算 )
 * @param	member			成员
 */
void clean_role_buff_list( fight_unit_t *member )
{
	int i;
	for ( i = 0; i < 2; ++i )
	{
		fight_buff_t *head_buff = member->buff_list[ i ];
		while ( NULL != head_buff )
		{
			fight_buff_t *del_buff = head_buff;
			head_buff = head_buff->next;
			destroy_fight_buff( del_buff, member->combat_info );
		}
	}

}

/**
 * 状态效果
 * @param	attack_member	状态来源者
 * @param	aim_member		状态承受者
 * @param	effect_head		效果头指针
 * @param	effect_do_type	效果生效或者失效标志 EFFECT_ADD: 效果生效  EFFECT_CLEAN:效果失效
 * @param	buff_eff_value	缓存值
 */
void buff_effect( fight_unit_t *attack_member, fight_unit_t *aim_member, sk_effect_t *effect_head, int effect_do_type, int buff_eff_value[] )
{
	int effect_index = 0;
	while ( NULL != effect_head )
	{
		int effect_value;
		//如果是效果失效且有传入缓存值，直接使用缓存值
		if ( EFFECT_CLEAN == effect_do_type && NULL != buff_eff_value )
		{
			effect_value = buff_eff_value[ effect_index++ ];
		}
		//需要计算效果值
		else
		{
			//填是填写的
			if ( 0 == effect_head->effect_value_type )
			{
				effect_value = effect_head->effect_value;
			}
			//公式计算
			else
			{
				effect_value = skill_formula( effect_head->effect_value, attack_member );
			}

			if ( NULL != buff_eff_value )
			{
				buff_eff_value[ effect_index++ ] = effect_value;
			}
		}
		//真正去调用skill_effect的处理入口 状态的效果都是 EFFECT_MANY: 持续生效
		do_skill_effect( aim_member, effect_head->effect_id, effect_value, effect_do_type, EFFECT_MANY );
		effect_head = effect_head->next;
	}
}

/**
 * 直接技能效果
 * @param	attack_member	状态来源者
 * @param	aim_member		状态承受者
 * @param	effect_head		效果头指针
 */
void direct_effect( fight_unit_t *attack_member, fight_unit_t *aim_member, sk_effect_t *effect_head )
{
	while ( NULL != effect_head )
	{
		int effect_value;
		//填是填写的
		if ( 0 == effect_head->effect_value_type )
		{
			effect_value = effect_head->effect_value;
		}
		//公式计算
		else
		{
			effect_value = skill_formula( effect_head->effect_value, attack_member );
		}
		//真正去调用skill_effect的处理入口
		do_skill_effect( aim_member, effect_head->effect_id, effect_value, EFFECT_ADD, EFFECT_ONCE );
		effect_head = effect_head->next;
	}
}

/**
 * 直接被动效果
 * @param	aim_member		效果承受者
 * @param	effect_head		效果头指针
 * @param	skill_id		当前技能ID
 */
void direct_extend_effect( fight_unit_t *aim_member, ext_effect_t *effect_head, int skill_id )
{
	//承受者死了
	if ( is_dead( aim_member ) )
	{
		return;
	}
	while ( NULL != effect_head )
	{
		//判断这个效果是不是
		if ( is_effect_take( effect_head ) )
		{
			//真正去调用skill_effect的处理入口
			do_skill_effect( aim_member, effect_head->effect_id, effect_head->effect_value, EFFECT_ADD, EFFECT_ONCE );
		}
		effect_head = effect_head->next;
	}
}

/**
 * 执行效果
 * @param	aim_member		效果承受者
 * @param	effect_id		效果ID
 * @param	effect_value	效果值
 * @param	effect_do_type	效果生效或者失效标志 EFFECT_ADD: 效果生效  EFFECT_CLEAN:效果失效
 * @param	effect_type		效果持续类型 EFFECT_ONCE: 单次 EFFECT_MANY: 持续效果
 */
void do_skill_effect( fight_unit_t *aim_member, int effect_id, int effect_value, int effect_do_type, int effect_type )
{
	#ifdef FIRST_DEBUG
	if ( EFFECT_ONCE == effect_type )
	{
		printf( "\n------" RED_FONT( "〖当下效果〗" ) "------" );
	}
	else
	{
		printf( "\n++++++" RED_FONT( "〖持续效果〗" ) "++++++" );
	}
	if ( EFFECT_ADD )
	{
		printf( GREEN_FONT( " 生效" ) );
	}
	else
	{
		printf( GREEN_FONT( " 失效" ) );
	}

	printf( " ID:" CYAN_FONT( "%d" ) " 名称: ", effect_id );
	debug_effect_name( effect_id );
	printf( " 值: " );
	debug_change_value( effect_value, "\n" );
	#endif
	skill_effect_arr[ effect_id ]( aim_member, effect_value, effect_do_type, effect_type );
}

/**
 * 查看是否拥有相同的状态
 * @param	aim_member		状态承受者
 * @param	buff_info		状态信息
 */
fight_buff_t *chk_have_buff( fight_unit_t *aim_member, buff_t *buff_info )
{
	if ( NULL == aim_member->buff_list[ buff_info->expire_type ] )
	{
		return NULL;
	}
	fight_buff_t *tmp_buff = aim_member->buff_list[ buff_info->expire_type ];
	while ( NULL != tmp_buff )
	{
		if ( tmp_buff->buff_info->buff_id == buff_info->buff_id )
		{
			return tmp_buff;
		}
		tmp_buff = tmp_buff->next;
	}
	return NULL;
}

/**
 * 选定技能目标
 * @param	aim				目标类型
 * @param	direct_aim		直接目标
 * @param	aim_list		作用范围目标列表
 * @param	aim_member		目标成员列表
 * @return 目标个数
 */
int find_indirect_aim( int aim_type, int direct_aim, int aim_list[], fight_unit_t *aim_member[] )
{
	int buf_pos = 0;
	aim_list[ buf_pos++ ] = direct_aim;
	switch ( aim_type )
	{
		case 1:						//扫荡攻击
		{
			int tmp_pos = ( direct_aim + 3 ) % 9;
			check_cell_member_alive( tmp_pos );
			tmp_pos = ( direct_aim + 6 ) % 9;
			check_cell_member_alive( tmp_pos );
			break;
		}
		case 2:						//突进攻击
		{
			int tmp_mod = direct_aim % 3;
			if ( 0 == tmp_mod )
			{
				check_cell_member_alive( direct_aim + 1 );
				check_cell_member_alive( direct_aim + 2 );
			}
			else if ( 1 == tmp_mod )
			{
				check_cell_member_alive( direct_aim + 1 );
			}
			break;
		}
		case 3:						//半突进攻击
		{
			int tmp_mod = direct_aim % 3;
			if ( 2 != tmp_mod )
			{
				check_cell_member_alive( direct_aim + 1 );
			}
			break;
		}
		case 4:						//传递进攻
		{
			if ( 2 != direct_aim % 3 )
			{
				check_cell_member_alive( direct_aim + 1 );
			}
			if ( 0 != direct_aim % 3 )
			{
				check_cell_member_alive( direct_aim - 1 );
			}
			if ( direct_aim > 2 )
			{
				check_cell_member_alive( direct_aim - 3 );
			}
			if ( direct_aim < 6 )
			{
				check_cell_member_alive( direct_aim + 3 );
			}
			break;
		}
		case 5:						//全攻击
		{
			int i;
			for ( i = 0; i < SIDE_MEMBER; ++i )
			{
				if ( i == direct_aim )
				{
					continue;
				}
				check_cell_member_alive( i );
			}
			break;
		}
	}
	return buf_pos;
}

/**
 * 找到一次出手的直接攻击对象
 * @param	my_cell			攻击者
 * @param	aim_member		目标成员列表
 * @param	find_level		递归防止死循环变量
 */
int find_direct_aim( int my_cell, fight_unit_t *aim_member[] )
{
	int line = floor( my_cell / 3 );
	int beg_pos;			//循环开始位置
	int find_direct = 1;	//当在一行没有找到时，向上还是向下找的控制符
	if ( 0 == line )
	{
		find_direct = -1;
	}
	int for_time, i;
	for ( for_time = 0; for_time <= 3; ++for_time )
	{
		beg_pos = line * 3;
		for ( i = beg_pos; i < beg_pos + 3; ++i )
		{
			if ( NULL == aim_member[ i ] || is_dead( aim_member[ i ] ) )
			{
				continue;
			}
			return i;
		}
		line -= find_direct;
		if ( line < 0 )
		{
			line = 2;
		}
	}
	return -1;
}

/**
 * 判断概率事件有没有发生
 * @param	prob			发生概率
 */
static int is_rand( int prob )
{
	if ( prob <= 0 )
	{
		return 0;
	}
	else if ( prob >= 100 )
	{
		return 1;
	}
	return prob <= first_rand( 1, 100 );
}


/**
 * php端要掉用到的公式统一入口
 * @param	formula_id		公式ID
 * @param	attack			战斗成员
 */
int skill_formula( int formula_id, fight_unit_t *attack )
{
	if ( formula_id < 0 || formula_id > SKILL_FORMULA_NUM )
	{
		#ifdef FIRST_DEBUG
		printf( RED_FONT( "！！！！！！技能公式: %d 不存在！！！！！！\n" ), formula_id );
		#endif
		return 0;
	}
	return skill_formula_arr[ formula_id ]( attack );
}

/**
 * 改变斗气值
 * @param	member			战斗成员
 * @param	value			改变值
 */
static void change_vigour_value( fight_unit_t *member, int value )
{
	member->vigour_now += value;
	if ( member->vigour_now > member->vigour_max )
	{
		member->vigour_now = member->vigour_max;
	}
}

/**
 * 改变怒气值
 * @param	member			战斗成员
 * @param	value			改变值
 */
void change_anger_value( fight_unit_t *member, int value )
{
	int new_anger = member->anger_now + value;
	//不能是负数
	if ( new_anger < 0 )
	{
		new_anger = 0;
	}

	//怒气真实改变
	if ( new_anger != member->anger_now )
	{
		//气血值有改变
		member->anger_now = new_anger;
		//存入结果
		result_pack_anger_t anger_pack;
		anger_pack.pack_id = RESULT_ANGER_ADD;
		anger_pack.index_id = member->index;
		anger_pack.new_anger = new_anger;
		push_fight_result( member->combat_info, &anger_pack, sizeof( result_pack_anger_t ) );
	}
}

/**
 * 气血值改变
 * @param	member			战斗成员
 * @param	damage_type		伤害类型
 * @param	value			伤害类型
 */
int change_life_value( fight_unit_t *member, int damage_type, int value )
{
	if ( is_dead( member ) )
	{
		#ifdef FIRST_DEBUG
		printf( "\n成员:" CYAN_FONT( "%d_%d" ) " 已经死亡！", member->side, member->cell_id );
		#endif
		return;
	}
	#ifdef FIRST_DEBUG
	printf( "\n成员:" CYAN_FONT( "%d_%d" ) " 气血改变:", member->side, member->cell_id );
	debug_change_value( value * -1, "" );
	printf( " 改变原因: " );
	#endif
	switch ( damage_type )
	{
		case DAMAGE_ATTACK:			//攻击造成伤害
		{
			#ifdef FIRST_DEBUG
			printf( CYAN_FONT( "「攻击伤害」" ) );
			#endif
			if ( 0 != member->append_harm )
			{
				value += member->append_harm;
				if ( value <= 0 )
				{
					value = 1;
				}
				#ifdef FIRST_DEBUG
				printf( " 伤害加深" RED_FONT( "%d" ), member->append_harm );
				#endif
			}
			break;
		}
		case DAMAGE_RETURN:			//伤害反弹
		{
			#ifdef FIRST_DEBUG
			printf( CYAN_FONT( "「伤害反弹」" ) );
			#endif
			if ( 0 != member->append_harm )
			{
				value += member->append_harm;
				if ( value <= 0 )
				{
					value = 1;
				}
				#ifdef FIRST_DEBUG
				printf( " 伤害加深" RED_FONT( "%d" ), member->append_harm );
				#endif
			}
			break;
		}
		#ifdef FIRST_DEBUG
		case DAMAGE_SUCK:			//吸血
			printf( CYAN_FONT( "「吸血」" ) );
		break;
		case DAMAGE_SKILL:			//技能直接改变
			printf( CYAN_FONT( "「技能效果」" ) );
		break;
		#endif
	}
	member->life_now += value;
	//如果是气血增加 不能超过life_max
	if ( value < 0 && member->life_now > member->life_max )
	{
		member->life_now = member->life_max;
	}
	combat_info_t *combat_info = member->combat_info;
	int abs_value = abs( value );
	//小气血数据包
	if ( abs_value <= 127 )
	{
		push_result_life_change( result_pack_life_s_t, RESULT_DAMAGE_S );
	}
	//中气血数据包
	else if ( abs_value <= 32767 )
	{
		push_result_life_change( result_pack_life_m_t, RESULT_DAMAGE_M );
	}
	//大气血数据包
	else
	{
		push_result_life_change( result_pack_life_l_t, RESULT_DAMAGE_L );
	}

	#ifdef FIRST_DEBUG
	printf( " 实际改变: " );
	debug_change_value( value * -1, "" );
	printf( " 剩余气血: " YELLOW_FONT( "%d\n" ), member->life_now );
	#endif
	int real_change_value = value;
	//死亡
	if ( member->life_now <= 0 )
	{
		#ifdef FIRST_DEBUG
		printf( CYAN_FONT( "【成员死亡】\n" ) );
		#endif
		real_change_value -= member->life_now;

		//输出结果
		result_pack_die_t pack_die;
		pack_die.pack_id = RESULT_MEMBER_DIE;
		pack_die.index_id = member->index;
		push_fight_result( combat_info, &pack_die, sizeof( result_pack_die_t ) );

		combat_info->side_num[ member->side ]--;
		//该立场面的人数为0
		if ( 0 == combat_info->side_num[ member->side ] )
		{
			combat_info->is_over = 1;
		}
	}
	else if ( member->life_now > member->life_max )
	{
		real_change_value += member->life_now - member->life_max;
		member->life_now = member->life_max;
	}
	return real_change_value;
}

/**
 * 普通攻击伤害值
 * @param	attack_member	攻击者
 * @param	aim_member		受伤者
 */
void normal_attack( fight_unit_t *attack_member, fight_unit_t *aim_member )
{
	//本次攻击产生的真实伤害
	int realy_damage;

	//随机攻击力
	int rand_damage = first_rand( attack_member->attack_min, attack_member->attack_max );
	//附加攻击力
	int base_damage = rand_damage + attack_member->buff_attack + attack_member->once_effect.attack;
	//攻击力加成（当下攻击力加成 和 状态产生的攻击力加成 ）
	int tmp_damage_add = attack_member->buff_attack_rate + attack_member->once_effect.attack_rate;
	if ( 0 != tmp_damage_add )
	{
		base_damage = ceil( base_damage * ( 1.00 + (float)tmp_damage_add / 100 ) );
	}
	//暴击判断
	if ( is_rand( get_ds_rate( attack_member, aim_member ) ) )
	{
		//如果暴击后会有附加状态
		if ( NULL != attack_member->ds_ext_buff )
		{
			//direct_extend_effect( aim_member, attack_member->ds_ext_buff, 0 );
		}
		//暴击伤害（要加上 暴击伤害加成、当下暴击伤害加成）
		base_damage = get_ds_damage( base_damage, attack_member->ds_damage + attack_member->once_effect.ds_damage, aim_member->tenacity );
	}
	//调用被攻击方承受伤害方法，返回值realy_damage 为真实伤害到的气血
	realy_damage = change_life_value( aim_member, DAMAGE_ATTACK, base_damage );

	//如果我还活着（有可能在之前已经被反弹死了），我要吸血！！
	if ( is_live( attack_member ) && ( attack_member->suck > 0 || attack_member->once_effect.suck > 0 ) )
	{
		int tmp_suck = attack_member->suck;
		if ( attack_member->once_effect.suck > 0 )
		{
			tmp_suck += attack_member->once_effect.suck;
		}
		//吸血
		int suck_life = ceil( (float)( realy_damage * tmp_suck ) / 100 );
		change_life_value( attack_member, DAMAGE_SUCK, suck_life );
	}

	//如果双方都还活着
	if ( is_live( attack_member ) && is_live( aim_member ) )
	{
		//如果被攻击方有反弹，计算伤害反弹
		if ( aim_member->damage_return > 0 )
		{
			//伤害反弹
			int return_life = ceil( (float)(realy_damage * aim_member->damage_return ) / 100 );
			change_life_value( attack_member, DAMAGE_RETURN, return_life );
		}
		//如果被攻击方有被攻击是的效果
		if ( NULL != aim_member->damage_ext_eff )
		{
			direct_extend_effect( aim_member, aim_member->damage_ext_eff, 0 );
		}
		//碾压率
		if ( is_rand( attack_member->press_ration + attack_member->once_effect.press ) )
		{
			//输出结果 todo

			//判断是否碾压无效
			if ( 0 == aim_member->avoid_press )
			{
				int damage = attack_press_damage( aim_member->life_now );
				change_life_value( aim_member, DAMAGE_PRESS, damage );
			}
		}
	}
}

/**
 * 加入成员
 * @param	member			加入者
 * @param	combat_info		战斗信息
 */
void join_member( fight_unit_t *member, combat_info_t *combat_info )
{
	member->index = combat_info->unit_index++;
	combat_info->side_num[ member->side ]++;
	//战斗继续
	if ( combat_info->is_over )
	{
		combat_info->is_over = 0;
	}
	//确定出手时间
	member->next_round = combat_info->second + member->base_speed;
	#ifdef FIRST_DEBUG
	printf( "第 " GREEN_FONT( "%d" ) " 秒", combat_info->second );
	debug_attack_side( member->side, "" );
	printf( "加入成员 格子:%d\n", member->cell_id );
	#endif

	//加到不同立场
	if ( ATTACK_SIDE == member->side )
	{
		combat_info->atk_member[ member->cell_id ] = member;
	}
	else
	{
		combat_info->def_member[ member->cell_id ] = member;
	}
	//存入战斗结果包
	result_pack_join_t join_pack;
	join_pack.pack_id = RESULT_JOIN_MEMBER;
	join_pack.index_id = member->index;
	join_pack.side = member->side;
	join_pack.cell_id = member->cell_id;
	join_pack.life_max = member->life_max;
	join_pack.swf_id = member->swf_id;
	memcpy( &join_pack.name, member->name, MEMBER_NAME_LEN );
	push_fight_result( combat_info, &join_pack, sizeof( result_pack_join_t ) );

	//初始化时的额外效果
	if ( NULL != member->init_effect )
	{
		#ifdef FIRST_DEBUG
		printf( GREEN_FONT( "\t〖效果〗" )"进入战斗的效果 " );
		#endif
		direct_extend_effect( member, member->init_effect, 0 );
		//用完就清除掉
		destroy_ext_effect_all( member->init_effect, combat_info );
	}
}

/**
 * 战斗中的每秒失血
 * @param	member_list		战斗的一方成员
 */
void fight_dot_life( fight_unit_t *member_list[] )
{
	int i;
	for ( i = 0; i < SIDE_MEMBER; ++i )
	{
		if ( NULL != member_list[ i ] && member_list[ i ]->dot_life != 0 )
		{
		//	change_life_value( member_list[ i ], DAMAGE_DOT, member_list[ i ]->dot_life, combat_info );
		}
	}
}

/**
 * 战斗中的状态过期
 * @param	member_list		战斗的一方成员
 */
void fight_buff_expire( fight_unit_t *member_list[] )
{
	int i;
	for ( i = 0; i < SIDE_MEMBER; ++i )
	{
		if ( NULL == member_list[ i ] )
		{
			continue;
		}
		fight_unit_t *member = member_list[ i ];
		//已经死亡
		if ( is_dead( member ) )
		{
			int side = member->side;
			int team_cell_id = member->team_cell_id;
			combat_info_t *combat_info = member->combat_info;
			clean_member_data( member );
			//死亡时检测有没有后续补位
			member_dead_callback( combat_info, side, team_cell_id );
		}
		else if ( NULL != member->buff_list )
		{
			//移除过期状态
			remove_buff( member, NULL, REMOVE_BUFF_EXPIRE );
		}
	}
}

/**
 * 清除一个已经死亡人的信息
 * @param	member			成员数据
 */
void clean_member_data( fight_unit_t *member )
{
	combat_info_t *combat_info = member->combat_info;
	//移除全部状态
	if ( NULL != member->buff_list )
	{
		clean_role_buff_list( member );
	}
	//暴击时额外状态
	if ( NULL != member->ds_ext_buff )
	{
		destroy_ext_buff_all( member->ds_ext_buff, combat_info );
	}
	//暴击时额外效果
	if ( NULL != member->ds_ext_effect )
	{
		destroy_ext_effect_all( member->ds_ext_effect, combat_info );
	}
	//初始化时的额外效果
	if ( NULL != member->init_effect )
	{
		destroy_ext_effect_all( member->init_effect, combat_info );
	}
	//攻击时己方直接目标效果
	if ( NULL != member->attack_self_eff[ AIM_DIRECT ] )
	{
		destroy_ext_effect_all( member->attack_self_eff[ AIM_DIRECT ], combat_info );
	}
	//攻击时己方间接目标效果
	if ( NULL != member->attack_self_eff[ AIM_INDIRECT ] )
	{
		destroy_ext_effect_all( member->attack_self_eff[ AIM_INDIRECT ], combat_info );
	}
	//攻击时己方间接目标(不含直接目标)效果
	if ( NULL != member->attack_self_eff[ AIM_EXCEPT_DIRECT ] )
	{
		destroy_ext_effect_all( member->attack_self_eff[ AIM_EXCEPT_DIRECT ], combat_info );
	}
	//攻击时己方直接目标状态
	if ( NULL != member->attack_self_buff[ AIM_DIRECT ] )
	{
		destroy_ext_buff_all( member->attack_self_buff[ AIM_DIRECT ], combat_info );
	}
	//攻击时己方间接目标状态
	if ( NULL != member->attack_self_buff[ AIM_INDIRECT ] )
	{
		destroy_ext_buff_all( member->attack_self_buff[ AIM_INDIRECT ], combat_info );
	}
	//攻击时己方间接目标(不含直接目标)状态
	if ( NULL != member->attack_self_buff[ AIM_EXCEPT_DIRECT ], combat_info )
	{
		destroy_ext_buff_all( member->attack_self_buff[ AIM_EXCEPT_DIRECT ], combat_info );
	}
	//攻击时对方直接目标效果
	if ( NULL != member->attack_object_eff[ AIM_DIRECT ] )
	{
		destroy_ext_effect_all( member->attack_object_eff[ AIM_DIRECT ], combat_info );
	}
	//攻击时对方间接目标效果
	if ( NULL != member->attack_object_eff[ AIM_INDIRECT ] )
	{
		destroy_ext_effect_all( member->attack_object_eff[ AIM_INDIRECT ], combat_info );
	}
	//攻击时对方间接目标(不含直接目标)效果
	if ( NULL != member->attack_object_eff[ AIM_EXCEPT_DIRECT ] )
	{
		destroy_ext_effect_all( member->attack_object_eff[ AIM_EXCEPT_DIRECT ], combat_info );
	}
	//攻击时对方直接目标状态
	if ( NULL != member->attack_object_buff[ AIM_DIRECT ] )
	{
		destroy_ext_buff_all( member->attack_object_buff[ AIM_DIRECT ], combat_info );
	}
	//攻击时对方间接目标状态
	if ( NULL != member->attack_object_buff[ AIM_INDIRECT ] )
	{
		destroy_ext_buff_all( member->attack_object_buff[ AIM_INDIRECT ], combat_info );
	}
	//攻击时对方间接目标(不含直接目标)状态
	if ( NULL != member->attack_object_buff[ AIM_EXCEPT_DIRECT ] )
	{
		destroy_ext_buff_all( member->attack_object_buff[ AIM_EXCEPT_DIRECT ], combat_info );
	}
	//使用的怒气技能清除
	if ( NULL != member->anger_skill )
	{
		destroy_use_skill( member->anger_skill, combat_info );
	}
	//清除格子
	if ( ATTACK_SIDE == member->side )
	{
		member->combat_info->atk_member[ member->cell_id ] = NULL;
	}
	else
	{
		member->combat_info->def_member[ member->cell_id ] = NULL;
	}
	//回收fight_unit_t
	destroy_fight_unit( member, combat_info );
}

/**
 * 战斗结束
 * @param	combat_info		战斗信息
 */
void fight_final_end( combat_info_t *combat_info )
{
	//胜负判断
	combat_info->result = 0 == combat_info->side_num[ 0 ] ? 1 : -1;
	//输出结果
	result_pack_end_t pack_end;
	pack_end.pack_id = RESULT_FIGHT_END;
	pack_end.result = combat_info->result;
	push_fight_result( combat_info, &pack_end, sizeof( result_pack_end_t ) );
	//清理数据
	int i = 0;
	for ( i = 0; i < SIDE_MEMBER; ++i )
	{
		if ( NULL != combat_info->atk_member[ i ] )
		{
			clean_member_data( combat_info->atk_member[ i ] );
		}
		if ( NULL != combat_info->def_member[ i ] )
		{
			clean_member_data( combat_info->def_member[ i ] );
		}
	}
}

/**
 * 初始化游戏属性
 */
void init_first_fight()
{
	//申请技能指针数组空间
	GLOBAL_VARS.skill_info_pool = ( skill_t** )malloc( sizeof( skill_t ) * DEF_SKILL_NUM );
	//申请状态指针数组空间
	GLOBAL_VARS.buff_info_pool = ( buff_t** )malloc( sizeof( buff_t ) * DEF_SKILL_BUFF_NUM );
	#ifdef FIRST_DEBUG
	DEBUG_BUFF_NAME = (char **)malloc( sizeof( char*  ) * DEF_SKILL_BUFF_NUM );
	DEBUG_SKILL_NAME = (skill_name_t **)malloc( sizeof( skill_name_t* ) * DEF_SKILL_NUM );
	#endif
	//初始化技能效果
	init_skill_effect();
}

/**
 * 初始化一个技能的属性
 * @param	skill_t			技能信息
 */
void init_skill_info( skill_t *sk_info )
{
	//该技能信息已经在技能池里
	if ( NULL != find_skill_info( sk_info->skill_id ) )
	{
		return;
	}
	int i = sk_info->skill_id % DEF_SKILL_NUM;
	if ( NULL == GLOBAL_VARS.skill_info_pool[ i ] )
	{
		GLOBAL_VARS.skill_info_pool[ i ] = sk_info;
	}
	else
	{
		sk_info->next = GLOBAL_VARS.skill_info_pool[ i ];
		GLOBAL_VARS.skill_info_pool[ i ] = sk_info;
	}
}

/**
 * 查找技能信息
 * @param	skill_id		技能ID
 */
static skill_t *find_skill_info( int skill_id )
{
	int i = skill_id % DEF_SKILL_NUM;
	skill_t *tmp_sk = GLOBAL_VARS.skill_info_pool[ i ];
	while ( NULL != tmp_sk )
	{
		if ( tmp_sk->skill_id == skill_id )
		{
			break;
		}
		tmp_sk = tmp_sk->next;
	}
	return tmp_sk;
}

/**
 * 查找一个信息的数据
 * @param	buff_id			状态ID
 */
static buff_t *find_buff_info( int buff_id )
{
	//id不能超过池最大值
	if ( buff_id >= DEF_SKILL_BUFF_NUM )
	{
		return NULL;
	}
	return GLOBAL_VARS.buff_info_pool[ buff_id ];
}


/**
 * 读取配置文件中的技能效果
 * @param	config_file		文件句柄
 * @param	read_num		读取个数
 * @param	is_error		是否出错的标志
 */
sk_effect_t *read_fight_config_effect( FILE *config_file, int read_num, int *is_error )
{
	#ifdef FIRST_DEBUG
	printf( "读取效果: %d 个\n", read_num );
	#endif
	sk_effect_t *result_head = NULL;
	int i;
	for ( i = 0; i < read_num; ++i )
	{
		tool_effect_t tmp_read_effect;
		int read_ret = fread( &tmp_read_effect, sizeof( tmp_read_effect ), 1, config_file );
		if ( 1 != read_ret )
		{
			*is_error = 1;
			return NULL;
		}
		sk_effect_t *new_effect = ( sk_effect_t* )malloc( sizeof( sk_effect_t ) );
		new_effect->effect_id = tmp_read_effect.effect_id;
		new_effect->effect_value_type = tmp_read_effect.effect_value_type;
		new_effect->effect_value = tmp_read_effect.eff_value;
		new_effect->next = result_head;
		result_head = new_effect;
	}
	return result_head;
}

/**
 * 读取配置文件中的技能状态
 * @param	config_file		文件句柄
 * @param	read_num		读取个数
 * @param	is_error		是否出错的标志
 */
sk_buff_t *read_fight_config_buff( FILE *config_file, int read_num, int *is_error )
{
	#ifdef FIRST_DEBUG
	printf( "读取状态: %d 个\n", read_num );
	#endif
	sk_buff_t *result_head = NULL;
	int i;
	for ( i = 0; i < read_num; ++i )
	{
		tool_sk_buff_t tmp_read_sk_buff;
		int read_ret = fread( &tmp_read_sk_buff, sizeof( tmp_read_sk_buff ), 1, config_file );
		if ( 1 != read_ret )
		{
			*is_error = 1;
			return NULL;
		}
		sk_buff_t *new_sk_buff = ( sk_buff_t* )malloc( sizeof( sk_buff_t ) );
		new_sk_buff->last_time_type = tmp_read_sk_buff.last_time_type;
		new_sk_buff->last_time = tmp_read_sk_buff.last_time;
		new_sk_buff->buff_info = find_buff_info( tmp_read_sk_buff.buff_id );
		new_sk_buff->next = result_head;
		result_head = new_sk_buff;
	}
	return result_head;
}

/**
 * 读取战斗的配置信息
 * @param	file_path		配置文件路径
 */
int read_fight_config_dat_file( char *file_path )
{
	FILE *config_file;
	if ( NULL == ( config_file = fopen( file_path, "r" ) ) )
	{
		return -1;
	}
	uint16_t buff_num = -1;
	uint16_t skill_num = -1;
	int read_ret = fread( &buff_num, sizeof( buff_num ), 1, config_file );
	check_read_ret( read_ret );
	#ifdef FIRST_DEBUG
	printf( "技能状态个数: %d \n", buff_num );
	#endif
	while ( buff_num > 0 )
	{
		--buff_num;
		tool_buff_t tmp_read_buff;
		read_ret = fread( &tmp_read_buff, sizeof( tmp_read_buff ), 1, config_file );
		check_read_ret( read_ret );
		buff_t *new_buff = ( buff_t* )malloc( sizeof( buff_t ) );
		new_buff->buff_id = tmp_read_buff.buff_id;
		new_buff->buff_type = tmp_read_buff.buff_type;
		new_buff->can_sup = tmp_read_buff.can_sup;
		int is_error = 0;
		new_buff->buff_effect = read_fight_config_effect( config_file, tmp_read_buff.effect_num, &is_error );
		if ( is_error )
		{
			return -3;
		}
		#ifdef FIRST_DEBUG
		printf( "新状态 ID:%d\n", tmp_read_buff.buff_id );
		#endif
		GLOBAL_VARS.buff_info_pool[ tmp_read_buff.buff_id ] = new_buff;
	}
	read_ret = fread( &skill_num, sizeof( skill_num ), 1, config_file );
	check_read_ret( read_ret );
	#ifdef FIRST_DEBUG
	printf( "技能个数: %d \n", skill_num );
	#endif
	while ( skill_num > 0 )
	{
		--skill_num;
		tool_skill_t tmp_read_skill;
		read_ret = fread( &tmp_read_skill, sizeof( tmp_read_skill ), 1, config_file );
		check_read_ret( read_ret );
		skill_t *new_skill = ( skill_t* )malloc( sizeof( skill_t ) );
		new_skill->skill_id = tmp_read_skill.skill_id;
		new_skill->hit_ration[ 0 ] = tmp_read_skill.self_hr_type;
		new_skill->hit_ration[ 1 ] = tmp_read_skill.self_hr;
		new_skill->hit_ration[ 0 ] = tmp_read_skill.object_hr_type;
		new_skill->hit_ration[ 1 ] = tmp_read_skill.object_hr;
		new_skill->attack_num = tmp_read_skill.attack_num;
		new_skill->object_aim = tmp_read_skill.object_aim;
		new_skill->self_aim = tmp_read_skill.self_aim;
		int is_error = 0;
		new_skill->self_effect = read_fight_config_effect( config_file, tmp_read_skill.self_effect_num, &is_error );
		if ( is_error )
		{
			return -3;
		}
		new_skill->object_effect = read_fight_config_effect( config_file, tmp_read_skill.object_effect_num, &is_error );
		if ( is_error )
		{
			return -3;
		}
		new_skill->self_buff = read_fight_config_buff( config_file, tmp_read_skill.self_buff_num, &is_error );
		if ( is_error )
		{
			return -4;
		}
		new_skill->object_buff = read_fight_config_buff( config_file, tmp_read_skill.object_buff_num, &is_error );
		if ( is_error )
		{
			return -4;
		}
		init_skill_info( new_skill );
	}
	fclose( config_file );
	return 0;
}

/**
 * 解析技能使用字符串
 * @param	member			战斗成员
 * @param	combat_info		战斗信息
 * @param	use_str			使用技能字符串
 * @param	str_len			字符串长度
 */
void parse_skill_use_str( fight_unit_t *member, char *use_str, int str_len )
{
	char tmp_use_skill_char[ PARSE_STR_CHAR_LEN ];
	if ( str_len >= PARSE_STR_CHAR_LEN )
	{
		return;
	}
	memcpy( tmp_use_skill_char, use_str, str_len );
	tmp_use_skill_char[ str_len ] = '\0';
	int use_arr[ 4 ];
	/*if ( 3 == fight_split_str( tmp_use_skill_char, str_len, ':', use_arr, 3 ) )
	{
		skill_t *skill_info = find_skill_info( use_arr[ 0 ] );
		if ( NULL == skill_info )
		{
			return;
		}
		use_skill_t *use_sk = create_use_skill( combat_info );
		use_sk->skill_id = use_arr[ 0 ];
		use_sk->skill_info = skill_info;
		use_sk->skill_level = use_arr[ 1 ];
		use_sk->use_anger = use_arr[ 2 ];
		use_sk->add_hr = use_arr[ 3 ];
		member->anger_skill = use_sk;
	}*/
}

/**
 * 解析被动技能效果字符串
 * @param	member			战斗成员
 * @param	use_str			使用技能字符串
 * @param	str_len			字符串长度
 */
void parse_ext_effect_str( fight_unit_t *member, combat_info_t *combat_info, char *use_str, int str_len )
{
	char tmp_effect_char[ PARSE_STR_CHAR_LEN ];
	//如果字符串太长，截取掉一些
	if ( str_len >= PARSE_STR_CHAR_LEN )
	{
		str_len = PARSE_STR_CHAR_LEN - 1;
	}
	memcpy( tmp_effect_char, use_str, str_len );
	tmp_effect_char[ str_len ] = '\0';
	char *each_str = strtok( tmp_effect_char, "," );
	ext_effect_t *new_ext_effect;
	while ( NULL != each_str )
	{
		int eff_arr[ 4 ];
		if ( 3 == fight_split_str( each_str, strlen( each_str ), ':', eff_arr, 4 ) )
		{
			new_ext_effect = create_ext_effect( combat_info );
			new_ext_effect->effect_id = eff_arr[ 1 ];
			new_ext_effect->effect_value = eff_arr[ 2 ];
			new_ext_effect->need_skill = eff_arr[ 3 ];
			//不同类型不同的处理方式
			switch ( eff_arr[ 0 ] )
			{
				case EFFECT_SPECIAL_ID:				//人物初始化效果
					new_ext_effect->next = member->init_effect;
					member->init_effect = new_ext_effect;
				break;
				case EFFECT_DS_ID:					//暴击被动效果
					new_ext_effect->next = member->ds_ext_effect;
					member->ds_ext_effect = new_ext_effect;
				break;
				case EFFECT_DAMAGE_ID:				//被攻击时的效果
					new_ext_effect->next = member->damage_ext_eff;
					member->damage_ext_eff = new_ext_effect;
				break;
				case EFFECT_ATTACK_5:				//出手时的效果 己方直接目标
					new_ext_effect->next = member->attack_self_eff[ AIM_DIRECT ];
					member->attack_self_eff[ AIM_DIRECT ] = new_ext_effect;
				break;
				case EFFECT_ATTACK_7:				//出手时的效果 己方间接目标
					new_ext_effect->next = member->attack_self_eff[ AIM_INDIRECT ];
					member->attack_self_eff[ AIM_INDIRECT ] = new_ext_effect;
				break;
				case EFFECT_ATTACK_9:				//出手时的效果 己方间接目标(不含直接目标)
					new_ext_effect->next = member->attack_self_eff[ AIM_EXCEPT_DIRECT ];
					member->attack_self_eff[ AIM_EXCEPT_DIRECT ] = new_ext_effect;
				break;
				case EFFECT_ATTACK_11:				//出手时的效果 对方直接目标
					new_ext_effect->next = member->attack_object_eff[ AIM_DIRECT ];
					member->attack_object_eff[ AIM_DIRECT ] = new_ext_effect;
				break;
				case EFFECT_ATTACK_13:				//出手时的效果 对方间接目标
					new_ext_effect->next = member->attack_object_eff[ AIM_INDIRECT ];
					member->attack_object_eff[ AIM_INDIRECT ] = new_ext_effect;
				break;
				case EFFECT_ATTACK_15:				//出手时的效果 对方间接目标(不含直接目标)
					new_ext_effect->next = member->attack_object_eff[ AIM_EXCEPT_DIRECT ];
					member->attack_object_eff[ AIM_EXCEPT_DIRECT ] = new_ext_effect;
				break;
			}
		}
		each_str = strtok( NULL, "," );
	}
}

/**
 * 解析被动技能状态字符串
 * @param	member			战斗成员
 * @param	combat_info		战斗信息
 * @param	use_str			使用技能字符串
 * @param	str_len			字符串长度
 */
void parse_ext_buff_str( fight_unit_t *member, char *use_str, int str_len )
{
	char tmp_ext_buff_char[ PARSE_STR_CHAR_LEN ];
	//如果字符串太长，截取掉一些
	if ( str_len >= PARSE_STR_CHAR_LEN )
	{
		str_len = PARSE_STR_CHAR_LEN - 1;
	}
	memcpy( tmp_ext_buff_char, use_str, str_len );
	tmp_ext_buff_char[ str_len ] = '\0';
	char *each_str = strtok( tmp_ext_buff_char, "," );
	ext_buff_t *new_ext_buff;
	while ( NULL != each_str )
	{
		int buff_arr[ 5 ];
		if ( 4 == fight_split_str( each_str, strlen( each_str ), ':', buff_arr, 5 ) )
		{
			combat_info_t *combat_info = NULL;
			buff_t *buff_info = find_buff_info( buff_arr[ 1 ] );
			if ( NULL != buff_info )
			{
				new_ext_buff = create_ext_buff( combat_info );
				new_ext_buff->buff_info = buff_info;
				new_ext_buff->level = buff_arr[ 2 ];
				new_ext_buff->last_time = buff_arr[ 3 ];
				new_ext_buff->need_skill = buff_arr[ 4 ];
				//不同类型不同的处理方式
				switch ( buff_arr[ 0 ] )
				{
					case BUFF_DS_ID:					//暴击被动状态
						new_ext_buff->next = member->ds_ext_buff;
						member->ds_ext_buff = new_ext_buff;
					break;
					case BUFF_DAMAGE_ID:				//被攻击时的状态
						new_ext_buff->next = member->damage_ext_buff;
						member->damage_ext_buff = new_ext_buff;
					break;
					case BUFF_ATTACK_6:					//出手时的状态 己方直接目标
						new_ext_buff->next = member->attack_self_buff[ AIM_DIRECT ];
						member->attack_self_buff[ AIM_DIRECT ] = new_ext_buff;
					break;
					case BUFF_ATTACK_8:					//出手时的状态 己方间接目标
						new_ext_buff->next = member->attack_self_buff[ AIM_INDIRECT ];
						member->attack_self_buff[ AIM_INDIRECT ] = new_ext_buff;
					break;
					case BUFF_ATTACK_10:				//出手时的状态 己方间接目标(不含直接目标)
						new_ext_buff->next = member->attack_self_buff[ AIM_EXCEPT_DIRECT ];
						member->attack_self_buff[ AIM_EXCEPT_DIRECT ] = new_ext_buff;
					break;
					case BUFF_ATTACK_12:				//出手时的状态 对方直接目标
						new_ext_buff->next = member->attack_object_buff[ AIM_DIRECT ];
						member->attack_object_buff[ AIM_DIRECT ] = new_ext_buff;
					break;
					case BUFF_ATTACK_14:				//出手时的状态 对方间接目标
						new_ext_buff->next = member->attack_object_buff[ AIM_INDIRECT ];
						member->attack_object_buff[ AIM_INDIRECT ] = new_ext_buff;
					break;
					case BUFF_ATTACK_16:				//出手时的状态 对方间接目标(不含直接目标)
						new_ext_buff->next = member->attack_object_buff[ AIM_EXCEPT_DIRECT ];
						member->attack_object_buff[ AIM_EXCEPT_DIRECT ] = new_ext_buff;
					break;
				}
			}

		}
		each_str = strtok( NULL, "," );
	}
}

/**
 * 切割字符串
 * @param	sour_str		字符串指针
 * @param	str_len			字符串长度
 * @param	split_char		分割字符
 * @param	arr_str			存结果的数组
 * @param	split_num		切割次数
 */
int fight_split_str( char *str, int str_len, char split_char, int arr_str[], int split_num )
{
	int i, beg_pos = 0, re_index = 0, re_split_num = 0;
	for ( i = 0; i < str_len; ++i )
	{
		//找到分割字符
		if ( str[ i ] == split_char )
		{
			str[ i ] = '\0';
			++re_split_num;
			arr_str[ re_index++ ] = atoi( str + beg_pos );
			beg_pos = i + 1;
		}
		//最后一位处理
		if ( re_split_num == split_num )
		{
			if ( i + 1 != '\0' )
			{
				arr_str[ re_index ] = atoi( str + i + 1 );
			}
			else
			{
				arr_str[ re_index ] = 0;
			}
			break;
		}
	}
	return re_split_num;
}

/**
 * 战斗一方小队变更数据包
 * @param	combat_info		战斗信息
 * @param	side			立场
 * @param	team_id			小队ID
 */
void push_change_team_result( combat_info_t *combat_info, int side, int team_id )
{
	result_pack_new_team_t pack_team;
	pack_team.pack_id = RESULT_NEW_TEAM;
	pack_team.side = side;
	pack_team.team_id = team_id;
	push_fight_result( combat_info, &pack_team, sizeof( result_pack_new_team_t ) );
}


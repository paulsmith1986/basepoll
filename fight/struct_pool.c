#include "struct_pool.h"

/**
 * 新创建一个fight-buff
 * @param	combat_info		战斗信息
 */
fight_buff_t *create_fight_buff( combat_info_t *combat_info )
{
	fight_buff_t *tmp_buff;
	if ( NULL == combat_info->global_var->fight_buff_head || 0 == combat_info->global_var->now_fight_buff )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请fight_buff_t】@@@@@@\n" );
		#endif
		tmp_buff = (fight_buff_t *)malloc( sizeof( fight_buff_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用fight_buff_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_fight_buff;
		tmp_buff = combat_info->global_var->fight_buff_head;
		combat_info->global_var->fight_buff_head = tmp_buff->next;
	}
	return tmp_buff;
}

/**
 * 回收一个fight_buff
 * @param	tmp_buff		状态指针
 * @param	combat_info		战斗信息
 */
void destroy_fight_buff( fight_buff_t *tmp_buff, combat_info_t *combat_info )
{
	if ( combat_info->global_var->now_fight_buff >= combat_info->global_var->max_fight_buff )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【释放fight_buff_t】@@@@@@\n" );
		#endif
		free( tmp_buff );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【回收fight_buff_t】@@@@@@\n" );
		#endif
		++combat_info->global_var->now_fight_buff;
		tmp_buff->next = combat_info->global_var->fight_buff_head;
		combat_info->global_var->fight_buff_head = tmp_buff;
	}
}

/**
 * 新创建一个fight_unit_t
 * @param	combat_info		战斗信息
 */
fight_unit_t *create_fight_unit( combat_info_t *combat_info )
{
	fight_unit_t *tmp_unit;
	if ( NULL == combat_info->global_var->fight_unit_head || 0 == combat_info->global_var->now_fight_unit )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请fight_unit_t】@@@@@@\n", sizeof( fight_unit_t ) );
		#endif
		tmp_unit = (fight_unit_t *)malloc( sizeof( fight_unit_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用fight_unit_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_fight_unit;
		tmp_unit = combat_info->global_var->fight_unit_head;
		combat_info->global_var->fight_unit_head = tmp_unit->next;
	}
	memset( tmp_unit, 0, sizeof( fight_unit_t ) );
	return tmp_unit;
}

/**
 * 回收一个fight_unit_t
 * @param	tmp_unit		unit指针
 * @param	combat_info		战斗信息
 */
void destroy_fight_unit( fight_unit_t *tmp_unit, combat_info_t *combat_info )
{
	if ( combat_info->global_var->now_fight_unit >= combat_info->global_var->max_fight_unit )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【释放fight_unit_t %d_%d size:%d 】@@@@@@\n", tmp_unit->side, tmp_unit->cell_id, sizeof( fight_unit_t ) );
		#endif
		free( tmp_unit );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【回收fight_unit_t】@@@@@@\n" );
		#endif
		++combat_info->global_var->now_fight_unit;
		tmp_unit->next = combat_info->global_var->fight_unit_head;
		combat_info->global_var->fight_unit_head = tmp_unit;
	}
}

/**
 * 创建一个被动技能效果
 * @param	combat_info		战斗信息
 */
ext_effect_t *create_ext_effect( combat_info_t *combat_info )
{
	ext_effect_t *tmp_eff;
	if ( NULL == combat_info->global_var->ext_effect_head || 0 == combat_info->global_var->now_ext_effect )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请ext_effect_t】@@@@@@\n" );
		#endif
		tmp_eff = (ext_effect_t *)malloc( sizeof( ext_effect_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用ext_effect_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_ext_effect;
		tmp_eff = combat_info->global_var->ext_effect_head;
		combat_info->global_var->ext_effect_head = tmp_eff->next;
	}
	return tmp_eff;
}

/**
 * 回收一个ext_effect完整链
 * @param	tmp_eff			效果指针
 * @param	combat_info		战斗信息
 */
void destroy_ext_effect_all( ext_effect_t *tmp_eff, combat_info_t *combat_info )
{
	while ( NULL != tmp_eff )
	{
		ext_effect_t *next_eff = tmp_eff->next;
		if ( combat_info->global_var->now_ext_effect >= combat_info->global_var->max_ext_effect )
		{
			#ifdef FIRST_DEBUG
			printf( "\n@@@@@@【释放ext_effect_t】@@@@@@\n" );
			#endif
			free( tmp_eff );
		}
		else
		{
			#ifdef FIRST_DEBUG
			printf( "\n@@@@@@【回收ext_effect_t】@@@@@@\n" );
			#endif
			++combat_info->global_var->now_ext_effect;
			tmp_eff->next = combat_info->global_var->ext_effect_head;
			combat_info->global_var->ext_effect_head = tmp_eff;
		}
		tmp_eff = next_eff;
	}
}

/**
 * 创建一个被动技能状态
 * @param	combat_info		战斗信息
 */
ext_buff_t *create_ext_buff( combat_info_t *combat_info )
{
	ext_buff_t *tmp_buff;
	if ( NULL == combat_info->global_var->ext_buff_head || 0 == combat_info->global_var->now_ext_buff )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请ext_buff_t】@@@@@@\n" );
		#endif
		tmp_buff = (ext_buff_t *)malloc( sizeof( ext_buff_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用ext_buff_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_ext_buff;
		tmp_buff = combat_info->global_var->ext_buff_head;
		combat_info->global_var->ext_buff_head = tmp_buff->next;
	}
	return tmp_buff;
}

/**
 * 回收一个ext_buff完整链
 * @param	tmp_eff			状态指针
 * @param	combat_info		战斗信息
 */
void destroy_ext_buff_all( ext_buff_t *tmp_buff, combat_info_t *combat_info )
{
	while ( NULL != tmp_buff )
	{
		ext_buff_t *next_buff = tmp_buff->next;
		if ( combat_info->global_var->now_ext_buff >= combat_info->global_var->max_ext_buff )
		{
			#ifdef FIRST_DEBUG
			printf( "\n@@@@@@【释放ext_buff_t】@@@@@@\n" );
			#endif
			free( tmp_buff );
		}
		else
		{
			#ifdef FIRST_DEBUG
			printf( "\n@@@@@@【回收ext_buff_t】@@@@@@\n" );
			#endif
			++combat_info->global_var->now_ext_buff;
			tmp_buff->next = combat_info->global_var->ext_buff_head;
			combat_info->global_var->ext_buff_head = tmp_buff;
		}
		tmp_buff = next_buff;
	}
}

/**
 * 新创建一个use_skill_t
 * @param	combat_info		战斗信息
 */
use_skill_t *create_use_skill( combat_info_t *combat_info )
{
	use_skill_t *tmp_skill;
	if ( NULL == combat_info->global_var->use_skill_head || 0 == combat_info->global_var->now_use_skill )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请use_skill_t】@@@@@@\n" );
		#endif
		tmp_skill = (use_skill_t *)malloc( sizeof( use_skill_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用use_skill_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_use_skill;
		tmp_skill = combat_info->global_var->use_skill_head;
		combat_info->global_var->use_skill_head = tmp_skill->next;
	}
	return tmp_skill;
}

/**
 * 回收一个use_skill_t
 * @param	tmp_skill		use_skill指针
 * @param	combat_info		战斗信息
 */
void destroy_use_skill( use_skill_t *tmp_skill, combat_info_t *combat_info )
{
	if ( combat_info->global_var->now_use_skill >= combat_info->global_var->max_use_skill )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【释放use_skill_t】@@@@@@\n" );
		#endif
		free( tmp_skill );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【回收use_skill_t】@@@@@@\n" );
		#endif
		++combat_info->global_var->now_use_skill;
		tmp_skill->next = combat_info->global_var->use_skill_head;
		combat_info->global_var->use_skill_head = tmp_skill;
	}
}

/**
 * 创建流血效果
 * @param	combat_info		战斗信息
 */
life_dot_t *create_life_dot( combat_info_t *combat_info )
{
	life_dot_t *tmp_dot;
	if ( NULL == combat_info->global_var->life_dot_head || 0 == combat_info->global_var->now_lift_dot )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请 life_dot_t】@@@@@@\n" );
		#endif
		tmp_dot = (life_dot_t *)malloc( sizeof( life_dot_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用 life_dot_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_lift_dot;
		tmp_dot = combat_info->global_var->life_dot_head;
		combat_info->global_var->life_dot_head = tmp_dot->next;
	}
	return tmp_dot;
}

/**
 * 回收一个流血效果
 * @param	tmp_dit			流血效果
 * @param	combat_info		战斗信息
 */
void destroy_life_dot( life_dot_t *tmp_dot, combat_info_t *combat_info )
{
	if ( combat_info->global_var->now_lift_dot >= combat_info->global_var->max_life_dot )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【释放 life_dot_t】@@@@@@\n" );
		#endif
		free( tmp_dot );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【回收 life_dot_t】@@@@@@\n" );
		#endif
		++combat_info->global_var->now_lift_dot;
		tmp_dot->next = combat_info->global_var->life_dot_head;
		combat_info->global_var->life_dot_head = tmp_dot;
	}
}

/**
 * 创建状态时间增加
 * @param	combat_info		战斗信息
 */
buff_addtime_t *create_buff_addtime( combat_info_t *combat_info )
{
	buff_addtime_t *tmp_add;
	if ( NULL == combat_info->global_var->buff_addtime_head || 0 == combat_info->global_var->now_buff_addtime )
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【申请 buff_addtime_t】@@@@@@\n" );
		#endif
		tmp_add = (buff_addtime_t *)malloc( sizeof( buff_addtime_t ) );
	}
	else
	{
		#ifdef FIRST_DEBUG
		printf( "\n@@@@@@【重用 buff_addtime_t】@@@@@@\n" );
		#endif
		--combat_info->global_var->now_buff_addtime;
		tmp_add = combat_info->global_var->buff_addtime_head;
		combat_info->global_var->buff_addtime_head = tmp_add->next;
	}
	return tmp_add;
}

/**
 * 回收状态时间增加
 * @param	tmp_dit			流血效果
 * @param	combat_info		战斗信息
 */
void destroy_buff_addtime( buff_addtime_t *tmp_add, combat_info_t *combat_info )
{
	while ( NULL != tmp_add )
	{
		buff_addtime_t *next_add = tmp_add->next;
		if ( combat_info->global_var->now_buff_addtime >= combat_info->global_var->max_buff_addtime )
		{
			#ifdef FIRST_DEBUG
			printf( "\n@@@@@@【释放 buff_addtime_t】@@@@@@\n" );
			#endif
			free( tmp_add );
		}
		else
		{
			#ifdef FIRST_DEBUG
			printf( "\n@@@@@@【回收 buff_addtime_t】@@@@@@\n" );
			#endif
			++combat_info->global_var->now_buff_addtime;
			tmp_add->next = combat_info->global_var->buff_addtime_head;
			combat_info->global_var->buff_addtime_head = tmp_add;
		}
		tmp_add = next_add;
	}
}
#ifndef FIRST_NEW_FIGHT_DEBUG_H
#define FIRST_NEW_FIGHT_DEBUG_H

//调试模式需要用到的一些宏
#ifdef FIRST_DEBUG
	#ifndef HTML_DEBUG
		#define FIRST_RED "\033[0;32;31m"
		#define FIRST_GREEN "\033[0;32;32m"
		#define FIRST_BLUE "\033[0;32;34m"
		#define FIRST_CYAN "\033[0;32;36m"
		#define FIRST_YELLOW "\033[0;32;33m"
		#define FIRST_PINK "\033[0;32;35m"
		#define FIRST_COLOR_END "\033[m"
	#else
		#define FIRST_RED "<font color=\"red\">"
		#define FIRST_GREEN "<font color=\"green\">"
		#define FIRST_BLUE "<font color=\"blue\">"
		#define FIRST_CYAN "<font color=\"cyan\">"
		#define FIRST_YELLOW "<font color=\"yellow\">"
		#define FIRST_PINK "<font color=\"magenta\">"
		#define FIRST_COLOR_END "</font>"
	#endif
	#define RED_FONT( format ) FIRST_RED format FIRST_COLOR_END
	#define GREEN_FONT( format ) FIRST_GREEN format FIRST_COLOR_END
	#define CYAN_FONT( format ) FIRST_CYAN format FIRST_COLOR_END
	#define BLUE_FONT( format ) FIRST_BLUE format FIRST_COLOR_END
	#define YELLOW_FONT( format ) FIRST_YELLOW format FIRST_COLOR_END
	#define PINK_FONT( format ) FIRST_PINK format FIRST_COLOR_END
	#define debug_attack_side( side, end_char ) ATTACK_SIDE == side ? printf( GREEN_FONT( "【攻击方】" ) end_char ) : printf( CYAN_FONT( "【防守方】" end_char ) )
	#define debug_change_value( value, end_char ) value > 0 ? printf( GREEN_FONT( "+%d" ) end_char, value ) : printf( RED_FONT( "%d" ) end_char, value )
	#define debug_buff_type( type ) 1 == type ? printf( " 〓增益状态〓 " ) : printf( " 〓减益状态〓 " )

	typedef struct skill_name_t skill_name_t;
	struct skill_name_t{
		int		skill_id;			//技能ID
		char*	skill_name;			//技能名称
		skill_name_t *next;
	};

	typedef struct buff_name_t buff_name_t;
	struct buff_name_t{
		int		buff_id;			//buff ID
		char*	buff_name;			//状态名称
		buff_name_t *next;
	};

	/**
	 * 获取技能名称
	 */
	buff_name_t *find_buff_name( int buff_id );

	skill_name_t *DEBUG_SKILL_NAME[ DEF_SKILL_NUM ];
	buff_name_t *DEBUG_BUFF_NAME[ DEF_SKILL_BUFF_NUM ];

	/**
	 * 获取技能名称
	 */
	skill_name_t *find_skill_name( int skill_id )
	{
		int i = skill_id % DEF_SKILL_NUM;
		skill_name_t *tmp_sk = DEBUG_SKILL_NAME[ i ];
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
	 * 显示技能名称
	 */
	const char *debug_skill_name( int skill_id )
	{
		skill_name_t *tmp_sk = find_skill_name( skill_id );
		if ( NULL == tmp_sk )
		{
			const char *sk_name = "无名称";
			return sk_name;
		}
		else
		{
			return tmp_sk->skill_name;
		}
	}

	/**
	 * 设置技能名称
	 */
	void set_skill_name( int skill_id, skill_name_t *sk_name )
	{
		int i = skill_id % DEF_SKILL_NUM;
		if ( NULL == DEBUG_SKILL_NAME[ i ] )
		{
			DEBUG_SKILL_NAME[ i ] = sk_name;
		}
		else
		{
			sk_name->next = DEBUG_SKILL_NAME[ i ];
			DEBUG_SKILL_NAME[ i ] = sk_name;
		}
	}

	/**
	 * 显示状态名
	 */
	const char *debug_buff_name( int buff_id )
	{
		buff_name_t *tmp_buff = find_buff_name( buff_id );
		if ( NULL == tmp_buff )
		{
			const char *buff_name = "未知";
			return buff_name;
		}
		else
		{
			return tmp_buff->buff_name;
		}
	}

	/**
	 * 获取技能名称
	 */
	buff_name_t *find_buff_name( int buff_id )
	{
		int i = buff_id % DEF_SKILL_BUFF_NUM;
		buff_name_t *tmp_buff = DEBUG_BUFF_NAME[ i ];
		while ( NULL != tmp_buff )
		{
			if ( tmp_buff->buff_id == buff_id )
			{
				break;
			}
			tmp_buff = tmp_buff->next;
		}
		return tmp_buff;
	}

	/**
	 * 设置状态名称
	 */
	void set_buff_name( int buff_id, buff_name_t *buff_name )
	{
		int i = buff_id % DEF_SKILL_BUFF_NUM;
		if ( NULL == DEBUG_BUFF_NAME[ i ] )
		{
			DEBUG_BUFF_NAME[ i ] = buff_name;
		}
		else
		{
			buff_name->next = DEBUG_BUFF_NAME[ i ];
			DEBUG_BUFF_NAME[ i ] = buff_name;
		}
	}

	/**
	 * 读取状态名称字符串
	 * @param	buff_name_list		状态名称
	 */
	void debug_read_buff_name( proto_list_buff_name_t *buff_name_list )
	{
		int i;
		for ( i = 0; i < buff_name_list->len; ++i )
		{
			proto_buff_name_t *tmp_name = &buff_name_list->item[ i ];
			buff_t *buff_info = find_buff_info( tmp_name->buff_id );
			if ( NULL != buff_info )
			{
				buff_name_t *buff_name = ( buff_name_t* )malloc( sizeof( buff_name_t ) );
				buff_name->buff_id = tmp_name->buff_id;
				buff_name->buff_name = strdup( tmp_name->buff_name );
				set_buff_name( tmp_name->buff_id, buff_name );
			}
		}
	}

	/**
	 * 读取技能名称字符串
	 * @param	skill_name_list		技能名称
	 */
	void debug_read_skill_name( proto_list_skill_name_t *skill_name_list )
	{
		int i;
		for ( i = 0; i < skill_name_list->len; ++i )
		{
			proto_skill_name_t *tmp_name = &skill_name_list->item[ i ];
			skill_t *skill_info = find_skill_info( tmp_name->skill_id );
			if ( NULL != skill_info )
			{
				skill_name_t *sk_name = (skill_name_t*)malloc( sizeof( skill_name_t ) );
				sk_name->skill_id = tmp_name->skill_id;
				sk_name->skill_name = strdup( tmp_name->skill_name );
				set_skill_name( tmp_name->skill_id, sk_name );
			}
		}
	}


	/**
	 * 显示效果名称
	 * @param	effect_id		效果ID
	 */
	void debug_effect_name( int effect_id )
	{
		char *effect_name_list[ SKILL_EFFECT_NUM + 1 ];
		effect_name_list[ 0 ] = "空效果";
		effect_name_list[ 1 ] = "攻击";
		effect_name_list[ 2 ] = "攻击百分比";
		effect_name_list[ 3 ] = "防御";
		effect_name_list[ 4 ] = "防御百分比";
		effect_name_list[ 5 ] = "命中率";
		effect_name_list[ 6 ] = "闪避率";
		effect_name_list[ 7 ] = "暴击率";
		effect_name_list[ 8 ] = "韧性";
		effect_name_list[ 9 ] = "暴伤";
		effect_name_list[ 10 ] = "反击率";
		effect_name_list[ 11 ] = "碾压";
		effect_name_list[ 12 ] = "伤害反弹";
		effect_name_list[ 13 ] = "吸血率";
		effect_name_list[ 14 ] = "怒气改变";
		effect_name_list[ 15 ] = "斗气上限";
		effect_name_list[ 16 ] = "无用";
		effect_name_list[ 17 ] = "个体负面清除";
		effect_name_list[ 18 ] = "个体正面清除";
		effect_name_list[ 19 ] = "个体所有状态清除";
		effect_name_list[ 20 ] = "无用";
		effect_name_list[ 21 ] = "DOT";
		effect_name_list[ 22 ] = "负面状态无效";
		effect_name_list[ 23 ] = "眩晕";
		effect_name_list[ 24 ] = "锁穴";
		effect_name_list[ 25 ] = "伤害加深";
		effect_name_list[ 26 ] = "无用";
		effect_name_list[ 27 ] = "无用";
		effect_name_list[ 28 ] = "无用";
		effect_name_list[ 29 ] = "无用";
		effect_name_list[ 30 ] = "气血改变";
		effect_name_list[ 31 ] = "无用";
		effect_name_list[ 32 ] = "镇静";
		effect_name_list[ 33 ] = "怒气改变";
		effect_name_list[ 34 ] = "碾压伤害";
		effect_name_list[ 35 ] = "碾压无效";
		effect_name_list[ 36 ] = "斗气改变";
		if ( effect_id > SKILL_EFFECT_NUM )
		{
			printf( RED_FONT( "！！！！！！！！！！！！！！不支持的效果！！！！！！！！！！！！！！！！！" ) );
		}
		else
		{
			printf( YELLOW_FONT( "%s" ), effect_name_list[ effect_id ] );
		}
	}
	/**
	 * 调试打印出移除状态的原因
	 * @param	remove_type		移除类型
	 */
	void debug_buff_remove_reason( int remove_type )
	{
		switch ( remove_type )
		{
			case REMOVE_BUFF_ONE:
				printf( "指定移除\n" );
			break;
			case REMOVE_BUFF_EXPIRE:
				printf( "状态过期\n" );
			break;
			case REMOVE_BUFF_ALL:
				printf( "移除全部状态\n" );
			break;
			case REMOVE_BUFF_BAD:
				printf( "移除全部减益状态\n" );
			break;
			case REMOVE_BUFF_GOOD:
				printf( "移除全部增益状态\n" );
			break;
		}
	}
#endif
#endif
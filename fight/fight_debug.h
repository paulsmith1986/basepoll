#ifndef YILE_NEW_FIGHT_DEBUG_H
#define YILE_NEW_FIGHT_DEBUG_H

//调试模式需要用到的一些宏
#ifdef YILE_DEBUG
	#ifndef HTML_DEBUG
		#define YILE_RED "\033[0;32;31m"
		#define YILE_GREEN "\033[0;32;32m"
		#define YILE_BLUE "\033[0;32;34m"
		#define YILE_CYAN "\033[0;32;36m"
		#define YILE_YELLOW "\033[0;32;33m"
		#define YILE_PINK "\033[0;32;35m"
		#define YILE_COLOR_END "\033[m"
	#else
		#define YILE_RED "<font color=\"red\">"
		#define YILE_GREEN "<font color=\"green\">"
		#define YILE_BLUE "<font color=\"blue\">"
		#define YILE_CYAN "<font color=\"cyan\">"
		#define YILE_YELLOW "<font color=\"yellow\">"
		#define YILE_PINK "<font color=\"magenta\">"
		#define YILE_COLOR_END "</font>"
	#endif
	#define RED_FONT( format ) YILE_RED format YILE_COLOR_END
	#define GREEN_FONT( format ) YILE_GREEN format YILE_COLOR_END
	#define CYAN_FONT( format ) YILE_CYAN format YILE_COLOR_END
	#define BLUE_FONT( format ) YILE_BLUE format YILE_COLOR_END
	#define YELLOW_FONT( format ) YILE_YELLOW format YILE_COLOR_END
	#define PINK_FONT( format ) YILE_PINK format YILE_COLOR_END
	#define debug_attack_side( side, end_char ) ATTACK_SIDE == side ? printf( GREEN_FONT( "【攻击方】" ) end_char ) : printf( CYAN_FONT( "【防守方】" end_char ) )
	#define debug_attack_type( type, end_char ) ATTACK_PHYSICS == type ? printf( CYAN_FONT( "【物理攻击】" ) end_char ) : printf( CYAN_FONT( "【法术攻击】" end_char ) )
	#define debug_change_value( value, end_char ) value > 0 ? printf( GREEN_FONT( "+%d" ) end_char, value ) : printf( RED_FONT( "%d" ) end_char, value )
	#define debug_buff_type( type ) 1 == type ? printf( GREEN_FONT( " 〓增益状态〓 " ) ) : printf( RED_FONT( " 〓减益状态〓 " ) )

	typedef struct skill_name_t skill_name_t;
	struct skill_name_t{
		int		skill_id;			//技能ID
		char*	skill_name;			//技能名称
		skill_name_t *next;
	};

	char **DEBUG_BUFF_NAME;
	skill_name_t **DEBUG_SKILL_NAME;
	#define debug_buff_name( buff_id ) DEBUG_BUFF_NAME[ buff_id ]

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
	char *debug_skill_name( int skill_id )
	{
		skill_name_t *tmp_sk = find_skill_name( skill_id );
		if ( NULL == tmp_sk )
		{
			return "无名称";
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
	 * 读取技能字符串
	 */
	int debug_read_config_skill_name( char *file_path )
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
		while ( buff_num > 0 )
		{
			--buff_num;
			uint16_t buff_id;
			int read_ret = fread( &buff_id, sizeof( buff_id ), 1, config_file );
			check_read_ret( read_ret );
			#ifdef YILE_DEBUG
			printf( "查找状态 ID:%d\n", buff_id );
			#endif
			buff_t *buff_info = find_buff_info( buff_id );
			if ( NULL != buff_info )
			{
				int is_error = 0;
				DEBUG_BUFF_NAME[ buff_id ] = debug_read_config_name( config_file, &is_error );
				if ( is_error )
				{
					return -5;
				}
			}
			else
			{
				return -7;
			}
		}
		read_ret = fread( &skill_num, sizeof( skill_num ), 1, config_file );
		check_read_ret( read_ret );
		while ( skill_num > 0 )
		{
			--skill_num;
			uint16_t skill_id;
			int read_ret = fread( &skill_id, sizeof( skill_id ), 1, config_file );
			check_read_ret( read_ret );
			skill_t *skill_info = find_skill_info( skill_id );
			if ( NULL != skill_info )
			{
				int is_error = 0;
				skill_name_t *sk_name = (skill_name_t*)malloc( sizeof( skill_name_t ) );
				sk_name->skill_id = skill_id;
				sk_name->skill_name = debug_read_config_name( config_file, &is_error );
				set_skill_name( skill_id, sk_name );
				if ( is_error )
				{
					return -6;
				}
			}
			else
			{
				return -8;
			}
		}
		return 0;
	}

	/**
	 * 读取字符串
	 */
	char *debug_read_config_name( FILE *config_file, int *is_error )
	{
		uint16_t name_len;
		int read_ret = fread( &name_len, sizeof( name_len ), 1, config_file );
		printf( "name_len : %d\n", name_len );
		if ( 1 != read_ret )
		{
			*is_error = 1;
			return NULL;
		}
		char *re_name = ( char* )malloc( name_len + 1 );
		read_ret = fread( re_name, name_len, 1, config_file );
		if ( 1 != read_ret )
		{
			*is_error = 1;
			return NULL;
		}
		re_name[ name_len ] = '\0';
		return re_name;
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
		effect_name_list[ 8 ] = "免暴率";
		effect_name_list[ 9 ] = "暴伤";
		effect_name_list[ 10 ] = "反伤";
		effect_name_list[ 11 ] = "吸血";
		effect_name_list[ 12 ] = "怒气值";
		effect_name_list[ 13 ] = "清除负面状态";
		effect_name_list[ 14 ] = "清除正面状态";
		effect_name_list[ 15 ] = "清除所有状态";
		effect_name_list[ 16 ] = "DOT";
		effect_name_list[ 17 ] = "晕眩";
		effect_name_list[ 18 ] = "锁穴";
		effect_name_list[ 19 ] = "伤害加深";
		effect_name_list[ 20 ] = "气血值";
		effect_name_list[ 21 ] = "气血值（按气血上限百分比）";
		effect_name_list[ 22 ] = "气血值（按当前气血百分比）";
		effect_name_list[ 23 ] = "镇静";
		effect_name_list[ 24 ] = "武力值";
		effect_name_list[ 25 ] = "智力值";
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
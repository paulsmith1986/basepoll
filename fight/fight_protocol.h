#ifndef FIRST_NEW_FIGHT_PROTOCOL_H
#define FIRST_NEW_FIGHT_PROTOCOL_H
#include "first_fight.h"
#pragma pack(1)

#define			RESULT_NEW_SEC				1			//每一秒开始
#define			RESULT_NEW_TEAM				2			//切换小队
#define			RESULT_JOIN_MEMBER			3			//加入新成员
#define			RESULT_ATTACK				4			//出手攻击
#define			RESULT_DAMAGE_S				5			//轻微气血改变
#define			RESULT_DAMAGE_M				6			//中量气血改变
#define			RESULT_DAMAGE_L				7			//大量气血改变
#define			RESULT_BUFF_ADD				8			//获得状态
#define			RESULT_BUFF_DEL				9			//状态移除
#define			RESULT_MEMBER_DR			10			//躲闪
#define			RESULT_MEMBER_DS			11			//暴击
#define			RESULT_MEMBER_DIE			12			//成员阵亡
#define			RESULT_FIGHT_END			13			//战斗结束
#define			RESULT_ANGER_ADD			14			//怒气改变

//切换小队包
typedef struct result_pack_new_team_t result_pack_new_team_t;
struct result_pack_new_team_t{
	char		pack_id;
	char		side;						//立场
	char		team_id;					//小队ID
};
//成员数据包
typedef struct result_pack_join_t result_pack_join_t;
struct result_pack_join_t{
	char		pack_id;
	uint8_t		index_id;					//在战斗中的序号
	char		side;						//立场
	char		cell_id;					//格子ID
	uint16_t	anger_max;					//怒气值上限
	int			life_max;					//最大气血
	uint32_t	swf_id;						//模型Id
	char		name[ MEMBER_NAME_LEN ];	//名称长度  MEMBER_NAME_LEN = 20
};

//新的一秒开始
typedef struct result_pack_sec_t result_pack_sec_t;
struct result_pack_sec_t{
	char		pack_id;
	uint16_t	sec;						//秒数
};

//成员出手攻击包
typedef struct result_pack_attack_t result_pack_attack_t;
struct result_pack_attack_t{
	char		pack_id;
	uint8_t		index_id;					//ID
	uint16_t	skill_id;					//技能ID
	char		attack_num;					//攻击次数
	char		attack_area;				//攻击范围
};

//小气血包
typedef struct result_pack_life_s_t result_pack_life_s_t;
struct result_pack_life_s_t{
	char		pack_id;
	uint8_t		index_id;					//ID
	char		reason;						//原因
	char		value;						//改变量
};
//中气血包
typedef struct result_pack_life_m_t result_pack_life_m_t;
struct result_pack_life_m_t{
	char		pack_id;
	uint8_t		index_id;					//ID
	char		reason;						//原因
	int16_t		value;						//改变量
};
//大气血包
typedef struct result_pack_life_l_t result_pack_life_l_t;
struct result_pack_life_l_t{
	char		pack_id;
	uint8_t		index_id;					//人物ID
	char		reason;						//原因
	int			value;						//改变量
};

//获得状态
typedef struct result_pack_buff_add_t result_pack_buff_add_t;
struct result_pack_buff_add_t{
	char		pack_id;
	uint8_t		index_id;					//成员ID
	char		buff_type;					//状态类型
	uint16_t	buff_id;					//状态ID
};

//移除状态
typedef struct result_pack_buff_del_t result_pack_buff_del_t;
struct result_pack_buff_del_t{
	char		pack_id;
	uint8_t		index_id;					//成员ID
	uint16_t	buff_id;					//状态ID
};

//怒气值改变
typedef struct result_pack_anger_t result_pack_anger_t;
struct result_pack_anger_t{
	char		pack_id;
	uint8_t		index_id;					//ID
	uint16_t	new_anger;					//新的怒气值
};

//躲闪
typedef struct result_pack_dr_t result_pack_dr_t;
struct result_pack_dr_t{
	char		pack_id;
	uint8_t		index_id;					//ID
};

//暴击
typedef struct result_pack_ds_t result_pack_ds_t;
struct result_pack_ds_t{
	char		pack_id;
	uint8_t		index_id;					//ID
};

//死亡
typedef struct result_pack_die_t result_pack_die_t;
struct result_pack_die_t{
	char		pack_id;
	uint8_t		index_id;					//ID
};

//战斗结束
typedef struct result_pack_end_t result_pack_end_t;
struct result_pack_end_t{
	char		pack_id;
	char		result;						//战斗最终结果
};
#pragma pack()
#endif
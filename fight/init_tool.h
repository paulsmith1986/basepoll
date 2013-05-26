#ifndef FIRST_FIGHT_INIT_TOOL_H
#define FIRST_FIGHT_INIT_TOOL_H
#include <stdint.h>
#pragma pack(1)
//状态信息_初始化状态工具所用
typedef struct tool_buff_t tool_buff_t;
struct tool_buff_t{
	uint16_t			buff_id;				//状态ID
	char				buff_type;				//状态类型
	char				can_sup;				//能否叠加
	char				effect_num;				//效果个数
};

//效果信息_初始化效果工具所用
typedef struct tool_effect_t tool_effect_t;
struct tool_effect_t{
	char				effect_id;				//效果ID
	char				effect_value_type;		//值类型
	uint16_t			eff_value;				//值
};
//技能描述_初始化技能所用
typedef struct tool_skill_t tool_skill_t;
struct tool_skill_t{
	uint16_t			skill_id;				//技能id
	uint16_t			self_hr_type;			//己方的命中率类型
	uint16_t			self_hr;				//己方的命中率值
	uint16_t			object_hr_type;			//对方的命中率类型
	uint16_t			object_hr;				//对方的命中率值
	char				attack_num;				//攻击次数
	char				attack_type;			//攻击类型 1 => 物理攻击 2 => 法术攻击
	char				object_aim;				//技能作用的范围 0:单体攻击 1:扫荡攻击 2:突进攻击 3:半突击攻击 4:传递攻击 5:全屏攻击
	char				self_aim;				//技能作用的范围 0:单体攻击 1:扫荡攻击 2:突进攻击 3:半突击攻击 4:传递攻击 5:全屏攻击
	char				self_buff_num;			//己方状态个数
	char				object_buff_num;		//对方状态个数
	char				self_effect_num;		//己方直接效果个数
	char				object_effect_num;		//对方直接效果个数
};

//技能状态描述_初始化技能所用
typedef struct tool_sk_buff_t tool_sk_buff_t;
struct tool_sk_buff_t{
	uint16_t			buff_id;				//状态ID
	char				last_time_type;			//持续时间类型 0:固定  1:公式计算
	uint16_t			last_time;				//时间值或者公式ID
};
#pragma pack()
#endif
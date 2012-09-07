// ********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: Wu yongmin
// Created: 2005/11/18
//
// ********************************************************************

#ifndef _GS_ENUM_DEFINE_H_
#define _GS_ENUM_DEFINE_H_

#define ID_NONE 0

//同客户端对应
enum USER_ATTR_TYPE
{
	USERATTRIB_INVALID	= -1,		// 非法值
	
	USERATTRIB_LIFE = 0,			//血
	USERATTRIB_MAXLIFE,				//最大血
	USERATTRIB_MONEY,				//金钱
	USERATTRIB_EXP,					//经验
	USERATTRIB_EMONEY,				//代币
	USERATTRIB_MANA,				//魔
	USERATTRIB_MAXMANA,				//最大魔
	USERATTRIB_PK,					//PK值
	USERATTRIB_PK_ENABLE,			//pk标记
	USERATTRIB_LEV,					//等级
	
	
	USERATTRIB_PHY_POINT,			//力量
	USERATTRIB_DEX_POINT,			//敏捷
	USERATTRIB_MAG_POINT,			//智力
	USERATTRIB_DEF_POINT,			//体质
	USERATTRIB_RESET_POINT,			//自由点
	
	
	USERATTRIB_POWER,				//暂不用
	USERATTRIB_MAXPOWER,			//最大魔
	USERATTRIB_REPUTE,				
	USERATTRIB_EXPSTEAL,
	USERATTRIB_EXPCREATIVE,
	USERATTRIB_EXPMEDICINE,
	USERATTRIB_EXPSMITH,
	USERATTRIB_MONEYSAVED,
	USERATTRIB_ADDPOINT,			//自由点数
	USERATTRIB_LOOK,
	USERATTRIB_PHYSIQUE,
	USERATTRIB_STAMINA,
	
	USERATTRIB_POSE,
	USERATTRIB_DEGREELEV,
	USERATTRIB_INTERLOCK,
	USERATTRIB_GODTYPE,
	USERATTRIB_METEMPSYCHOSIS,
	USERATTRIB_DEED,
	USERATTRIB_CYCLEREBORN,
	USERATTRIB_FACE,
	
	USERATTRIB_CLOTHES,
	USERATTRIB_CLOTHES_COLORINDEX,
	USERATTRIB_HAIR,
	USERATTRIB_HAIR_COLORINDEX,
	USERATTRIB_WEAPON,
	USERATTRIB_WEAPON_COLORINDEX,
	
	USERATTRIB_EXPBLACKSMITH_LEVEL,
	USERATTRIB_EXPMEDICAL,
	USERATTRIB_EXPMEDICAL_LEVEL,		//下一级需要经验
	USERATTRIB_PETEXP,			
	USERATTRIB_PETLIFE,		
	USERATTRIB_ATTACK,		
	USERATTRIB_DEFENCE,		
	USERATTRIB_DEXTERITY,	
	USERATTRIB_REIKI,				
	USERATTRIB_STATUS,					//玩家状态
	USERATTRIB_CAMP,
	USERATTRIB_EXTRA_PHY_POINT, // 附加力量
	USERATTRIB_EXTRA_DEX_POINT, // 附加敏捷
	USERATTRIB_EXTRA_MAG_POINT, // 附加智力
	USERATTRIB_EXTRA_DEF_POINT, // 附加体质
	USERATTRIB_PHY_ATK, // 玩家物理攻击值 = 玩家等级基础攻击点数+升级配点+装备附加+状态附加
	USERATTRIB_PHY_DEF, // 玩家物理防御
	USERATTRIB_MAGIC_ATK, // 玩家魔法攻击
	USERATTRIB_MAGIC_DEF, // 玩家魔法防御
	USERATTRIB_ATK_SPEED, // 攻击速度
	USERATTRIB_HITRATE, // 玩家基础命中
	USERATTRIB_DODGE, // 玩家基础闪避
	USERATTRIB_HARDHIT, // 暴击率
	USERATTRIB_SAVED2,				// 积分
	USER_ATTRIB_ACTIVITY_POINT,		// 活力
	USER_ATTRIB_MAX_ACTIVITY,		// 活力上限
	USERATTRIB_MAX_SKILL_SLOT,		// 技能槽上限
	USERATTRIB_VIP_RANK,	// vip等级
	USERATTRIB_VIP_EXP,		// vip经验
	USERATTRIB_PEERAGE,		// 爵位
	USERATTRIB_END
};

enum EQUIP_TYPE
{
	EQUIP_TYPE_BASE		  = 100000, //十万位	
	EQUIP_NONE			  = 0,
	EQUIP_HELMET		  = 41,//头盔
	EQUIP_SHOULDER_ARMOR  = 42,//肩膀
	EQUIP_CHEST_ARMOR     = 43,//胸甲
	EQUIP_CUFF			  = 44,//护腕
	EQUIP_SASH			  = 45,//披风
	EQUIP_LEG_GUARD		  = 46,//护腿
	EQUIP_NECKLACE		  = 47,//鞋子
	EQUIP_ERRING		  = 51,//项链
	EQUIP_PHYLACTERY	  = 52,//耳环
	EQUIP_PROTECTED_SYMBOL= 53,//护符
	EQUIP_RING			  = 54,//左戒指
	
	EQUIP_WEAPON_DB_KNIFE = 11,//刀
	EQUIP_WEAPON_DB_SWORD = 12,//剑
	EQUIP_WEAPON_DB_STICK = 13,//杖	
	EQUIP_WEAPON_DB_BOW	  = 14,//弓
	EQUIP_WEAPON_DB_SPEAR = 15,//枪
	EQUIP_WEAPON_DB_STICK_EX= 16,//杖	
	
	EQUIP_WEAPON_KNIFE				= 21,//刀
	EQUIP_WEAPON_SWORD				= 22,//剑
	EQUIP_WEAPON_DAGGE				= 23,//匕首	
	EQUIP_WEAPON_MOON_SHAPE_KNIFE	= 24,//护手勾
	
	EQUIP_WEAPON_DP_SHIELD			= 31,//盾牌
	EQUIP_WEAPON_DP_MAGIC			= 32,//法器
};

enum PET_TYPE
{
	PET_TYPE_BASE			= 1000000,	// 百万位
	PET_EGG					= 0,		// 宠物蛋
	PET_PET					= 1,		// 宠物
	PET_MOUNTS				= 4,		// 骑宠
	
};

enum EXPENDABLE_TYPE
{
	EXPENDABLE_TYPE_1 =1,
};

enum EQUIP_SORT
{
	EQUIP_SORT_BASE			= 10000000,//千万位
	EQUIP_SORT_BASE_2		= 100000, //十万位		
	EQUIP_SORT_BASE1		= 10,//
	EQUIP_SORT_EQUIP		= 0,//装备
	EQUIP_SORT_PET			= 1,//宠物
	EQUIP_SORT_EXPENDABLE   = 2,//消耗品
	EQUIP_SORT_TASK			= 3,//任务物品,
	EQUIP_SORT_MAP			= 4,//地图物品
	EQUIP_SORT_BUILD		= 5,//城堡建设
	EQUIP_SORT_MEDICINE		= 61,//草药
	EQUIP_SORT_BOX			= 7,//礼包和宝箱类
	EQUIP_SORT_TASK_BIAO_YIN= 380,	//镖银,
	EQUIP_SORT_BOX_KEY		= 73,//需要钥匙的礼包
};

enum
{
	BATTLE_JUDGE_INTERVAL   = 1000,
	BATTLE_JUDGE_RANGE	    = 15,
	BATTLE_SINGLE_USER_MONSTERLIMIT = 3,
	BATTLE_RATE				= 100,
};

enum WUXING
{
	SPECIES_SPECIAL			=1, 
	SPECIES_WATER, 
	SPECIES_FIRE, 
	SPECIES_GOLD, 
	SPECIES_WOOD, 
	SPECIES_EARTH, 
	SPECIES_UNEVOLVE
};

enum CLASS
{
	CLASS_NONE				=0, 
	CLASS_WATER				=2, 
	CLASS_FIRE, 
	CLASS_METAL, 
	CLASS_WOOD, 
	CLASS_EARTH
};

enum WUXING_RELATION
{
	WUXING_NONE				=0, 
	WUXING_SUPRESS			=1, 
	WUXING_BESUPRESSED		=2, 
	WUXING_BENIFIT			=5, 
	WUXING_BEBENIFITED		=6
};

enum ITEMPOSITION
{
	POSITION_NONE					= 255,
	POSITION_PACK					= 0,//玩家身上的背包
	POSITION_EQUIP_BEGIN			= 1,//玩家身上装备
	POSITION_EQUIP_HELMET			= POSITION_EQUIP_BEGIN,//头盔
	POSITION_EQUIP_SHOULDER_ARMOR   = 2,//肩膀
	POSITION_EQUIP_CHEST_ARMOR      = 3,//胸甲
	POSITION_EQUIP_CUFF			    = 4,//护腕
	POSITION_EQUIP_SASH			    = 5,//
	POSITION_EQUIP_LEG_GUARD	    = 6,//护腿
	POSITION_EQUIP_NOT_SHOW_BEGIN	= POSITION_EQUIP_LEG_GUARD,		//不展示
	POSITION_EQUIP_NECKLACE		    = 7,//鞋子
	POSITION_EQUIP_ERRING		    = 8,//项链
	POSITION_EQUIP_PHYLACTERY	    = 9,//耳环
	POSITION_EQUIP_PROTECTED_SYMBOL = 10,//护符
	POSITION_EQUIP_LEFT_RING		= 11,//左戒指
	POSITION_EQUIP_RIGHT_RING	    = 12,//右戒指
	POSITION_EQUIP_NOT_SHOW_END		= POSITION_EQUIP_RIGHT_RING, //不展示
	POSITION_EQUIP_LEFT_WEAPON	    = 13,//左武器
	POSITION_EQUIP_RIGHT_WEAPON	    = 14,//右武器
	
	POSITION_MOUNTS					= 80,//坐骑
	POSITION_PET					= 81,//宠物
	POSITION_EQUIP_END	            = POSITION_PET,//玩家身上装备
	
	
	POSITION_STORAGE	            = 90,//物品在仓库
	POSITION_AUCTION				= 91,//物品在拍卖行
	POSITION_MAIL					= 92,//邮件物品
	POSITION_SYSTEM					= 93,//系统赠品
	POSITION_SOLD					= 100,//已售物品
};

enum ITEMSORT
{
	ITEMSORT_BASE0			= 10000000,
	ITEMSORT_PET_TYPE		= 1000000,		
	ITEMSORT_PILE			= 2,
	ITEMSORT_TASK			= 3,
	ITEMSORT_PET			= 1,
};

enum ITEM_QUALITY
{
	ITEM_QUALITY_NONE,
	ITEM_QUALITY_COMMON = 5,	//普通
	ITEM_QUALITY_PRISE,     //精制<优秀>
	ITEM_QUALITY_RARE,		//稀有
	ITEM_QUALITY_EPIC,		//史诗
	ITEM_QUALITY_LEGEND,	//传说
};

enum EUDEMONSTATE
{ 
	EUDEMONSTATE_NORMAL		=0, 
	EUDEMONSTATE_SHOW,				//展示
	EUDEMONSTATE_FOLLOW,			//逛街
	EUDEMONSTATE_STORAGE,			//仓库 
};

enum USERSTATE
{
	USERSTATE_NORMAL		= 0x00, 
	USERSTATE_FREEZED		= 0x01,			//冰冻1
	USERSTATE_FIGHTING		= 0x02,			//战斗2
	USERSTATE_BOOTH			= 0x04,			//摆摊状态4
	USERSTATE_DEAD			= 0x08,			//死亡状态8
	USERSTATE_DOUBLE_EXP	= 0x0010,		//双倍经验16
	USERSTATE_PVE			= 0x0020,		//免PK32
	USERSTATE_STEALTH		= 0x0040,		//隐形64
	USERSTATE_SHAPESHIFT	= 0x0080,		//变形128
	USERSTATE_CAMP		 	= 0x0100,		//战争状态256
	USERSTATE_HU_BIAO	 	= 0x0200,		//护镖任务状态512
	USERSTATE_SPEED_UP	 	= 0x0400,		//加速状态1024
	USERSTATE_WATCH		 	= 0x0800,		//观战2048
	USERSTATE_NUM_ONE	 	= 0x1000,		//状元4096
	USERSTATE_BATTLE_POSITIVE		= 0x2000,				//战斗正 - 捕,镖
	USERSTATE_BATTLE_NEGATIVE		= 0x4000,				//战斗负 - 盗
	USERSTATE_FLY					= 0x8000,				// 飞行状态
	USERSTATE_BF_WAIT_RELIVE		= 0x10000,				// 等待复活
	USERSTATE_BATTLEFIELD			= 0x20000,				// 战场状态
};

enum USERPOSE
{
	POSE_NORMAL				=0, 
	POSE_SAYHELLO, 
	POSE_LIE, 
	POSE_SITDOWN, 
	POSE_KNEELDOWN, 
	POSE_FURY, 
	POSE_LAUGH, 
	POSE_POLITENESS, 
	POSE_FLY
};

enum MAPDATA
{
	MAPDATA_ID		= 0,
	MAPDATA_NAME,
	MAPDATA_DESCRIBE,
	MAPDATA_MAPDOC,
	MAPDATA_TYPE,
	
	MAPDATA_OWNER_ID,
	MAPDATA_OWNER_TYPE,
	
	MAPDATA_PORTAL0_X,
	MAPDATA_PORTAL0_Y,
	
	
	MAPDATA_REBORN_MAP_ID,
	MAPDATA_REBORN_X,
	MAPDATA_REBORN_Y,
	
	MAPDATA_LINK_MAP_ID,
	MAPDATA_LINK_MAP_X,
	MAPDATA_LINK_MAP_Y,	
	
	MAPDATA_RESOURCE_LEV,
	
	MAPDATA_IDSERVER,
};

enum NPCDATA
{
	NPCDATA_ID		= 0,
	NPCDATA_NAME,
	NPCDATA_TYPE,
	NPCDATA_LOOKFACE,
	NPCDATA_BASE,
	
	NPCDATA_IDSERVER,
	NPCDATA_IDMAP,
	NPCDATA_CELLX,
	NPCDATA_CELLY,
	
	NPCDATA_ID_ACTION,
	NPCDATA_TASK0,
	NPCDATA_TASK1,
	NPCDATA_TASK2,
	NPCDATA_TASK3,
	NPCDATA_TASK4,
	NPCDATA_TASK5,
	NPCDATA_TASK6,
	NPCDATA_TASK7,
	NPCDATA_TASK8,
	NPCDATA_TASK9,
	
	NPCDATA_DATA0,
	NPCDATA_DATA1,
	NPCDATA_DATA2,
	NPCDATA_DATA3,
	
	NPCDATA_STR,
	NPCDATA_SORT,
	
	NPCDATA_SHOPID,
	NPCDATA_CAMP,
	NPCDATA_DYNAFLAG,
	NPCDATA_ID_MONSTER_TYPE,
	NPCDATA_GENERATE,
};

enum NPC_SORT
{
	NPCSORT_NONE		= 0,
	NPCSORT_TASK		= 1,			// 任务类
	NPCSORT_RECYCLE		= 2,			// 可回收类
	NPCSORT_SCENE		= 4,			// 场景类(带地图物件)
	NPCSORT_LINKMAP		= 8,			// 挂地图类(LINKID为地图ID，与其它使用LINKID的互斥)
	NPCSORT_DIEACTION	= 16,			// 带死亡任务(LINKID为ACTION_ID，与其它使用LINKID的互斥)
	NPCSORT_DELENABLE	= 32,			// 可以手动删除(不是指通过任务)
	NPCSORT_EVENT		= 64,			// 带定时任务, 时间在data3中，格式为DDWWHHMMSS。(LINKID为ACTION_ID，与其它使用LINKID的互斥)
	NPCSORT_TABLE		= 128,			// 带数据表类型
	
	NPCSORT_SHOP		= 256,			// 商店类
	NPCSORT_VIPSHOP		= 512,			// VIP商店类
	//		NPCSORT_DICE		= ,		// 骰子NPC
};

enum NPC_TYPE
{
	NPCTYPE_TASK					= 0,			// 任务NPC
	NPCTYPE_CAMP_BOSS				= 1,			// 阵营BOSS
	NPCTYPE_CAMP_BANNER				= 2,			// 旗帜
	NPCTYPE_CAMP_SOLDIER			= 3,			// 士兵NPC
	NPCTYPE_DYNAMIC					= 4,			// 动态NPC
};

enum SOLDIER_RANK
{
	SOLDIER_RANK_NONE = ID_NONE,	//普通士兵
	SOLDIER_RANK_LV_1,				//校尉
	SOLDIER_RANK_LV_2,				//都尉
	SOLDIER_RANK_LV_3,				//偏将
	SOLDIER_RANK_LV_4,				//典军中郎将
	SOLDIER_RANK_LV_5,				//建威总参将
	SOLDIER_RANK_LV_6,				//定北将军
	SOLDIER_RANK_LV_7,				//征西将军
	SOLDIER_RANK_LV_8,				//镇南将军
	SOLDIER_RANK_LV_9,				//平东将军
	SOLDIER_RANK_LV_10,				//护国将军
	SOLDIER_RANK_LV_11,				//辅国将军
	SOLDIER_RANK_LV_12,				//右武卫将军
	SOLDIER_RANK_LV_13,				//左武卫将军
	SOLDIER_RANK_LV_14,				//骁骑大将军
	SOLDIER_RANK_LV_15,				//三军统帅	
};

//////////////////////////////////////////////////////////////////////////
//逻辑对应
enum USER_ATTR
{
	//帐户相关
	USER_ATTR_ID		= 0,			//人物角色ID
	USER_ATTR_ACCOUNT_ID,			//玩家帐户ID
	USER_ATTR_NAME,					//玩家名字
	USER_ATTR_TITLE,					//头衔
	USER_ATTR_CREATETIME,			//创建时间	
	
	USER_ATTR_LOOKFACE,				//人物外形头像
	USER_ATTR_PROFESSION,			//职业			
	USER_ATTR_HAIR,					//发型
	
	USER_ATTR_MONEY,					//身上现金数
	USER_ATTR_MONEY_SAVED,			//存款
	USER_ATTR_MONEY_SAVED2,			//存款
	
	
	USER_ATTR_EMONEY,				//身上现太阳石数
	USER_ATTR_EMONEY_SAVE,			//仓库代币
	USER_ATTR_EMONEY_CHK,			//身上现太阳石数
	
	USER_ATTR_STORAGELIMIT,			//仓库重量上限
	USER_ATTR_PACKAGE_LIMIT,		//背包上限	
	USER_ATTR_LOCK_KEY,				//二级密码
	
	USER_ATTR_RECORD_MAP,			//玩家离线时所在的地图id
	USER_ATTR_RECORD_X,				//玩家离线时所在的地图x坐标
	USER_ATTR_RECORD_Y,				//玩家离线时所在的地图y坐标
	
	USER_ATTR_LAST_LOGIN,			//上次登陆时间点（年月日，如20040623）]
	USER_ATTR_ONLINE_TIME2,			//在线时间2
	USER_ATTR_OFFLINE_TIME,          //离线时间
	USER_ATTR_LAST_LOGOUT2,          //离线时间点
	USER_ATTR_LOGIN_TIME,			//登陆时间点
	
	USER_ATTR_IP,					//玩家注册时的IP地址
	USER_ATTR_REBORN_MAPID,			//玩家复活的地图id
	
	USER_ATTR_ONLINE_TIME,			//在线时间
	USER_ATTR_AUTO_EXERCISE,		//环保练功的标志
	
	USER_ATTR_LEVEL,				//等级
	USER_ATTR_EXP,					//经验值
	USER_ATTR_PK,					//PK值
	
	USER_ATTR_LIFE,					//生命值
	USER_ATTR_MANA,					//魔法值
	
	USER_ATTR_PHY_POINT,			//力量点数
	USER_ATTR_DEX_POINT,			//敏捷点数
	USER_ATTR_MAG_POINT,			//智力点数
	USER_ATTR_DEF_POINT,			//体质点数
	
	USER_ATTR_FORB_WORDS,			//禁言标志 
	USER_ATTR_RANK,					//军衔
	USER_ATTR_CAMP,					//阵营
	USER_ATTR_MARRY_ID,				//结婚id
	USER_ATTR_REIKI,				//灵气值
	USER_ATTR_STATUS,				//玩家状态
	USER_ATTR_CHANNEL,				//渠道ID
	USER_ATTR_GMONEY,				//赠币
	USER_ATTR_GMONEY_SAVE,			//仓库赠币
	USER_ATTR_ACCOUNT_NAME,			//帐号名
	//////////////////////////////////////////////////////////////////////////
	//以上和USERDATA枚举对应，非数据库变更不要修改以上顺序和插入删除操作
	//以下为逻辑定义区域
	//////////////////////////////////////////////////////////////////////////
	
	USER_ATTR_LOGIC_BEGIN,			//逻辑定义起点
	USER_ATTR_PHY_ATK,				//玩家物理攻击值 = 玩家等级基础攻击点数+升级配点+装备附加+状态附加
	USER_ATTR_PHY_DEF,				//玩家物理防御
	USER_ATTR_MAGIC_ATK,			//玩家魔法攻击
	USER_ATTR_MAGIC_DEF,			//玩家魔法防御
	
	USER_ATTR_PHY_BASE_ATK,			//玩家物理攻击基础值 = 玩家等级基础攻击点数+升级配点
	USER_ATTR_PHY_BASE_DEF,			//玩家基础防御
	USER_ATTR_MAGIC_BASE_ATK,		//玩家基础魔攻
	USER_ATTR_MAGIC_BASE_DEF,		//玩家基础魔防
	
	USER_ATTR_MAX_LIFE,				//玩家生命值上限
	USER_ATTR_MAX_MANA,				//玩家魔法值上限
	USER_ATTR_FREE_POINT,			//玩家可分配的自由点数
	USER_ATTR_ATK_SPEED,			//攻击速度
	USER_ATTR_HITRATE,				//玩家基础命中
	USER_ATTR_DODGE,				//玩家基础闪避
	USER_ATTR_HARDHIT,				//暴击率
};

//////////////////////////////////////////////////////////////////////////
//用户附加属性变更类型
enum USER_ADDTION_TYPE
{
	USER_ADDTION_TYPE_NONE	= ID_NONE,
	USER_ADDTION_TYPE_EQUIP,
	USER_ADDTION_TYPE_STATUS,
};

enum EUDEMONDATA
{
	EUDEMONDATA_NAME=0,					//幻兽名字
	EUDEMONDATA_LOOK,					//幻兽名字
	EUDEMONDATA_ATTACK,					//攻击
	EUDEMONDATA_DEFENCE,				//防御
	EUDEMONDATA_DEXTERITY,				//敏捷
	EUDEMONDATA_BASE_ATTACK,			//基本攻击
	EUDEMONDATA_BASE_DEFENCE,			//基本防御
	EUDEMONDATA_BASE_DEXTERITY,			//基本敏捷
	EUDEMONDATA_LEVEL,					//当前等级
	EUDEMONDATA_EXP,					//当前经验值
	EUDEMONDATA_LIFE,					//当前生命
	EUDEMONDATA_GROW_RATE,				//成长率
	EUDEMONDATA_GENERATION,				//宠物代数(即该宠物为第几代宠)
	EUDEMONDATA_POS_X,					//地图上X坐标
	EUDEMONDATA_POS_Y,					//地图上Y坐标
	EUDEMONDATA_ATTACK_COUNT,			//宠物攻击技能总数
	EUDEMONDATA_ATTACK_0,				//宠物攻击技能1
	EUDEMONDATA_ATTACK_1,				//宠物攻击技能2
	EUDEMONDATA_ATTACK_2,				//宠物攻击技能3
	EUDEMONDATA_ATTACK_3,				//宠物攻击技能4
	EUDEMONDATA_ATTACK_4,				//宠物攻击技能5
	EUDEMONDATA_CAP_LEVEL,				//捕捉的级别
	EUDEMONDATA_OWNER_ID,				//拥用者ID
	EUDEMONDATA_ID,						//宠物id
	EUDEMONDATA_TREATURE_ID,			//宠物衣服
	
	EUDEMONDATA_MEDAL_ATTACK,			//攻击勋章
	EUDEMONDATA_MEDAL_DEFENCE,			//防御勋章
	EUDEMONDATA_MEDAL_DEXTERITY,		//敏捷勋章
	
	EUDEMONDATA_BASE_LIFE,				//基本生命值(1级时的记录值)
	EUDEMONDATA_MAX_LIFE,				//最大生命
	EUDEMONDATA_CLASS,					//宠物类别
	EUDEMONDATA_FIDELITY,				//忠诚度
	EUDEMONDATA_LIFE_RISE,				//生命率
	EUDEMONDATA_ATTACK_RATE,			//攻击率	
	EUDEMONDATA_DEFENCE_RATE,			//防御率
	EUDEMONDATA_DEXTERITY_RATE,			//敏捷率
	EUDEMONDATA_STATE,					//
	
	EUDEMONDATA_HUE0,					//
	EUDEMONDATA_SATURATION0,			//
	EUDEMONDATA_BRIGHT0,				//
	EUDEMONDATA_HUE1,					//
	EUDEMONDATA_SATURATION1,			//
	EUDEMONDATA_BRIGHT1,				//
	EUDEMONDATA_HUE2,					//
	EUDEMONDATA_SATURATION2,			//
	EUDEMONDATA_BRIGHT2,				//
	
	EUDEMONDATA_ORG_GROWRATE,			//
};

enum SKILLTYPEDATA
{
	SKILLTYPEDATA_NAME,
	SKILLTYPEDATA_SORT,
	SKILLTYPEDATA_OWNER_ID,
	SKILLTYPEDATA_LOOK,
	SKILLTYPEDATA_TRACE,
	SKILLTYPEDATA_BLAST,
	SKILLTYPEDATA_LEVEL_REQUIRED,
	SKILLTYPEDATA_POWER,
	SKILLTYPEDATA_ATTACK_COEFFICIENT,
	SKILLTYPEDATA_DEFENCE_COEFFICIENT,
	SKILLTYPEDATA_RECOVER_COEFFICIENT,
	SKILLTYPEDATA_MAGIC_COEFFICIENT,
	SKILLTYPEDATA_CAST_COEFFICIENT,
	SKILLTYPEDATA_LOST_COEFFICIENT,
	SKILLTYPEDATA_MAGIC_ABOUT,
	SKILLTYPEDATA_RANGE,
	SKILLTYPEDATA_MAGIC_RANGE,
	SKILLTYPEDATA_MAGIC_SORT,
	SKILLTYPEDATA_ID,
	SKILLTYPEDATA_SELECT_FLAG,
	SKILLTYPEDATA_EXP,
	SKILLTYPEDATA_INVENTER_NAME,
};

enum SKILLDATA
{
	SKILLDATA_NAME,
	SKILLDATA_SORT,
	SKILLDATA_OWNER_ID,
	SKILLDATA_LOOK,
	SKILLDATA_TRACE,
	SKILLDATA_BLAST,
	SKILLDATA_LEVEL_REQUIRED,
	SKILLDATA_POWER,
	SKILLDATA_ATTACK_COEFFICIENT,
	SKILLDATA_DEFENCE_COEFFICIENT,
	SKILLDATA_RECOVER_COEFFICIENT,
	SKILLDATA_MAGIC_COEFFICIENT,
	SKILLDATA_CAST_COEFFICIENT,
	SKILLDATA_LOST_COEFFICIENT,
	SKILLDATA_MAGIC_ABOUT,
	SKILLDATA_RANGE,
	SKILLDATA_MAGIC_RANGE,
	SKILLDATA_MAGIC_SORT,
	SKILLDATA_ID,
	SKILLDATA_SELECT_FLAG,
	SKILLDATA_EXP,
	SKILLDATA_INVENTER_NAME,
};

enum GODSPECIALITY
{
	GODSPECIALITY_NONE=0, 
	GODSPECIALITY_PET,
	GODSPECIALITY_DEGREELEV,
	GODSPECIALITY_DEED,
	GODSPECIALITY_SMITH,
	GODSPECIALITY_KONGFU,
	GODSPECIALITY_STEAL,
	GODSPECIALITY_REPUTE
};

enum ITEMTYPEDATA	//itemtype 表
{		
	ITEMTYPEDATA_ID = 0,
	ITEMTYPEDATA_NAME,			
	ITEMTYPEDATA_LEVEL,			
	ITEMTYPEDATA_DROP_FLAG,		
	
	ITEMTYPEDATA_REQ_LEVEL,
	ITEMTYPEDATA_REQ_SEX,
	ITEMTYPEDATA_REQ_PHY,
	ITEMTYPEDATA_REQ_DEX,
	ITEMTYPEDATA_REQ_MAG,
	ITEMTYPEDATA_REQ_DEF,
	
	ITEMTYPEDATA_PRICE,
	ITEMTYPEDATA_EMONEY,
	ITEMTYPEDATA_SAVE_TIME,
	
	ITEMTYPEDATA_ID_ACTION,
	ITEMTYPEDATA_LIFE,
	ITEMTYPEDATA_MANA,
	ITEMTYPEDATA_AMOUNT_LIMIT,
	
	ITEMTYPEDATA_STATE_ID,
	
	ITEMTYPEDATA_HARD_HITERATE,
	
	ITEMTYPEDATA_MANA_LIMIT,
	
	ITEMTYPEDATA_ATK_POINT_ADD,
	ITEMTYPEDATA_DEF_POINT_ADD,
	ITEMTYPEDATA_MAG_POINT_ADD,
	ITEMTYPEDATA_DEX_POINT_ADD,
	
	ITEMTYPEDATA_ATK,
	ITEMTYPEDATA_MAG_ATK,
	ITEMTYPEDATA_DEF,
	ITEMTYPEDATA_MAG_DEF,
	ITEMTYPEDATA_HITRATE,
	ITEMTYPEDATA_ATK_SPEED,
	ITEMTYPEDATA_DODGE,
	ITEMTYPEDATA_MONOPOLY,
	ITEMTYPEDATA_LOOKFACE,
	ITEMTYPEDATA_ICONINDEX,
	ITEMTYPEDATA_HOLE,		// 洞
	ITEMTYPEDATA_DATA,		// 机动字段
	ITEMTYPEDATA_DESCRIPT,	// 物品描述
	ITEMTYPEDATA_SKILLPOINT,// 附带技能
	ITEMTYPEDATA_FLAG,		// 新加物品标记
	
	ITEMTYPEDATA_END
};


enum ITEMDATA
{
	ITEMDATA_ID =0,		
	ITEMDATA_TYPE,			
	ITEMDATA_OWNERTYPE,			
	ITEMDATA_OWNER_ID,		
	ITEMDATA_PLAYER_ID,
	ITEMDATA_POSITION,
	ITEMDATA_AMOUNT,
	ITEMDATA_MAX_HP,
	ITEMDATA_BIND_STATE,
	ITEMDATA_UNBIND_TIMELIMIT,
	ITEMDATA_DATA,
	ITEMDATA_PLUNDER,
	ITEMDATA_SALE_TIME,
	ITEMDATA_CHK_SUM,
	ITEMDATA_FORGENAME,
	ITEMDATA_SPECIALFLAG,
	ITEMDATA_ADDITION,
	ITEMDATA_HOLE,
	ITEMDATA_CREATE_TIME,
};


enum SYNDICATE_RANK
{ 
	RANK_NONE				=0, 
	RANK_CHAIRMAN			=100,		//帮主
	RANK_VICE_CHAIRMAN		=90,		//副帮主
	RANK_PRESBYTER			=80,		//长老
	RANK_CAPTAIN			=70,		//堂主
	RANK_MONITOR			=60,		//副堂主
	RANK_NEWBIE				=50			//帮众
};

enum SYNDICATE_UPDATE
{ 
	UPDATE_NONE=0, 
	UPDATE_SYNDICATE, 
	UPDATE_SUBGROUP, 
	UPDATE_USER,
	COMBINE_SYNDICATE_SYN,		// Add by Mr.chaoxin.Z 2008.05.10
	COMBINE_SYNDICATE_SUB,
	COMBINE_SYNDICATE_MEMBER,
	MODIFY_TITLE_BY_RANK,
};

enum EVENTN_NPC
{
	EVENT_BEACTIVED = 0, 
	EVENT_JOINBETOUTSIDE, 
	EVENT_LEAVEBETOUTSIDE, 
	EVENT_DELNPC, 
	EVENT_JOINCASINO, 
	EVENT_LEAVECASINO, 
	EVENT_QUERYPACK, 
	EVENT_CHECKIN, EVENT_CHECKOUT, 
	EVENT_CHANGELOOK,
	EVENT_CHANGSTATUS,
};

//BATTLE
enum FIGHTER_TYPE
{
	FIGHTER_TYPE_INVALID = ID_NONE,
	FIGHTER_TYPE_PET,	
	FIGHTER_TYPE_MONSTER,
	FIGHTER_TYPE_EUDEMON,
	FIGHTER_TYPE_ELITE_MONSTER,
	Fighter_TYPE_RARE_MONSTER,
};	

enum BATTLE_GROUP
{
	BATTLE_GROUP_NONE	= ID_NONE,
	BATTLE_GROUP_ATTACK,
	BATTLE_GROUP_DEFENCE,
};

enum BATTLE_STAGE
{
	BATTLE_STAGE_NONE	 = ID_NONE,
	BATTLE_STAGE_START,
	BATTLE_STAGE_TURN_END,
	BATTLE_STAGE_WATCH,
	BATTLE_STAGE_LEAVE_WATCH,
	BATTLE_STAGE_END,
};

enum
{
	USER_WAIT       = 0,
	USER_ANSWER     = 1,
	USER_NOT_ANSWER = 2,
};

enum BATTLE_TYPE
{
	BATTLE_TYPE_NONE	= ID_NONE,
	BATTLE_TYPE_MONSTER,			//怪物战
	BATTLE_TYPE_BOSS,				//BOSS战
	BATTLE_TYPE_SPORTS,				//竞技场
	BATTLE_TYPE_PLAYBACK,			//战斗回放
	//以下已不使用
	BATTLE_TYPE_REHEARSE,			//切磋
	BATTLE_TYPE_PK,					//PK
	BATTLE_TYPE_ELITE_MONSTER,
	BATTLE_TYPE_NPC,
	BATTLE_TYPE_PRACTICE,			//练功
	BATTLE_TYPE_COMPETITION,		//比赛
};

enum BATTLE_ACTION
{
	BATTLE_ACT_NONE = ID_NONE,
	BATTLE_ACT_CREATE,
	BATTLE_ACT_CREATE_ELITE,		//精英怪战斗
	BATTLE_ACT_CREATE_BOSS,		//boss怪战斗
	BATTLE_ACT_INVALID,
	//以下部份已经不使用
	BATTLE_ACT_PHY_ATK,
	BATTLE_ACT_PHY_DEF,
	BATTLE_ACT_MAG_ATK,
	BATTLE_ACT_AUTO_ATK,
	BATTLE_ACT_PET_PHY_ATK,			//宠物攻击
	BATTLE_ACT_PET_PHY_DEF,			//宠物防御
	BATTLE_ACT_PET_MAG_ATK,			//宠物技能
	BATTLE_ACT_ESCAPE,
	BATTLE_ACT_USEITEM,
	//BATTLE_ACT_CREATE,
	BATTLE_ACT_PK,					//PK
	BATTLE_ACT_TURN_END,			//客户端回合表现结束通知
	BATTLE_ACT_REHEARSE,			//比武
	//BATTLE_ACT_CREATE_ELITE,		//精英怪战斗
	BATTLE_ACT_PET_AUTO_ATK,		//宠物自动攻击
	BATTLE_ACT_ARISE,				//什么也不做
	BATTLE_ACT_PET_ESCAPE,			//宠物逃跑
	BATTLE_ACT_CATCH,				//捕捉宠物
	BATTLE_ACT_WATCH,				//观看
	BATTLE_ACT_LEAVE,				//离开
	BATTLE_ACT_PET_USEITEM,
	BATTLE_ACT_USER_COLLIDE,

};

//BATTLE
enum FIGHTER_STATUS
{
	FIGHTER_STATUS_NONE = ID_NONE,
	FIGHTER_STATUS_DEF,				//防御
	FIGHTER_STATUS_DEAD,			//死亡
	FIGHTER_STATUS_LEFT,			//离开状态
	FIGHTER_STATUS_ESCAPE,			//逃跑
};


//BATTLE
enum BATTLE_PROPERTY_TYPE
{
	BATTLE_PROPERTY_TYPE_NONE = ID_NONE,
	BATTLE_PROPERTY_TYPE_LIFE,
	BATTLE_PROPERTY_TYPE_MAXLIFE,
	BATTLE_PROPERTY_TYPE_MANA,
	BATTLE_PROPERTY_TYPE_MAXMANA,
	BATTLE_PROPERTY_TYPE_PHY_ATK,
	BATTLE_PROPERTY_TYPE_PHY_DEF,
	BATTLE_PROPERTY_TYPE_MAGIC_ATK,
	BATTLE_PROPERTY_TYPE_MAGIC_DEF,
	BATTLE_PROPERTY_TYPE_SPEED,
	BATTLE_PROPERTY_TYPE_DODGE,
	BATTLE_PROPERTY_TYPE_HITERATE,
	BATTLE_PROPERTY_TYPE_HARD_HITERATE,	
	BATTLE_PROPERTY_TYPE_STA,
	BATTLE_PROPERTY_TYPE_STR,
	BATTLE_PROPERTY_TYPE_INT,	//智力
	BATTLE_PROPERTY_TYPE_AGI,	//敏捷
};


enum LEVEL_EXP_TYPE
{
	LEVEL = ID_NONE,	// 等级
	METEMPSYCHOISIS,	
	EXP,				// 升至该级需要的经验	
	PER_BATTLE_MAX,		// 该等级每次战斗获取的最大经验值
};

enum ATTACH_STATE
{
	ATTACH_NO				= 0x00,		// 没有附件
	
	// 物品
	ATTACH_ITEM_ID			= 0x01,		// 附件含 item
	ATTACH_ITEM_TYPE		= 0x02,		// 仅系统邮件允许使用
	
	// 赠币
	ATTACH_MONEY			= 0x04,		// 附件含 attach_money
	ATTACH_EMONEY			= 0x08,		// 附件含 attach_emoney
	
	// 收费
	ATTACH_REQUIRE_MONEY	= 0x10,		// 附件含 require_money
	ATTACH_REQUIRE_EMONEY	= 0x20,		// 附件含 require_emoney
	
	// 附件状态 (客户端不发送,由服务端更改)
	ATTACH_ACCEPTION		= 0x40,		// 附件已接收
	ATTACH_REJECTION		= 0x80,		// 附件已拒收
};


enum COUPLE_DATA
{	
	COUPLEDATA_ID	    = 0,
	COUPLEDATA_HUSBID,
	COUPLEDATA_WIFEID,
	COUPLEDATA_HUSBNAME,
	COUPLEDATA_WIFENAME,
};

enum
{
	USER_SEX_MALE=1,
	USER_SEX_FEMALE=2,
	USER_SEXMASK_BASE = 100000000,
	USER_FACEMASK_BASE= 10000,
};

// 声望类型（即阵营）
enum CAMP_TYPE
{
	CAMP_TYPE_NONE		= 0,	// 中立
	CAMP_TYPE_TANG		= 1,	// 唐
	CAMP_TYPE_SUI		= 2,	// 犍
	CAMP_TYPE_TU		= 3,	// 突厥
	CAMP_TYPE_WA		= 4,	// 瓦军
	CAMP_TYPE_WANG		= 5,	// 王军
	CAMP_TYPE_END		= 6,
	//	CAMP_TYPE_BATTLE	= 9,	// 战场
};

// 声望等级
enum REPUTE_LEVEL
{
	REPUTE_LEVEL_MAX		= 15000,
	REPUTE_LEVEL_A			= 5000,
	REPUTE_LEVEL_B			= 1500,
	REPUTE_LEVEL_C			= 500,
	REPUTE_LEVEL_D			= 100,
	REPUTE_LEVEL_E			= -100,
	REPUTE_LEVEL_F			= -500,
	REPUTE_LEVEL_G			= -1500,
	REPUTE_LEVEL_MIN		= REPUTE_LEVEL_G,
	REPUTE_LEVEL_DEFAULT	= 0,
	REPUTE_LEVEL_INIT		= 100,
};

// 商店根据声望的购物折扣率
enum SHOP_REPUTE_LEVEL_RATE
{
	SHOP_REPUTE_LEVEL_RATE_A		= 97,
	SHOP_REPUTE_LEVEL_RATE_B		= 98,
	SHOP_REPUTE_LEVEL_RATE_C		= 99,
	SHOP_REPUTE_LEVEL_RATE_D		= 100,
	SHOP_REPUTE_LEVEL_RATE_E		= 100,
	SHOP_REPUTE_LEVEL_RATE_F		= 100,
	SHOP_REPUTE_LEVEL_RATE_G		= 100,
	SHOP_REPUTE_LEVEL_RATE_DEFAULT	= 100,
};

// 帮众等级
enum SYNMEMBER_RANK
{
	MBRRANK_NO_RANK				= 0,	// 成员
	
	// 门主
	MBRRANK_MENZU_SHENGMEN		= 11,	// 生门
	MBRRANK_MENZU_SHANGMEN		= 12,	// 伤门
	MBRRANK_MENZU_XIUMEN		= 13,	// 休门
	MBRRANK_MENZU_DUMEN			= 14,	// 杜门
	MBRRANK_MENZU_JINGMEN		= 15,	// 景门
	MBRRANK_MENZU_SIMEN			= 16,	// 死门
	MBRRANK_MENZU_JINGMEN1		= 17,	// 惊门
	MBRRANK_MENZU_KAIMEN		= 18,	// 开门
	
	// 堂主
	MBRRANK_TANGZU_TIANLONG		= 51,	// 天龙
	MBRRANK_TANGZU_QINMU		= 52,	// 青木
	MBRRANK_TANGZU_CHIHUO		= 53,	// 赤火
	MBRRANK_TANGZU_XIJIN		= 54,	// 西金
	MBRRANK_TANGZU_XUANSHUI		= 55,	// 玄水
	MBRRANK_TANGZU_HUANGTU		= 56,	// 黄土
	
	// 元老
	MBRRANK_YUANLAO_XUANWU		= 101,	// 玄武
	MBRRANK_YUANLAO_QINGLONG	= 102,	// 青龙
	MBRRANK_YUANLAO_BAIHU		= 103,	// 白虎
	MBRRANK_YUANLAO_ZHUQUE		= 104,	// 朱雀
	
	MBRRANK_VICE_LEADER			= 111,	// 副团长
	
	MBRRANK_LEADER				= 121,	// 团长
};

enum ITEMTYPE_MONOPOLY
{
	ITEMTYPE_MONOPOLY_NONE			= 0x00,	//一般物品
	ITEMTYPE_MONOPOLY_NOT_TRADE		= 0x01,	//不可以交易（同时包括不可交易、不可拍卖、不可摆摊出售）
	ITEMTYPE_MONOPOLY_NOT_STORAGE	= 0x02, //不可存仓（同时包括常规仓库和幻兽仓库）
	ITEMTYPE_MONOPOLY_NOT_DROP		= 0x04, //丢弃提示（丢弃时客户端跳出确认窗口，提示内容：请确定要丢弃）
	ITEMTYPE_MONOPOLY_NOT_SALE_TIP	= 0x08, //出售提示（出售给商店时客户端跳出确认窗口，提示内容：请确定要出售）
	ITEMTYPE_MONOPOLY_NOT_MISS		= 0x10, //死亡不会掉落（在掉落相关规则中优先级最高，比如即使人物是黑名，被杀死也不会爆）16
	ITEMTYPE_MONOPOLY_NOT_SALE		= 0x20, //不可出售32
	ITEMTYPE_MONOPOLY_BATTLE		= 0x40, //战斗内可用64
	ITEMTYPE_MONOPOLY_NOT_USE		= 0x80, //不可使用128
	ITEMTYPE_MONOPOLY_NOT_USE_TIP   = 0x0100,//使用提示256
	ITEMTYPE_MONOPOLY_NOT_MAIL		= 0x0200,//不可邮寄512
	ITEMTYPE_MONOPOLY_NOT_ENHANCE   = 0x0400,//不可强化1024
	ITEMTYPE_MONOPOLY_NOT_DEL		= 0x0800,//不可丢弃2048
};

enum BATTLE_LINE
{
	BATTLE_LINE_NONE,
	BATTLE_LINE_FRONT,
	BATTLE_LINE_BACK,
};

enum PET_ATTR
{
	PET_ATTR_BEGIN			= 1,
	PET_ATTR_LEVEL			= PET_ATTR_BEGIN,	//等级INT
	PET_ATTR_EXP			,	//经验INT
	PET_ATTR_LIFE			,	//生命值INT
	PET_ATTR_MAX_LIFE		,	//最大生命值INT
	PET_ATTR_MANA			,	//魔法值INT
	PET_ATTR_MAX_MANA		,	//最大魔法值INT
	PET_ATTR_STR			,	//力量INT
	PET_ATTR_STA			,	//体质INT
	PET_ATTR_AGI			,	//敏捷INT
	PET_ATTR_INI 		    ,	//智力INT
	PET_ATTR_LEVEL_INIT		,   //初始等级INT
	PET_ATTR_STR_INIT		,	//初始力量INT
	PET_ATTR_STA_INIT		,	//初始体质INT
	PET_ATTR_AGI_INIT		,	//初始敏捷INT
	PET_ATTR_INI_INIT	    ,	//初始智力INT
	PET_ATTR_LOYAL			,	//忠诚度INT	
	PET_ATTR_AGE			,	//寿命INT
	PET_ATTR_FREE_SP		,	//剩余技能点数INT
	PET_ATTR_STR_RATE		,	//力量成长率INT
	PET_ATTR_STA_RATE		,	//体质成长率INT
	PET_ATTR_AGI_RATE		,	//敏捷成长率INT
	PET_ATTR_INI_RATE		,	//智力成长率INT
	PET_ATTR_HP_RATE		,	//生命成长率INT
	PET_ATTR_MP_RATE		,	//魔法成长率INT
	PET_ATTR_LEVEUP_EXP		,	//升级经验
	PET_ATTR_PHY_ATK		,	//物理攻击力INT
	PET_ATTR_PHY_DEF		,	//物理防御INT
	PET_ATTR_MAG_ATK		,	//法术攻击力INT
	PET_ATTR_MAG_DEF	    ,	//法术抗性INT
	PET_ATTR_HARD_HIT		,	//暴击
	PET_ATTR_DODGE			,	//闪避
	PET_ATTR_ATK_SPEED		,	//攻击速度
	PET_ATTR_TYPE			,	//类型
	PET_ATTR_LOOKFACE		,	//LOOKFACE
	PET_ATTR_SKILL_1		,	//技能1UINT
	PET_ATTR_SKILL_2		,	//技能2UINT
	PET_ATTR_SKILL_3		,	//技能3UINT	
	PET_ATTR_END			,	//结束
	PET_ATTR_NAME			,	//名字
};

//BATTLE EFFECT
enum BIND_STATE
{
	BIND_STATE_NONE		= 0,	// 无绑定状态
	BIND_STATE_BIND		= 1,	// 已经绑定
	BIND_STATE_UNBIND	= 2,	// 处于解除绑定状态
};

enum EFFECT_ACTION
{
	EFFECT_ACTION_NONE,
	EFFECT_ACTION_BEGIN = EFFECT_ACTION_NONE,
	EFFECT_ACTION_END,
};

/*enum EFFECT_TYPE
{
	EFFECT_TYPE_NONE,
	EFFECT_TYPE_CHANGELIFE,
	EFFECT_TYPE_ESCAPE,
	EFFECT_TYPE_ESCAPE_FAIL,
	EFFECT_TYPE_DODGE,
	EFFECT_TYPE_DEFENCE,
	EFFECT_TYPE_LEFT,
	EFFECT_TYPE_CATCH,
	EFFECT_TYPE_CATCH_FAIL,
};*/

enum ATKTYPE
{
	ATKTYPE_NEAR = 0,
	ATKTYPE_DISTANCE,
};

/*enum EFFECT_CHANGE_LIFE_TYPE
{
	EFFECT_CHANGE_LIFE_TYPE_NONE,
	EFFECT_CHANGE_LIFE_TYPE_PHY_ATK,
	EFFECT_CHANGE_LIFE_TYPE_PHY_HARDATK,	
	EFFECT_CHANGE_LIFE_TYPE_PHY_DEF,
	EFFECT_CHANGE_LIFE_TYPE_SKILL_ATK,
	EFFECT_CHANGE_LIFE_TYPE_SKILL_HARDATK,
	EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_ADD,	
	EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS,
	EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST,	
	EFFECT_CHANGE_LIFE_TYPE_USE_ITEM,
	EFFECT_CHANGE_LIFE_TYPE_USE_SKILL,
	EFFECT_CHANGE_LIFE_TYPE_CHG_MAX,
	EFFECT_CHANGE_LIFE_TYPE_PROTECTED,
};*/

enum BATTLE_RESULT_TYPE
{
	BATTLE_RESULT_TYPE_LOSE = ID_NONE,  //平局
	BATTLE_RESULT_TYPE_WIN,
	BATTLE_RESULT_TYPE_DRAW,
};

enum SKILL_TYPE
{
	SKILL_TYPE_NONE,
	SKILL_TYPE_ATTACK  = 1,			//主动攻击
	SKILL_TYPE_PASSIVE = 2,			//被动技能
};


enum PROFESSION_TYPE
{
	PROFESSION_TYPE_INVALID = ID_NONE,
	PROFESSION_TYPE_SOLDIER,		//战士
	PROFESSION_TYPE_MAGE,			//法师	
	PROFESSION_TYPE_ASN,			//刺客
};

enum BATTLE_GROUP_STATUS
{
	BATTLE_GROUP_STATUS_NONE		= ID_NONE,
	BATTLE_GROUP_STATUS_DOUBLE_EXP	= 0x01,		//双倍经验
	BATTLE_GROUP_STATUS_CAMP		= 0x02,		//战争状态
};


enum ELITE_MONSTER_STATE
{
	ELITE_MONSTER_STATE_DEAD	= ID_NONE,	// 死亡
	ELITE_MONSTER_STATE_NORMAL,				// 正常
	ELITE_MONSTER_STATE_BATTLE,				// 战斗中
};

enum EMONEY_INOUT_TYPE
{
	// xyol_emoney_water
	EMONEY_INOUT_TYPE_BOOTH = 1,				//摆摊
	EMONEY_INOUT_TYPE_TRADE,					//交易
	EMONEY_INOUT_TYPE_PM_AWARD,					//PM命令
	EMONEY_INOUT_TYPE_TASK,						//任务奖励
	EMONEY_INOUT_TYPE_MAIL,						//邮件
	EMONEY_INOUT_TYPE_BUY_BAG,					//购买增值背包
	EMONEY_INOUT_TYPE_BUY_STORAGE,				//开放仓库
	EMONEY_INOUT_TYPE_AUCTION,					//拍卖
	EMONEY_INOUT_TYPE_SHOP,						//商店
	EMONEY_INOUT_TYPE_BATTLE,					//战斗奖励
	EMONEY_INOUT_TYPE_REPUTE,					//捐献换声望,领取军饷
	EMONEY_INOUT_TYPE_LIFE_SKILL,				//生活技能
	EMONEY_INOUT_TYPE_SYN_STORAGE,				//军团捐献
	EMONEY_INOUT_TYPE_BOX,						//礼包获得
	EMONEY_INOUT_TYPE_CARD,						//充值卡
	EMONEY_INOUT_TYPE_RACE,						//擂台赛报名费
	
	// emoney
	EMONEY_TYPE_GIVE_SOURCE		= 1,			//Id_Source获得魔石点数(赠送)
	EMONEY_TYPE_GIVE_TARGET		= 2,			//Id_Target获得魔石点数(赠送)
	EMONEY_TYPE_SOURCE_EXPENSE	= 3,			//Id_Source消费魔石点数
	EMONEY_TYPE_TRADE			= 4,			//游戏内交易
	EMONEY_TYPE_CARD1			= 6,			//从card表领取270点卡(表示商城充值)
	EMONEY_TYPE_CARD2			= 8,			//从card2表领取1380点卡(表示商城充值)
	EMONEY_TYPE_CARD3			= 9,			//手机总值
};

enum TASK_TYPE
{
	TASK_TYPE_NORMAL,			// 普通任务
	TASK_TYPE_CAMP,				// 阵营任务
	TASK_TYPE_SYNDICATE,		// 军团任务
	TASK_TYPE_HU_BIAO,			// 护镖任务
};

enum TASK_STATE
{
	TASK_STATE_NONE				= 0,
	TASK_STATE_AVAILABLE		= 1,	// 任务可接
	TASK_STATE_UNCOMPLETE		= 2,	// 任务已接未完成
	TASK_STATE_COMPLETE			= 3,	// 任务已接已完成
	TASK_STATE_NOT_AVAILABLE	= 4,	// 任务未做不可接 (前置任务未完成,等级差5级以上)
	TASK_STATE_NUM_NOT_ALLOWED	= 5,	// 任务已做不可接, 次数限制
	TASK_STATE_LV_NOT_ALLOWED	= 6,	// 任务未做不可接, 等级限制在5级以内
	TASK_SUMMIT					= 7,	// 任务已交
};

enum NPC_STATE
{
	NPC_STATE_NO_MARK					= 0x01,	// 无任何符号
	NPC_STATE_GRAY_EXCALMATORY_MARK		= 0x02,	// 灰色叹号
	NPC_STATE_COLOR_EXCALMATORY_MARK	= 0x04,	// 彩色叹号
	NPC_STATE_GRAY_QUESTION_MARK		= 0x08,	// 灰色问号
	NPC_STATE_COLOR_QUESTION_MARK		= 0x10,	// 彩色问号
	NPC_STATE_DEAD						= 0x20,	// 死亡
	NPC_STATE_NORMAL					= 0x40, // 正常
	NPC_STATE_BATTLE					= 0x80,	// 战斗
};

enum AWARD_FLAG
{
	AWARD_NONE = 0,			// 不奖励
	AWARD_MULTIPLE = 1,		// 奖励多件
	AWARD_SELECT_ONE = 2,	// 选择一件
};

enum PET_QUALITY
{
	PET_QUALITY_NONE			= ID_NONE,
	PET_QUALITY_NORMAL			= 5,	//普通
	PET_QUALITY_EXCELL			= 6,	//优秀
	PET_QUALITY_RARE			= 7,	//稀有
	PET_QUALITY_PERFECT			= 8,	//完美
	PET_QUALITY_LEGEND			= 9,	//传说
};

enum BILLBOARD_TYPE
{
	PLAYER_LEVEL	= 1,	// 玩家等级排行榜
	PLAYER_HONOUR	= 2,	// 荣誉排行榜
	BILL_TYPE_ALL_FIRST,	// 状元榜
	BILL_TYPE_LAST_CPT,		// 英雄榜//上届比赛排行
};

enum PAY_TYPE
{
	PAY_TYPE_MONEY	= ID_NONE,	//银两支付
	PAY_TYPE_EMONEY,			//代币支付
};

enum DROP_TYPE
{
	DROP_TYPE_NONE,				//不掉落
	DROP_TYPE_PRIVATE,			//指定掉落
	DROP_TYPE_NUM	 = 8,		//按数量掉落
	DROP_TYPE_COMMON = 9,		//通用掉落
};

enum NPC_DYNA_FLAG
{
	NPC_DYNA_FLAG_NONE = ID_NONE,	
	NPC_DYNA_FLAG_DYNA = 9,			//动态
};

enum WEEK_DAY
{
	WEEK_SUNDAY,		// 星期天
	WEEK_MONDAY,		// 星期一
	WEEK_TUESDAY,		// 星期二
	WEEK_WEDNESDAY,		// 星期三
	WEEK_THURSDAY,		// 星期四
	WEEK_FRIDAY,		// 星期五
	WEEK_SATURDAY,		// 星期六
};

enum WEAPON_REQUIRE
{
	WEAPON_REQUIRE_NONE,
	WEAPON_REQUIRE_KNIFE,
	WEAPON_REQUIRE_SWORD,
	WEAPON_REQUIRE_DAGGER,
	WEAPON_REQUIRE_BOW,
	WEAPON_REQUIRE_GUN,
	WEAPON_REQUIRE_BATTLE,
};

enum SKILL_ATK_TYPE {
	SKILL_ATK_TYPE_NONE				= 0x00,
	SKILL_ATK_TYPE_NEAR				= 0x01,		//近身 1
	SKILL_ATK_TYPE_REMOTE			= 0x02,		//远程 2
	SKILL_ATK_TYPE_ENEMY			= 0x04,		//施法目标敌人 4
	SKILL_ATK_TYPE_SELF				= 0x08,		//施法目标自己 8
	SKILL_ATK_TYPE_FRIEND			= 0x0010,   //施法目标队友 16
	SKILL_ATK_TYPE_MAGIC			= 0x0020,   //魔法攻击 32
	SKILL_ATK_TYPE_PHY				= 0x0040,	//物理攻击 64
	SKILL_ATK_TYPE_AREA				= 0x0080,	//固定伤害 128
	SKILL_ATK_TYPE_RESTORE			= 0x0100,	//回复 256
	SKILL_ATK_TYPE_ROHS				= 0x0200,	//封印 512
	SKILL_ATK_TYPE_PLUS				= 0x0400,	//增益 1024
	SKILL_ATK_TYPE_DEBUFF			= 0x0800,	//减益 2048
};

enum MSGDIALOG {
	MSGDIALOG_TEXT = 1,
	MSGDIALOG_LINK = 2,
	MSGDIALOG_EDIT = 3,
	MSGDIALOG_PIC = 4,
	// data : npc face
	MSGDIALOG_PWDEDIT = 5, // 密码框
	MSGDIALOG_MENUBUTTON = 6, // 按钮
	MSGDIALOG_LISTLINE = 7,
	MSGDIALOG_DLG = 8, // 弹出对话框
	MSGDIALOG_USER_OPEN_DLG = 9, // 打开一个界面
	MSGDIALOG_CREATE = 100,
	MSGDIALOG_ANSWER = 101,
	MSGDIALOG_TASKID = 102,
	MSGDIALOG_MAGIC_EFFECT = 103,
	MSGDIALOG_TITLE = 104,
	MSGDIALOG_LEAVE = 105,
	MSGDIALOG_NO_TALK = 110,
	MSGDIALOG_LINK_EX = 106,//选项（带提示箭头）
	MSGDIALOG_CREATE_EX = 107,//结束（无离开按钮）
};

enum MAPTYPE {
	MAPTYPE_NORMAL = 0x00,
	MAPTYPE_PKFIELD = 0x01, // PK区域
	MAPTYPE_CHGMAP_DISABLE = 0x02, // 不能允许切换地图
	MAPTYPE_RECORD_DISABLE = 0x04, // 不能作为记录点
	MAPTYPE_PK_DISABLE = 0x08, // 不允许PK
	MAPTYPE_BOOTH_ENABLE = 0x0010, // 允许摆摊
	MAPTYPE_BATTLE_DISABLE = 0x0020, // 不允许战斗
	MAPTYPE_TELEPORT_DISABLE = 0x0040,
	MAPTYPE_SYN_MAP = 0x0080,
	MAPTYPE_PRISON_MAP = 0x0100,
	MAPTYPE_FLY_DISABLE = 0x0200, // 不允许飞行
	MAPTYPE_DRIVE_DISABLE = 0x0400, // 不允许骑马
	MAPTYPE_MINEFIELD = 0x0800,
	MAPTYPE_CALLNEWBIE_DISABLE = 0x00001000,
	MAPTYPE_REBORN_NOW_ENABLE = 0x00002000,
	MAPTYPE_NEWBIE_PROTECT = 0x00004000,
	MAPTYPE_OFFLINE_TRAINING_DISABLE = 0x00008000,
	MAPTYPE_NOT_INVITE = 0x00010000,
	MAPTYPE_FAMILY_BATTLE_FIELD = 0x00020000,
	MAPTYPE_MOUNT_DISABLE = 0x00040000,
	MAPTYPE_NOT_INVITE_TO_FAMILY_BATTLE = 0x00080000,
	MAPTYPE_NO_TREASURE_HUNT = 0x00200000, //不允许寻宝
};

enum {
	MAX_MONEY = 200000000,
	MAX_EMONEY = 200000000,
};

enum BOOTH {
	BOOTH_QUEST = 0, // 查询摊位
	BOOTH_BUY = 1, // 购买
	BOOTH_SUCCESS = 2, // 购买
	BOOTH_REPEAL = 3, // 撤消摆摊
	BOOTH_ADD = 4, // 增加摊位
};

enum BATTLE_EFFECT_TYPE
{
	BATTLE_EFFECT_TYPE_NONE = ID_NONE,
	// 发起
	BATTLE_EFFECT_TYPE_ATK,   // 攻击
	BATTLE_EFFECT_TYPE_SKILL,  // 技能
	
	BATTLE_EFFECT_TYPE_SKILL_TARGET,// 技能ID
	BATTLE_EFFECT_TYPE_LIFE,  // 生命
	BATTLE_EFFECT_TYPE_MANA,  // 气势
	BATTLE_EFFECT_TYPE_DODGE,  // 闪避
	BATTLE_EFFECT_TYPE_DRITICAL, // 暴击
	BATTLE_EFFECT_TYPE_BLOCK,  // 格挡
	BATTLE_EFFECT_TYPE_COMBO,	//连击
	BATTLE_EFFECT_TYPE_STATUS_ADD, // 加状态
	BATTLE_EFFECT_TYPE_STATUS_LOST, // 取消状态
	BATTLE_EFFECT_TYPE_STATUS_LIFE, // 状态去血
	BATTLE_EFFECT_TYPE_DEAD,
	
	EFFECT_TYPE_TURN_END,   // 回合结束
	EFFECT_TYPE_BATTLE_BEGIN,  // 战斗开始
	EFFECT_TYPE_BATTLE_END,   // 战斗结束
};

#endif // end of define _GS_ENUM_DEFINE_H_
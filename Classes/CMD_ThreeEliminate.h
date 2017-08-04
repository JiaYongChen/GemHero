#ifndef CMD_CHESS_HEAD_FILE
#define CMD_CHESS_HEAD_FILE
#include "cocos2d.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//编译版本
#define GAME_BUILD_VERSION			1									//游戏编译版本号

#define KIND_ID						404									//游戏 I D
#define GAME_PLAYER					2									//游戏人数
#define GAME_NAME					TEXT("宝石英雄")			//游戏名字
#define WINDOWS_WIDTN			1000 //窗口的宽
#define WINDOWS_HEIGHT			700 //窗口的高


#define  ROW_COUNT			9			//列
#define  COL_COUNT			9			//行

#define  ALL_COUNT			(ROW_COUNT)*(COL_COUNT)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//游戏定义

#define NORMAL_GEM_SCORE  15
#define SPECIAL_GEM_SCORE  30

#define MAX_LEVELNUMBER 100	//最大关卡数目
#define MAX_STRENGTH 30				//最大体力值
#define MAX_TOTALLISTNUM 100	//总排行榜最大数目
#define MAX_LEVELLISTNUM 10	//关卡排行榜最大数目
#define ADD_STRENGTH_TIME	6	//增加体力消耗的时间（分钟）
#define LOST_STRENGTH 5		//消耗体力
#define SP_LOST_STRING 15		//特殊关卡消耗体力

#define  MINUTES_NUMBER  60

//关卡运行类型
enum LevelStartType
{
	LEVELSTART_NULL = -1,
	LEVELSTART_STEP = 0,
	LEVELSTART_TIME = 1,
	LEVELSTART_SETPANDTIME = 2
};

//游戏类型
enum LevelGameType
{
	LEVELGAME_NULL = 0,
	LEVELGAME_TIME = 1,
	LEVELGAME_COLLECT = 2,
	LEVELGAME_SLYDRIS = 3,
	COUNT_LEVELGAME_TYPE
};

//父类型
enum ParentType
{
	NULL_PARENT_TYPE = -1,
	NORMAL_GEM_PARENT_TYPE = 0,
	SPECIAL_GEM_PARENT_TYPE = 1,
	STUMBLING_BLOCK_PARENT_TYPE = 2,
	MAP_PARENT_TYPE = 3,
	COUNT_PARENT_TYPE
};

//宝石颜色
enum GemColor
{
	GEM_COLOR_NO = -1,
	GEM_COLOR_RED = 0,
	GEM_COLOR_GREEN = 1,
	GEM_COLOR_BLUE = 2,
	GEM_COLOR_CYAN = 3,
	GEM_COLOR_YELLOW = 4,
	GEM_COLOR_PURPLE = 5,
	GEM_COLOR_COUNT
};

//宝石状态
enum GemStatus
{
	GEM_STATUS_NULL = -1,
	GEM_STATUS_NORMAL = 0,
	GEM_STATUS_VERTICAL = 1,
	GEM_STATUS_HORIZONTAL = 2,
	GEM_STATUS_HV = 3,
	GEM_STATUS_BOM = 4,
	GEM_STATUS_COLORFUL = 5,
	GEM_STATUS_CHANGE = 6,
	GEM_STATUS_TIME_BOM = 7,
	GEM_STATUS_ADD_STEP = 8,
	GEM_STATUS_TREASURE_CHEST = 9,	//宝箱
	GEM_STATUS_COUNT
};

//非宝石物品
enum StumblingBlock
{
	STUMBLING_BLOCK_NULL = -1,
	STUMBLING_BLOCK_GOLD_POD = 0,			//金豆荚
	STUMBLING_BLOCK_PARTITION = 1,			//隔板
	STUMBLING_BLOCK_GLASS = 2,					//玻璃
	STUMBLING_BLOCK_WOOD = 3,					//木板
	STUMBLING_BLOCK_BULB = 4,						//毛球
	STUMBLING_BLOCK_STENCIL = 5,					//钢网
	STUMBLING_BLOCK_JELLY = 6,						//果冻
	STUMBLING_BLOCK_SILVER_COIN = 7,			//银币
	STUMBLING_BLOCK_PORTAL = 8,					//传送门
	STUMBLING_BLOCK_COUNT
};

enum enDirection
{
	CENT_DIRECTION = 0, //中间
	UP_DIRECTION = 1,	//上
	DOWN_DIRECTION = 2,	//下
	LEFT_DIRECTION = 3,	//左
	RIGHT_DIRECTION= 4	//右
};

//关卡进行时的状态
enum GamePlayingStatus
{
	GAME_PLAYING_STATUS_NORMAL = 0,
	GAME_PLAYING_STATUS_SWAP_GEM = 1,
	GAME_PLAYING_STATUS_SHOW_ANIMATION = 2,
	GAME_PLAYING_STATUS_MOVE = 3,
	GAME_PLAYING_STATUS_SPECIAL = 4,
	GAME_PLAYING_STATUS_COLORFUL
};

//关卡游戏时的状态
enum LevelsGameStatus
{
	GAME_STATUS_NULL = -1,
	GAME_STATUS_PLAYING = 0,		//正常游戏
	GAME_STATUS_USE_PROP,			//使用道具
	GAME_STATUS_BONUS_TIME,		//BONUSTIME
	GAME_STATUS_GAME_END,
	GAME_STATUS_END
};

//宝石交换类型
enum swapGemStatus
{
	NULL_SWAP_STATUS = 0,
	NORMAL_SWAP_STATUS = 1,
	SPECIAL_SWAP_STATUS=2
};

//游戏结束类型
enum gameEndStatus
{
	NO_GAME_END = 0,
	NO_STEPORTIME_GAME_END = 1,
	SCORES_LESS_THAN_GAME_END = 2,
	NO_THREE_SAME_COLOR_GEM,
	TIME_BOMB_GAME_END,
	SUCCESS_GAME_END
};

//格子状态
enum enBrickType
{
	NULL_TYPE = -1,
	NO_GEM_AND_BRICK_TYPE = 0,	//没有宝石和障碍
	GEM_AND_BRICK_TYPE = 1,			//宝石障碍都存在
	GEM_TYPE = 2,								//只有宝石
	BRICK_TYPE = 3							//只有障碍
};

//商店类型
enum enShopType
{
	NULL_SHOP_TYPE = -1,
	HOT_SHOP_TYPE = 0,			//热销商品
	DISCOUNT_SHOP_TYPE,		//折扣商品
	ALL_SHOP_TYPE					//所有商品
};

//商店道具
enum enProp
{
	PROP_NULL = -1,

	PROP_Change = 0,	//变换
	PROP_Bomb = 1,	//炸弹
	PROP_Hammer = 2,	//铁锤
	PROP_SwapGem = 3,	//强制交换位置
	PROP_SameColor = 4,	//消除同色宝石
	PROP_AddStep = 5,		//增加5步
	PROP_Det = 6,	//引爆器
	PROP_Back = 7,	//返回上一步
	PROP_Rearrange = 8,	//重新排列
	PROP_Strength = 9,	//增加体力

	PROP_NUMBER
};
//////////////////////////////////////////////////////////////////////////////////


//Clicent Struct Data
////////////////////////////////////////////////////////////////////////////
//宝石棋盘内数据类

enum enRunAction
{
	NORMAL_ACTION,
	DISAPPERA_ACTION,
	MOVE_ACTION,
};

//基础数据
struct tagBaseData
{
	int id;
	int indexX;
	int indexY;
	ParentType parentType;
	int childType;
	bool isShow;
	enRunAction moveAction;

	bool operator==(const tagBaseData& target) const throw()
	{
		return (target.childType == childType && (target.indexX == indexX && target.indexY == indexY) && (target.parentType == parentType));
	}

	tagBaseData() {
		clear();
	}

	virtual void clear() {
		id = -1;
		indexX = -1;
		indexY = -1;
		parentType = NULL_PARENT_TYPE;
		childType = -1;
		isShow = false;
		moveAction = NORMAL_ACTION;
	}
};
typedef std::vector<tagBaseData> BaseData;
typedef std::vector<tagBaseData*> PBaseData;

//地图形状
struct tagMapData : public tagBaseData
{
	bool collect;

	virtual void clear() {
		tagBaseData::clear();
		collect = false;
	}
};
typedef std::vector<tagMapData> MapData;

//宝石
struct tagGemData : tagBaseData
{
	//GemColor color;		//宝石颜色
	GemStatus gemStatus;		//宝石状态
	bool bMove;	//是否可以移动
	unsigned char attribute;
	bool operator==(const tagGemData& target) const throw()
	{
		return (target.childType == childType && (target.indexX == indexX && target.indexY == indexY) && (target.gemStatus == gemStatus));
	}

	virtual void clear() {
		tagBaseData::clear();
		gemStatus = GEM_STATUS_NORMAL;
		bMove = true;
		attribute = -1;
	}
};
typedef std::vector<tagGemData> GemData;

//特殊障碍
struct tagSpecialBrickData : public tagBaseData
{
//	StumblingBlock type;//	<type>12</type>
	int score;//	<score>1000</score>
	int number;//	<number>0</number>
	unsigned char direction;	//0.中间 1.上 2.下 3.左 4.右
	unsigned char attribute;	//传送门的对称位置
	bool bMove;//move

	bool operator==(const tagSpecialBrickData& target) const throw()
	{
		return (target.childType == childType && target.direction == direction && (target.indexX == indexX && target.indexY == indexY));
	}

	virtual void clear() {
		tagBaseData::clear();
		score = 0;
		number = 0;
		direction = 0;
		attribute = -1;
		bMove = false;
	}
};
typedef std::vector<tagSpecialBrickData> SpecialBrickData;

//砖块
struct TileBrickData : public tagBaseData
{
	tagMapData map;
	tagGemData gem;
	SpecialBrickData specialBrick;
};
typedef std::vector<TileBrickData> TileBrick;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//读取关卡数据

//关卡内出现的宝石类型
struct tagLevelsGem
{
	int ID;
	GemColor color;
	bool operator==(tagLevelsGem target) const throw()
	{
		return (target.color == color);
	}
};
typedef std::vector<tagLevelsGem>	LevelsGem;

//关卡失败条件
struct  tagRulesData
{
	int num;		//关卡数字
	LevelStartType type;		//类型
	LevelGameType gameType;	//游戏类型
	int step;		//步数
	int time;		//时间
};

//通关目标
struct tagTargetData
{
	int ID;
	ParentType parentType;
	int type;
	int number;

	bool operator==(const tagTargetData &target) const throw()
	{
		return (target.parentType == parentType && target.type == type);
	}
};
typedef std::vector<tagTargetData> TargetData;

//关卡获取星星的分数
struct tagStartScore
{
	int maxScore;
	int score_1;
	int score_2;
	int score_3;
};
////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//读取关卡数据
///////////////////////////////////////////////////////////////////////////////////
struct ImageData 
{
	int _type;
	int _num;
	int _attribute;
	std::string _imageName;
};

struct tileImageInfo
{
	int _posX;
	int _posY;
	std::vector<ImageData> _imageInfo;
};

typedef std::vector<tileImageInfo>	TileImageInfo;

struct ObtainSpecialGemInfo
{
	cocos2d::Vec2 point;
	GemStatus flag;

	bool operator==(ObtainSpecialGemInfo pPoint) const throw()
	{
		return (pPoint.point == point);
	}
};

struct changeGemColour
{
	cocos2d::Vec2 point;
	GemColor color;

	bool operator == (const changeGemColour& pGemColour) const throw()
	{
		return (pGemColour.point == point);
	}
};
typedef std::vector<changeGemColour> ChangeGemColour;

struct moveSpecialBrick
{
	cocos2d::Vec2 oldPoint;
	cocos2d::Vec2 newPoint;
	StumblingBlock type;
};

typedef std::vector<moveSpecialBrick> MoveSpecialBrick;

struct tagShopArticleInfo
{
	enProp ID;		//物品ID
	int score;	//物品价格
	int discount;		//折扣 ,100不打折
	bool isHot;		//是否热销产品
};

#define SafeErase(pObject) {if (!pObject.empty()) { pObject.erase(pObject.begin(),pObject.end()); }}
//////////////////////////////////////////////////////////////////////////
enum enServerSubCmd
{
	enServerSubCmd_Begin = 1000,

	SUB_S_LEVEL_DATA,					//关卡数据
	SUB_S_GAME_MAP,					//地图
	SUB_S_GAME_GEM,					//关卡宝石
	SUB_S_GAME_SPECIAL,				//关卡障碍
	SUB_S_GAME_TARGET,				//关卡获胜条件
	SUB_S_GAME_INIT_GEM,			//关卡初始宝石
	SUB_S_GAME_START,				//开始关卡游戏
	SUB_S_GAME_PROP,					//关卡中可以使用的道具及数量
	SUB_S_TOTAL_LIST,					//总排行数据
	SUB_S_LEVEL_LIST,					//关卡排行榜数据
	SUB_S_BAG_DATA,					//背包数据
	SUB_S_SHOP_DATA,					//商店数据
	SUB_S_LEVEL_SCORE_STAR,		//每个关卡的分数和星星数
	SUB_S_BUY_PROP,					//购买道具结果
	SUB_S_USE_PROP,						//使用道具结果
	SUB_S_CHANGE_PROP_NUM,	//改变道具数目
	SUB_S_SHOW_STRENGTH,			//显示增加体力
	SUB_S_ADD_STRENGTH,			//增加体力
	SUB_S_ADD_STEP,				//增加步数
	SUB_S_GAME_END,				//结束得分信息
	SUB_S_REWARDCH,					//奖励

	CMD_SC_MATCH_OVER_KICK,						//比赛结束离开座位

	enServerSubCmd_end
};

//游戏开始
struct CMD_S_GameStart
{
	unsigned short wCurrentUser;	//当前玩家
	int levelID;	//关卡ID
	unsigned char strength; //剩余体力
	bool isCan;	//false 体力不足，true 开始
};

//关卡数据
struct CMD_S_LevelData
{
	int levelNum;
	unsigned char flag;		//0.true 1.2. false
	unsigned char starNumber;
	tagRulesData ruleData;
	tagStartScore starScore;
};

//购买道具
struct CMD_S_BuyProp
{
	unsigned short	wCurrentUser;					//当前玩家
	bool flag;		//true 购买成功；false 购买失败
	long long IScore;	//购买后剩余金钱
};

struct CMD_S_UseProp
{
	unsigned short						wCurrentUser;					//当前玩家
	bool flag;		//true 成功；false 失败
	int type;
	int number;
	int addInfo;
	long long IScore;
};
//某人没棋
struct CMD_S_ChangePropNUM
{
	unsigned short wCurrentUser;					//当前玩家
	int type;
	int num;
};
//结束关卡游戏
struct CMD_S_GameEnd
{
	unsigned short wCurrentUser; //当前玩家
	int levelID; //关卡ID
	unsigned char starNumber; //星星数
	int score; //分数
	int countStar;	//总星星数
	int currentLevel;//当前所处关卡
	int unlockLevelNum;//解锁的关卡数
};
//奖励
struct CMD_S_RewardCh
{
	int type; //类型
	int number;//数量
};

//显示增加体力
struct CMD_S_ShowAddStrength
{
	unsigned short wCurrentUser;
	int strength;
};

//增加体力
struct CMD_S_AddStrength
{
	unsigned short wCurrentUser;
	int type; //1自动增加，2购买体力 3 道具增加
	bool flag;
	int number;
	int propNum;
};

//增加步数
struct CMD_S_AddStep
{
	unsigned short wCurrentUser;
	bool flag;
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
enum enClientSubCmd
{
	enClientSubCmd_Begin = 2000,

	SUB_C_LevelData,						//请求关卡信息
	SUB_C_GameStart,					//开始关卡游戏
	SUB_C_BagData,						//请求背包数据
	SUB_C_TotalList,						//请求总排行榜
	SUB_C_LevelList,						//请求关卡排行
	SUB_C_WriteLevelScore,			//记录分数
	SUB_C_Shop,								//商店信息
	SUB_C_BuyProp,						//购买道具
	SUB_C_UseProp,						//使用道具
	SUB_C_ShowAddStrength,			//显示增加体力
	SUB_C_AddStrength,					//增加体力
	SUB_C_AddStep,				//增加步数

	enClientSubCmd_End
};

//关卡信息
struct CMD_C_LevelData
{
	unsigned short wChairID;
	int levelID;			//关卡ID
};

//开始关卡游戏
struct CMD_C_GameStart 
{
	unsigned short wChairID;
	int levelID;			//关卡ID
};

//背包信息
struct CMD_C_BagData
{
	unsigned short wChairID;
};

//总排行榜信息
struct CMD_C_TotalList
{
	unsigned short wChairID;
};

//商店信息
struct  CMD_C_ShopData
{
	unsigned short wChairID;
	enShopType type;
};

//写入分数
struct CMD_C_WriteLevelScore
{
	unsigned short wChairID;
	int levelID;
	unsigned char starNum;
	int score;
};

//购买道具
struct tagBuyPropInfo
{
	int type;
	int number;
};

typedef std::vector<tagBuyPropInfo> BuyProp;

//使用道具
struct CMD_C_UseProp 
{
	unsigned short wChairID;
	int type;
};

//显示增加体力
struct CMD_C_ShowAddStrength
{
	unsigned short wChairID;
};

//增加体力
struct CMD_C_AddStrength 
{
	unsigned short wChairID;
	int flag; //1  自动增加， 2 购买体力 3 道具增加
	int number;
};

struct CMD_C_AddStep 
{
	unsigned short wChairID;
};
//////////////////////////////////////////////////////////////////////////
#endif
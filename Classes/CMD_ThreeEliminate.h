#ifndef CMD_CHESS_HEAD_FILE
#define CMD_CHESS_HEAD_FILE
#include "cocos2d.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//�����궨��

//����汾
#define GAME_BUILD_VERSION			1									//��Ϸ����汾��

#define KIND_ID						404									//��Ϸ I D
#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("��ʯӢ��")			//��Ϸ����
#define WINDOWS_WIDTN			1000 //���ڵĿ�
#define WINDOWS_HEIGHT			700 //���ڵĸ�


#define  ROW_COUNT			9			//��
#define  COL_COUNT			9			//��

#define  ALL_COUNT			(ROW_COUNT)*(COL_COUNT)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ����

#define NORMAL_GEM_SCORE  15
#define SPECIAL_GEM_SCORE  30

#define MAX_LEVELNUMBER 100	//���ؿ���Ŀ
#define MAX_STRENGTH 30				//�������ֵ
#define MAX_TOTALLISTNUM 100	//�����а������Ŀ
#define MAX_LEVELLISTNUM 10	//�ؿ����а������Ŀ
#define ADD_STRENGTH_TIME	6	//�����������ĵ�ʱ�䣨���ӣ�
#define LOST_STRENGTH 5		//��������
#define SP_LOST_STRING 15		//����ؿ���������

#define  MINUTES_NUMBER  60

//�ؿ���������
enum LevelStartType
{
	LEVELSTART_NULL = -1,
	LEVELSTART_STEP = 0,
	LEVELSTART_TIME = 1,
	LEVELSTART_SETPANDTIME = 2
};

//��Ϸ����
enum LevelGameType
{
	LEVELGAME_NULL = 0,
	LEVELGAME_TIME = 1,
	LEVELGAME_COLLECT = 2,
	LEVELGAME_SLYDRIS = 3,
	COUNT_LEVELGAME_TYPE
};

//������
enum ParentType
{
	NULL_PARENT_TYPE = -1,
	NORMAL_GEM_PARENT_TYPE = 0,
	SPECIAL_GEM_PARENT_TYPE = 1,
	STUMBLING_BLOCK_PARENT_TYPE = 2,
	MAP_PARENT_TYPE = 3,
	COUNT_PARENT_TYPE
};

//��ʯ��ɫ
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

//��ʯ״̬
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
	GEM_STATUS_TREASURE_CHEST = 9,	//����
	GEM_STATUS_COUNT
};

//�Ǳ�ʯ��Ʒ
enum StumblingBlock
{
	STUMBLING_BLOCK_NULL = -1,
	STUMBLING_BLOCK_GOLD_POD = 0,			//�𶹼�
	STUMBLING_BLOCK_PARTITION = 1,			//����
	STUMBLING_BLOCK_GLASS = 2,					//����
	STUMBLING_BLOCK_WOOD = 3,					//ľ��
	STUMBLING_BLOCK_BULB = 4,						//ë��
	STUMBLING_BLOCK_STENCIL = 5,					//����
	STUMBLING_BLOCK_JELLY = 6,						//����
	STUMBLING_BLOCK_SILVER_COIN = 7,			//����
	STUMBLING_BLOCK_PORTAL = 8,					//������
	STUMBLING_BLOCK_COUNT
};

enum enDirection
{
	CENT_DIRECTION = 0, //�м�
	UP_DIRECTION = 1,	//��
	DOWN_DIRECTION = 2,	//��
	LEFT_DIRECTION = 3,	//��
	RIGHT_DIRECTION= 4	//��
};

//�ؿ�����ʱ��״̬
enum GamePlayingStatus
{
	GAME_PLAYING_STATUS_NORMAL = 0,
	GAME_PLAYING_STATUS_SWAP_GEM = 1,
	GAME_PLAYING_STATUS_SHOW_ANIMATION = 2,
	GAME_PLAYING_STATUS_MOVE = 3,
	GAME_PLAYING_STATUS_SPECIAL = 4,
	GAME_PLAYING_STATUS_COLORFUL
};

//�ؿ���Ϸʱ��״̬
enum LevelsGameStatus
{
	GAME_STATUS_NULL = -1,
	GAME_STATUS_PLAYING = 0,		//������Ϸ
	GAME_STATUS_USE_PROP,			//ʹ�õ���
	GAME_STATUS_BONUS_TIME,		//BONUSTIME
	GAME_STATUS_GAME_END,
	GAME_STATUS_END
};

//��ʯ��������
enum swapGemStatus
{
	NULL_SWAP_STATUS = 0,
	NORMAL_SWAP_STATUS = 1,
	SPECIAL_SWAP_STATUS=2
};

//��Ϸ��������
enum gameEndStatus
{
	NO_GAME_END = 0,
	NO_STEPORTIME_GAME_END = 1,
	SCORES_LESS_THAN_GAME_END = 2,
	NO_THREE_SAME_COLOR_GEM,
	TIME_BOMB_GAME_END,
	SUCCESS_GAME_END
};

//����״̬
enum enBrickType
{
	NULL_TYPE = -1,
	NO_GEM_AND_BRICK_TYPE = 0,	//û�б�ʯ���ϰ�
	GEM_AND_BRICK_TYPE = 1,			//��ʯ�ϰ�������
	GEM_TYPE = 2,								//ֻ�б�ʯ
	BRICK_TYPE = 3							//ֻ���ϰ�
};

//�̵�����
enum enShopType
{
	NULL_SHOP_TYPE = -1,
	HOT_SHOP_TYPE = 0,			//������Ʒ
	DISCOUNT_SHOP_TYPE,		//�ۿ���Ʒ
	ALL_SHOP_TYPE					//������Ʒ
};

//�̵����
enum enProp
{
	PROP_NULL = -1,

	PROP_Change = 0,	//�任
	PROP_Bomb = 1,	//ը��
	PROP_Hammer = 2,	//����
	PROP_SwapGem = 3,	//ǿ�ƽ���λ��
	PROP_SameColor = 4,	//����ͬɫ��ʯ
	PROP_AddStep = 5,		//����5��
	PROP_Det = 6,	//������
	PROP_Back = 7,	//������һ��
	PROP_Rearrange = 8,	//��������
	PROP_Strength = 9,	//��������

	PROP_NUMBER
};
//////////////////////////////////////////////////////////////////////////////////


//Clicent Struct Data
////////////////////////////////////////////////////////////////////////////
//��ʯ������������

enum enRunAction
{
	NORMAL_ACTION,
	DISAPPERA_ACTION,
	MOVE_ACTION,
};

//��������
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

//��ͼ��״
struct tagMapData : public tagBaseData
{
	bool collect;

	virtual void clear() {
		tagBaseData::clear();
		collect = false;
	}
};
typedef std::vector<tagMapData> MapData;

//��ʯ
struct tagGemData : tagBaseData
{
	//GemColor color;		//��ʯ��ɫ
	GemStatus gemStatus;		//��ʯ״̬
	bool bMove;	//�Ƿ�����ƶ�
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

//�����ϰ�
struct tagSpecialBrickData : public tagBaseData
{
//	StumblingBlock type;//	<type>12</type>
	int score;//	<score>1000</score>
	int number;//	<number>0</number>
	unsigned char direction;	//0.�м� 1.�� 2.�� 3.�� 4.��
	unsigned char attribute;	//�����ŵĶԳ�λ��
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

//ש��
struct TileBrickData : public tagBaseData
{
	tagMapData map;
	tagGemData gem;
	SpecialBrickData specialBrick;
};
typedef std::vector<TileBrickData> TileBrick;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//��ȡ�ؿ�����

//�ؿ��ڳ��ֵı�ʯ����
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

//�ؿ�ʧ������
struct  tagRulesData
{
	int num;		//�ؿ�����
	LevelStartType type;		//����
	LevelGameType gameType;	//��Ϸ����
	int step;		//����
	int time;		//ʱ��
};

//ͨ��Ŀ��
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

//�ؿ���ȡ���ǵķ���
struct tagStartScore
{
	int maxScore;
	int score_1;
	int score_2;
	int score_3;
};
////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//��ȡ�ؿ�����
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
	enProp ID;		//��ƷID
	int score;	//��Ʒ�۸�
	int discount;		//�ۿ� ,100������
	bool isHot;		//�Ƿ�������Ʒ
};

#define SafeErase(pObject) {if (!pObject.empty()) { pObject.erase(pObject.begin(),pObject.end()); }}
//////////////////////////////////////////////////////////////////////////
enum enServerSubCmd
{
	enServerSubCmd_Begin = 1000,

	SUB_S_LEVEL_DATA,					//�ؿ�����
	SUB_S_GAME_MAP,					//��ͼ
	SUB_S_GAME_GEM,					//�ؿ���ʯ
	SUB_S_GAME_SPECIAL,				//�ؿ��ϰ�
	SUB_S_GAME_TARGET,				//�ؿ���ʤ����
	SUB_S_GAME_INIT_GEM,			//�ؿ���ʼ��ʯ
	SUB_S_GAME_START,				//��ʼ�ؿ���Ϸ
	SUB_S_GAME_PROP,					//�ؿ��п���ʹ�õĵ��߼�����
	SUB_S_TOTAL_LIST,					//����������
	SUB_S_LEVEL_LIST,					//�ؿ����а�����
	SUB_S_BAG_DATA,					//��������
	SUB_S_SHOP_DATA,					//�̵�����
	SUB_S_LEVEL_SCORE_STAR,		//ÿ���ؿ��ķ�����������
	SUB_S_BUY_PROP,					//������߽��
	SUB_S_USE_PROP,						//ʹ�õ��߽��
	SUB_S_CHANGE_PROP_NUM,	//�ı������Ŀ
	SUB_S_SHOW_STRENGTH,			//��ʾ��������
	SUB_S_ADD_STRENGTH,			//��������
	SUB_S_ADD_STEP,				//���Ӳ���
	SUB_S_GAME_END,				//�����÷���Ϣ
	SUB_S_REWARDCH,					//����

	CMD_SC_MATCH_OVER_KICK,						//���������뿪��λ

	enServerSubCmd_end
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	unsigned short wCurrentUser;	//��ǰ���
	int levelID;	//�ؿ�ID
	unsigned char strength; //ʣ������
	bool isCan;	//false �������㣬true ��ʼ
};

//�ؿ�����
struct CMD_S_LevelData
{
	int levelNum;
	unsigned char flag;		//0.true 1.2. false
	unsigned char starNumber;
	tagRulesData ruleData;
	tagStartScore starScore;
};

//�������
struct CMD_S_BuyProp
{
	unsigned short	wCurrentUser;					//��ǰ���
	bool flag;		//true ����ɹ���false ����ʧ��
	long long IScore;	//�����ʣ���Ǯ
};

struct CMD_S_UseProp
{
	unsigned short						wCurrentUser;					//��ǰ���
	bool flag;		//true �ɹ���false ʧ��
	int type;
	int number;
	int addInfo;
	long long IScore;
};
//ĳ��û��
struct CMD_S_ChangePropNUM
{
	unsigned short wCurrentUser;					//��ǰ���
	int type;
	int num;
};
//�����ؿ���Ϸ
struct CMD_S_GameEnd
{
	unsigned short wCurrentUser; //��ǰ���
	int levelID; //�ؿ�ID
	unsigned char starNumber; //������
	int score; //����
	int countStar;	//��������
	int currentLevel;//��ǰ�����ؿ�
	int unlockLevelNum;//�����Ĺؿ���
};
//����
struct CMD_S_RewardCh
{
	int type; //����
	int number;//����
};

//��ʾ��������
struct CMD_S_ShowAddStrength
{
	unsigned short wCurrentUser;
	int strength;
};

//��������
struct CMD_S_AddStrength
{
	unsigned short wCurrentUser;
	int type; //1�Զ����ӣ�2�������� 3 ��������
	bool flag;
	int number;
	int propNum;
};

//���Ӳ���
struct CMD_S_AddStep
{
	unsigned short wCurrentUser;
	bool flag;
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
enum enClientSubCmd
{
	enClientSubCmd_Begin = 2000,

	SUB_C_LevelData,						//����ؿ���Ϣ
	SUB_C_GameStart,					//��ʼ�ؿ���Ϸ
	SUB_C_BagData,						//���󱳰�����
	SUB_C_TotalList,						//���������а�
	SUB_C_LevelList,						//����ؿ�����
	SUB_C_WriteLevelScore,			//��¼����
	SUB_C_Shop,								//�̵���Ϣ
	SUB_C_BuyProp,						//�������
	SUB_C_UseProp,						//ʹ�õ���
	SUB_C_ShowAddStrength,			//��ʾ��������
	SUB_C_AddStrength,					//��������
	SUB_C_AddStep,				//���Ӳ���

	enClientSubCmd_End
};

//�ؿ���Ϣ
struct CMD_C_LevelData
{
	unsigned short wChairID;
	int levelID;			//�ؿ�ID
};

//��ʼ�ؿ���Ϸ
struct CMD_C_GameStart 
{
	unsigned short wChairID;
	int levelID;			//�ؿ�ID
};

//������Ϣ
struct CMD_C_BagData
{
	unsigned short wChairID;
};

//�����а���Ϣ
struct CMD_C_TotalList
{
	unsigned short wChairID;
};

//�̵���Ϣ
struct  CMD_C_ShopData
{
	unsigned short wChairID;
	enShopType type;
};

//д�����
struct CMD_C_WriteLevelScore
{
	unsigned short wChairID;
	int levelID;
	unsigned char starNum;
	int score;
};

//�������
struct tagBuyPropInfo
{
	int type;
	int number;
};

typedef std::vector<tagBuyPropInfo> BuyProp;

//ʹ�õ���
struct CMD_C_UseProp 
{
	unsigned short wChairID;
	int type;
};

//��ʾ��������
struct CMD_C_ShowAddStrength
{
	unsigned short wChairID;
};

//��������
struct CMD_C_AddStrength 
{
	unsigned short wChairID;
	int flag; //1  �Զ����ӣ� 2 �������� 3 ��������
	int number;
};

struct CMD_C_AddStep 
{
	unsigned short wChairID;
};
//////////////////////////////////////////////////////////////////////////
#endif
#pragma once
#include "GamePlayingSceneData.h"
#include "CMD_ThreeEliminate.h"

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
public:
	static CGameLogic& getInstance() {
		static CGameLogic _instance;

		return _instance;
	}

	//函数定义
public:
	//析构函数
	virtual ~CGameLogic();

private:
	//构造函数
	CGameLogic();
	CGameLogic(const CGameLogic&);
	CGameLogic& operator= (const CGameLogic&);
	
	

public:
	//读取关卡数据
	void loadLevelDataFromeXML(int levelsID);
	void loadLevelDataFromeFile(int levelsID);
	bool loadLevelDataFromeData(const CGamePlayingSceneData& pPlayingData);
	//重新开始游戏
	void restartGame();
	//获取当前关卡数字
	int getCurrentGateID(){return m_gateNumber;}
	//获取当前关卡的数据
	std::string getCurrentLevelsData();
	//获取当前地图数据
	const PBaseData& getCurrentMapData();
	//获取障碍
	const PBaseData& getCurrentSpecialBrickData();
	//获取宝石
	const PBaseData& getCurrentGemData();
	//获取
	TileBrick& getCurrentTileBrick();
	//获取过关条件信息
	const TargetData& getSuccessCondition();
	//获取当前关卡的得分条件
	const tagStartScore& getScoreCondition();
	//设置当前关卡的步数
	void setCurrentGateStep(int step){m_currentStep = step;}
	//获取当前关卡步数
	int getCurrentGateStep(){return m_currentStep;}
	//获取关卡目标
	const TargetData& getCurrentSuccessCondition();
	//获取当前关卡获取的分值
	int GetCurrentGateScore();
	//设置游戏状态
	void setGameStatus(LevelsGameStatus status){m_gameStatus = status;}
	//获取游戏状态
	LevelsGameStatus getGameStatus(){return m_gameStatus;}
	//游戏运行状态
	void setGamePlayingStatus(GamePlayingStatus status){m_gamePlayingStatus = status;}
	//获取游戏运行状态
	GamePlayingStatus getGamePlayingStatus(){return m_gamePlayingStatus;}
	//获取当前所有的特殊宝石
	void getAllSpecilGem(std::vector<cocos2d::Vec2> &GemPos);
	//初始化宝石
	void setInitGemList(GemData& gemItem){m_initGemList = gemItem;}
	//获取游戏链表长度
	int getPlayingGameListNumber(){return m_playingGameList.size();}
	//获取横消和竖消宝石个数
	TileBrick getSpecialGem();
	//获取当前连击数
	int getCurrentComboNumber(){return m_currentComboNumber;}
	void setCurrentComboNumber(int combo){m_currentComboNumber = combo;}
	//判断是否可以使用道具
	bool isCanUseProp(int posX,int posY);
	bool isCanUseColorProp();
	//逻辑函数
public:
	//检查移动
	swapGemStatus IsValidMove(unsigned short scoreX, unsigned short scoreY, unsigned short targetX, unsigned short targetY,bool flag = true);  // 0.无效交换；1.普通交换；2.特殊宝石交换
	//消除宝石
	bool checkConnect(std::vector<cocos2d::Vec2 > &ConnectGemPos,bool flag = true);
	//获取特殊宝石
	void ObtainSpecialGems(std::vector<ObtainSpecialGemInfo> &outInfos, std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos);
	//触发特殊效果
	void touchSpecialGems(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos,std::vector<ObtainSpecialGemInfo> &outInfos);
	//特殊效果消除的宝石
	void SpecialEffectsToEliminateGems(TileBrickData &pTile,std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos);
	//获取下落的宝石数据
	bool DropGem(std::vector<cocos2d::Vec2 > &dropPos, GemData &pGemList);
	//获取每一列的最小行数
	int getMinRowNumber(int colNumber);
	//获取当前坐标的数据
	TileBrickData* currentTileBrickItem(int posX,int posY);
	//判断当前位置是否存在宝石或障碍:0.宝石障碍都不存在，1.宝石障碍都存在，2.只存在宝石，3.只存在障碍，4.其他
	enBrickType IsHaveGemOrStumbling(const TileBrickData &pTile);
	//获取当前格子可以消除的障碍物
	tagSpecialBrickData* getRemoveSpecialBrickItem(int posX,int posY);
	//特殊宝石交换
	void swapSpecialGem(std::vector<cocos2d::Vec2> &connectGemPos,std::vector<ObtainSpecialGemInfo> &outInfos,SpecialBrickData &StumblingBlockPos);
	//是否能结束游戏
	bool IsCanEndGame();
	//结束游戏类型
	gameEndStatus GameEndStatus();
	//获取当前关卡获取的星值
	int GetCurrentGateStar();
	//获取bonusTime时需要转化成的特殊宝石
	void getBonusTimeGem(std::vector<ObtainSpecialGemInfo> &outInfos);
	//消除障碍物
	void removeSpecialBrickItm(int posX,int posY,TileBrickData *pTileBrick,SpecialBrickData &StumblingBlockPos);
	//改变过关目标的数量
	void changeTargetNum(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos,std::vector<ObtainSpecialGemInfo> &outInfos);
	//检索是否有可以消除的宝石
	bool retrieveConnectGem(cocos2d::Vec2 point1 = cocos2d::Vec2(0,0),cocos2d::Vec2 point2 = cocos2d::Vec2(0,0));
	//棋盘初始不可有可以直接消除的宝石
	bool  retrieveTheBoard(bool flag = true);
	//生成特殊障碍物
	void ObtainSpecialBrickItems(MoveSpecialBrick &StumblingBlockPos);
	//改变宝石颜色
	void changeGemColor(ChangeGemColour &outInfo);
	//增加金钻
	void addGoldPod(tagGemData &pItem);
	//得分百分比
	void setScorePrecent(float precent){m_precentScore += precent;}
	//返回上一步
	void goBack();
	//使用道具类型
	void setUsePropType(enProp type){m_usePropType = type;}
	//获取使用道具类型
	enProp getUsePropType(){return m_usePropType;}
	//变换宝石
	bool changeGemType(int posX,int posY,GemStatus type);
	//获取是否移动了宝石
	bool getIsSwapGem(){return m_bSwapGem;}
	//改变炸弹宝石的时间
	void changTimeBomGemTime();
	
	//向下掉落
	bool dropBottom(TileBrickData *pTileBrick, TileBrickData *bottomBrick, cocos2d::Vec2 &dropPos);
	//向左下掉落
	bool dropLeftBottom(TileBrickData *pTileBrick, TileBrickData *leftBottomBrick, cocos2d::Vec2 &dropPos);
	//向右下掉落
	bool dropRightBottom(TileBrickData *pTileBrick, TileBrickData *rightBottomBrick, cocos2d::Vec2 &dropPos);
	//生成宝石
	bool addNewGem(TileBrickData *pTileBrick, tagGemData &pGemList);
	//判断当前格子是否有下传送门
	bool IsHaveDownPortal(int posX, int PosY, tagSpecialBrickData *downPortal);
	//获取对应创送门的位置
	tagSpecialBrickData* getAnotherPortal(const tagSpecialBrickData& pItem, bool flag);

public:
	unsigned short GetNextChair(unsigned short chair){return (chair+1)%GAME_PLAYER;}
	unsigned short GetAheadChair(unsigned short chair){return (GAME_PLAYER+chair-1)%GAME_PLAYER;}

	static tagGemData randomCreateGem(LevelsGem levelsGem);

private:
	template<class Type> 
	void addVectorData(std::vector<Type> &pVec,Type pData);

	//内部函数
private:
	//初始化数据
	void InitGame();
	//清除数据
	void clearListData();
	//能否生成宝石
	bool IsCanCreateGem(const TileBrickData &pTileBrick);
	//消除障碍
	void removeStumblingBlock(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos);
	//障碍是否能消除
	bool  IsCanRemoveStumblingBlock(int posX,int posY,tagSpecialBrickData &pStumblingBlock);
	//障碍物是否能移动
	bool IsCanMoveStumblingBlock(tagSpecialBrickData &pStumblingBlock);
	//获取左边的数据
	TileBrickData* leftTileBriteItem(int posX,int posY,TileBrickData *outTileBrick = NULL);
	//获取右边的数据
	TileBrickData* rightTileBriteItem(int posX,int posY,TileBrickData *outTileBrick  = NULL);
	//获取上边的数据
	TileBrickData* upTileBriteItem(int posX,int posY,TileBrickData *outTileBrick  = NULL);
	//获取下面的数据
	TileBrickData* bottomTileBriteItem(int posX,int posY,TileBrickData *outTileBrick  = NULL);
	//能否交换宝石
	bool IsCanSwapGem(const TileBrickData &tile,const TileBrickData &pTile,bool flag = true);
	//重置宝石
	void resetGem(bool flag = true);
	//交换宝石
	void swapGem(tagGemData &scoreGem,tagGemData &targetGem);
	//交换障碍物
	void swapSpecialBrick(tagSpecialBrickData &sourceItem,tagSpecialBrickData targetItem);
	//获取特殊效果宝石
	GemStatus getSpecialGemType(cocos2d::Vec2 point);
	//是否可以生成宝石
	bool IsCanAddGem(TileBrickData *pTile);
	//计算分数
	void CalculateScore(const TileBrickData &pTile);
	//获取横竖相连的个数
	cocos2d::Vec2 getROWAndCOLCount(cocos2d::Vec2 point,std::vector<cocos2d::Vec2> &outPoint,std::vector<cocos2d::Vec2> &connectGemPos);
	//获取当前格子的最上层障碍物类型
	tagSpecialBrickData* getTheTopSpecialBrickItem(int posX,int posY);
	//判断当前列是否有宝石可以下落
	bool IsHaveGemDrop(int posY,TileBrickData *pTileBrick,bool flag = true);
	//毛球移动
	void moveBulb(MoveSpecialBrick &pSpecial);
	//增加果冻
	void addJelly(MoveSpecialBrick &pSpecial);
	//判断当前格子是否有上传送门
	bool IsHaveUpPortal(int posX,int posY,tagSpecialBrickData *upPortal = NULL);
	//判断是否有相消
	bool IsCanConnectGem(TileBrickData &tile,TileBrickData &otherTile);
	//判断当前格子宝石能否移动
	bool IsCanMoveGem(int posX,int PosY);
	//判断当前格子是否可以连接
	bool IsCanConnect(int posX,int posY);
	//保存上步关卡数据
	void saveLevelData();
	//获取移动后的点位
	TileBrickData* getMovedPosition(int dir,int posX,int posY);
	//获取新
	TileBrickData* getNewSpecialBlock(tagSpecialBrickData &pItem);
	//更改目标数目
	void changeTargetCount(tagTargetData target);
	//获取当前颜色宝石数目
	int getGemNumberByColor(GemColor color);
	//是否含有timeBombGem
	GemData getTimeGem();

public:
	//测试用函数，显示当前宝石
	void showCurrentTile();

private:
	TileBrickData		m_tileBrick[ROW_COUNT][COL_COUNT];		//当前屏幕显示砖块数据数组
	TileBrick				m_brickList;
//	std::vector<cocos2d::Vec2> m_currentConnect;						//当前要消除的宝石的索引
	std::vector<TileBrick> m_playingGameList;							//游戏步骤链表

private: 
	int m_sourceX;
	int m_sourceY;
	int m_targetX;
	int m_targetY;

	//初始关卡数据
private:
	PBaseData m_gemData;							//所有宝石链表
	PBaseData m_specialBrick;			//所有障碍链表
	PBaseData m_mapList;							//地图形状
	TargetData m_targetList;						//过关目标
	LevelsGem m_levelsGemList;					//关卡可以随机出现的宝石

	tagRulesData  m_rulesData;		//关卡失败条件
	tagStartScore  m_starScore;	//星的分值
	std::vector<int>	m_oldStarScoreList;

	int m_gateNumber;				//当前关卡数字

	GemData m_initGemList;		//初始化宝石数据

private:
	TargetData m_currentTargetList;	//当前关卡已经消除的目标数量
	std::vector<TargetData> m_gameTargetList;
	LevelStartType m_levelStartType;	//关卡开始类型
	int m_scoreNumber;						//当前关卡所得分数
	int m_currentStep;						//当前剩余步数
	int m_currentTime;						//当前时间
	int m_currentComboNumber;		//当前连击数
	float m_precentScore;					//得分百分比

	//状态标示
	LevelsGameStatus m_gameStatus;
	GamePlayingStatus m_gamePlayingStatus;
	enProp m_usePropType; //使用道具类型
	bool m_bSwapGem;
	bool m_bDisappearJelly;

	public:
		swapGemStatus m_swapGemStatus;
};

//////////////////////////////////////////////////////////////////////////

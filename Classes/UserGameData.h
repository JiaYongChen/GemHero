#ifndef _USER_GAME_DATA_H_
#define _USER_GAME_DATA_H_
#include "CMD_ThreeEliminate.h"

//用户信息
struct tagUserGameData 
{
	unsigned char strength;		//体力
	int countStar;	//总共获得的星星数
	int unlockLevel;	//解锁的关卡数
	int currentLevel;	//当前所处关卡
	time_t oldTime;		//上次游戏结束时间
};

//关卡信息
struct tagLeveStarScorelInfo
{
	int levelID;				//关卡ID
	unsigned char starNumber;		//星星数
	int	score;				//分数
};
typedef std::vector<tagLeveStarScorelInfo> LevelData;

class CUserGameData
{
public:
	CUserGameData();
	~CUserGameData();

public:

	void clearUserGameData();
	//设置玩家数据
	void setUserGameData(const tagUserGameData& pGameData){m_userGameData = pGameData;}
	//获取玩家数据
	tagUserGameData& getUserGameData(){return m_userGameData;}
	//获取背包
	//CBag& getBag(){return m_bag;}
	//增加玩家关卡数据
	void addLevelData(const tagLeveStarScorelInfo &pData);
	//获取玩家关卡信息
	LevelData& getLevelData(){return m_levelData;}
	//获取玩家关卡信息
	tagLeveStarScorelInfo* getLevelInfoByLevelID(int ID);
	//获取总星星数
	int getCountStarNumber();

private:
	tagUserGameData m_userGameData;		//玩家游戏信息
	LevelData  m_levelData;	//关卡信息
	//CBag m_bag;	//背包

public:
	int m_bagNumber;
	int m_bagUnlockNumber;
};
#endif
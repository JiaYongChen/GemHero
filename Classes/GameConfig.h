#ifndef  _GAME_CONFIG_H_
#define _GAME_CONFIG_H_
#include "CMD_ThreeEliminate.h"

class CGameConfig
{
public:
	CGameConfig(void);
	~CGameConfig(void);

	//初始化数据
	void initialise();
	//清除数据
	void cleanup();
	//读取配置文件
	void load_script(const char* fileName);
	//获取最大关卡数
	int getMaxLevelNumber(){return m_maxLevelNumber;}
	//获取最大总排行榜数目
	int getMaxTotalNumber(){return m_maxTotalNumber;}
	//获取最大关卡排行榜数目
	int getMaxLevelListNumber(){return m_maxLevelListNumber;}
	//获取最大体力值
	int getMaxStrength(){return m_maxStrength;}
	//获取增加体力的时间
	int getAddStrengthTime(){return m_addStrengthTime;}
	//获取消耗体力值
	int getLostStrength(){return m_lostStrength;}
	//获取特殊消耗体力值
	int getSPLostStrength(){return m_specielLostStrength;}
	//获取购买体力消耗
	long long getBuyStrengthMoney(){return m_buyStrengthMoney;}
	//或取一次解锁的关卡数目
	int getUnlockLevelNumber(){return m_UnlockLevelNumber;}
	//获取增加步数时间
	int getAddStepTime(){return m_addStepTime;}
	//获取增加的步数
	int getAddStepNumber(){return m_addStepNum;}
	//获取增加步数需要的金豆数
	long long getAddStepMoney(){return m_addStepMoney;}
	//获取
	int getAddScorePrecent(){return m_addScorePrecent;}

	//是否超级用户
	bool IsSuperUser(unsigned long userID);

private:
	//关卡
	int m_maxLevelNumber;		//最大关卡数
	int m_maxTotalNumber;		//最大排行帮个数
	int m_maxLevelListNumber;	//最大关卡排行榜个数
	int m_UnlockLevelNumber;	//一次解锁的关卡数目

	//体力
	int m_addStrengthTime;		//增加体力耗时
	int m_maxStrength;	//最大体力值
	int m_lostStrength;	//消耗的体力值
	int m_specielLostStrength;	//特殊关卡消耗的体力值
	long long m_buyStrengthMoney; //购买体力消耗
	int m_addStepTime;//增加步数的时间
	int m_addStepNum;	//增加步数
	long long m_addStepMoney;//增加步数需要的金豆数
	int m_addScorePrecent;		//增加分数的比例


	//超级用户
	std::vector<unsigned long> m_superUser;
};

#endif
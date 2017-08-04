#ifndef _GAME_PLAYING_SCENE_DATA_H_
#define _GAME_PLAYING_SCENE_DATA_H_
#include "CMD_ThreeEliminate.h"

class CGamePlayingSceneData
{
public:
	static CGamePlayingSceneData& getInstance()
	{
		static CGamePlayingSceneData _instance;
		return _instance;
	}

private:
	CGamePlayingSceneData();
	~CGamePlayingSceneData();

public:
	//清除数据
	void clear();
	//初始化关卡数据
	 bool  initialization(int levelID);
	 //设置地形
	 void setMapData(MapData& pData)
	 {
		 m_mapData.clear();
		 SafeErase(m_mapData);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_mapData.push_back(&pData[i]);
		 }
	 }
	 //获取地形
	 const PBaseData& getMapData(){return m_mapData;}
	 //设置宝石
	 void setGemItem(GemData& pData)
	 {
		 m_gemData.clear();
		 SafeErase(m_gemData);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_gemData.push_back(&pData[i]);
		 }
	 }
	 //获取宝石链表
	 const PBaseData& getGemItem(){return m_gemData;}
	 //设置障碍物
	 void setSpecialBrickItem(SpecialBrickData& pData)
	 {
		 m_specialBrick.clear();
		 SafeErase(m_specialBrick);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_specialBrick.push_back(&pData[i]);
		 }
	 }
	 //获取障碍物链表
	 const PBaseData& getSpecialBrickItem(){return m_specialBrick;}
	 //设置关卡允许出现的宝石
	 void setLevelsGem(const LevelsGem& pData)
	 {
		 m_levelsGem.clear();
		 SafeErase(m_levelsGem);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_levelsGem.push_back(pData[i]);
		 }
	 }
	 //获取关卡允许出现的宝石
	 const LevelsGem& getLevelsGem(){return m_levelsGem;}
	 //设置过关目标
	 void setTargetData(const TargetData& pData)
	 {
		 m_targetData.clear();
		 SafeErase(m_targetData);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_targetData.push_back(pData[i]);
		 }
	 }
	 //获取过关目标
	 const TargetData& getTargetData(){return m_targetData;}
	 //设置初始化宝石
	 void setInitGemData(GemData& pData)
	 {
		 m_initGemList.clear();
		 SafeErase(m_initGemList);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_initGemList.push_back(&pData[i]);
		 }
	 }
	 //获取初始化宝石
	 const GemData& getInitGemData();
	 //设置得分信息
	 void setStarScore(const tagStartScore& pData){m_starScore = pData;}
	 //获取得分信息
	 const tagStartScore& getStarScore(){return m_starScore;}
	 //设置失败条件
	 void setRulesData(const tagRulesData& pData){m_rulesData = pData;}
	 //获取失败条件
	 const tagRulesData& getRulesData(){return m_rulesData;}

private:
	//关卡初始化宝石
	void initializationGem();
	//清除数据
	void clearData(PBaseData& data);

private:
	PBaseData m_mapData;	//地图形状
	PBaseData m_gemData;	//所有宝石链表
	PBaseData m_specialBrick;	//所有障碍链表
	TargetData m_targetData; //过关目标;
	LevelsGem m_levelsGem;	//关卡出现的宝石;
	tagStartScore m_starScore; //关卡得分信息;
	tagRulesData  m_rulesData;	//关卡失败条件;

	static PBaseData m_initGemList;		//初始化宝石数据

	int m_gateNumber;	//关卡数字
};

#endif
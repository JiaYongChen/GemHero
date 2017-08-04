#ifndef _READ_LEVEL_INFO_H_
#define _READ_LEVEL_INFO_H_
#include <string>
#include "CMD_ThreeEliminate.h"

class CReadLevelInfo
{
public:
	CReadLevelInfo(void);
	~CReadLevelInfo(void);

	bool ReadDataFromeFileForIOS(const std::string &fileName);
	void ClearData();

public:
	PBaseData getMapList();
	TargetData getTargetList(){return m_targetList;}		//过关目标
	LevelsGem getLevelsGemList(){return m_levelsGemList;}		//关卡可以随机出现的宝石
	PBaseData getSpecialBricksList(){return m_specialBricksList;}	//特殊障碍
	PBaseData getGemItemList(){return m_gemList;}

	tagRulesData  getRulesData(){return m_rulesData;}		//关卡失败条件
	tagStartScore  getStarScore(){return m_starScore;}	//星的分值

private:
	void ReadDataFromeData(const TileImageInfo &ImageInfo, const std::vector<ImageData> &targetData, const std::vector<std::string> &gemName);
	void ReadTargetData(const std::vector<ImageData> &targetData);
	void ReadGemData(const std::vector<std::string> &gemName);
	void ReadTileMapData(const TileImageInfo &ImageInfo);

	int readCharToInt(const char *file,long &offLength);

public:
	PBaseData m_mapList;

	TargetData m_targetList;						//过关目标
	LevelsGem m_levelsGemList;					//关卡可以随机出现的宝石
	PBaseData m_specialBricksList;		//特殊障碍
	PBaseData m_gemList;							//关卡初始宝石

	tagRulesData  m_rulesData;		//关卡失败条件
	tagStartScore  m_starScore;		//星的分值

private:
	//关卡
	int m_levelNumber;
	// 步数
	int m_stepNum;
	// 时间
	int m_time;
	// 最大分数
	int m_maxScore;
	// 一星分数
	int m_score1;
	// 二星分数
	int m_score2;
	// 三星分数
	int m_score3;
	//游戏类型
	int m_levelGameType;
};
#endif
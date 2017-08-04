#include "GamePlayingSceneData.h"
#include "ReadLevelInfo.h"

USING_NS_CC;

PBaseData CGamePlayingSceneData::m_initGemList;

CGamePlayingSceneData::CGamePlayingSceneData()
{
	initialization(0);
}

CGamePlayingSceneData::~CGamePlayingSceneData()
{
	
}

void CGamePlayingSceneData::clearData(PBaseData& data) {
	for (size_t i = 0; i < data.size(); ++i)
	{
		if (data[i] != nullptr)
		{
			CC_SAFE_DELETE(data[i]);
		}
	}
	SafeErase(data);
}

void CGamePlayingSceneData::clear()
{
	clearData(m_mapData);
	clearData(m_gemData);
	clearData(m_specialBrick);

	SafeErase(m_levelsGem);
	SafeErase(m_targetData);

	m_starScore.maxScore = 0;
	m_starScore.score_1 = 0;
	m_starScore.score_2 = 0;
	m_starScore.score_3 = 0;

	m_rulesData.gameType = LEVELGAME_NULL;
	m_rulesData.num = 0;
	m_rulesData.step = 0;
	m_rulesData.time = 0;
	m_rulesData.type = LEVELSTART_NULL;

	m_gateNumber = 0;
}

bool CGamePlayingSceneData::initialization(int levelID)
{
	clear();

	char str[256];
	CReadLevelInfo pLevelInfo;
	auto name = cocos2d::FileUtils::getInstance()->getSearchPaths();
	if (levelID<= 0 && m_initGemList.empty())
	{	
		snprintf(str,sizeof(str),"%smap/%d.data",name[1].c_str(),levelID);
		pLevelInfo.ReadDataFromeFileForIOS(str);

		m_initGemList = pLevelInfo.getGemItemList();
	}
	else if (levelID > 0)
	{
		snprintf(str, sizeof(str), "%smap/%d.data", name[1].c_str(), levelID);
		if (pLevelInfo.ReadDataFromeFileForIOS(str) == false)
		{
			return false;
		}

		m_mapData = pLevelInfo.getMapList();						//地图形状
		m_targetData = pLevelInfo.getTargetList();				//过关目标
		m_levelsGem = pLevelInfo.getLevelsGemList();			//关卡可以随机出现的宝石
		m_specialBrick = pLevelInfo.getSpecialBricksList();		//特殊障碍
		m_gemData = pLevelInfo.getGemItemList();				//关卡初始宝石

		m_rulesData = pLevelInfo.getRulesData();		//关卡失败条件
		m_starScore = pLevelInfo.getStarScore();	//星的分值

		m_gateNumber = levelID;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/gameScene.plist");

	return true;
}

const GemData& CGamePlayingSceneData::getInitGemData()
{
	static GemData item;
	item.clear();

	int randPosX = 0;
	if (m_initGemList.size() > COL_COUNT*ROW_COUNT)
	{
		randPosX = rand() % 90;
	}
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			int posX = randPosX + x;
			if (posX >= 90)
			{
				posX = posX % 90; 
			}

			tagGemData *pData = (tagGemData*)m_initGemList[posX*9+y];
			pData->id = x * 9 + y;
			pData->indexX = x;
			pData->indexY = y;
			item.push_back(*pData);
		}
	}

	return item;
}
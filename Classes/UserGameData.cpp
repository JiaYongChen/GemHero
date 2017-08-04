#include "UserGameData.h"

CUserGameData::CUserGameData()
{
	clearUserGameData();
}

CUserGameData::~CUserGameData()
{
	clearUserGameData();
}

void CUserGameData::clearUserGameData()
{
	m_userGameData.countStar = 0;
	m_userGameData.currentLevel = 0;
	m_userGameData.oldTime = 0;
	m_userGameData.strength = 0;
	m_userGameData.unlockLevel = 0;

	SafeErase(m_levelData);
	//m_bag.clear();
	m_levelData.clear();
}

void CUserGameData::addLevelData(const tagLeveStarScorelInfo &pData)
{
	tagLeveStarScorelInfo *pInfo = getLevelInfoByLevelID(pData.levelID);
	if (pInfo == NULL)
	{
		m_levelData.push_back(pData);
	}
	else
	{
		for (LevelData::iterator ite = m_levelData.begin(); ite != m_levelData.end(); ++ite)
		{
			if (ite->levelID == pData.levelID)
			{
				ite->starNumber = pData.starNumber;
				ite->score = pData.score;
			}
		}
	}
}

tagLeveStarScorelInfo* CUserGameData::getLevelInfoByLevelID(int ID)
{
	for (LevelData::iterator ite = m_levelData.begin(); ite != m_levelData.end(); ++ite)
	{
		if (ite->levelID == ID)
		{
			return &(*(ite));
		}
	}

	return NULL;
}

int CUserGameData::getCountStarNumber()
{
	int countStar = 0;
	for (LevelData::iterator ite = m_levelData.begin(); ite != m_levelData.end(); ++ite)
	{
		if (ite->levelID >0)
		{
			countStar += ite->starNumber;
		}
	}

	return countStar;
}
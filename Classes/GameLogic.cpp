#include "GameLogic.h"
#include <algorithm>
#include "ReadLevelInfo.h"
#include <boost/lexical_cast.hpp>

///////////////////////////////////////////////////////////////////////

using namespace std;

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
	clearListData();
}

void CGameLogic::clearListData()
{
	SafeErase(m_gemData);
	SafeErase(m_levelsGemList);
	SafeErase(m_mapList);
	SafeErase(m_specialBrick);
	SafeErase(m_targetList);
	SafeErase(m_oldStarScoreList);
	SafeErase(m_gameTargetList);
	SafeErase(m_playingGameList);

	m_rulesData.step = 0;
	m_rulesData.time = 0;
	m_rulesData.type = LEVELSTART_NULL;

	m_starScore.maxScore = 0;
	m_starScore.score_1 = 0;
	m_starScore.score_2 = 0;
	m_starScore.score_3 = 0;

	m_gateNumber = 0;

	m_currentComboNumber = 0;
	m_bSwapGem = false;
	m_bDisappearJelly = false;
	m_scoreNumber = 0;

	m_precentScore = 1.0f;

	m_gameStatus = GAME_STATUS_NULL;
	m_gamePlayingStatus = GAME_PLAYING_STATUS_NORMAL;
	m_swapGemStatus = NULL_SWAP_STATUS;
}

void CGameLogic::restartGame()
{
	for (int x = 0; x < ROW_COUNT;++x)
	{
		for(int y = 0; y < COL_COUNT; ++y)
		{
			m_tileBrick[x][y].id = x*9+y;
			m_tileBrick[x][y].indexX = x;
			m_tileBrick[x][y].indexY = y;
			m_tileBrick[x][y].isShow = false;
			m_tileBrick[x][y].map.clear();
			m_tileBrick[x][y].gem.clear();
			m_tileBrick[x][y].specialBrick.erase(m_tileBrick[x][y].specialBrick.begin(),m_tileBrick[x][y].specialBrick.end());
		}
	}
	SafeErase(m_currentTargetList);
	SafeErase(m_playingGameList);
	SafeErase(m_oldStarScoreList);
	SafeErase(m_gameTargetList);
	m_currentComboNumber = 0;
	m_precentScore = 1.0f;
	m_scoreNumber = 0;

	m_gameStatus = GAME_STATUS_PLAYING;
	m_gamePlayingStatus = GAME_PLAYING_STATUS_NORMAL;
	m_swapGemStatus = NULL_SWAP_STATUS;

	InitGame();
}

void CGameLogic::loadLevelDataFromeXML(int levelsID)
{
	clearListData();

// 	CMyXML pxml;
// 
// 	char str[256];
// 	snprintf(str,"map\\%d.xml",levelsID);
// 
// 	//pxml.loadXML(getAbsolueDir(FLASH_PATH, str));
// 
// 	m_mapList = pxml.getMapList();								//地图形状
// 	m_targetList = pxml.getTargetList();							//过关目标
// 	m_levelsGemList = pxml.getLevelsGemList();				//关卡可以随机出现的宝石
// 	m_specialBrick = pxml.getSpecialBricksList();		//特殊障碍
// 	m_gemData = pxml.getGemItemList();				//关卡初始宝石
// 
// 	m_rulesData = pxml.getRulesData();		//关卡失败条件
// 	m_starScore = pxml.getStarScore();	//星的分值
// 
// 	pxml.clearListData();
}

void CGameLogic::loadLevelDataFromeFile(int levelsID)
{
	clearListData();

	CReadLevelInfo pLevelInfo;
	std::string str;
	str += "map\\";
	str = boost::lexical_cast<std::string>(levelsID);
	str += ".data";

	//pLevelInfo.ReadDataFromeFile(getAbsolueDir(FLASH_PATH, str));

	m_mapList = pLevelInfo.getMapList();								//地图形状
	m_targetList = pLevelInfo.getTargetList();							//过关目标
	m_levelsGemList = pLevelInfo.getLevelsGemList();				//关卡可以随机出现的宝石
	m_specialBrick = pLevelInfo.getSpecialBricksList();		//特殊障碍
	m_gemData = pLevelInfo.getGemItemList();				//关卡初始宝石

	m_rulesData = pLevelInfo.getRulesData();		//关卡失败条件
	m_starScore = pLevelInfo.getStarScore();	//星的分值

	pLevelInfo.ClearData();
}

bool CGameLogic::loadLevelDataFromeData(const CGamePlayingSceneData& pPlayingData)
{
	clearListData();
	m_mapList = const_cast<CGamePlayingSceneData&>(pPlayingData).getMapData();	//地图形状
	m_targetList = const_cast<CGamePlayingSceneData&>(pPlayingData).getTargetData();				//过关目标
	m_levelsGemList = const_cast<CGamePlayingSceneData&>(pPlayingData).getLevelsGem();				//关卡可以随机出现的宝石
	m_specialBrick = const_cast<CGamePlayingSceneData&>(pPlayingData).getSpecialBrickItem();		//特殊障碍
	m_gemData = const_cast<CGamePlayingSceneData&>(pPlayingData).getGemItem();				//关卡初始宝石

	m_rulesData = const_cast<CGamePlayingSceneData&>(pPlayingData).getRulesData();		//关卡失败条件
	m_starScore = const_cast<CGamePlayingSceneData&>(pPlayingData).getStarScore();	//星的分值
	m_initGemList = const_cast<CGamePlayingSceneData&>(pPlayingData).getInitGemData();		//初始化宝石

	return true;
}

void CGameLogic::InitGame()
{
	m_gateNumber = m_rulesData.num;
	m_levelStartType = m_rulesData.type;
	m_currentStep = m_rulesData.step;
	m_currentTime = m_rulesData.time;
	m_currentComboNumber = 0;
	m_bSwapGem = false;
	m_bDisappearJelly = false;

	//初始地图
	for (size_t i = 0; i < m_mapList.size(); ++i)
	{
		tagMapData* pData = (tagMapData*)m_mapList[i];

		int x = pData->indexX;
		if (x < 0 || x >= COL_COUNT)
		{
			continue;
		}
		int y = pData->indexY;
		if (y < 0 || y >= COL_COUNT)
		{
			continue;
		}

		m_tileBrick[x][y].map = *pData;

		m_tileBrick[x][y].id = pData->id;
		m_tileBrick[x][y].indexX = pData->indexX;
		m_tileBrick[x][y].indexY = pData->indexY;
		m_tileBrick[x][y].isShow = pData->isShow;
	}

	//初始障碍物
	for (int i = 0; i < (int)m_specialBrick.size(); ++i)
	{
		tagSpecialBrickData *pData = (tagSpecialBrickData*)m_specialBrick[i];

		if ((pData->indexX * 9 + pData->indexY)> 80)
		{
			continue;
		}

		if (IsCanMoveStumblingBlock(*pData))
		{
			pData->bMove = true;
		}		

		for (int x = 0; x < COL_COUNT; ++x)
		{
			for (int y = 0; y < ROW_COUNT; ++y)
			{
				if (pData->indexX == x && pData->indexY == y)
				{
					m_tileBrick[x][y].specialBrick.push_back(*pData);
				}
			}
		}
	}

	//初始宝石
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			tagGemData pData = m_initGemList[x*9+y];
			pData.id = x * 9 + y;
			pData.indexX = x;
			pData.indexY = y;
			pData.bMove = true;
			if (IsCanCreateGem(m_tileBrick[x][y]) == false)
			{
				pData.childType = GEM_COLOR_NO;
				pData.gemStatus = GEM_STATUS_NORMAL;
				pData.bMove = false;
			}

			m_tileBrick[x][y].gem = pData;
			m_tileBrick[x][y].gem.bMove = IsCanMoveGem(x,y);
		}
	}

	retrieveTheBoard(false);

	if (!m_gemData.empty())
	{
		for (size_t i = 0; i < m_gemData.size();++i)
		{
			tagGemData* gemData = (tagGemData*)m_gemData[i];
			tagGemData& pData = m_tileBrick[gemData->indexX][gemData->indexY].gem;
			pData.gemStatus = gemData->gemStatus;
			pData.attribute = gemData->attribute;
			if (pData.gemStatus == GEM_STATUS_COLORFUL)
			{
				pData.childType = GEM_COLOR_NO;
			}
		}
	}

	m_currentTargetList = m_targetList;
	for (TargetData::iterator ite = m_currentTargetList.begin();ite != m_currentTargetList.end();++ite)
	{
		ite->number = 0;
	}
}


std::string CGameLogic::getCurrentLevelsData()
{
	std::string buff = "";

// 	buff += getCurrentMapData();
// 
// 	if (buff != "")
// 		buff += "|";
// 	buff += getCurrentSpecialBrickData();
// 
// 	if (buff != "")
// 		buff += "|";
// 	buff += getCurrentGemData();

	return buff;
}

const TargetData& CGameLogic::getSuccessCondition()
{
	return m_targetList;
}

const TargetData& CGameLogic::getCurrentSuccessCondition()
{
	std::string buff = "";

	buff += boost::lexical_cast<std::string>(m_currentStep);
	buff += "|";
	buff += boost::lexical_cast<std::string>(m_currentTime);
	buff += "|";

	for (int i = 0; i < (int)m_currentTargetList.size();++i)
	{
		tagTargetData pTargetData = m_currentTargetList[i];
		buff += boost::lexical_cast<std::string>(pTargetData.ID);
		buff += ",";
		buff += boost::lexical_cast<std::string>(pTargetData.parentType);
		buff += ",";
		buff += boost::lexical_cast<std::string>(pTargetData.type);
		buff += ",";
		buff += boost::lexical_cast<std::string>(pTargetData.number);
		buff += ";";
	}

	return m_currentTargetList;
}

int CGameLogic::GetCurrentGateScore()
{
	return m_scoreNumber;
}

const tagStartScore& CGameLogic::getScoreCondition()
{
	return m_starScore;
}

TileBrick& CGameLogic::getCurrentTileBrick()
{
	m_brickList.clear();
	for (int x = 0; x < ROW_COUNT;++x)
	{
		for(int y = 0; y < COL_COUNT; ++y)
		{
			m_brickList.push_back(m_tileBrick[x][y]);
		}
	}

	return m_brickList;
}

const PBaseData& CGameLogic::getCurrentMapData()
{
	m_mapList.clear();
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			if (m_tileBrick[x][y].map.isShow)
			{
				m_mapList.push_back(&m_tileBrick[x][y].map);
			}
		}
	}

	return m_mapList;
}

const PBaseData& CGameLogic::getCurrentSpecialBrickData()
{
	m_specialBrick.clear();
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			SpecialBrickData specialItem = m_tileBrick[x][y].specialBrick;
			for (int i = 0; i < (int)specialItem.size(); ++i)
			{
				m_specialBrick.push_back(&m_tileBrick[x][y].specialBrick[i]);
			}
		}
	}

	return m_specialBrick;
}

const PBaseData& CGameLogic::getCurrentGemData()
{
	m_gemData.clear();
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			tagGemData& pData = m_tileBrick[x][y].gem;
			m_gemData.push_back(&m_tileBrick[x][y].gem);
		}
	}		

	return m_gemData;
}

void CGameLogic::getAllSpecilGem(std::vector<cocos2d::Vec2> &GemPos)
{
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			TileBrickData &pTile = m_tileBrick[x][y];
			if (pTile.gem.gemStatus > GEM_STATUS_NORMAL && pTile.gem.gemStatus < GEM_STATUS_CHANGE)
			{
				GemPos.push_back(cocos2d::Vec2(x,y));
			}
		}
	}
}

bool CGameLogic::IsCanCreateGem(const TileBrickData &pTileBrick)
{
	enBrickType IsHaveGem = IsHaveGemOrStumbling(pTileBrick);
	if (IsHaveGem == NULL_TYPE)
	{
		return false;
	}
	if (IsHaveGem == GEM_TYPE)
	{
		return false;
	}
	else	if (IsHaveGem == GEM_AND_BRICK_TYPE)
	{
		return false;
	}
	else	if (IsHaveGem == NO_GEM_AND_BRICK_TYPE)
	{
		return true;
	}

	bool result = false;
	const SpecialBrickData  &item = pTileBrick.specialBrick;
	for (size_t i = 0; i < item.size(); ++i)
	{
		switch(item[i].childType)
		{
		case STUMBLING_BLOCK_GOLD_POD:
		case STUMBLING_BLOCK_WOOD:
		case STUMBLING_BLOCK_JELLY:
		case STUMBLING_BLOCK_SILVER_COIN:
			{
				return false;
			}
			break;
		default:
			{
				result = true;
			}

			break;
		}

		if (!result)
		{
			break;
		}
	}

	return result;
}

tagGemData CGameLogic::randomCreateGem(LevelsGem levelsGem)
{
	do 
	{
		unsigned char color = rand() % GEM_COLOR_COUNT;

		bool flag = false;

		for (LevelsGem::iterator ite = levelsGem.begin(); ite != levelsGem.end(); ++ite)
		{
			if (ite->color == color)
			{
				flag = true;
			}
		}

		if (flag)
		{
			tagGemData Gem;
			Gem.parentType = NORMAL_GEM_PARENT_TYPE;
			Gem.childType = GemColor(color);
			Gem.gemStatus = GEM_STATUS_NORMAL;
			Gem.bMove = true;
			return Gem;
		}
	} while (true);
}

swapGemStatus CGameLogic::IsValidMove(unsigned short sourceX, unsigned short sourceY, unsigned short targetX, unsigned short targetY,bool flag /*= true*/)
{
	//效验点是否在棋盘范围内
	CCASSERT(sourceX>=0&&sourceX<ROW_COUNT&&targetX>=0&&targetX<ROW_COUNT,"source falied");
	CCASSERT(sourceY>=0&&sourceY<COL_COUNT&&targetY>=0&&targetY<COL_COUNT,"target falied");

	if((m_gameStatus != GAME_STATUS_PLAYING && m_gameStatus != GAME_STATUS_USE_PROP ) && m_gamePlayingStatus != GAME_PLAYING_STATUS_SWAP_GEM) return NULL_SWAP_STATUS;

	m_sourceX = sourceX;m_sourceY = sourceY;
	m_targetX = targetX;m_targetY = targetY;

	tagGemData &sourceGem = m_tileBrick[m_sourceX][m_sourceY].gem;
	tagGemData &targetGem =  m_tileBrick[m_targetX][m_targetY].gem;

	if ((sourceGem.childType <= GEM_COLOR_NO && sourceGem.gemStatus != GEM_STATUS_COLORFUL) || (targetGem.childType <= GEM_COLOR_NO && targetGem.gemStatus != GEM_STATUS_COLORFUL))
	{
		if (sourceGem.childType <= GEM_COLOR_NO && sourceGem.gemStatus != GEM_STATUS_COLORFUL)
		{
			if (m_tileBrick[m_sourceX][m_sourceY].specialBrick.empty())
			{
				return NULL_SWAP_STATUS;
			}
			else
			{
				SpecialBrickData &item = m_tileBrick[m_sourceX][m_sourceY].specialBrick;
				for (SpecialBrickData::iterator ite = item.begin(); ite != item.end(); ++ite)
				{
					if (ite->childType == STUMBLING_BLOCK_BULB || ite->childType == STUMBLING_BLOCK_JELLY || ite->childType == STUMBLING_BLOCK_STENCIL || ite ->childType == STUMBLING_BLOCK_WOOD)
					{
						return NULL_SWAP_STATUS;
					}
				}
			}
		}

		if (targetGem.childType <= GEM_COLOR_NO && targetGem.gemStatus != GEM_STATUS_COLORFUL)
		{
			if (m_tileBrick[m_targetX][m_targetY].specialBrick.empty())
			{
				return NULL_SWAP_STATUS;
			}
			else
			{
				SpecialBrickData &item = m_tileBrick[m_targetX][m_targetY].specialBrick;
				for (SpecialBrickData::iterator ite = item.begin(); ite != item.end(); ++ite)
				{
					if (ite->childType == STUMBLING_BLOCK_BULB || ite->childType == STUMBLING_BLOCK_JELLY || ite->childType == STUMBLING_BLOCK_STENCIL || ite ->childType == STUMBLING_BLOCK_WOOD)
					{
						return NULL_SWAP_STATUS;
					}
				}
			}
		}
	}

	if (sourceGem.gemStatus > GEM_STATUS_NORMAL  && sourceGem.gemStatus <= GEM_STATUS_COLORFUL && m_gameStatus == GAME_STATUS_PLAYING)
	{
		if(sourceGem.gemStatus == GEM_STATUS_COLORFUL && targetGem.gemStatus >= GEM_STATUS_NORMAL && targetGem.gemStatus <= GEM_STATUS_COLORFUL
			&& (targetGem.childType > GEM_COLOR_NO || targetGem.childType == GEM_COLOR_NO && targetGem.gemStatus == GEM_STATUS_COLORFUL))
		{
			if (flag == true)
			{
				saveLevelData();
				swapGem(m_tileBrick[sourceX][sourceY].gem,m_tileBrick[targetX][targetY].gem);
				if (m_currentStep >0 ) m_currentStep--;
				m_currentComboNumber = 0;
				m_bSwapGem = true;

				m_swapGemStatus = SPECIAL_SWAP_STATUS;
			}
			return SPECIAL_SWAP_STATUS;
		}
		else if (sourceGem.gemStatus == GEM_STATUS_COLORFUL && targetGem.gemStatus > GEM_STATUS_COLORFUL && targetGem.gemStatus <= GEM_STATUS_TREASURE_CHEST)
		{
			if (flag == true)
			{
				saveLevelData();
				swapGem(m_tileBrick[sourceX][sourceY].gem,m_tileBrick[targetX][targetY].gem);
				if (m_currentStep >0 ) m_currentStep--;
				m_currentComboNumber = 0;
				m_bSwapGem = true;

				m_swapGemStatus = SPECIAL_SWAP_STATUS;
			}
			return SPECIAL_SWAP_STATUS;
		}
		else if (sourceGem.gemStatus < GEM_STATUS_COLORFUL&&targetGem.gemStatus > GEM_STATUS_NORMAL && targetGem.gemStatus <= GEM_STATUS_COLORFUL)
		{
			if (flag == true)
			{
				saveLevelData();
				swapGem(m_tileBrick[sourceX][sourceY].gem,m_tileBrick[targetX][targetY].gem);
				if (m_currentStep >0 ) m_currentStep--;
				m_currentComboNumber = 0;
				m_bSwapGem = true;
				m_swapGemStatus = SPECIAL_SWAP_STATUS;
			}
			return SPECIAL_SWAP_STATUS;
		}
		else if (sourceGem.gemStatus > GEM_STATUS_COLORFUL&&targetGem.gemStatus == GEM_STATUS_COLORFUL)
		{
			if (flag == true)
			{
				saveLevelData();
				swapGem(m_tileBrick[sourceX][sourceY].gem,m_tileBrick[targetX][targetY].gem);
				if (m_currentStep >0 ) m_currentStep--;
				m_currentComboNumber = 0;
				m_bSwapGem = true;
				m_swapGemStatus = SPECIAL_SWAP_STATUS;
			}
			return SPECIAL_SWAP_STATUS;
		}
	}
	else if ((sourceGem.gemStatus == GEM_STATUS_NORMAL || sourceGem.gemStatus > GEM_STATUS_COLORFUL && sourceGem.gemStatus <= GEM_STATUS_TREASURE_CHEST)
		&& (sourceGem.childType > GEM_COLOR_NO || sourceGem.childType == GEM_COLOR_NO && sourceGem.gemStatus == GEM_STATUS_COLORFUL)
		&& m_gameStatus == GAME_STATUS_PLAYING)
	{
		if (targetGem.gemStatus == GEM_STATUS_COLORFUL)
		{
			if (flag == true)
			{
				saveLevelData();
				swapGem(m_tileBrick[sourceX][sourceY].gem,m_tileBrick[targetX][targetY].gem);
				if (m_currentStep >0 ) m_currentStep--;
				m_currentComboNumber = 0;
				m_bSwapGem = true;
				m_swapGemStatus = SPECIAL_SWAP_STATUS;
			}
			return SPECIAL_SWAP_STATUS;
		}
	}

	//放大数组
	GemColor tempGemInfo[13][13];
	memset(tempGemInfo,GEM_COLOR_NO,sizeof(tempGemInfo));

	//偏移位置
	sourceX+=2;	sourceY+=2;
	targetX+=2;	targetY+=2;

	//拷贝数据
	for(int i=2;i<11;i++) for(int j=2;j<11;j++)		tempGemInfo[i][j]= GemColor(m_tileBrick[i-2][j-2].gem.childType);

	GemColor color = tempGemInfo[sourceX][sourceY];
	tempGemInfo[sourceX][sourceY] = tempGemInfo[targetX][targetY];
	tempGemInfo[targetX][targetY] = color;

	enDirection dire = CENT_DIRECTION;
	if (m_sourceX - m_targetX > 0)
	{
		dire = UP_DIRECTION;
	}
	else if (m_sourceX - m_targetX < 0)
	{
		dire = DOWN_DIRECTION;
	}
	else if (m_sourceY - m_targetY > 0)
	{
		dire = LEFT_DIRECTION; 
	}
	else if (m_sourceY - m_targetY < 0)
	{
		dire = RIGHT_DIRECTION;
	}

	bool result = false;	
	if (m_gameStatus == GAME_STATUS_PLAYING)
	{
		//判断开始点移动后有无可消除
		GemColor x0=tempGemInfo[sourceX-2][sourceY];
		GemColor x1=tempGemInfo[sourceX-1][sourceY];
		GemColor x2=tempGemInfo[sourceX][sourceY];
		GemColor x3=tempGemInfo[sourceX+1][sourceY];
		GemColor x4=tempGemInfo[sourceX+2][sourceY];

		int posX = sourceX - 2,posY = sourceY - 2;  //因为将数组扩大，所以使用前先缩小为原来坐标点
		bool b0=IsCanConnect(posX-2,posY);
		bool b1=IsCanConnect(posX-1,posY);
		bool b2=IsCanConnect(m_targetX,m_targetY);
		bool b3=IsCanConnect(posX+1,posY);
		bool b4=IsCanConnect(posX+2,posY);

		if (dire == UP_DIRECTION)
		{
			b1 = IsCanConnect(m_sourceX,m_sourceY);
		}
		else if (dire == DOWN_DIRECTION)
		{
			b3 = IsCanConnect(m_sourceX,m_sourceY);
		}

		if(x0==x1&&x1==x2&&x1 != GEM_COLOR_NO&&b0&&b1&&b2)
			result = true;
		else if(x1==x2&&x2==x3&&x2 != GEM_COLOR_NO&&b1&&b2&&b3)
			result = true;
		else if(x2==x3&&x3==x4&&x3 != GEM_COLOR_NO&&b2&&b3&&b4)
			result = true;

		GemColor y0=tempGemInfo[sourceX][sourceY-2];
		GemColor y1=tempGemInfo[sourceX][sourceY-1];
		GemColor y2=tempGemInfo[sourceX][sourceY];
		GemColor y3=tempGemInfo[sourceX][sourceY+1];
		GemColor y4=tempGemInfo[sourceX][sourceY+2];

		b0=IsCanConnect(posX,posY-2);
		b1=IsCanConnect(posX,posY-1);
		b2=IsCanConnect(m_targetX,m_targetY);
		b3=IsCanConnect(posX,posY+1);
		b4=IsCanConnect(posX,posY+2);

		if (dire == LEFT_DIRECTION)
		{
			b1 = IsCanConnect(m_sourceX,m_sourceY);
		}
		else if ( dire == RIGHT_DIRECTION)
		{
			b3 = IsCanConnect(m_sourceX,m_sourceY);
		}

		if(	y0==y1&&y1==y2&&y1!=GEM_COLOR_NO&&b0&&b1&&b2)
			result = true;
		else if(y1==y2&&y2==y3&&y2!=GEM_COLOR_NO&&b1&&b2&&b3)
			result = true;
		else if(y2==y3&&y3==y4&&y3!=GEM_COLOR_NO&&b2&&b3&&b4)
			result =  true;

		if (result == false)
		{
			//判断结束点移动后有无可消除
			x0=tempGemInfo[targetX-2][targetY];
			x1=tempGemInfo[targetX-1][targetY];
			x2=tempGemInfo[targetX][targetY];
			x3=tempGemInfo[targetX+1][targetY];
			x4=tempGemInfo[targetX+2][targetY];

			posX = targetX - 2,posY = targetY - 2; //因为将数组扩大，所以使用前先缩小为原来坐标点
			b0=IsCanConnect(posX-2,posY);
			b1=IsCanConnect(posX-1,posY);
			b2=IsCanConnect(m_sourceX,m_sourceY);
			b3=IsCanConnect(posX+1,posY);
			b4=IsCanConnect(posX+2,posY);

			if (dire == UP_DIRECTION)
			{
				b3 = IsCanConnect(m_targetX,m_targetY);
			}
			else if (dire == DOWN_DIRECTION)
			{
				b1 = IsCanConnect(m_targetX,m_targetY);
			}

			if(x0==x1&&x1==x2&&x1 != GEM_COLOR_NO&&b0&&b1&&b2)
				result = true;
			else if(x1==x2&&x2==x3&&x2 != GEM_COLOR_NO&&b1&&b2&&b3)
				result = true;
			else if(x2==x3&&x3==x4&&x3 != GEM_COLOR_NO&&b2&&b3&&b4)
				result = true;

			y0=tempGemInfo[targetX][targetY-2];
			y1=tempGemInfo[targetX][targetY-1];
			y2=tempGemInfo[targetX][targetY];
			y3=tempGemInfo[targetX][targetY+1];
			y4=tempGemInfo[targetX][targetY+2];

			b0=IsCanConnect(posX,posY-2);
			b1=IsCanConnect(posX,posY-1);
			b2=IsCanConnect(m_sourceX,m_sourceY);
			b3=IsCanConnect(posX,posY+1);
			b4=IsCanConnect(posX,posY+2);

			if (dire == LEFT_DIRECTION)
			{
				b3 = IsCanConnect(m_targetX,m_targetY);
			}
			else if (dire == RIGHT_DIRECTION)
			{
				b1 = IsCanConnect(m_targetX,m_targetY);
			}

			if(	y0==y1&&y1==y2&&y1!=GEM_COLOR_NO&&b0&&b1&&b2)
				result = true;
			else if(y1==y2&&y2==y3&&y2!=GEM_COLOR_NO&&b1&&b2&&b3)
				result = true;
			else if(y2==y3&&y3==y4&&y3!=GEM_COLOR_NO&&b2&&b3&&b4)
				result =  true;
		}
	}
	else if (m_gameStatus == GAME_STATUS_USE_PROP)
	{
		result = true;
	}


	if (result)
	{
		if (flag)
		{
			if (m_gameStatus == GAME_STATUS_PLAYING)
			{
				saveLevelData();
			}

			swapGem(m_tileBrick[sourceX - 2][sourceY - 2].gem,m_tileBrick[targetX - 2][targetY - 2].gem);
			TileBrickData *pTileBrick = &m_tileBrick[sourceX - 2][sourceY - 2];
			TileBrickData *targetBrick = &m_tileBrick[targetX - 2][targetY - 2];

			bool bMove = false;
			for (SpecialBrickData::iterator ite = pTileBrick->specialBrick.begin(); ite != pTileBrick->specialBrick.end();)
			{
				if (IsCanMoveStumblingBlock(*ite))
				{
					ite->indexX = targetBrick->indexX;
					ite->indexY = targetBrick->indexY;
					targetBrick->specialBrick.push_back(*ite);
					ite = pTileBrick->specialBrick.erase(ite);
					bMove = true;
				}
				else
				{
					++ite;
				}
			}

			if (bMove == false)
			{
				for (SpecialBrickData::iterator ite = targetBrick->specialBrick.begin(); ite != targetBrick->specialBrick.end();)
				{
					if (IsCanMoveStumblingBlock(*ite))
					{
						ite->indexX = pTileBrick->indexX;
						ite->indexY = pTileBrick->indexY;
						pTileBrick->specialBrick.push_back(*ite);
						ite = targetBrick->specialBrick.erase(ite);
					}
					else
					{
						++ite;
					}
				}
			}
		}
	}
	else
	{
		m_sourceX = -1;m_sourceY = -1;
		m_targetX = -1;m_targetY = -1;
	}
	
	if (result && m_gameStatus == GAME_STATUS_PLAYING)
	{
		if (flag)
		{
			if (m_currentStep >0 ) m_currentStep--;
			m_currentComboNumber = 0;
			m_bSwapGem = true;
			m_swapGemStatus = NORMAL_SWAP_STATUS;
		}
		return NORMAL_SWAP_STATUS;
	}
	else
	{
		m_swapGemStatus = NULL_SWAP_STATUS;
		return NULL_SWAP_STATUS;
	}
}

bool CGameLogic::checkConnect(std::vector<cocos2d::Vec2 > &ConnectGemPos,bool flag/*=true*/)
{
	unsigned char ConnectCount=0;//连接个数;
	unsigned char tempCounterX=0;//临时计数器
	unsigned char tempCounterY=0;

	GemColor tempGemInfo[COL_COUNT][ROW_COUNT];
	memset(tempGemInfo,-1,sizeof(tempGemInfo));

	for(int i=0;i<ROW_COUNT;i++) 
		for(int j=0;j<COL_COUNT;j++)		
			tempGemInfo[i][j]=GemColor(m_tileBrick[i][j].gem.childType);

	//纵向检测 
	for(unsigned char y=0;y<ROW_COUNT;y++) 
	{
		for(unsigned char x=0;x<COL_COUNT-2;)
		{
			if(tempGemInfo[x][y]> GEM_COLOR_NO && IsCanConnect(x,y))
			{
				if(	tempGemInfo[x+1][y]==tempGemInfo[x][y]&& IsCanConnect(x+1,y)&&tempGemInfo[x+2][y]==tempGemInfo[x][y]&& IsCanConnect(x+2,y))
				{
					if (flag == false)
					{
						return true;
					}
					addVectorData(ConnectGemPos,cocos2d::Vec2(x,y));
					addVectorData(ConnectGemPos,cocos2d::Vec2(x+1,y));
					addVectorData(ConnectGemPos,cocos2d::Vec2(x+2,y));
					tempCounterX+=3;//下次不再检测此3个棋子
					if(x+3<COL_COUNT) if(tempGemInfo[x+3][y]==tempGemInfo[x][y]&& IsCanConnect(x+3,y))//第4连
					{
						addVectorData(ConnectGemPos,cocos2d::Vec2(x+3,y));
						tempCounterX++;//增加计数
						if(x+4<COL_COUNT) if(tempGemInfo[x+4][y]==tempGemInfo[x][y]&& IsCanConnect(x+4,y))//第5连
						{
							addVectorData(ConnectGemPos,cocos2d::Vec2(x+4,y));

							tempCounterX++;//增加计数
							if(x+5<COL_COUNT) if(tempGemInfo[x+5][y]==tempGemInfo[x][y]&& IsCanConnect(x+5,y))//第6连
							{
								addVectorData(ConnectGemPos,cocos2d::Vec2(x+5,y));
								tempCounterX++;//增加计数
								if(x+6<COL_COUNT) if(tempGemInfo[x+6][y]==tempGemInfo[x][y]&& IsCanConnect(x+6,y))//第7连
								{
									addVectorData(ConnectGemPos,cocos2d::Vec2(x+6,y));
									tempCounterX++;//增加计数
									if(x+7<COL_COUNT) if(tempGemInfo[x+7][y]==tempGemInfo[x][y]&& IsCanConnect(x+7,y))//第8连
									{
										addVectorData(ConnectGemPos,cocos2d::Vec2(x+7,y));
										tempCounterX++;//增加计数
										if(x+8<COL_COUNT) if(tempGemInfo[x+8][y]==tempGemInfo[x][y]&& IsCanConnect(x+8,y))//第9连
										{
											addVectorData(ConnectGemPos,cocos2d::Vec2(x+8,y));
											tempCounterX++;//增加计数
										}
									}
								}	
							}
						}
					}
				}
			}
			if(tempCounterX>0) x+=tempCounterX;//跳过已检测的棋子,避免重复检测 (例如7连时,7连内会包含6连5连4连这样)
			else x++;
			tempCounterX=0;
		}
	}

	//横向检测
	tempCounterY=0;
	for(unsigned char x=0;x<ROW_COUNT;x++)
	{
		for(unsigned char y=0;y<COL_COUNT-2;)
		{
			if(tempGemInfo[x][y]> GEM_COLOR_NO&&IsCanConnect(x,y))
			{
				if(	tempGemInfo[x][y+1]==tempGemInfo[x][y]&&IsCanConnect(x,y+1)&&tempGemInfo[x][y+2]==tempGemInfo[x][y]&&IsCanConnect(x,y+2))
				{
					if (flag == false)
					{
						return true;
					}
					addVectorData(ConnectGemPos,cocos2d::Vec2(x,y));
					addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+1));
					addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+2));
					tempCounterY+=3;
					if(y+3<COL_COUNT) if(tempGemInfo[x][y+3]==tempGemInfo[x][y]&&IsCanConnect(x,y+3))//第4连
					{
						addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+3));
						tempCounterY++;
						if(y+4<COL_COUNT) if(tempGemInfo[x][y+4]==tempGemInfo[x][y]&&IsCanConnect(x,y+4))//第5连	
						{
							addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+4));
							tempCounterY++;
							if(y+5<COL_COUNT) if(tempGemInfo[x][y+5]==tempGemInfo[x][y]&&IsCanConnect(x,y+5))//第6连
							{
								addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+5));
								tempCounterY++;
								if(y+6<COL_COUNT) if(tempGemInfo[x][y+6]==tempGemInfo[x][y]&&IsCanConnect(x,y+6))//第7连
								{
									addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+6));
									tempCounterY++;//增加计数
									if(y+7<COL_COUNT) if(tempGemInfo[x][y+7]==tempGemInfo[x][y]&&IsCanConnect(x,y+7))//第8连
									{
										addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+7));
										tempCounterY++;//增加计数
										if(y+8<COL_COUNT) if(tempGemInfo[x][y+8]==tempGemInfo[x][y]&&IsCanConnect(x,y+8))//第9连
										{
											addVectorData(ConnectGemPos,cocos2d::Vec2(x,y+8));
											tempCounterY++;//增加计数
										}
									}
								}	
							}
						}
					}
				}
			}
			if(tempCounterY>0) y+=tempCounterY;
			else y++;
			tempCounterY=0;
		}
	}

	if (ConnectGemPos.size() > 0 && flag == true)
	{
		m_currentComboNumber++;
		return true;
	}

	return false;
}

void CGameLogic::ObtainSpecialGems(std::vector<ObtainSpecialGemInfo> &outInfos, std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos)
{
	if (connectGemPos.empty()) return;

	outInfos.clear();
	std::vector<ObtainSpecialGemInfo> specialGemInfoV;

	std::vector<cocos2d::Vec2> pGemPos;
	pGemPos.insert(pGemPos.begin(),connectGemPos.begin(),connectGemPos.end());

	//交换前的点位
	std::vector<cocos2d::Vec2> outPointS;
	cocos2d::Vec2 rcCount = getROWAndCOLCount(cocos2d::Vec2(m_sourceX,m_sourceY),outPointS, connectGemPos);
	GemStatus  status = getSpecialGemType(rcCount);
	if (status > GEM_STATUS_NORMAL)
	{
		ObtainSpecialGemInfo info;
		info.flag = status;

		tagGemData sourceGem = m_tileBrick[m_sourceX][m_sourceY].gem;
		if (sourceGem.gemStatus == GEM_STATUS_NORMAL || (sourceGem.gemStatus > GEM_STATUS_COLORFUL && sourceGem.gemStatus <= GEM_STATUS_TREASURE_CHEST))
		{
			info.point = cocos2d::Vec2(m_sourceX,m_sourceY);
		}
		else
		{
			int index;
			do 
			{
				index = rand() % outPointS.size();
				tagGemData gem = m_tileBrick[(int)outPointS[index].x][(int)outPointS[index].y].gem;
				if ((outPointS[index].x != m_sourceX || outPointS[index].y != m_sourceY) && (gem.gemStatus == GEM_STATUS_NORMAL || (gem.gemStatus > GEM_STATUS_COLORFUL && gem.gemStatus <= GEM_STATUS_TREASURE_CHEST)))
				{
					break;
				}
			} while (true);
			info.point = outPointS[index];
		}
		addVectorData(specialGemInfoV,info);

		for (size_t i = 0; i < outPointS.size(); ++i)
		{
			std::vector<cocos2d::Vec2>::const_iterator ite = find(pGemPos.begin(),pGemPos.end(),outPointS[i]);
			if (ite != pGemPos.end())
			{
				pGemPos.erase(ite);
			}
		}
	}
	else
	{
		std::vector<cocos2d::Vec2>::const_iterator sourceIte = find(pGemPos.begin(),pGemPos.end(),cocos2d::Vec2(m_sourceX,m_sourceY));
		if (sourceIte != pGemPos.end())
		{
			pGemPos.erase(sourceIte);
		}
	}

	//交换后的点位
	std::vector<cocos2d::Vec2> outPointT;
	rcCount = getROWAndCOLCount(cocos2d::Vec2(m_targetX,m_targetY),outPointT,connectGemPos);
	status = getSpecialGemType(rcCount);
	if (status > GEM_STATUS_NORMAL)
	{
		ObtainSpecialGemInfo info;
		info.flag = status;
		tagGemData targetGem = m_tileBrick[m_targetX][m_targetY].gem;
		if (targetGem.gemStatus == GEM_STATUS_NORMAL || (targetGem.gemStatus > GEM_STATUS_COLORFUL && targetGem.gemStatus <= GEM_STATUS_TREASURE_CHEST))
		{
			info.point = cocos2d::Vec2(m_targetX,m_targetY);
		}
		else
		{
			int index;
			do 
			{
				index = rand() % outPointT.size();
				tagGemData gem = m_tileBrick[(int)outPointT[index].x][(int)outPointT[index].y].gem;
				if ((outPointT[index].x != m_targetX || outPointT[index].y != m_targetY) && (gem.gemStatus == GEM_STATUS_NORMAL || (gem.gemStatus > GEM_STATUS_COLORFUL && gem.gemStatus <= GEM_STATUS_TREASURE_CHEST)))
				{
					break;
				}
			} while (true);
			info.point = outPointT[index];
		}
		addVectorData(specialGemInfoV,info);
		for (size_t i = 0; i < outPointT.size(); ++i)
		{
			std::vector<cocos2d::Vec2>::const_iterator ite = find(pGemPos.begin(),pGemPos.end(),outPointT[i]);
			if (ite != pGemPos.end())
			{
				pGemPos.erase(ite);
			}
		}
	}
	else
	{
		std::vector<cocos2d::Vec2>::const_iterator targetIte = find(pGemPos.begin(),pGemPos.end(),cocos2d::Vec2(m_targetX,m_targetY));
		if (targetIte != pGemPos.end())
		{
			pGemPos.erase(targetIte);
		}
	}

	while (!pGemPos.empty())
	{
		std::vector<cocos2d::Vec2>::const_iterator ite = pGemPos.begin();
		std::vector<cocos2d::Vec2> outPoint;
		cocos2d::Vec2 point(ite->x,ite->y);
		rcCount = getROWAndCOLCount(point,outPoint,connectGemPos);
		GemStatus  status = getSpecialGemType(rcCount);
		if (status > GEM_STATUS_NORMAL)
		{
			ObtainSpecialGemInfo info;
			info.flag = status;
			tagGemData pGem = m_tileBrick[(int)point.x][(int)point.y].gem;
			if (pGem.gemStatus == GEM_STATUS_NORMAL || (pGem.gemStatus > GEM_STATUS_COLORFUL && pGem.gemStatus <= GEM_STATUS_TREASURE_CHEST))
			{
				info.point = cocos2d::Vec2(point.x,point.y);
			}
			else
			{
				int ite;
				do 
				{
					ite = rand() % outPoint.size();
					tagGemData gem = m_tileBrick[(int)outPoint[ite].x][(int)outPoint[ite].y].gem;
					if ((outPoint[ite].x != point.x || outPoint[ite].y != point.y) && (gem.gemStatus == GEM_STATUS_NORMAL || gem.gemStatus > GEM_STATUS_COLORFUL && gem.gemStatus <= GEM_STATUS_TREASURE_CHEST))
					{
						break;
					}
				} while (true);
				info.point = outPoint[ite];
			}
			addVectorData(specialGemInfoV,info);
			for (size_t i = 0; i < outPoint.size(); ++i)
			{
				std::vector<cocos2d::Vec2>::const_iterator ite = find(pGemPos.begin(),pGemPos.end(),outPoint[i]);
				if (ite != pGemPos.end())
				{
					pGemPos.erase(ite);
				}
			}
		}
		else
		{
			std::vector<cocos2d::Vec2>::const_iterator posIte = find(pGemPos.begin(),pGemPos.end(),point);
			if (posIte != pGemPos.end())
			{
				pGemPos.erase(posIte);
			}
		}
	}

	removeStumblingBlock(connectGemPos,StumblingBlockPos);

	for (int i = 0; i < (int)specialGemInfoV.size();++i)
	{
		ObtainSpecialGemInfo spGemInfo = specialGemInfoV[i];

		if (find(outInfos.begin(),outInfos.end(),spGemInfo) == outInfos.end())
		{
			outInfos.push_back(spGemInfo);
			int posX = spGemInfo.point.x;
			int posY = spGemInfo.point.y;
			m_tileBrick[posX][posY].gem.gemStatus = spGemInfo.flag;
			if (spGemInfo.flag == GEM_STATUS_COLORFUL)
			{
				m_tileBrick[posX][posY].gem.childType = GEM_COLOR_NO;
			}

			std::vector<cocos2d::Vec2>::const_iterator ite;
			ite = find(connectGemPos.begin(),connectGemPos.end(),cocos2d::Vec2(spGemInfo.point.x,spGemInfo.point.y));
			if (ite != connectGemPos.end())
			{
				connectGemPos.erase(ite);
			}
		}
	}
}

/*
@point 检索的起始点位
@outPoint 被检索的所有点位
@return 从起始点位开始的横竖相连颜色的个数
*/
cocos2d::Vec2 CGameLogic::getROWAndCOLCount(cocos2d::Vec2 point,std::vector<cocos2d::Vec2> &outPoint, std::vector<cocos2d::Vec2> &connectGemPos)
{
	if (point.x < 0 || point.y <0)
	{
		return cocos2d::Vec2(0,0);
	}
	int countX = 0, countY = 0;
	int sourceX = point.x, sourceY = point.y;
	TileBrickData &tileBrick = m_tileBrick[sourceX][sourceY];
	for (int x = sourceX + 1; x < COL_COUNT; ++x)
	{
		TileBrickData &item = m_tileBrick[x][sourceY];
		std::vector<cocos2d::Vec2>::const_iterator ite = find(connectGemPos.begin(),connectGemPos.end(),cocos2d::Vec2(x,sourceY));
		if (tileBrick.gem.childType == item.gem.childType && item.gem.childType > GEM_COLOR_NO && ite != connectGemPos.end())
		{
			countX++;
			addVectorData(outPoint,cocos2d::Vec2(x,sourceY));
		}
		else
		{
			break;
		}
	}

	for (int x = sourceX ; x >= 0; --x)
	{
		TileBrickData &item = m_tileBrick[x][sourceY];
		std::vector<cocos2d::Vec2>::const_iterator ite = find(connectGemPos.begin(),connectGemPos.end(),cocos2d::Vec2(x,sourceY));
		if (tileBrick.gem.childType == item.gem.childType && item.gem.childType > GEM_COLOR_NO && ite != connectGemPos.end())
		{
			countX++;
			addVectorData(outPoint,cocos2d::Vec2(x,sourceY));
		}
		else
		{
			break;
		}
	}

	for (int y = sourceY + 1; y < COL_COUNT; ++y)
	{
		TileBrickData &item = m_tileBrick[sourceX][y];
		std::vector<cocos2d::Vec2>::const_iterator ite = find(connectGemPos.begin(),connectGemPos.end(),cocos2d::Vec2(sourceX,y));
		if (tileBrick.gem.childType == item.gem.childType && item.gem.childType > GEM_COLOR_NO && ite != connectGemPos.end())
		{
			countY++;
			addVectorData(outPoint,cocos2d::Vec2(sourceX,y));
		}
		else
		{
			break;
		}
	}

	for (int y = sourceY ; y >= 0; --y)
	{
		TileBrickData &item = m_tileBrick[sourceX][y];
		std::vector<cocos2d::Vec2>::const_iterator ite = find(connectGemPos.begin(),connectGemPos.end(),cocos2d::Vec2(sourceX,y));
		if (tileBrick.gem.childType == item.gem.childType && item.gem.childType > GEM_COLOR_NO && ite != connectGemPos.end())
		{
			countY++;
			addVectorData(outPoint,cocos2d::Vec2(sourceX,y));
		}
		else
		{
			break;
		}
	}

	return cocos2d::Vec2(countX,countY);
}

void CGameLogic::touchSpecialGems(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos,std::vector<ObtainSpecialGemInfo> &outInfos)
{
	std::vector<cocos2d::Vec2> position; //记录检索过的特殊宝石
	do 
	{
		
		TileBrickData sourceItem;
		if (m_sourceX != -1 && m_sourceY != -1)
		{
			sourceItem = m_tileBrick[m_sourceX][m_sourceY];
		}
		TileBrickData targetItem;
		if (m_targetX != -1 && m_targetY != -1)
		{
			targetItem = m_tileBrick[m_targetX][m_targetY];
		}
		if (m_swapGemStatus == SPECIAL_SWAP_STATUS && sourceItem.gem.gemStatus == GEM_STATUS_COLORFUL && targetItem.gem.gemStatus == GEM_STATUS_COLORFUL)
		{
			break;				
		}

		bool result = true;
		int num = connectGemPos.size();
		for (int i = 0;i < num;++i)
		{
			cocos2d::Vec2 point = connectGemPos[i];
			int posX = point.x;
			int posY = point.y;
			TileBrickData &pTile = m_tileBrick[posX][posY];
			if (pTile.gem.gemStatus > GEM_STATUS_NORMAL && pTile.gem.gemStatus <= GEM_STATUS_COLORFUL)
			{
				bool IsCheck = find(position.begin(),position.end(),point) == position.end()?false:true;
				if (IsCheck == false)
				{
					if (m_swapGemStatus == SPECIAL_SWAP_STATUS && sourceItem.gem.gemStatus == GEM_STATUS_BOM && targetItem.gem.gemStatus == GEM_STATUS_BOM)
					{
						if (point == cocos2d::Vec2(m_sourceX,m_sourceY) || point == cocos2d::Vec2(m_sourceX,m_sourceY))
						{
							position.push_back(point);
							continue;
						}
					}
					result = false;
					SpecialEffectsToEliminateGems(pTile,connectGemPos,StumblingBlockPos);
					position.push_back(point);
				}
			}
		}

		if (result)
		{
			break;
		}
	} while (true);

	changeTargetNum(connectGemPos,StumblingBlockPos,outInfos);
}

void CGameLogic::removeStumblingBlock(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos)
{
	for (std::vector<cocos2d::Vec2>::const_iterator ite = connectGemPos.begin();ite != connectGemPos.end();++ite)
	{
		int posX = ite->x,posY = ite->y;

		int topX = (posX - 1) < 0?0:(posX-1), topY = posY;
		int bottomX = (posX + 1) > (ROW_COUNT - 1)?(ROW_COUNT - 1):(posX+1), bottomY = posY;
		int leftX = posX, leftY = (posY - 1) < 0?0:(posY - 1);
		int rightX = posX, rightY = posY + 1 > (COL_COUNT - 1)?(COL_COUNT - 1):(posY + 1);

		TileBrickData *pTileBrick;

		pTileBrick = currentTileBrickItem(posX,posY);
		removeSpecialBrickItm(posX,posY,pTileBrick,StumblingBlockPos);

		pTileBrick = currentTileBrickItem(topX,topY);
		if (pTileBrick != NULL)
		{
			removeSpecialBrickItm(posX,posY,pTileBrick,StumblingBlockPos);
		}
		
		pTileBrick = currentTileBrickItem(bottomX,bottomY);
		if (pTileBrick != NULL)
		{
			removeSpecialBrickItm(posX,posY,pTileBrick,StumblingBlockPos);
		}

		pTileBrick = currentTileBrickItem(leftX,leftY);
		if (pTileBrick != NULL)
		{
			removeSpecialBrickItm(posX,posY,pTileBrick,StumblingBlockPos);
		}

		pTileBrick = currentTileBrickItem(rightX,rightY);
		if (pTileBrick != NULL)
		{
			removeSpecialBrickItm(posX,posY,pTileBrick,StumblingBlockPos);
		}
	}
}

bool CGameLogic::IsCanRemoveStumblingBlock(int posX,int posY,tagSpecialBrickData &pStumblingBlock)
{
	if (pStumblingBlock.childType <= STUMBLING_BLOCK_NULL || pStumblingBlock.childType >= STUMBLING_BLOCK_COUNT)
	{
		return false;
	}

	bool result = false;

	switch (pStumblingBlock.childType)
	{
	case STUMBLING_BLOCK_GOLD_POD:
	case STUMBLING_BLOCK_PARTITION:
	case STUMBLING_BLOCK_PORTAL:
		result = false;
		break;
	case STUMBLING_BLOCK_GLASS:
	case STUMBLING_BLOCK_STENCIL:
		{
			if (pStumblingBlock.indexX == posX && pStumblingBlock.indexY == posY)
			{
				result = true;
			}
		}
		break;
	default:
		result = true;
		break;
	}

	return result;
}

/*
@posX 当前格子X
@posY 当前格子Y
@*pTileBrick 将要消除的障碍物的格子
@&StumblingBlockPos 存储消除的障碍物
*/
void CGameLogic::removeSpecialBrickItm(int posX,int posY,TileBrickData *pTileBrick,SpecialBrickData &StumblingBlockPos)
{
	if (pTileBrick == NULL || pTileBrick->specialBrick.empty())
	{
		return;
	}
	tagSpecialBrickData *item = getRemoveSpecialBrickItem(pTileBrick->indexX,pTileBrick->indexY);
	if (pTileBrick->isShow == true && item != NULL && item->number > 0 && IsCanRemoveStumblingBlock(posX,posY,*item) && item->childType != STUMBLING_BLOCK_GOLD_POD)
	{
		if (item->number > 0)
		{
			if ((item->childType == STUMBLING_BLOCK_GLASS || item->childType == STUMBLING_BLOCK_STENCIL) && (posX != item->indexX || posY != item->indexY))
			{
				return;
			}

			item->number--;
			StumblingBlockPos.push_back(*item);
			if (item->number <= 0 /*&& item->childType == STUMBLING_BLOCK_STENCIL || item->childType == STUMBLING_BLOCK_BULB*/)
			{
				pTileBrick->gem.bMove = true;
			}

			if (item->childType == STUMBLING_BLOCK_JELLY)
			{
				m_bDisappearJelly = true;
			}
		}
	}
}

bool CGameLogic::IsCanMoveStumblingBlock(tagSpecialBrickData &pStumblingBlock)
{
	if (pStumblingBlock.childType <= STUMBLING_BLOCK_NULL || pStumblingBlock.childType >= STUMBLING_BLOCK_COUNT)
	{
		return false;
	}

	switch(pStumblingBlock.childType)
	{
	case STUMBLING_BLOCK_GOLD_POD:
	case STUMBLING_BLOCK_SILVER_COIN:
		return true;
	default:
		break;
	}

	return false;
}

bool CGameLogic::DropGem(std::vector<cocos2d::Vec2 > &dropPos, GemData &pGemList)
{
// 	do 
// 	{
// 		bool result = false;
// 		for (int posX = COL_COUNT-1; posX >= 0;--posX)
// 		{
// 			for (int posY = ROW_COUNT-1; posY >= 0;--posY)
// 			{
// 
// 				TileBrickData *pTileBrick = &m_tileBrick[posX][posY];
// 
// 				if (pTileBrick->isShow == false || 
// 					(pTileBrick->gem.bMove == false && (pTileBrick->gem.childType > GEM_COLOR_NO||
// 					(pTileBrick->gem.childType <= GEM_COLOR_NO&&pTileBrick->gem.gemStatus == GEM_STATUS_COLORFUL))))
// 				{
// 					continue;
// 				}
// 
// 				TileBrickData *bottomBrick = currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY);
// 
// 				tagSpecialBrickData downPortal;
// 				if(IsHaveDownPortal(pTileBrick->indexX,pTileBrick->indexY,&downPortal))
// 				{	
// 					tagSpecialBrickData *pItem = getAnotherPortal(downPortal,true);
// 					if (pItem != NULL)
// 					{
// 						bottomBrick = currentTileBrickItem(pItem->indexX,pItem->indexY);
// 					}
// 				}
// 
// 				TileBrickData *leftBottomBrick = currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY - 1);
// 				TileBrickData *rightBottomBrick = currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY + 1);
// 
// 				if (dropBottom(pTileBrick,bottomBrick,dropPos) == false)
// 				{
// 					if (dropLeftBottom(pTileBrick,leftBottomBrick,dropPos) == false)
// 					{
// 						if (dropRightBottom(pTileBrick,rightBottomBrick,dropPos) == false)
// 						{
// 							if (!result)
// 							{
// 								result = false;
// 							}
// 						}
// 						else
// 						{
// 							result = true;
// 							//showCurrentTile();
// 						}
// 					}
// 					else
// 					{
// 						result = true;
// 						//showCurrentTile();
// 					}
// 				}
// 				else
// 				{
// 					result = true;
// 					//showCurrentTile();
// 				}
// 
// 				if (result == false)
// 				{
// 					result = addNewGem(pTileBrick,pGemList);
// 				}
// 				else
// 				{
// 					addNewGem(pTileBrick,pGemList);
// 				}
// 			}
// 		}
// 
// 		if (result == false)
// 		{
// 			return false;
// 		}
// 	} while (true);
	return false;
}

bool CGameLogic::dropBottom(TileBrickData *pTileBrick,TileBrickData *bottomBrick,cocos2d::Vec2 &dropPos)
{
	bool result = false;

	if (bottomBrick != NULL)
	{
		if (bottomBrick->isShow == true)
		{
			if (IsCanSwapGem(*pTileBrick,*bottomBrick,true) && 
				(pTileBrick->gem.childType > GEM_COLOR_NO || pTileBrick->gem.childType == GEM_COLOR_NO && pTileBrick->gem.gemStatus == GEM_STATUS_COLORFUL)
				&& pTileBrick->gem.bMove == true && pTileBrick->gem.parentType != STUMBLING_BLOCK_PARENT_TYPE)
			{
				dropPos = cocos2d::Vec2(bottomBrick->indexX, bottomBrick->indexY);
				pTileBrick->gem.moveAction = MOVE_ACTION;
				swapGem(pTileBrick->gem,bottomBrick->gem);
				result = true;
			}
			else if (pTileBrick->specialBrick.empty() == false)
			{
				for (SpecialBrickData::iterator ite = pTileBrick->specialBrick.begin(); ite != pTileBrick->specialBrick.end();)
				{
					if (IsCanMoveStumblingBlock(*ite) && bottomBrick->gem.childType <= GEM_COLOR_NO && bottomBrick->gem.gemStatus != GEM_STATUS_COLORFUL && IsCanCreateGem(*bottomBrick))
					{ 
						dropPos = cocos2d::Vec2(bottomBrick->indexX, bottomBrick->indexY);
						ite->indexX = bottomBrick->indexX;
						ite->indexY = bottomBrick->indexY;
						bottomBrick->specialBrick.push_back(*ite);
						bottomBrick->gem.bMove = true;
						ite = pTileBrick->specialBrick.erase(ite);
						pTileBrick->gem.bMove = true;
						pTileBrick->gem.moveAction = MOVE_ACTION;
						swapGem(pTileBrick->gem, bottomBrick->gem);
						result = true;
					}
					else
					{
						++ite;
					}
				}
			}
		}
	}

	return result;
}

bool CGameLogic::dropLeftBottom(TileBrickData *pTileBrick,TileBrickData *leftBottomBrick,cocos2d::Vec2 &dropPos)
{
	if (leftBottomBrick != NULL)
	{
		if (leftBottomBrick->isShow == true)
		{
			if (IsCanSwapGem(*pTileBrick,*leftBottomBrick, false) 
				&& (pTileBrick->gem.childType > GEM_COLOR_NO || pTileBrick->gem.childType == GEM_COLOR_NO && pTileBrick->gem.gemStatus == GEM_STATUS_COLORFUL) 
				&& pTileBrick->gem.bMove == true && IsHaveGemDrop(leftBottomBrick->indexY,pTileBrick) == false)
			{
				dropPos = cocos2d::Vec2(leftBottomBrick->indexX, leftBottomBrick->indexY);
				pTileBrick->gem.moveAction = MOVE_ACTION;
				swapGem(pTileBrick->gem,leftBottomBrick->gem);
				return true;
			}
			else if(IsHaveGemDrop(leftBottomBrick->indexY,pTileBrick) == false)
			{
				for (SpecialBrickData::iterator ite = pTileBrick->specialBrick.begin(); ite != pTileBrick->specialBrick.end();)
				{
					if (IsCanMoveStumblingBlock(*ite) && leftBottomBrick->gem.childType <= GEM_COLOR_NO && leftBottomBrick->gem.gemStatus != GEM_STATUS_COLORFUL && IsCanCreateGem(*leftBottomBrick)
						&&IsHaveUpPortal(leftBottomBrick->indexX,leftBottomBrick->indexY) == false)
					{
						dropPos = cocos2d::Vec2(leftBottomBrick->indexX, leftBottomBrick->indexY);
						ite->indexX = leftBottomBrick->indexX;
						ite->indexY = leftBottomBrick->indexY;
						leftBottomBrick->specialBrick.push_back(*ite);
						ite = pTileBrick->specialBrick.erase(ite);
						pTileBrick->gem.bMove = true;
						pTileBrick->gem.moveAction = MOVE_ACTION;
						swapGem(pTileBrick->gem, leftBottomBrick->gem);
						return true;
					}
					else
					{
						++ite;
					}
				}
			}
		}
	}

	return false;
}

bool CGameLogic::dropRightBottom(TileBrickData *pTileBrick,TileBrickData *rightBottomBrick,cocos2d::Vec2 &dropPos)
{
	if (rightBottomBrick != NULL)
	{
		if (rightBottomBrick->isShow == true)
		{
			if (IsCanSwapGem(*pTileBrick,*rightBottomBrick, false) 
				&& (pTileBrick->gem.childType > GEM_COLOR_NO || pTileBrick->gem.childType == GEM_COLOR_NO && pTileBrick->gem.gemStatus == GEM_STATUS_COLORFUL) 
				&& pTileBrick->gem.bMove == true && IsHaveGemDrop(rightBottomBrick->indexY,pTileBrick) == false)
			{
				dropPos = cocos2d::Vec2(rightBottomBrick->indexX, rightBottomBrick->indexY);
				pTileBrick->gem.moveAction = MOVE_ACTION;
				swapGem(pTileBrick->gem,rightBottomBrick->gem);
				return true;
			}
			else if(IsHaveGemDrop(rightBottomBrick->indexY,pTileBrick) == false)
			{
				for (SpecialBrickData::iterator ite = pTileBrick->specialBrick.begin(); ite != pTileBrick->specialBrick.end();)
				{
					if (IsCanMoveStumblingBlock(*ite) && rightBottomBrick->gem.childType <= GEM_COLOR_NO && rightBottomBrick->gem.gemStatus != GEM_STATUS_COLORFUL&& IsCanCreateGem(*rightBottomBrick)
						&&IsHaveUpPortal(rightBottomBrick->indexX,rightBottomBrick->indexY) == false)
					{
						dropPos = cocos2d::Vec2(rightBottomBrick->indexX, rightBottomBrick->indexY);
						ite->indexX = rightBottomBrick->indexX;
						ite->indexY = rightBottomBrick->indexY;
						rightBottomBrick->specialBrick.push_back(*ite);
						ite = pTileBrick->specialBrick.erase(ite);
						pTileBrick->gem.bMove = true;
						pTileBrick->gem.moveAction = MOVE_ACTION;
						swapGem(pTileBrick->gem, rightBottomBrick->gem);
						return true;
					}
					else
					{
						++ite;
					}
				}
			}
		}
	}

	return false;
}

bool CGameLogic::addNewGem(TileBrickData *pTileBrick, tagGemData &pGemList)
{
	if (pTileBrick->isShow == true && pTileBrick->gem.childType <= GEM_COLOR_NO && pTileBrick->gem.gemStatus != GEM_STATUS_COLORFUL)
	{
		if (IsCanCreateGem(*pTileBrick))
		{
			if (IsHaveUpPortal(pTileBrick->indexX,pTileBrick->indexY) == false)
			{
				TileBrickData *upBrick = currentTileBrickItem(pTileBrick->indexX - 1, pTileBrick->indexY);
				if ((upBrick == NULL || (upBrick!= NULL && upBrick->indexX <= getMinRowNumber(upBrick->indexY))) && IsCanAddGem(upBrick))
				{
					tagGemData addGem = randomCreateGem(m_levelsGemList);
					addGem.id = pTileBrick->indexX*COL_COUNT+pTileBrick->indexY;
					addGem.indexX = pTileBrick->indexX;
					addGem.indexY = pTileBrick->indexY;
					addGem.moveAction = MOVE_ACTION;
					pTileBrick->gem = addGem;
					pGemList = addGem;
					pGemList.parentType = NORMAL_GEM_PARENT_TYPE;
					addGoldPod(pGemList);
					return true;
				}
			}
		}
	}

	return false;
}

bool CGameLogic::IsHaveGemDrop(int posY,TileBrickData *pTileBrick,bool flag/* = true*/)
{
	bool result = false;
	int posX = pTileBrick->indexX;
	for (int x = posX;x >= 0;--x)
	{
		pTileBrick = &m_tileBrick[x][posY];
		tagSpecialBrickData downPortal;
		if (pTileBrick->indexX >= getMinRowNumber(posY))
		{
			if (pTileBrick->isShow == false )
			{
				return false;
			}

			if (IsHaveDownPortal(x,posY,&downPortal))
			{
				tagSpecialBrickData *item = getAnotherPortal(downPortal,true);
				if (item != NULL && item->indexY != posY && flag)
				{
					return false;
				}
				else if (item != NULL && item->indexY == posY && item->indexX > posX && flag)
				{
					return false;
				}
			}

			if (IsCanMoveGem(x,posY) == false)
			{
				return false;
			}
		}

		tagSpecialBrickData upPortal;
// 		if (pTileBrick->gem.childType <= GEM_COLOR_NO && pTileBrick->gem.flag != GEM_STATUS_COLORFUL && IsHaveUpPortal(pTileBrick->posX,pTileBrick->posY,&upPortal) == false)
// 		{
// 			continue;
// 		}

		TileBrickData *bottomBrick = currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY);
		if(IsHaveDownPortal(pTileBrick->indexX,pTileBrick->indexY,&downPortal))
		{	
			tagSpecialBrickData *pItem = getAnotherPortal(downPortal,true);
			if (pItem != NULL)
			{
				bottomBrick = currentTileBrickItem(pItem->indexX,pItem->indexY);
			}
		}
		if (bottomBrick != NULL)
		{
			if (bottomBrick->isShow == true)
			{
				if (IsCanSwapGem(*pTileBrick,*bottomBrick,true) && 
					(pTileBrick->gem.childType > GEM_COLOR_NO || pTileBrick->gem.childType == GEM_COLOR_NO && pTileBrick->gem.gemStatus == GEM_STATUS_COLORFUL)
					&& pTileBrick->gem.bMove == true)
				{
					result = true;
				}
				else if (pTileBrick->specialBrick.empty() == false)
				{
					for (SpecialBrickData::iterator ite = pTileBrick->specialBrick.begin(); ite != pTileBrick->specialBrick.end(); ++ite)
					{
						if (IsCanMoveStumblingBlock(*ite) && bottomBrick->gem.childType <= GEM_COLOR_NO && bottomBrick->gem.gemStatus != GEM_STATUS_COLORFUL && IsCanCreateGem(*bottomBrick))
						{ 
							result = true;
						}
					}
				}
			}
		}

		if (pTileBrick->isShow == true && pTileBrick->gem.childType <= GEM_COLOR_NO && pTileBrick->gem.gemStatus != GEM_STATUS_COLORFUL)
		{					
			if (IsCanCreateGem(*pTileBrick))
			{
				TileBrickData *upBrick = currentTileBrickItem(pTileBrick->indexX - 1, pTileBrick->indexY);
				if (pTileBrick->gem.gemStatus <= GEM_STATUS_NORMAL && IsHaveUpPortal(pTileBrick->indexX,pTileBrick->indexY) == false)
				{
					if ((upBrick == NULL || (upBrick!= NULL && upBrick->indexX <= getMinRowNumber(upBrick->indexY))) && IsCanAddGem(upBrick))
					{
						result = true;
					}
				}
			}
		}

		if (result)
		{
			return true;
		}
		else
		{
			if (IsHaveUpPortal(pTileBrick->indexX,pTileBrick->indexY,&upPortal))
			{
				tagSpecialBrickData *pItem = getAnotherPortal(upPortal,false);
				if (pItem != NULL /*&& pItem->posY != posY*/)
				{
					if (IsHaveGemDrop(pItem->indexY,&m_tileBrick[pItem->indexX][pItem->indexY],false))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

TileBrickData* CGameLogic::currentTileBrickItem(int posX,int posY)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1)
	{
		return NULL;
	}
	return &m_tileBrick[posX][posY];
}

TileBrickData* CGameLogic::leftTileBriteItem(int posX,int posY,TileBrickData *outTileBrick/* = NULL*/)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1 || posY - 1 < 0)
	{
		return NULL;
	}

	if (outTileBrick != NULL)
		*outTileBrick = m_tileBrick[posX][posY - 1];

	return &m_tileBrick[posX][posY - 1];
}

TileBrickData* CGameLogic::rightTileBriteItem(int posX,int posY,TileBrickData *outTileBrick/* = NULL*/)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1 || posY + 1 > COL_COUNT - 1)
	{
		return NULL;
	}

	if (outTileBrick != NULL)
		*outTileBrick = m_tileBrick[posX][posY + 1];

	return &m_tileBrick[posX][posY + 1];
}

TileBrickData* CGameLogic::upTileBriteItem(int posX,int posY,TileBrickData *outTileBrick /*= NULL*/)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1 || posX - 1 < 0)
	{
		return NULL;
	}

	if (outTileBrick != NULL)
		*outTileBrick = m_tileBrick[posX - 1][posY];

	return &m_tileBrick[posX - 1][posY];
}

TileBrickData* CGameLogic::bottomTileBriteItem(int posX,int posY,TileBrickData *outTileBrick/* = NULL*/)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1 || posX + 1 > ROW_COUNT-1)
	{
		return NULL;
	}

	if (outTileBrick != NULL)
		*outTileBrick = m_tileBrick[posX + 1][posY];

	return &m_tileBrick[posX + 1][posY];
}

bool CGameLogic::IsCanSwapGem(const TileBrickData &tile,const TileBrickData &pTile,bool flag /*= true*/)
{
	enBrickType type = IsHaveGemOrStumbling(pTile);
	if ( type == NO_GEM_AND_BRICK_TYPE && pTile.isShow == true)
	{
		return true;
	}
	else if (type == GEM_AND_BRICK_TYPE || type == GEM_TYPE)
	{
		return false;
	}
	else if (type == BRICK_TYPE)
	{
		if (IsCanCreateGem(pTile))
		{
			if (IsHaveUpPortal(pTile.indexX,pTile.indexY) == false)
			{
				bool result = true;
// 				for (size_t i = 0; i < pTile.specialBrick.size();++i)
// 				{
// 					tagSpecialBrickData pItem = pTile.specialBrick[i];
// 					if (pItem.childType == STUMBLING_BLOCK_PARTITION && pItem.direction == UP_DIRECTION)
// 					{
// 						result = false;
// 						break;
// 					}
// 				}

				return result;
			}
			else if (flag == true)
			{
				tagSpecialBrickData special;
				if (IsHaveDownPortal(tile.indexX,tile.indexY,&special))
				{
					return true;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return false;
}

void CGameLogic::swapGem(tagGemData &scoreGem,tagGemData &targetGem)
{
	tagGemData pGem = scoreGem;

	//scoreGem.bMove = targetGem.bMove;
	scoreGem.parentType = targetGem.parentType;
	scoreGem.childType = targetGem.childType;
	scoreGem.gemStatus = targetGem.gemStatus;
	scoreGem.attribute = targetGem.attribute;
	scoreGem.moveAction = targetGem.moveAction;

	//targetGem.bMove = pGem.bMove;
	targetGem.parentType = pGem.parentType;
	targetGem.childType = pGem.childType;
	targetGem.gemStatus = pGem.gemStatus;
	targetGem.attribute = pGem.attribute;
	targetGem.moveAction = pGem.moveAction;
}

GemStatus CGameLogic::getSpecialGemType(cocos2d::Vec2 point)
{
	if (point.x >=5 || point.y >= 5)
	{
		return GEM_STATUS_COLORFUL;
	}
	else if (point.x >=3 && point.y >= 3)
	{
		return GEM_STATUS_BOM;
	}
	else if (point.x >=4 || point.y >= 4)
	{
		if (point.x >=4)
		{
			return GEM_STATUS_HORIZONTAL;
		}
		if (point.y >= 4)
		{
			return GEM_STATUS_VERTICAL;
		}
	}

	return GEM_STATUS_NORMAL;
}

template<class Type>
void CGameLogic::addVectorData(std::vector<Type> &pVec,Type pData)
{
	auto  ite = find(pVec.begin(),pVec.end(),pData);
	if (ite == pVec.end())
	{
		pVec.push_back(pData);
	}
}

enBrickType CGameLogic::IsHaveGemOrStumbling(const TileBrickData &pTile)
{
	if (pTile.isShow == false)
	{
		return NO_GEM_AND_BRICK_TYPE;
	}

	if ((pTile.gem.childType <= GEM_COLOR_NO && pTile.gem.gemStatus != GEM_STATUS_COLORFUL || pTile.gem.childType >=GEM_COLOR_COUNT) && pTile.specialBrick.empty())
	{
		return NO_GEM_AND_BRICK_TYPE;
	}

	if ((pTile.gem.childType > GEM_COLOR_NO && pTile.gem.childType < GEM_COLOR_COUNT || pTile.gem.childType <= GEM_COLOR_NO && pTile.gem.gemStatus == GEM_STATUS_COLORFUL) && !pTile.specialBrick.empty())
	{
		return GEM_AND_BRICK_TYPE;
	}

	if ((pTile.gem.childType > GEM_COLOR_NO && pTile.gem.childType < GEM_COLOR_COUNT || pTile.gem.childType <= GEM_COLOR_NO && pTile.gem.gemStatus == GEM_STATUS_COLORFUL) && pTile.specialBrick.empty())
	{
		return GEM_TYPE;
	}

	if ((pTile.gem.childType <= GEM_COLOR_NO && pTile.gem.gemStatus != GEM_STATUS_COLORFUL || pTile.gem.childType >= GEM_COLOR_COUNT) && !pTile.specialBrick.empty())
	{
		return BRICK_TYPE;
	}

	return NULL_TYPE;
}

bool CGameLogic::IsCanAddGem(TileBrickData *pTile)
{
	if (pTile == NULL || pTile->isShow == false)
	{
		return true;
	}

	return false;
}

int CGameLogic::getMinRowNumber(int colNumber)
{
	int result = COL_COUNT-1;
	for (int x = COL_COUNT-1;x >=0;--x)
	{
		TileBrickData *pTileBrick = &m_tileBrick[x][colNumber];
		TileBrickData *upTile = currentTileBrickItem(x-1,colNumber);

		if(pTileBrick->isShow )
		{
			if (upTile ==NULL||upTile->isShow == false)
			{
				if (result > pTileBrick->indexX)
				{
					result = pTileBrick->indexX;
				}
			}
		}
	}

	return result;
}

void CGameLogic::SpecialEffectsToEliminateGems(TileBrickData &pTile,std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos)
{
	switch (pTile.gem.gemStatus)
	{
	case GEM_STATUS_VERTICAL:
		{
			for (int x = pTile.indexX; x >= 0; --x)
			{
				TileBrickData &item = m_tileBrick[x][pTile.indexY];
				if (item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE) continue;
				if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
				{
					addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));
				}
				else if (item.isShow && item.gem.childType <= GEM_COLOR_NO && item.gem.gemStatus != GEM_STATUS_COLORFUL)
				{
					item.gem.gemStatus = GEM_STATUS_NORMAL;
				}

				if (item.isShow && !item.specialBrick.empty())
				{
					removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);

					bool result = false;
					for (SpecialBrickData::const_iterator ite = item.specialBrick.begin(); ite != item.specialBrick.end(); ++ite)
					{
						if (ite->childType == STUMBLING_BLOCK_SILVER_COIN)
						{
							result = true;
							break;
						}
					}

					if (result) 
					{
						break;
					}
				}
			}
			for (int x = pTile.indexX + 1;x < ROW_COUNT;++x)
			{
				TileBrickData &item = m_tileBrick[x][pTile.indexY];
				if (item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE) continue;
				if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
				{
					addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));
				}
				else if (item.isShow && item.gem.childType <= GEM_COLOR_NO && item.gem.gemStatus != GEM_STATUS_COLORFUL)
				{
					item.gem.gemStatus = GEM_STATUS_NORMAL;
				}

				if (item.isShow && !item.specialBrick.empty())
				{
					removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);

					bool result = false;
					for (SpecialBrickData::const_iterator ite = item.specialBrick.begin(); ite != item.specialBrick.end(); ++ite)
					{
						if (ite->childType == STUMBLING_BLOCK_SILVER_COIN)
						{
							result = true;
							break;
						}
					}

					if (result) 
					{
						break;
					}
				}
			}
		}
		break;
	case GEM_STATUS_HORIZONTAL:
		{
			for (int y = pTile.indexY; y >=0; --y)
			{
				TileBrickData &item = m_tileBrick[pTile.indexX][y];
				if (item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE) continue;
				if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
				{
					addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));					
				}
				else if (item.isShow && item.gem.childType <= GEM_COLOR_NO && item.gem.gemStatus != GEM_STATUS_COLORFUL)
				{
					item.gem.gemStatus = GEM_STATUS_NORMAL;
				}

				if (item.isShow && !item.specialBrick.empty())
				{
					removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);

					bool result = false;
					for (SpecialBrickData::const_iterator ite = item.specialBrick.begin(); ite != item.specialBrick.end(); ++ite)
					{
						if (ite->childType == STUMBLING_BLOCK_SILVER_COIN)
						{
							result = true;
							break;
						}
					}

					if (result) 
					{
						break;
					}
				}
			}

			for (int y = pTile.indexY+1;y < COL_COUNT;++y)
			{
				TileBrickData &item = m_tileBrick[pTile.indexX][y];
				if (item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE) continue;
				if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
				{
					addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));					
				}
				else if (item.isShow && item.gem.childType <= GEM_COLOR_NO && item.gem.gemStatus != GEM_STATUS_COLORFUL)
				{
					item.gem.gemStatus = GEM_STATUS_NORMAL;
				}

				if (item.isShow && !item.specialBrick.empty())
				{
					removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);

					bool result = false;
					for (SpecialBrickData::const_iterator ite = item.specialBrick.begin(); ite != item.specialBrick.end(); ++ite)
					{
						if (ite->childType == STUMBLING_BLOCK_SILVER_COIN)
						{
							result = true;
							break;
						}
					}

					if (result) 
					{
						break;
					}
				}
			}
		}
		break;
	case GEM_STATUS_HV:
		break;
	case GEM_STATUS_BOM:
		{
			for (int x = pTile.indexX - 1; x <= pTile.indexX + 1;++x)
			{
				for (int y = pTile.indexY-1;y <= pTile.indexY+1;++y)
				{
					if (x < 0 || x > ROW_COUNT - 1 || y < 0 || y > COL_COUNT - 1)
					{
						continue;
					}

					TileBrickData &item = m_tileBrick[x][y];
					if (item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE) continue;
					if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
					{
						addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));					
					}
					else if (item.isShow && item.gem.childType <= GEM_COLOR_NO && item.gem.gemStatus != GEM_STATUS_COLORFUL)
					{
						item.gem.gemStatus = GEM_STATUS_NORMAL;
					}

					if (item.isShow && !item.specialBrick.empty())
					{
						removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);
					}
				}
			}
		}
		break;
	case GEM_STATUS_COLORFUL:
		{
			for (int x = 0;x < ROW_COUNT;++x)
			{
				for (int y = 0;y < COL_COUNT;++y)
				{
					GemColor color = GemColor(pTile.gem.childType);
					color = color > GEM_COLOR_NO?color:GemColor(randomCreateGem(m_levelsGemList).childType);
					TileBrickData &item = m_tileBrick[x][y];
					if (item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE) continue;
					if (item.gem.childType == color)
					{
						addVectorData(connectGemPos,cocos2d::Vec2(x,y));

						if (item.isShow && !item.specialBrick.empty())
						{
							removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}

	//pTile.gem.flag = GEM_STATUS_NORMAL;
}

void CGameLogic::swapSpecialGem(std::vector<cocos2d::Vec2> &connectGemPos,std::vector<ObtainSpecialGemInfo> &outInfos,SpecialBrickData &StumblingBlockPos)
{
	TileBrickData &sourceBrickItem = m_tileBrick[m_sourceX][m_sourceY];
	TileBrickData &targetBrickItem = m_tileBrick[m_targetX][m_targetY];
	tagGemData &sourceGem = sourceBrickItem.gem;
	tagGemData &targetGem = targetBrickItem.gem;

	if (sourceGem.gemStatus > GEM_STATUS_NORMAL)
	{
		if (targetGem.gemStatus > GEM_STATUS_NORMAL)
		{
 			if (sourceGem.gemStatus > GEM_STATUS_NORMAL && sourceGem.gemStatus < GEM_STATUS_HV && targetGem.gemStatus > GEM_STATUS_NORMAL && targetGem.gemStatus < GEM_STATUS_HV)
 			{
				if (abs(m_sourceX - m_targetX) > 0)
				{
					ObtainSpecialGemInfo spGemInfo;
					spGemInfo.point = cocos2d::Vec2(m_targetX,m_targetY);
					spGemInfo.flag = GEM_STATUS_HORIZONTAL;
					targetGem.gemStatus = GEM_STATUS_HORIZONTAL;
					addVectorData(outInfos,spGemInfo);

					spGemInfo.point = cocos2d::Vec2(m_sourceX,m_sourceY);
					spGemInfo.flag = GEM_STATUS_VERTICAL;
					sourceGem.gemStatus = GEM_STATUS_VERTICAL;
					addVectorData(outInfos,spGemInfo);
				}
				else if (abs(m_sourceY - m_targetY) > 0)
				{
					ObtainSpecialGemInfo spGemInfo;
					spGemInfo.point = cocos2d::Vec2(m_targetX,m_targetY);
					spGemInfo.flag = GEM_STATUS_VERTICAL;
					targetGem.gemStatus = GEM_STATUS_VERTICAL;
					addVectorData(outInfos,spGemInfo);

					spGemInfo.point = cocos2d::Vec2(m_sourceX,m_sourceY);
					spGemInfo.flag = GEM_STATUS_HORIZONTAL;
					sourceGem.gemStatus = GEM_STATUS_HORIZONTAL;
					addVectorData(outInfos,spGemInfo);
				}

				addVectorData(connectGemPos,cocos2d::Vec2(m_sourceX,m_sourceY));
				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,m_targetY));
 			}
 			else if (sourceGem.gemStatus == GEM_STATUS_BOM && targetGem.gemStatus == GEM_STATUS_HORIZONTAL)
 			{
 				unsigned char up = m_targetX-1 > 0?m_targetX-1:0;
 				unsigned char bottom = m_targetX+1 < ROW_COUNT?m_targetX+1:ROW_COUNT-1;
 
 				m_tileBrick[up][m_targetY].gem.gemStatus = GEM_STATUS_HORIZONTAL;
 				targetGem.gemStatus = GEM_STATUS_HORIZONTAL;
 				m_tileBrick[bottom][m_targetY].gem.gemStatus = GEM_STATUS_HORIZONTAL;
 
				ObtainSpecialGemInfo spGemInfo;
				spGemInfo.point = cocos2d::Vec2(up,m_targetY);
				spGemInfo.flag = m_tileBrick[up][m_targetY].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(bottom,m_targetY);
				spGemInfo.flag = m_tileBrick[bottom][m_targetY].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(m_targetX,m_targetY);
				spGemInfo.flag = targetGem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,m_targetY));
				addVectorData(connectGemPos,cocos2d::Vec2(up,m_targetY));
				addVectorData(connectGemPos,cocos2d::Vec2(bottom,m_targetY));
 			}
 			else if (sourceGem.gemStatus == GEM_STATUS_BOM && targetGem.gemStatus == GEM_STATUS_VERTICAL)
 			{
 				unsigned char left = m_targetY-1 > 0?m_targetY-1:0;
 				unsigned char right = m_targetY+1 < COL_COUNT?m_targetY+1:COL_COUNT-1;
 
 				m_tileBrick[m_targetX][left].gem.gemStatus = GEM_STATUS_VERTICAL;
 				targetGem.gemStatus = GEM_STATUS_VERTICAL;
 				m_tileBrick[m_targetX][right].gem.gemStatus = GEM_STATUS_VERTICAL;
 
				ObtainSpecialGemInfo spGemInfo;
				spGemInfo.point = cocos2d::Vec2(m_targetX,left);
				spGemInfo.flag = m_tileBrick[m_targetX][left].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(m_targetX,right);
				spGemInfo.flag = m_tileBrick[m_targetX][right].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(m_targetX,m_targetY);
				spGemInfo.flag = targetGem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,m_targetY));
				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,left));
				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,right));
 			}
 			else if (sourceGem.gemStatus == GEM_STATUS_HORIZONTAL && targetGem.gemStatus == GEM_STATUS_BOM)
 			{
 				unsigned char up = m_targetX-1 > 0?m_targetX-1:0;
 				unsigned char bottom = m_targetX+1 < ROW_COUNT?m_targetX+1:ROW_COUNT-1;
 
 				m_tileBrick[up][m_targetY].gem.gemStatus = GEM_STATUS_HORIZONTAL;
 				targetGem.gemStatus = GEM_STATUS_HORIZONTAL;
 				m_tileBrick[bottom][m_targetY].gem.gemStatus = GEM_STATUS_HORIZONTAL;
 
				ObtainSpecialGemInfo spGemInfo;
				spGemInfo.point = cocos2d::Vec2(up,m_targetY);
				spGemInfo.flag = m_tileBrick[up][m_targetY].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(bottom,m_targetY);
				spGemInfo.flag = m_tileBrick[bottom][m_targetY].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(m_targetX,m_targetY);
				spGemInfo.flag = targetGem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,m_targetY));
				addVectorData(connectGemPos,cocos2d::Vec2(up,m_targetY));
				addVectorData(connectGemPos,cocos2d::Vec2(bottom,m_targetY));
 			}
 			else if (sourceGem.gemStatus == GEM_STATUS_VERTICAL && targetGem.gemStatus == GEM_STATUS_BOM)
 			{
 				unsigned char left = m_targetY-1 > 0?m_targetY-1:0;
 				unsigned char right = m_targetY+1 < COL_COUNT?m_targetY+1:COL_COUNT-1;
 
 				m_tileBrick[m_targetX][left].gem.gemStatus = GEM_STATUS_VERTICAL;
 				targetGem.gemStatus = GEM_STATUS_VERTICAL;
 				m_tileBrick[m_targetX][right].gem.gemStatus = GEM_STATUS_VERTICAL;
 
				ObtainSpecialGemInfo spGemInfo;
				spGemInfo.point = cocos2d::Vec2(m_targetX,left);
				spGemInfo.flag = m_tileBrick[m_targetX][left].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(m_targetX,right);
				spGemInfo.flag = m_tileBrick[m_targetX][right].gem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				spGemInfo.point = cocos2d::Vec2(m_targetX,m_targetY);
				spGemInfo.flag = targetGem.gemStatus;
				addVectorData(outInfos,spGemInfo);

				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,m_targetY));
				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,left));
				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,right));
 			}
 			else if (sourceGem.gemStatus == GEM_STATUS_BOM && targetGem.gemStatus == GEM_STATUS_BOM)
 			{
 				for (int x = m_targetX - 2; x <= m_targetX + 2;++x)
 				{
 					for (int y = m_targetY-2;y <= m_targetY+2;++y)
 					{
 						if (x < 0 || x > ROW_COUNT - 1 || y < 0 || y > COL_COUNT - 1)
 						{
 							continue;
 						}

						TileBrickData &item = m_tileBrick[x][y];
						if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
						{
							addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));
						}

						if (item.isShow && !item.specialBrick.empty())
						{
							removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);
						}
					}
 				}
 			}
 			else if(sourceGem.gemStatus == GEM_STATUS_COLORFUL && targetGem.gemStatus == GEM_STATUS_COLORFUL)
 			{
 				for (int x = 0;x < ROW_COUNT;++x)
 				{
 					for (int y = 0;y < COL_COUNT;++y)
 					{
 						TileBrickData &item = m_tileBrick[x][y];
 						if (item.isShow && item.gem.childType > GEM_COLOR_NO || (item.gem.childType == GEM_COLOR_NO && item.gem.gemStatus == GEM_STATUS_COLORFUL))
 						{
 							addVectorData(connectGemPos,cocos2d::Vec2(item.indexX,item.indexY));
 						}
 
						if (item.isShow && !item.specialBrick.empty())
						{
							removeSpecialBrickItm(item.indexX,item.indexY,&item,StumblingBlockPos);
						}
 					}
 				}
 			}
 			else if(sourceGem.gemStatus == GEM_STATUS_COLORFUL && targetGem.gemStatus < GEM_STATUS_COLORFUL)
 			{
 				sourceGem.gemStatus = GEM_STATUS_NORMAL;
				addVectorData(connectGemPos,cocos2d::Vec2(m_sourceX,m_sourceY));
				GemColor color = GemColor(targetGem.childType);
 				for (int x = 0;x < ROW_COUNT;++x)
 				{
 					for (int y = 0;y < COL_COUNT;++y)
 					{
 						if (m_tileBrick[x][y].gem.childType == color)
 						{
 							if (targetGem.gemStatus == GEM_STATUS_BOM)
 							{
 								m_tileBrick[x][y].gem.gemStatus = GEM_STATUS_BOM;
 							}
 							else if (targetGem.gemStatus == GEM_STATUS_VERTICAL || targetGem.gemStatus == GEM_STATUS_HORIZONTAL)
 							{
 								m_tileBrick[x][y].gem.gemStatus = rand() % 2 == 0?GEM_STATUS_HORIZONTAL:GEM_STATUS_VERTICAL;
 							}
 							ObtainSpecialGemInfo spGemInfo;
 							spGemInfo.point = cocos2d::Vec2(x,y);
 							spGemInfo.flag = m_tileBrick[x][y].gem.gemStatus;
 							addVectorData(outInfos,spGemInfo);
							addVectorData(connectGemPos,cocos2d::Vec2(x,y));
 						}
 					}
 				}

 			}
 			else if(sourceGem.gemStatus < GEM_STATUS_COLORFUL && targetGem.gemStatus == GEM_STATUS_COLORFUL)
 			{
 				targetGem.gemStatus = GEM_STATUS_NORMAL;
				addVectorData(connectGemPos,cocos2d::Vec2(m_targetX,m_targetY));
				GemColor color = GemColor(sourceGem.childType);
 				for (int x = 0;x < ROW_COUNT;++x)
 				{
 					for (int y = 0;y < COL_COUNT;++y)
 					{
 						if (m_tileBrick[x][y].gem.childType == color)
 						{
 							if (sourceGem.gemStatus == GEM_STATUS_BOM)
 							{
 								m_tileBrick[x][y].gem.gemStatus = GEM_STATUS_BOM;
 							}
 							else if (sourceGem.gemStatus == GEM_STATUS_VERTICAL || sourceGem.gemStatus == GEM_STATUS_HORIZONTAL)
 							{
 								m_tileBrick[x][y].gem.gemStatus = rand() % 2 == 0?GEM_STATUS_HORIZONTAL:GEM_STATUS_VERTICAL;
 							}
 							ObtainSpecialGemInfo spGemInfo;
 							spGemInfo.point = cocos2d::Vec2(x,y);
 							spGemInfo.flag = m_tileBrick[x][y].gem.gemStatus;
 							addVectorData(outInfos,spGemInfo);
							addVectorData(connectGemPos,cocos2d::Vec2(x,y));
 						}
 					}
 				}
 			}
			else if (sourceGem.gemStatus > GEM_STATUS_COLORFUL && targetGem.gemStatus == GEM_STATUS_COLORFUL)
			{
				TileBrickData &pGem = targetBrickItem;
				pGem.gem.childType = sourceGem.childType;
				addVectorData(connectGemPos,cocos2d::Vec2(pGem.indexX,pGem.indexY));
			}
			else if (sourceGem.gemStatus == GEM_STATUS_COLORFUL && targetGem.gemStatus > GEM_STATUS_COLORFUL)
			{
				TileBrickData &pGem = sourceBrickItem;
				pGem.gem.childType = targetGem.childType;
				addVectorData(connectGemPos,cocos2d::Vec2(pGem.indexX,pGem.indexY));
			}
		}
		else if (targetGem.gemStatus == GEM_STATUS_NORMAL && sourceGem.gemStatus == GEM_STATUS_COLORFUL)
		{
			TileBrickData &pGem = sourceBrickItem;
			pGem.gem.childType = targetGem.childType;
			addVectorData(connectGemPos,cocos2d::Vec2(pGem.indexX,pGem.indexY));
		}
	}
	else if (sourceGem.gemStatus == GEM_STATUS_NORMAL || sourceGem.gemStatus > GEM_STATUS_COLORFUL)
	{
		if (targetGem.gemStatus == GEM_STATUS_COLORFUL)
		{
			TileBrickData &pGem = targetBrickItem;
			pGem.gem.childType = sourceGem.childType;
			addVectorData(connectGemPos,cocos2d::Vec2(pGem.indexX,pGem.indexY));
		}
	}
}

void CGameLogic::changeTargetCount(tagTargetData target)
{
	TargetData::iterator targetIte = find(m_targetList.begin(),m_targetList.end(),target);
	if (targetIte != m_targetList.end())
	{
		TargetData::iterator CurrentIter= find(m_currentTargetList.begin(),m_currentTargetList.end(),target);
		if (CurrentIter != m_currentTargetList.end())
		{
			if (CurrentIter->number != targetIte->number)
			{
				CurrentIter->number++;
			}
		}
		else
		{
			target.ID = targetIte->ID;
			target.number++;

			addVectorData(m_currentTargetList,target);
		}
	}
}

void CGameLogic::changeTargetNum(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos,std::vector<ObtainSpecialGemInfo> &outInfos)
{
	//宝石
	for (std::vector<cocos2d::Vec2>::iterator ite = connectGemPos.begin(); ite != connectGemPos.end();)
	{
		cocos2d::Vec2 point = *ite;
		int posX = point.x;
		int posY = point.y;
		
		TileBrickData &tileBrick = m_tileBrick[posX][posY];
		tagTargetData target;
		target.number = 0;
		
		if (tileBrick.gem.gemStatus > GEM_STATUS_NORMAL)
		{
			target.parentType = SPECIAL_GEM_PARENT_TYPE;
			target.type = tileBrick.gem.gemStatus;
			tagTargetData pTarget;
			pTarget.parentType = SPECIAL_GEM_PARENT_TYPE;
			pTarget.type = GEM_STATUS_HV;
			if (find(m_targetList.begin(),m_targetList.end(),pTarget) != m_targetList.end())
			{
				if (tileBrick.gem.gemStatus == GEM_STATUS_HORIZONTAL || tileBrick.gem.gemStatus == GEM_STATUS_VERTICAL)
				{
					target.type = GEM_STATUS_HV;
				}
			}
			changeTargetCount(target);
		}

		target.parentType = NORMAL_GEM_PARENT_TYPE;
		target.type = tileBrick.gem.childType;
		changeTargetCount(target);

		if (m_gameStatus != GAME_STATUS_BONUS_TIME)
		{
			CalculateScore(tileBrick);
		}
		
		switch(tileBrick.gem.gemStatus)
		{
		case GEM_STATUS_ADD_STEP:
			m_currentStep += 5;
			tileBrick.gem.childType = GEM_COLOR_NO;
			tileBrick.gem.gemStatus = GEM_STATUS_NORMAL;
			++ite;
			break;
		case GEM_STATUS_TREASURE_CHEST:
			{
				ObtainSpecialGemInfo gemInfo;
				gemInfo.point = cocos2d::Vec2(tileBrick.indexX,tileBrick.indexY);
				do 
				{
					GemStatus status = GemStatus(rand() % 5 + 1);
					if (status != GEM_STATUS_HV)
					{
						gemInfo.flag = status;
						break;
					}
				} while (true);
				tileBrick.gem.gemStatus = gemInfo.flag;
				if (tileBrick.gem.gemStatus == GEM_STATUS_COLORFUL)
				{
					tileBrick.gem.childType = GEM_COLOR_NO;
				}
				outInfos.push_back(gemInfo);
				ite = connectGemPos.erase(ite);
			}
			break;
		case GEM_STATUS_TIME_BOM:
			tileBrick.gem.childType = GEM_COLOR_NO;
			tileBrick.gem.gemStatus = GEM_STATUS_NORMAL;
			tileBrick.gem.attribute = 0;
			++ite;
			break;
		default:
			tileBrick.gem.childType = GEM_COLOR_NO;
			tileBrick.gem.gemStatus = GEM_STATUS_NORMAL;
			++ite;
			break;
		}
	}
	
	//障碍物
	for (int i = 0; i < (int)StumblingBlockPos.size(); ++i)
	{
		cocos2d::Vec2 point(StumblingBlockPos[i].indexX,StumblingBlockPos[i].indexY);
		int posX = point.x;
		int posY = point.y;
		TileBrickData &tileBrick = m_tileBrick[posX][posY];

		if (!tileBrick.specialBrick.empty())
		{
			tagSpecialBrickData &specialBrick = StumblingBlockPos[i];

			tagTargetData target;
			target.number = 0;
			target.parentType = STUMBLING_BLOCK_PARENT_TYPE;
			target.type = specialBrick.childType;

			changeTargetCount(target);

			//CalculateScore(tileBrick);

			if (specialBrick.number < 1)
			{
				SpecialBrickData::const_iterator iter = find(tileBrick.specialBrick.begin(),tileBrick.specialBrick.end(),specialBrick);
				if (iter != tileBrick.specialBrick.end())
				{
					tileBrick.specialBrick.erase(iter);
				}
			}
		}
	}

	//金钻
	for (int x = 0;x < ROW_COUNT;++x)
	{
		for (int y = 0;y < COL_COUNT;++y)
		{
			TileBrickData &pItem = m_tileBrick[x][y];
			if (pItem.map.collect)
			{
				for (SpecialBrickData::iterator ite = pItem.specialBrick.begin(); ite != pItem.specialBrick.end();)
				{
					if (ite->childType == STUMBLING_BLOCK_GOLD_POD)
					{
						tagTargetData target;
						target.number = 0;
						target.parentType = STUMBLING_BLOCK_PARENT_TYPE;
						target.type = ite->childType;

						TargetData::iterator targetIte = find(m_targetList.begin(),m_targetList.end(),target);
						if (targetIte != m_targetList.end())
						{
							TargetData::iterator CurrentIter= find(m_currentTargetList.begin(),m_currentTargetList.end(),target);
							if (CurrentIter != m_currentTargetList.end())
							{
								if (CurrentIter->number != targetIte->number)
								{
									CurrentIter->number++;
								}
							}
							else
							{
								target.ID = m_currentTargetList.size();
								target.number++;
								addVectorData(m_currentTargetList,target);
							}

							ite = pItem.specialBrick.erase(ite);
							pItem.gem.bMove = true;
							connectGemPos.push_back(cocos2d::Vec2(x,y));

							m_bSwapGem = false;
						}
						else
						{
							++ite;
						}
					}
					else
					{
						++ite;
					}
				}
			}
		}
	}

	m_sourceX = -1;
	m_sourceY = -1;
	m_targetX = -1;
	m_targetY = -1;

	m_swapGemStatus = NULL_SWAP_STATUS;
}

bool CGameLogic::IsCanEndGame()
{
	int completeNum = 0;
	for (int i = 0;i < (int)m_currentTargetList.size();++i)
	{
		tagTargetData target = m_currentTargetList[i];
		TargetData::iterator ite = find(m_targetList.begin(),m_targetList.end(),target);
		if (ite != m_targetList.end())
		{
			if (ite->number == target.number)
			{
				completeNum++;
			}
		}
	}

	if (completeNum == m_targetList.size())
	{
		return true;
	}

	GemData timeGemItem = getTimeGem();
	if (timeGemItem.empty() == false)
	{
		for (GemData::const_iterator ite = timeGemItem.begin(); ite != timeGemItem.end(); ++ite)
		{
			if (ite->attribute <= 0)
			{
				return true;
			}
		}
	}

	bool result = true;
	for (LevelsGem::const_iterator ite = m_levelsGemList.begin(); ite != m_levelsGemList.end(); ++ite)
	{
		if (getGemNumberByColor(ite->color) >= 3)
		{
			result = false;
		}
	}

	if (result)
	{
		return result;
	}

	switch(m_levelStartType)
	{
	case LEVELSTART_STEP:
		if (m_currentStep <= 0)
		{
			return true;
		}
		break;
	case LEVELSTART_TIME:
		if (m_currentTime <= 0)
		{
			return true;
		}
		break;
	case LEVELSTART_SETPANDTIME:
		if (m_currentTime <= 0 || m_currentStep <= 0)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

gameEndStatus CGameLogic::GameEndStatus()
{
	gameEndStatus status = NO_GAME_END;

	bool result = false;

	if (IsCanEndGame() == false)
	{
		status = NO_GAME_END;
	}
	else
	{
		bool bCompleteTarget = false;
		int completeNum = 0;
		for (int i = 0;i < (int)m_currentTargetList.size();++i)
		{
			tagTargetData target = m_currentTargetList[i];
			TargetData::iterator ite = find(m_targetList.begin(),m_targetList.end(),target);
			if (ite != m_targetList.end())
			{
				if (ite->number == target.number)
				{
					completeNum++;
				}
			}
		}

		if (completeNum == m_targetList.size())
		{
			bCompleteTarget =  true;
		}

		if (bCompleteTarget == false)
		{
			GemData timeGemItem = getTimeGem();
			if (timeGemItem.empty() == false)
			{
				for (GemData::const_iterator ite = timeGemItem.begin(); ite != timeGemItem.end(); ++ite)
				{
					if (ite->attribute <= 0)
					{
						return TIME_BOMB_GAME_END;
					}
				}
			}

			bool result = true;
			for (LevelsGem::const_iterator ite = m_levelsGemList.begin(); ite != m_levelsGemList.end(); ++ite)
			{
				if (getGemNumberByColor(ite->color) >= 3)
				{
					result = false;
				}
			}

			if (result)
			{
				return NO_THREE_SAME_COLOR_GEM;
			}
		}

		if (m_levelStartType == LEVELSTART_STEP && m_currentStep <= 0 && bCompleteTarget == false)
		{
			status = NO_STEPORTIME_GAME_END;
		}
		else if (m_levelStartType == LEVELSTART_TIME && m_currentTime <= 0 && bCompleteTarget == false)
		{
			status = NO_STEPORTIME_GAME_END;
		}
		else if (m_levelStartType == LEVELSTART_SETPANDTIME && (m_currentTime <= 0 || m_currentStep <= 0) && bCompleteTarget == false)
		{
			status = NO_STEPORTIME_GAME_END;
		}
		else if(m_gameStatus == GAME_STATUS_BONUS_TIME && m_scoreNumber < m_starScore.score_1 && bCompleteTarget == true)
		{
			status = SCORES_LESS_THAN_GAME_END;
		}
		else if(bCompleteTarget == true)
		{
			status = SUCCESS_GAME_END;
		}
	}

	return status;
}

int CGameLogic::GetCurrentGateStar()
{
	if (m_scoreNumber >= m_starScore.score_3)
	{
		return 3;
	}
	else if (m_scoreNumber >= m_starScore.score_2)
	{
		return 2;
	}
	else if (m_scoreNumber >= m_starScore.score_1)
	{
		return 1;
	}

	return 0;
}

void CGameLogic::getBonusTimeGem(std::vector<ObtainSpecialGemInfo> &outInfos)
{
	if (m_currentStep <= 0)
	{
		return;
	}

	do 
	{
		int x = rand() % 9;
		int y = rand() % 9;
		TileBrickData *item = &m_tileBrick[x][y];
		if (item->isShow && item->gem.childType != GEM_COLOR_NO && item->gem.gemStatus == GEM_STATUS_NORMAL)
		{
			unsigned char flag = rand() % 2;
			item->gem.gemStatus = flag == 0?GEM_STATUS_VERTICAL:GEM_STATUS_HORIZONTAL;
			ObtainSpecialGemInfo pGemInfo;
			pGemInfo.point.x = x;
			pGemInfo.point.y = y;
			pGemInfo.flag = item->gem.gemStatus;
			outInfos.push_back(pGemInfo);
			--m_currentStep;
		}

		m_scoreNumber += (m_starScore.score_3  / (NORMAL_GEM_SCORE*m_rulesData.step) + 1) * NORMAL_GEM_SCORE;

		if ((int)outInfos.size() >= m_currentStep)
		{
			return;
		}
	} while (m_currentStep);
}

void CGameLogic::CalculateScore(const TileBrickData &pTile)
{
	if (pTile.isShow)
	{
		if (pTile.gem.childType > GEM_COLOR_NO || (pTile.gem.childType == GEM_COLOR_NO && pTile.gem.gemStatus == GEM_STATUS_COLORFUL))
		{
			m_currentComboNumber = m_currentComboNumber == 0?1:m_currentComboNumber;
			m_scoreNumber = m_scoreNumber + m_currentComboNumber * NORMAL_GEM_SCORE;
		}

		m_scoreNumber = int(m_scoreNumber * m_precentScore);
	}
}

bool CGameLogic::retrieveTheBoard(bool flag /*= true*/)
{
	for (int x = 0; x < COL_COUNT; ++x)
	{
		for (int y = 0; y < ROW_COUNT; ++y)
		{
			tagGemData& pData = m_tileBrick[x][y].gem;

			if (!m_tileBrick[x][y].isShow)
			{
				pData.childType = GEM_COLOR_NO;
				pData.gemStatus = GEM_STATUS_NORMAL;
			}

			if (pData.childType!= GEM_COLOR_NO )
			{
				tagLevelsGem gem;
				gem.ID = 0;
				gem.color =GemColor(pData.childType);
				if (find(m_levelsGemList.begin(),m_levelsGemList.end(),gem) == m_levelsGemList.end())
				{
					pData.childType = randomCreateGem(m_levelsGemList).childType;
				}
			}
		}
	}

	std::vector<cocos2d::Vec2> pos;
	SpecialBrickData item;

	do 
	{
		do
		{
 			resetGem(flag);
			bool flag = checkConnect(pos, false);
			if (flag == false)
			{
				break;
			}
		}while (true);

		bool typ = retrieveConnectGem();
		if (typ == true)
		{
			break;
		}
	} while (false);

	return true;
}

tagSpecialBrickData* CGameLogic::getTheTopSpecialBrickItem(int posX,int posY)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1)
	{
		return NULL;
	}

	SpecialBrickData &item = m_tileBrick[posX][posY].specialBrick;
	
	return item.empty()?NULL:&(*(item.rbegin()));
}

tagSpecialBrickData* CGameLogic::getRemoveSpecialBrickItem(int posX,int posY)
{
	if (posX  < 0 || posY < 0 || posX > COL_COUNT - 1 || posY > ROW_COUNT - 1)
	{
		return NULL;
	}

	SpecialBrickData &item = m_tileBrick[posX][posY].specialBrick;
	SpecialBrickData::reverse_iterator ite = item.rbegin();
	while(ite != item.rend())
	{
		if (IsCanRemoveStumblingBlock(posX,posY,*ite))
		{
			return &(*ite);
		}
		++ite;
	}

	return NULL;
}

/*
* @flag true 获取UP_DIRECTION PORTAL； false 获取DOWN_DIRECTION PORTAL
*/
tagSpecialBrickData* CGameLogic::getAnotherPortal(const tagSpecialBrickData& pItem,bool flag)
{
	for (int i = ALL_COUNT-1; i >=0; --i)
	{
		TileBrickData *pTileBrick = &m_tileBrick[i/9][i%9];

		if (pTileBrick->isShow == false)
		{
			continue;
		}

		if (pTileBrick->specialBrick.empty())
		{
			continue;
		}

		TileBrickData *bottomBrick = NULL;
		for (size_t i = 0; i < pTileBrick->specialBrick.size(); ++i)
		{
			tagSpecialBrickData item = pTileBrick->specialBrick[i];
			if (item.childType == STUMBLING_BLOCK_PORTAL && item.id != pItem.id &&item.direction == (unsigned char)UP_DIRECTION && item.attribute == pItem.attribute && flag == true)
			{
				return &(pTileBrick->specialBrick[i]);
			}
			else 	if (item.childType == STUMBLING_BLOCK_PORTAL && item.id != pItem.id &&item.direction == (unsigned char)DOWN_DIRECTION && item.attribute == pItem.attribute && flag == false)
			{
				return &(pTileBrick->specialBrick[i]);
			}
		}
	}

	return NULL;
}

bool CGameLogic::retrieveConnectGem(cocos2d::Vec2 point1 /* = cocos2d::Vec2(0,0) */,cocos2d::Vec2 point2 /* = cocos2d::Vec2(0,0) */)
{
	for (int x = 0;x < ROW_COUNT;++x)
	{
		for (int y = 0;y < COL_COUNT;++y)
		{
			TileBrickData &pTileItem = m_tileBrick[x][y];
			if (pTileItem.isShow == false || (pTileItem.gem.childType <= GEM_COLOR_NO && pTileItem.gem.gemStatus != GEM_STATUS_COLORFUL)|| pTileItem.gem.bMove == false)
			{
				continue;
			}

			if (IsCanConnect(x,y) == false)
			{
				continue;
			}

			int posX = x,posY = y;
			int bottomX = posX + 1, bottomY = posY;
			int rightX = posX, rightY = posY + 1;

			//水平方向
			TileBrickData *pRightItem = currentTileBrickItem(rightX,rightY);
			if (pRightItem != NULL && pRightItem->isShow && pRightItem->gem.bMove)
			{
				if (IsCanConnectGem(*pRightItem,pTileItem))
				{
					point1.x = x;
					point1.y = y;

					point2.x = rightX;
					point2.y = rightY;
					return true;
				}
			}

			//垂直方向
			TileBrickData *pBottomItem =currentTileBrickItem(bottomX,bottomY);
			if (pBottomItem != NULL && pBottomItem->isShow && pBottomItem->gem.bMove)
			{
				if (IsCanConnectGem(*pBottomItem,pTileItem))
				{
					point1.x = x;
					point1.y = y;

					point2.x = bottomX;
					point2.y = bottomY;
					return true;
				}
			}
		}
	}

	return false;
}

void CGameLogic::resetGem(bool flag /*= true*/)
{
	for (int x = 0;x < ROW_COUNT;++x)
	{
		for (int y = 0;y < COL_COUNT;++y)
		{
			TileBrickData &tileItem = m_tileBrick[x][y];

			if (tileItem.isShow == false )
			{
				continue;
			}

			if (tileItem.gem.childType <= GEM_COLOR_NO  && tileItem.gem.gemStatus != GEM_STATUS_COLORFUL)
			{
				continue;
			}

			if ( tileItem.gem.bMove == false && flag)
			{
				continue;
			}

			int posX = (x + rand() % 9) % 9, posY = (y + rand() % 9) % 9;
			TileBrickData &pSwapItem = m_tileBrick[posX][posY];

			if (pSwapItem.isShow== false || pSwapItem.id == tileItem.id || (pSwapItem.gem.childType <= GEM_COLOR_NO && pSwapItem.gem.gemStatus != GEM_STATUS_COLORFUL) || pSwapItem.gem.bMove == false
				|| pSwapItem.gem.childType == tileItem.gem.childType)
			{
				continue;
			}
			else
			{
				swapGem(m_tileBrick[x][y].gem,m_tileBrick[posX][posY].gem);
			}
		}
	}
}

void CGameLogic::ObtainSpecialBrickItems(MoveSpecialBrick &StumblingBlockPos)
{
	//土壤
	addJelly(StumblingBlockPos);
	//毛球
	moveBulb(StumblingBlockPos);

	m_bDisappearJelly = false;
	m_bSwapGem = false;
}

void CGameLogic::moveBulb(MoveSpecialBrick &pSpecial)
{
	TileBrick tileBrick = getCurrentTileBrick();
	for (size_t i = 0; i < tileBrick.size(); ++i)
	{
			TileBrickData &tileItem = tileBrick[i];

			if (tileItem.isShow == false || tileItem.specialBrick.empty())
			{
				continue;
			}

			for (SpecialBrickData::iterator ite = tileItem.specialBrick.begin(); ite != tileItem.specialBrick.end(); ++ite)
			{
				tagSpecialBrickData &pItem = *ite;
				if (pItem.childType != STUMBLING_BLOCK_BULB) 
				{
					continue;
				}

				TileBrickData* pTileItem = getNewSpecialBlock(pItem);
				if (pTileItem == NULL)
				{
					continue;
				}

				TileBrickData &item = m_tileBrick[tileItem.indexX][tileItem.indexY];
				item.specialBrick.erase(find(item.specialBrick.begin(),item.specialBrick.end(),pItem));
				item.gem.bMove = true;

				pItem.indexX = pTileItem->indexX;
				pItem.indexY = pTileItem->indexY;
				m_tileBrick[pTileItem->indexX][pTileItem->indexY].specialBrick.push_back(pItem);
				m_tileBrick[pTileItem->indexX][pTileItem->indexY].gem.bMove = false;

				moveSpecialBrick brickPoint;
				brickPoint.oldPoint.x = tileItem.indexX;
				brickPoint.oldPoint.y = tileItem.indexY;
				brickPoint.newPoint.x = pTileItem->indexX;
				brickPoint.newPoint.y = pTileItem->indexY; 
				brickPoint.type = StumblingBlock(pItem.childType);
				pSpecial.push_back(brickPoint);
			}
	}
}

void CGameLogic::addJelly(MoveSpecialBrick &pSpecial)
{
	if (m_bDisappearJelly)
	{
		return;
	}

	//获取获取果冻
	SpecialBrickData pBrickItem;
	for (int x = 0;x < ROW_COUNT;++x)
	{
		for (int y = 0;y < COL_COUNT;++y)
		{
			TileBrickData &tileItem = m_tileBrick[x][y];

			if (tileItem.isShow == false || tileItem.specialBrick.empty())
			{
				continue;
			}
			for (size_t i = 0; i < tileItem.specialBrick.size();++i)
			{
				if (tileItem.specialBrick[i].childType == STUMBLING_BLOCK_JELLY)
				{
					pBrickItem.push_back(tileItem.specialBrick[i]);
				}
			}
		}
	}

	

	do
	{
		if (pBrickItem.empty())
		{
			break;
		}

		int index = rand() % pBrickItem.size();
		tagSpecialBrickData pItem = pBrickItem[index];

		TileBrickData* pTileItem = getNewSpecialBlock(pItem);
		if (pTileItem != NULL)
		{
			tagSpecialBrickData item = pItem;
			item.id = pBrickItem.size();
			item.indexX = pTileItem->indexX;
			item.indexY = pTileItem->indexY;
			m_tileBrick[pTileItem->indexX][pTileItem->indexY].specialBrick.push_back(item);
			m_tileBrick[pTileItem->indexX][pTileItem->indexY].gem.childType = GEM_COLOR_NO;
			m_tileBrick[pTileItem->indexX][pTileItem->indexY].gem.gemStatus = GEM_STATUS_NORMAL;

			moveSpecialBrick brickPoint;
			brickPoint.oldPoint.x = pItem.indexX;
			brickPoint.oldPoint.y = pItem.indexY;
			brickPoint.newPoint.x = pTileItem->indexX;
			brickPoint.newPoint.y = pTileItem->indexY;
			brickPoint.type = StumblingBlock(item.childType);
			pSpecial.push_back(brickPoint);
			break;
		}

		pBrickItem.erase(find(pBrickItem.begin(),pBrickItem.end(),pItem));
	}while (true);
}

TileBrickData* CGameLogic::getNewSpecialBlock(tagSpecialBrickData &pItem)
{
	TileBrickData *pTileItem;
	//获取移动点位
	int posX = pItem.indexX,posY = pItem.indexY;
	int dir = rand()%4;		//随机方向
	bool IsNew = false;
	for (int i = dir; i < 4; ++i)		//正向寻找
	{
		pTileItem = getMovedPosition(i,posX,posY);
		if (pTileItem != NULL)
		{
			IsNew = true;
			break;
		}
	}
	if (IsNew == false)
	{
		//反向寻找
		for (int i = dir; i >= 0; --i)
		{
			pTileItem = getMovedPosition(i,posX,posY);
			if (pTileItem != NULL)
			{
				IsNew = true;
				break;
			}
		}
	}

	if (pTileItem != NULL)
	{
		return &m_tileBrick[pTileItem->indexX][pTileItem->indexY];
	}
	
	return NULL;
}

TileBrickData* CGameLogic::getMovedPosition(int dir,int posX,int posY)
{
	switch (dir)
	{
	case 0:
		posX--;
		break;
	case 1:
		posX++;
		break;
	case 2:
		posY--;
		break;
	case 3:
		posY++;
		break;
	}
	if (posX < 0 || posX > ROW_COUNT-1 || posY < 0 || posY > COL_COUNT-1)
	{
		return NULL;
	}

	TileBrickData &pbrickItem = m_tileBrick[posX][posY];
	if (pbrickItem.isShow == false)
	{
		return NULL;
	}
	
	if (pbrickItem.isShow == true && (IsHaveGemOrStumbling(pbrickItem) == NO_GEM_AND_BRICK_TYPE || IsHaveGemOrStumbling(pbrickItem) == BRICK_TYPE))
	{
		return NULL;
	}

	if (pbrickItem.isShow == true && IsHaveGemOrStumbling(pbrickItem) == GEM_AND_BRICK_TYPE)
	{
		bool result = false;
		for (size_t k = 0; k < pbrickItem.specialBrick.size(); ++k)
		{
			tagSpecialBrickData item = pbrickItem.specialBrick[k];
			if (item.childType != STUMBLING_BLOCK_GLASS && item.childType != STUMBLING_BLOCK_PORTAL && item.childType != STUMBLING_BLOCK_PARTITION)
			{
				result = false;
				break;
			}
			else
			{
				result = true;
			}
		}
		if (result == false)
		{
			return NULL;
		}
	}

	return &m_tileBrick[posX][posY];
}

void CGameLogic::addGoldPod(tagGemData &pItem)
{
	int currentTileGoldNum = 0;
	for (int x = 0;x < ROW_COUNT;++x)
	{
		for (int y = 0;y < COL_COUNT;++y)
		{
			TileBrickData &tileItem = m_tileBrick[x][y];
			for (size_t i = 0; i < tileItem.specialBrick.size(); ++i)
			{
				if (tileItem.specialBrick[i].childType == STUMBLING_BLOCK_GOLD_POD)
				{
					currentTileGoldNum++;
				}
			}
		}
	}

	int targetPodNum  = 0,currentGoldPodNum = 0;
	tagTargetData data;
	data.parentType = STUMBLING_BLOCK_PARENT_TYPE;
	data.type = STUMBLING_BLOCK_GOLD_POD;
	TargetData::iterator ite = find(m_targetList.begin(),m_targetList.end(),data);
	if (ite == m_targetList.end())
	{
		return;
	}
	else
	{
		targetPodNum = ite->number;
	}

	ite = find(m_currentTargetList.begin(),m_currentTargetList.end(),data);
	if (ite != m_currentTargetList.end())
	{
		currentGoldPodNum = ite->number;
	}

	if (currentTileGoldNum >= targetPodNum - currentGoldPodNum)
	{
		return;
	}

	//int index = rand() % pItem.size();
	tagGemData &item = pItem;//pItem[index];

	tagSpecialBrickData brickItem;
	brickItem.id = m_specialBrick.size();
	brickItem.indexX = item.indexX;
	brickItem.indexY = item.indexY;
	brickItem.childType = STUMBLING_BLOCK_GOLD_POD;
	brickItem.score = 100;
	brickItem.number = 1;
	brickItem.direction = 0;
	brickItem.attribute = 1;
	brickItem.bMove = true;

	if (m_starScore.score_1 <= 0)
	{
		m_tileBrick[brickItem.indexX][brickItem.indexY].gem.childType = GEM_COLOR_NO;
		m_tileBrick[brickItem.indexX][brickItem.indexY].specialBrick.push_back(brickItem);
		item.childType = STUMBLING_BLOCK_GOLD_POD + GEM_COLOR_COUNT;
		item.parentType = STUMBLING_BLOCK_PARENT_TYPE;
	}
	else if ((((float)m_scoreNumber / (float)m_starScore.score_1) >= (((float)(currentGoldPodNum+currentTileGoldNum)) / (float)targetPodNum) || currentTileGoldNum == 0) && m_bSwapGem)
	{
		m_tileBrick[brickItem.indexX][brickItem.indexY].gem.childType = GEM_COLOR_NO;
		m_tileBrick[brickItem.indexX][brickItem.indexY].specialBrick.push_back(brickItem);
		item.childType = STUMBLING_BLOCK_GOLD_POD + GEM_COLOR_COUNT;
		item.parentType = STUMBLING_BLOCK_PARENT_TYPE;
	}
}

void CGameLogic::changeGemColor(ChangeGemColour &outInfo)
{
	for (int posX = 0; posX < COL_COUNT; ++posX)
	{
		for (int posY = 0; posY < ROW_COUNT; ++posY)
		{
			TileBrickData &pItem = m_tileBrick[posX][posY];
			if (pItem.isShow == false || (pItem.gem.childType <= GEM_COLOR_NO && pItem.gem.gemStatus != GEM_STATUS_COLORFUL) 
				|| pItem.gem.gemStatus != GEM_STATUS_CHANGE)
			{
				continue;
			}

			pItem.gem.childType = randomCreateGem(m_levelsGemList).childType;
			changeGemColour pChange;
			pChange.point = cocos2d::Vec2(posX,posY);
			pChange.color = GemColor(pItem.gem.childType);
			outInfo.push_back(pChange);
		}
	}
}

bool CGameLogic::IsHaveUpPortal(int posX,int posY,tagSpecialBrickData *upPortal)
{
	if (posX < 0 || posX > COL_COUNT-1 || posY < 0 || posY > ROW_COUNT-1)
	{
		return false;
	}
	SpecialBrickData item = m_tileBrick[posX][posY].specialBrick;
	for (size_t i = 0; i < item.size(); ++i)
	{
		if (item[i].childType == STUMBLING_BLOCK_PORTAL && item[i].direction == UP_DIRECTION)
		{
			if (upPortal != NULL)
			{
				*upPortal = item[i];
			}
			return true;
		}
	}
	return false;
}
//@downPortal 找到的传送门
bool CGameLogic::IsHaveDownPortal(int posX,int posY,tagSpecialBrickData *downPortal)
{
	if (posX < 0 || posX > COL_COUNT-1 || posY < 0 || posY > ROW_COUNT-1)
	{
		return false;
	}
	SpecialBrickData item = m_tileBrick[posX][posY].specialBrick;
	for (size_t i = 0; i < item.size(); ++i)
	{
		if (item[i].childType == STUMBLING_BLOCK_PORTAL && item[i].direction == DOWN_DIRECTION)
		{
			*downPortal = item[i];
			return true;
		}
	}
	return false;
}

bool CGameLogic::IsCanConnectGem(TileBrickData &tile,TileBrickData &otherTile)
{
	std::vector<cocos2d::Vec2> outPoint;
	SpecialBrickData outSpecialBrick;

	if (tile.specialBrick.empty() == false)
	{
		for (SpecialBrickData::const_iterator spIte = tile.specialBrick.begin(); spIte != tile.specialBrick.end(); ++spIte)
		{
			enDirection dir = CENT_DIRECTION;
			if ((tile.indexX - otherTile.indexX) > 0)
			{
				dir = UP_DIRECTION;
			}
			else if ((tile.indexX - otherTile.indexX) < 0)
			{
				dir = DOWN_DIRECTION;
			}
			else if ((tile.indexY - otherTile.indexY) > 0)
			{
				dir = LEFT_DIRECTION;
			}
			else if ((tile.indexY - otherTile.indexY) < 0)
			{
				dir = RIGHT_DIRECTION;
			}

			if (spIte->childType == STUMBLING_BLOCK_PARTITION && spIte->direction == dir)
			{
				return false;
			}
		}
	}

	if (otherTile.specialBrick.empty() == false)
	{
		for (SpecialBrickData::const_iterator spIte = otherTile.specialBrick.begin(); spIte != otherTile.specialBrick.end(); ++spIte)
		{
			enDirection dir = CENT_DIRECTION;
			if ((otherTile.indexX - tile.indexX) > 0)
			{
				dir = UP_DIRECTION;
			}
			else if ((otherTile.indexX - tile.indexX) < 0)
			{
				dir = DOWN_DIRECTION;
			}
			else if ((otherTile.indexY - tile.indexY) > 0)
			{
				dir = LEFT_DIRECTION;
			}
			else if ((otherTile.indexY - tile.indexY) < 0)
			{
				dir = RIGHT_DIRECTION;
			}

			if (spIte->childType == STUMBLING_BLOCK_PARTITION && spIte->direction == dir)
			{
				return false;
			}
		}
	}

	bool result = false;
	if (IsValidMove(tile.indexX,tile.indexY,otherTile.indexX,otherTile.indexY,false) != NULL_SWAP_STATUS)
	{
		result =  true;
	}

	return result;
}

bool CGameLogic::IsCanMoveGem(int posX,int PosY)
{
	SpecialBrickData &brickItem = m_tileBrick[posX][PosY].specialBrick;
	for (size_t i = 0; i < brickItem.size(); ++i)
	{
		switch(brickItem[i].childType)
		{
		case STUMBLING_BLOCK_BULB:
		case STUMBLING_BLOCK_STENCIL:
		case  STUMBLING_BLOCK_JELLY:
		case STUMBLING_BLOCK_WOOD:
			return false;
		default:
			break;
		}
	}

	return true;
}

bool CGameLogic::IsCanConnect(int posX,int posY)
{
	if (posX < 0 || posX > COL_COUNT-1 || posY < 0 || posY > ROW_COUNT-1)
	{
		return false;
	}

	TileBrickData &item = m_tileBrick[posX][posY];
	if (item.isShow == false || item.gem.parentType == STUMBLING_BLOCK_PARENT_TYPE)
	{
		return false;
	}
	else if (IsHaveGemOrStumbling(item) == BRICK_TYPE)
	{
		return false;
	}
	else if (IsHaveGemOrStumbling(item) == NO_GEM_AND_BRICK_TYPE)
	{
		return false;
	}
	else if (IsHaveGemOrStumbling(item) == GEM_AND_BRICK_TYPE)
	{
		SpecialBrickData &spItem = item.specialBrick;
		for (SpecialBrickData::const_iterator ite = spItem.begin(); ite != spItem.end();++ite)
		{
			if (ite->childType == STUMBLING_BLOCK_BULB || ite->childType == STUMBLING_BLOCK_JELLY)
			{
				return false;
			}
		}
	}

	return true;
}

void CGameLogic::saveLevelData()
{
	TileBrick brick;
	brick.clear();
	for (int posX = 0; posX < ROW_COUNT; ++posX)
	{
		for (int posY = 0; posY < COL_COUNT; ++posY)
		{
			brick.push_back(m_tileBrick[posX][posY]);
		}
	}

	m_playingGameList.push_back(brick);
	m_oldStarScoreList.push_back(m_scoreNumber);
	m_gameTargetList.push_back(m_currentTargetList);
}

void CGameLogic::goBack()
{
	//棋盘返回上一步
	if (m_playingGameList.empty() == false)
	{
		TileBrick &brick = *(m_playingGameList.rbegin());
		for (size_t i = 0; i < brick.size(); ++i)
		{
			int posX = i / COL_COUNT;
			int posY = i % ROW_COUNT;

			m_tileBrick[posX][posY].id = brick[i].id;
			m_tileBrick[posX][posY].indexX = brick[i].indexX;
			m_tileBrick[posX][posY].indexY = brick[i].indexY;
			m_tileBrick[posX][posY].gem = brick[i].gem;
			m_tileBrick[posX][posY].isShow = brick[i].isShow;
			m_tileBrick[posX][posY].map.collect = brick[i].map.collect;

			m_tileBrick[posX][posY].specialBrick.clear();
			m_tileBrick[posX][posY].specialBrick.insert(m_tileBrick[posX][posY].specialBrick.begin(),brick[i].specialBrick.begin(),brick[i].specialBrick.end());
		}
		m_playingGameList.erase(m_playingGameList.end()-1);
	}


	//目标返回上一步
	if (m_gameTargetList.empty() == false)
	{
		TargetData &targetData = *(m_gameTargetList.rbegin());
		SafeErase(m_currentTargetList);
		m_currentTargetList.clear();
		for (size_t i = 0; i < targetData.size(); ++i)
		{
			m_currentTargetList.push_back(targetData[i]);
		}
		m_gameTargetList.erase(m_gameTargetList.end()-1);
	}
	
	//分数返回上一步
	if (m_oldStarScoreList.empty() == false)
	{
		m_scoreNumber = *(m_oldStarScoreList.rbegin());
		m_currentStep++;
		m_oldStarScoreList.erase(m_oldStarScoreList.end()-1);
	}

	m_bSwapGem = false;
}

TileBrick CGameLogic::getSpecialGem()
{
	TileBrick tile;
	tile.clear();
	for (int posX = 0; posX < ROW_COUNT; ++posX)
	{
		for (int posY = 0; posY < COL_COUNT; ++posY)
		{
			TileBrickData item = m_tileBrick[posX][posY];
			if (item.gem.gemStatus > GEM_STATUS_NORMAL && item.gem.gemStatus <= GEM_STATUS_COLORFUL)
			{
				tile.push_back(item);
			}
		}
	}

	return tile;
}

bool CGameLogic::changeGemType(int posX,int posY,GemStatus childType)
{
	TileBrickData &item = m_tileBrick[posX][posY];

	if (item.gem.childType <= GEM_COLOR_NO)
	{
		return false;
	}

	bool result = false;
	for (size_t i = 0; i < item.specialBrick.size(); ++i)
	{
		if (item.specialBrick[i].childType == STUMBLING_BLOCK_BULB || item.specialBrick[i].childType == STUMBLING_BLOCK_STENCIL)
		{
			result = true;
			break;
		}
	}

	if (item.isShow == false || item.gem.gemStatus > GEM_STATUS_NORMAL || IsHaveGemOrStumbling(item) == BRICK_TYPE
		|| (IsHaveGemOrStumbling(item) == GEM_AND_BRICK_TYPE && result))
	{
		return false;
	}
	item.gem.gemStatus = childType;
	if (childType == GEM_STATUS_COLORFUL)
	{
		item.gem.childType = GEM_COLOR_NO;
	}
	return true;
}

int CGameLogic::getGemNumberByColor(GemColor color)
{
	int number = 0;
	for (int posX = 0; posX < ROW_COUNT; ++posX)
	{
		for (int posY = 0; posY < COL_COUNT; ++posY)
		{
			if(m_tileBrick[posX][posY].gem.childType == color)
			{
				++number;
			}
		}
	}

	return number;
}

GemData CGameLogic::getTimeGem()
{
	GemData gemItem;
	gemItem.clear();
	for (int posX = 0; posX < ROW_COUNT; ++posX)
	{
		for (int posY = 0; posY < COL_COUNT; ++posY)
		{
			if(m_tileBrick[posX][posY].gem.gemStatus == GEM_STATUS_TIME_BOM)
			{
				gemItem.push_back(m_tileBrick[posX][posY].gem);
			}
		}
	}

	return gemItem;
}

void CGameLogic::changTimeBomGemTime()
{
	for (int posX = 0; posX < ROW_COUNT; ++posX)
	{
		for (int posY = 0; posY < COL_COUNT; ++posY)
		{
			if(m_tileBrick[posX][posY].gem.gemStatus == GEM_STATUS_TIME_BOM)
			{
				--m_tileBrick[posX][posY].gem.attribute;
			}
		}
	}
}

bool CGameLogic::isCanUseProp(int posX,int posY)
{
	TileBrickData &item = m_tileBrick[posX][posY];

	bool result = false;
	for (size_t i = 0; i < item.specialBrick.size(); ++i)
	{
		if (item.specialBrick[i].childType == STUMBLING_BLOCK_BULB || item.specialBrick[i].childType == STUMBLING_BLOCK_STENCIL)
		{
			result = true;
			break;
		}
	}

	if (item.isShow == false || item.gem.gemStatus > GEM_STATUS_NORMAL || IsHaveGemOrStumbling(item) == BRICK_TYPE
		|| (IsHaveGemOrStumbling(item) == GEM_AND_BRICK_TYPE && result))
	{
		return false;
	}
	return true;
}

bool CGameLogic::isCanUseColorProp()
{
	bool isCan = false;
	const TileBrick &tilerick = getCurrentTileBrick();
	for (TileBrick::const_iterator ite = tilerick.begin(); ite != tilerick.end(); ++ite)
	{
		const TileBrickData &item = *ite;

		bool result = false;
		for (size_t i = 0; i < item.specialBrick.size(); ++i)
		{
			if (item.specialBrick[i].childType == STUMBLING_BLOCK_BULB || item.specialBrick[i].childType == STUMBLING_BLOCK_STENCIL)
			{
				result = true;
				break;
			}
		}

		if (item.isShow == false || item.gem.gemStatus > GEM_STATUS_NORMAL || IsHaveGemOrStumbling(item) == BRICK_TYPE
			|| (IsHaveGemOrStumbling(item) == GEM_AND_BRICK_TYPE && result))
		{
			continue;
		}

		isCan = true;
	}
	
	return isCan;
}

void CGameLogic::showCurrentTile()
{
	CCLOG("%s\n","showCurrentTile");
	const TileBrick &pTile = getCurrentTileBrick();
	std::string str;
	std::string bufferData = "";
	for (int i = 0;i < (int)pTile.size();++i)
	{
		const tagGemData &pGem = pTile[i].gem;
		switch (pGem.childType)
		{
		case GEM_COLOR_RED:
			str = boost ::lexical_cast<std::string>("R");
			break;
		case GEM_COLOR_GREEN:
			str = boost::lexical_cast<std::string>("G");
			break;
		case GEM_COLOR_BLUE:
			str = boost::lexical_cast<std::string>("B");
			break;
		case GEM_COLOR_CYAN:
			str = boost::lexical_cast<std::string>("C");
			break;
		case GEM_COLOR_YELLOW:
			str = boost::lexical_cast<std::string>("Y");
			break;
		case GEM_COLOR_PURPLE:
			str = boost::lexical_cast<std::string>("P");
			break;
		default:
			str = boost::lexical_cast<std::string>("NU");
			break;
		}

		bufferData += str;
		bufferData += boost::lexical_cast<std::string>(pGem.gemStatus);

		if ((i % 9) == 0 && i > 0)
		{
			CCLOG("%s",bufferData.c_str());
			bufferData.clear();
		}
	}
}
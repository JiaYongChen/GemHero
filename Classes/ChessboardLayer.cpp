#include "ChessboardLayer.h"
#include "GameLogic.h"
#include "GameTargetNode.h"
#include "GameEnd.h"

USING_NS_CC;

ChessboardLayer::ChessboardLayer()
{
}

ChessboardLayer::~ChessboardLayer()
{
}

bool ChessboardLayer::init() {
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	_mapLayer = GameMapLayer::create();
	this->addChild(_mapLayer, 1, "mapLayer");

	_roleLayer = CGameRoleLayer::create();
	this->addChild(_roleLayer, 1, "roleLayer");
	_roleLayer->setDelegator(this);
	return true;
}

void ChessboardLayer::setMapData(const PBaseData& data) {
	_mapLayer->setLayerData(data);
}

void ChessboardLayer::setRoleData(const PBaseData& gemdata, const PBaseData& stumblingdata) {
	_roleLayer->setRoleData(gemdata,stumblingdata);
}

void ChessboardLayer::callBack(int type1, int type2, int type3) {
	switch (type1)
	{
	case GAME_STATUS_PLAYING:
		onPlayingGame(GamePlayingStatus(type2), type3);
		break;
	case GAME_STATUS_USE_PROP:
		onUseProp(enProp(type2));
		break;
	case GAME_STATUS_BONUS_TIME:
		onBonusTime();
		break;
	case GAME_STATUS_GAME_END:
		//CCLOG("GAME_STATUS_GAME_END");
		break;
	default:
		break;
	}
}

void ChessboardLayer::onPlayingGame(GamePlayingStatus playingStatus, int type) {
	switch (playingStatus)
	{
	case GAME_PLAYING_STATUS_NORMAL:
// 		CCLOG("GAME_PLAYING_STATUS_NORMAL");
		break;
	case GAME_PLAYING_STATUS_SWAP_GEM:
		swapGem(swapGemStatus(type));
		break;
	case GAME_PLAYING_STATUS_SHOW_ANIMATION:
	{
		CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_MOVE);
	}
		break;
	case GAME_PLAYING_STATUS_MOVE:
	{
		CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_NORMAL);
		connectGem();
	}
		break;
	case GAME_PLAYING_STATUS_SPECIAL:
		break;
	case GAME_PLAYING_STATUS_COLORFUL:
		break;
	default:
		break;
	}
}

void ChessboardLayer::swapGem(swapGemStatus status) {
	auto scene = Director::getInstance()->getRunningScene();
	CGameTargetNode *_targetNode = nullptr;
	if (scene != nullptr && strcmp(scene->getName().c_str(), "gamePlayingScene") == 0)
	{
		auto node = scene->getChildByName("gameLayer");
		if (node != nullptr)
		{
			_targetNode = (CGameTargetNode*)node->getChildByName("target");
			if (_targetNode != nullptr)
			{
				_targetNode->setSteps(CGameLogic::getInstance().getCurrentGateStep());
			}
		}
	}

	std::vector<Vec2> removeGem, removeStumbling;
	SpecialBrickData spData;
	std::vector<ObtainSpecialGemInfo> GemsInfo;
	removeGem.clear();
	spData.clear();
	GemsInfo.clear();
	if (status == NORMAL_SWAP_STATUS)
	{
		bool result = CGameLogic::getInstance().checkConnect(removeGem);
		if (result)
		{
			CGameLogic::getInstance().ObtainSpecialGems(GemsInfo, removeGem, spData);
			sendRemoveData(removeGem,spData,GemsInfo);
		}
	}
	else if (status == SPECIAL_SWAP_STATUS) 
	{
		CCLOG("SPECIAL_SWAP_STATUS");
		CGameLogic::getInstance().swapSpecialGem(removeGem, GemsInfo, spData);

// 		TCHAR str[256] = { 0 };
// 		_sntprintf_s(str, sizeof(str), "%d,%d,%d,%d", scoreX, scoreY, targetX, targetY);
// 		std::string strData = str;
// 
// 		const TileBrickItem* sourceT = m_GameClientView.m_GameLogic.currentTileBrickItem(scoreX, scoreY);
// 		const TileBrickItem* targetT = m_GameClientView.m_GameLogic.currentTileBrickItem(targetX, targetY);
// 		if (sourceT->gem.flag == GEM_STATUS_BOM && targetT->gem.flag == GEM_STATUS_BOM)
// 		{
// 			m_GameClientView.m_flashWnd.CallFlashFunction("showAnimation", 1, strData.c_str());
// 			m_GameClientView.m_flashWnd.CallFlashFunction("swapGemStatus", NORMAL_SWAP_STATUS);
// 		}
// 		else if (sourceT->gem.flag == GEM_STATUS_COLORFUL && targetT->gem.flag == GEM_STATUS_COLORFUL)
// 		{
// 			m_GameClientView.m_flashWnd.CallFlashFunction("showAnimation", 2, strData.c_str());
// 			m_GameClientView.m_flashWnd.CallFlashFunction("swapGemStatus", NORMAL_SWAP_STATUS);
// 		}
// 		else
// 		{
// 			m_GameClientView.m_flashWnd.CallFlashFunction("swapGemStatus", SPECIAL_SWAP_STATUS);
// 		}

		sendRemoveData(removeGem, spData, GemsInfo);
	}
	else
	{
		;
	}
}

void ChessboardLayer::connectGem() {
	LevelsGameStatus status = CGameLogic::getInstance().getGameStatus();

	/*收集金钻*/
	std::vector<Vec2> pPoint,spPoint;
	SpecialBrickData pBlockPoint;
	std::vector<ObtainSpecialGemInfo> pObtainSpecialGem;
	pBlockPoint.clear();
	pPoint.clear();
	pObtainSpecialGem.clear();
	CGameLogic::getInstance().changeTargetNum(pPoint, pBlockPoint, pObtainSpecialGem);
	if (!pPoint.empty() && status < GAME_STATUS_BONUS_TIME)
	{
		CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_SHOW_ANIMATION);
		_roleLayer->removeRoleData(pPoint, spPoint);
		//sendDataToFlash(pPoint, "disappearGem");

		if (status < GAME_STATUS_BONUS_TIME)
		{
			changeTargetNumber(CGameLogic::getInstance().getCurrentSuccessCondition());
		}

		//m_GameClientView.m_flashWnd.CallFlashFunction("setScoreNumber", buff.c_str());
	}

	pPoint.clear();
	pBlockPoint.clear();
	bool result = CGameLogic::getInstance().checkConnect(pPoint);

	if (result)
	{
		std::vector<ObtainSpecialGemInfo> GemsInfo;
		GemsInfo.clear();
		CGameLogic::getInstance().ObtainSpecialGems(GemsInfo, pPoint, pBlockPoint);
		sendRemoveData(pPoint, pBlockPoint, GemsInfo);
	}
	else
	{
		bool result = CGameLogic::getInstance().IsCanEndGame();
		if (result)
		{
			gameEndStatus flag = CGameLogic::getInstance().GameEndStatus();
			if (status == GAME_STATUS_BONUS_TIME)
			{
				std::vector<Vec2> spPoint;
				spPoint.clear();
				CGameLogic::getInstance().getAllSpecilGem(spPoint);
				if (spPoint.empty())
				{
					showGameEnd(flag);
				}
				else
				{
					touchSpecialGems();
				}
			}
			else if ((status == GAME_STATUS_PLAYING || status == GAME_STATUS_USE_PROP) && flag == SUCCESS_GAME_END)
			{
				touchSpecialGems();
			}
			else if ((status == GAME_STATUS_PLAYING || status == GAME_STATUS_USE_PROP) && flag < SUCCESS_GAME_END)
			{
				showGameEnd(flag);
			}
		}
		else if (status == GAME_STATUS_PLAYING)
		{
			GamePlayingStatus playingStatus = CGameLogic::getInstance().getGamePlayingStatus();
			if (playingStatus == GAME_PLAYING_STATUS_MOVE)
			{
				CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_SPECIAL);

				bool isSwapGem = CGameLogic::getInstance().getIsSwapGem();
				//特殊宝石变换颜色
				ChangeGemColour pChange;
				pChange.clear();
				if (isSwapGem)
				{
					CGameLogic::getInstance().changeGemColor(pChange);
				}

				//炸弹宝石
				if (isSwapGem)
				{
					CGameLogic::getInstance().changTimeBomGemTime();
					//m_GameClientView.m_flashWnd.CallFlashFunction("changTimeGemTimeNumber");
				}

				//特殊障碍增加或变换
				MoveSpecialBrick specialBrick;
				specialBrick.clear();
				if (isSwapGem)
				{
					CGameLogic::getInstance().ObtainSpecialBrickItems(specialBrick);
				}

				if (specialBrick.empty() && pChange.empty())
				{
					connectGem();
				}
				else
				{
					//变换颜色
// 					std::string bufferData = "";
// 					for (size_t i = 0; i < pChange.size(); ++i)
// 					{
// 						changeGemColour pColor = pChange[i];
// 						if (pColor.point.x >= 0 && pColor.point.y >= 0)
// 						{
// 							TCHAR str[256] = { 0 };
// 							_sntprintf_s(str, sizeof(str), "%d,%d,%d;", pColor.point.x, pColor.point.y, pColor.color);
// 							//InsertGeneralString(str,RGB(0,255,0),true);
// 							bufferData += str;
// 						}
// 					}

// 					bufferData += "|";
// 					//增加特殊障碍
// 					for (size_t i = 0; i < specialBrick.size(); ++i)
// 					{
// 						CPoint oldPoint = specialBrick[i].oldPoint;
// 						CPoint newPoint = specialBrick[i].newPoint;
// 						if ((newPoint.x >= 0 && newPoint.y >= 0 && newPoint.x < ROW_COUNT && newPoint.y < COL_COUNT)
// 							&& (oldPoint.x >= 0 && oldPoint.y >= 0 && oldPoint.x < ROW_COUNT && oldPoint.y < COL_COUNT))
// 						{
// 							TCHAR str[256] = { 0 };
// 							_sntprintf_s(str, sizeof(str), "%d,%d,%d,%d,%d;", oldPoint.x, oldPoint.y, newPoint.x, newPoint.y, specialBrick[i].type);
// 							//InsertGeneralString(str,RGB(0,0,255),true);
// 							bufferData += str;
// 						}
// 					}
// 
// 					m_GameClientView.m_flashWnd.CallFlashFunction("changGemColorAndAddSpecialBrick", bufferData.c_str());
				}
			}
			else if (playingStatus == GAME_PLAYING_STATUS_SPECIAL)
			{
				CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_NORMAL);
				bool result = CGameLogic::getInstance().retrieveConnectGem(Vec2(0, 0), Vec2(0, 0));
				if (result == false)
				{
					//m_GameClientView.m_flashWnd.CallFlashFunction("showCapion", 1);
				}
				else
				{
					int number = CGameLogic::getInstance().getCurrentComboNumber();
					if (number >= 5)
					{
						number = 5;
					}
					if (number > 1)
					{
						//m_GameClientView.m_flashWnd.CallFlashFunction("showCapion", number);
					}
					CGameLogic::getInstance().setCurrentComboNumber(0);
				}
			}
		}
		else if (status == GAME_STATUS_USE_PROP)
		{
			CGameLogic::getInstance().setGameStatus(GAME_STATUS_PLAYING);
			CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_NORMAL);
			CGameLogic::getInstance().setUsePropType(PROP_NULL);

			bool result = CGameLogic::getInstance().retrieveConnectGem(Vec2(0, 0), Vec2(0, 0));
			if (result == false)
			{
				//m_GameClientView.m_flashWnd.CallFlashFunction("showCapion", 1);
			}
		}
	}
}

void ChessboardLayer::sendRemoveData(std::vector<Vec2> &gem, SpecialBrickData &stumbling, std::vector<ObtainSpecialGemInfo> &info) {
	CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_SHOW_ANIMATION);
	CGameLogic::getInstance().touchSpecialGems(gem, stumbling, info);
	std::vector<Vec2> point;
	point.clear();
	for (tagSpecialBrickData data : stumbling)
	{
		point.push_back(Vec2(data.indexX, data.indexY));
	}
	_roleLayer->removeRoleData(gem, point);
	_roleLayer->setSpecialRoleData(info);
	changeTargetNumber(CGameLogic::getInstance().getCurrentSuccessCondition());
}

void ChessboardLayer::touchSpecialGems() {
	std::vector<Vec2> gemData;
	SpecialBrickData pBlockPoint;
	std::vector<ObtainSpecialGemInfo> pObtainSpecialGem;
	CGameLogic::getInstance().getAllSpecilGem(gemData);

	if (gemData.empty() && CGameLogic::getInstance().getGameStatus() != GAME_STATUS_BONUS_TIME)
	{
		CGameLogic::getInstance().setGameStatus(GAME_STATUS_BONUS_TIME);
		std::vector<ObtainSpecialGemInfo> GemsInfo;
		GemsInfo.clear();
		CGameLogic::getInstance().getBonusTimeGem(GemsInfo);
		_roleLayer->setSpecialRoleData(GemsInfo);
		touchSpecialGems();
		//sendSpecialGems(GemsInfo, "showBonusTime");
	}
	else if (gemData.empty() && CGameLogic::getInstance().getGameStatus() == GAME_STATUS_BONUS_TIME)
	{
		connectGem();
	}
	else
	{
		CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_SHOW_ANIMATION);
		CGameLogic::getInstance().touchSpecialGems(gemData, pBlockPoint, pObtainSpecialGem);
		std::vector<Vec2> stumbling;
		_roleLayer->removeRoleData(gemData, stumbling);
		
// 		std::string buff;
// 		TCHAR score[256] = { 0 };
// 		_sntprintf_s(score, sizeof(score), "%d", m_GameClientView.m_GameLogic.GetCurrentGateScore());
// 		buff += score;
// 		m_GameClientView.m_flashWnd.CallFlashFunction("setScoreNumber", buff.c_str());
	}
}

void ChessboardLayer::changeTargetNumber(const TargetData &data) {
	auto scene = Director::getInstance()->getRunningScene();
	CGameTargetNode *_targetNode = nullptr;
	if (scene != nullptr && strcmp(scene->getName().c_str(), "gamePlayingScene") == 0)
	{
		auto node = scene->getChildByName("gameLayer");
		if (node != nullptr)
		{
			_targetNode = (CGameTargetNode*)node->getChildByName("target");
		}
	}
	for (auto pData : data)
	{
		if (_targetNode != nullptr)
		{
			_targetNode->changeTargetNumber(pData.parentType, pData.type, pData.number);
		}
	}
}

void ChessboardLayer::onUseProp(enProp propType) {

}

void ChessboardLayer::onBonusTime() {
	CGameLogic::getInstance().setGameStatus(GAME_STATUS_PLAYING);
	CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_SHOW_ANIMATION);
	//connectGem();
}

void ChessboardLayer::showGameEnd(gameEndStatus success) {
	CGameLogic::getInstance().setGameStatus(GAME_STATUS_GAME_END);

	auto scene = Director::getInstance()->getRunningScene();
	CGameEndNode *gameEnd = nullptr;
	if (scene != nullptr && strcmp(scene->getName().c_str(), "gamePlayingScene") == 0)
	{
		gameEnd = (CGameEndNode*)scene->getChildByName("gameLayer")->getChildByName("gameEnd");
		if (gameEnd != nullptr)
		{
			gameEnd->showGame(success == SUCCESS_GAME_END?true:false, CGameLogic::getInstance().GetCurrentGateScore(), CGameLogic::getInstance().GetCurrentGateStar());
		}
	}
}
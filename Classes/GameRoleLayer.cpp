#include "GameRoleLayer.h"
#include "GameStumblingLayer.h"
#include "Resource.h"
#include "GameLogic.h"
#include "ChessboardLayer.h"

USING_NS_CC;

CGameGemLayer::CGameGemLayer()
{
	_touchBeginIndex.setPoint(-1, -1);
	_touchEndIndex.setPoint(-1, -1);
	_isTouch = false;
	_beginNode = nullptr;
	_endNode = nullptr;
	m_speed = 400;
}

CGameGemLayer::~CGameGemLayer()
{

}

bool CGameGemLayer::init()
{
	if (!CGamingLayer::init())
	{
		return false;
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(CGameGemLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(CGameGemLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(CGameGemLayer::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(CGameGemLayer::onTouchCancelled, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	//this->scheduleUpdate();
	return true;
}

void CGameGemLayer::update(float delta) {
	_runAction = false;
	GamePlayingStatus status = CGameLogic::getInstance().getGamePlayingStatus();
	switch (status)
	{
	case GAME_PLAYING_STATUS_SHOW_ANIMATION:
	{
		for (auto sprite : _data) {
			if ((SpriteNode*)sprite->getRunAction())
			{
				_runAction = true;
			}
		}
	}
		break;
	case GAME_PLAYING_STATUS_MOVE:
	{
		_runAction = true;
		bool result = false;
		CGameLogic *gameLogic = &(CGameLogic::getInstance());
		
		float  length  = m_speed * delta + 0.5 * 9.8 * delta * delta;
		m_speed += 9.8 * delta;
		m_speed = m_speed > 550 ? 550 : m_speed;

		for (int posX = COL_COUNT - 1; posX >= 0; --posX)
		{
			for (int posY = ROW_COUNT - 1; posY >= 0; --posY)
			{
				TileBrickData *pTileBrick = gameLogic->currentTileBrickItem(posX,posY);//&m_tileBrick[posX][posY];
				if (pTileBrick == nullptr /*|| (pTileBrick != nullptr && pTileBrick->gem.parentType == STUMBLING_BLOCK_PARENT_TYPE)*/)
				{
					continue;
				}
				if (pTileBrick->isShow == false ||
					(pTileBrick->gem.bMove == false && (pTileBrick->gem.childType > GEM_COLOR_NO ||
					(pTileBrick->gem.childType <= GEM_COLOR_NO&&pTileBrick->gem.gemStatus == GEM_STATUS_COLORFUL))))
				{
					continue;
				}

				auto visibleSize = Director::getInstance()->getVisibleSize();
				int offsetH = (visibleSize.height - ROW_COUNT * 61) / 2 + ROW_COUNT * 61;
				int offserW = (visibleSize.width - COL_COUNT * 61) / 2;

				if (pTileBrick->gem.moveAction == MOVE_ACTION)
				{
					SpriteNode *node = getSpriteNode(posX, posY);
					if (node != nullptr)
					{
						tagBaseData *pGem = node->getData();
						if (node != nullptr)
						{
							float moveX = node->getPositionX();
							float moveY = node->getPositionY() - length;
							float leftMove = node->getPositionX() - length;
							float rightMove = node->getPositionX() + length;

							enDirection moveDir = node->getMoveDirection();
							result = true;
							switch (moveDir)
							{
							case UP_DIRECTION:
								break;
							case DOWN_DIRECTION:
								node->setPositionY(moveY);
								//CCLOG("DOWN_DIRECTION:%d %d positionY%f",posX,posY,moveY);
								break;
							case LEFT_DIRECTION:
								node->setPosition(Vec2(leftMove, moveY));
								//CCLOG("LEFT_DIRECTION:%d %d", posX, posY);
								break;
							case RIGHT_DIRECTION:
								node->setPosition(Vec2(rightMove, moveY));
								//CCLOG("RIGHT_DIRECTION:%d %d", posX, posY);
								break;
							case CENT_DIRECTION: {
								node->setPosition(offserW + pGem->indexY * node->getSpriteNodeWidth() + node->getSpriteNodeWidth() / 2, offsetH - pGem->indexX * node->getSpriteNodeHeight() - node->getSpriteNodeHeight() / 2);
							}
								break;
							}

							if (node->getPositionY() <= offsetH - pGem->indexX * node->getSpriteNodeHeight() - node->getSpriteNodeHeight() / 2)
							{
								node->setMoveDirection(CENT_DIRECTION);
								setGemMove(pGem->indexX, pGem->indexY, NORMAL_ACTION);
							}
							else
							{
								continue;
							}

							if (node->getPositionY() <=  offsetH - pGem->indexX * node->getSpriteNodeHeight())
							{
								node->setVisible(true);
							}
						}
					}
				}

				TileBrickData *bottomBrick = gameLogic->currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY);
				tagSpecialBrickData downPortal;
				if (gameLogic->IsHaveDownPortal(pTileBrick->indexX, pTileBrick->indexY, &downPortal))
				{
					tagSpecialBrickData *pItem = gameLogic->getAnotherPortal(downPortal, true);
					if (pItem != NULL)
					{
						bottomBrick = gameLogic->currentTileBrickItem(pItem->indexX, pItem->indexY);
					}
				}

				TileBrickData *leftBottomBrick = gameLogic->currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY - 1);
				TileBrickData *rightBottomBrick = gameLogic->currentTileBrickItem(pTileBrick->indexX + 1, pTileBrick->indexY + 1);

				Vec2 dropPos;
				dropPos.x = -1;
				dropPos.y = -1;

				if (gameLogic->dropBottom(pTileBrick, bottomBrick, dropPos) == false)
				{
					if (gameLogic->dropLeftBottom(pTileBrick, leftBottomBrick, dropPos) == false)
					{
						if (gameLogic->dropRightBottom(pTileBrick, rightBottomBrick, dropPos) == false)
						{
							SpriteNode *currNode = getSpriteNode(pTileBrick->indexX, pTileBrick->indexY);
							if (currNode != nullptr)
							{
								auto visibleSize = Director::getInstance()->getVisibleSize();
								int offsetH = (visibleSize.height - ROW_COUNT * 61) / 2 + ROW_COUNT * 61;
								int offserW = (visibleSize.width - COL_COUNT * 61) / 2;
								currNode->setMoveDirection(CENT_DIRECTION);
								setGemMove(pTileBrick->indexX, pTileBrick->indexY,NORMAL_ACTION);
								currNode->setPosition(offserW + pTileBrick->indexY * currNode->getSpriteNodeWidth() + currNode->getSpriteNodeWidth() / 2, offsetH - pTileBrick->indexX * currNode->getSpriteNodeHeight() - currNode->getSpriteNodeHeight() / 2);
							}
						}
						else
						{
							SpriteNode *currNode = getSpriteNode(pTileBrick->indexX, pTileBrick->indexY);
							currNode->setData(&(rightBottomBrick->gem));
							currNode->setMoveDirection(RIGHT_DIRECTION);
							result = true;
						}
					}
					else
					{
						SpriteNode *currNode = getSpriteNode(pTileBrick->indexX, pTileBrick->indexY);
						currNode->setData(&(leftBottomBrick->gem));
						currNode->setMoveDirection(LEFT_DIRECTION);
						result = true;
					}
				}
				else
				{
					SpriteNode *currNode = getSpriteNode(pTileBrick->indexX, pTileBrick->indexY);
					currNode->setData(&(bottomBrick->gem));
					currNode->setMoveDirection(DOWN_DIRECTION);
					result = true;
				}

				tagGemData pGold;
				pGold.parentType = NULL_PARENT_TYPE;
				gameLogic->addNewGem(pTileBrick, pGold);				
				if (pGold.parentType != NULL_PARENT_TYPE)
				{
					result = true;
					TileBrickData *data = gameLogic->currentTileBrickItem(pGold.indexX, pGold.indexY);
					PBaseData pdata;
					if (data != nullptr)
					{
						if (pGold.parentType == NORMAL_GEM_PARENT_TYPE)
						{
							pdata.push_back(&(data->gem));
						}
						else if (pGold.parentType == STUMBLING_BLOCK_PARENT_TYPE)
						{								
							data->gem.childType = STUMBLING_BLOCK_GOLD_POD + GEM_COLOR_COUNT;
							pdata.push_back(&(data->gem));
						}
					}
					this->setLayerData(pdata);
					for (tagBaseData *basedata : pdata)
					{
						SpriteNode* node =  getSpriteNode(basedata->indexX, basedata->indexY);
						if (node != nullptr)
						{
							node->setPositionY(offsetH - basedata->indexX * node->getSpriteNodeHeight() + node->getSpriteNodeHeight() / 2);
							node->setVisible(false);
							node->setMoveDirection(DOWN_DIRECTION);
						}
					}
				}
			}
		}

		if (result == false)
		{
			_runAction = false;
			//_delegator->callBack(CGameLogic::getInstance().getGamePlayingStatus(), CGameLogic::getInstance().getGamePlayingStatus(), -1);
		}
	}
		break;
	default:
		break;
	}
}

void CGameGemLayer::setGemMove(int indexX, int indexY , enRunAction action) {
	for (auto sprite : _data)
	{
		if (sprite->getData()->indexX == indexX && sprite->getData()->indexY == indexY)
		{
			sprite->getData()->moveAction = action;
		}
	}
}

bool CGameGemLayer::onTouchBegan(Touch *touch, Event *unused_event) {
	if (_runAction)
	{
		return false;
	}
	auto target = static_cast<CGameGemLayer*>(unused_event->getCurrentTarget());
	for (auto sprite : _data)
	{
		Vec2 layerPoint = touch->getLocation();
		if (sprite->containsPointTest(layerPoint))
		{
			if (_isTouch == false)
			{
				_touchBeginIndex = sprite->getSpriteNodeIndex();
				_beginNode = sprite;
			}
			
			return true;
		}
	}
	_isTouch = false;
	_touchBeginIndex.setPoint(-1, -1);
	_touchEndIndex.setPoint(-1, -1);
	return false;
}

void CGameGemLayer::onTouchMoved(Touch *touch, Event *unused_event) {
	
	auto target = static_cast<CGameGemLayer*>(unused_event->getCurrentTarget());
	for (auto sprite : _data)
	{
		Vec2 layerPoint = touch->getLocation();
		if (sprite->containsPointTest(layerPoint))
		{
			Vec2 index = sprite->getSpriteNodeIndex();

			if (index != _touchBeginIndex && _isTouch == false)
			{
				_isTouch = true;
			}

			break;
		}
	}
}

void CGameGemLayer::onTouchEnded(Touch *touch, Event *unused_event) {
	auto target = static_cast<CGameGemLayer*>(unused_event->getCurrentTarget());
	for (auto sprite : _data)
	{
		Vec2 layerPoint = touch->getLocation();
		if (sprite->containsPointTest(layerPoint))
		{
			_touchEndIndex = sprite->getSpriteNodeIndex();
			_endNode = sprite;
			
			if (_touchBeginIndex == _touchEndIndex)
			{
				_isTouch = true;
			}
			break;
		}
	}

	if (_touchBeginIndex != _touchEndIndex && _isTouch)
	{
		tagGemData *scoreData = (tagGemData *)getData(_touchBeginIndex.x, _touchBeginIndex.y);
		tagGemData *targetData = (tagGemData *)getData(_touchEndIndex.x, _touchEndIndex.y);
		if (scoreData == nullptr || targetData == nullptr) return;
		if (scoreData->bMove == false || targetData->bMove == false)
		{
			_isTouch = false;
			_touchBeginIndex.setPoint(-1, -1);
			_touchEndIndex.setPoint(-1, -1);
			return;
		}
		if (abs(_touchBeginIndex.x - _touchEndIndex.x) <= 1 && abs(_touchBeginIndex.y - _touchEndIndex.y) <= 1 && abs(_touchBeginIndex.x - _touchEndIndex.x) + abs(_touchBeginIndex.y - _touchEndIndex.y) <= 1)
		{
			float MoveTime = 0.2f;
			_beginNode->runAction(MoveTo::create(MoveTime, _endNode->getPosition()));
			_endNode->runAction(Sequence::create(MoveTo::create(MoveTime, _beginNode->getPosition()), CallFunc::create([this,MoveTime]() {
				unsigned short scorex = static_cast<unsigned short>(_touchBeginIndex.x);
				unsigned short scorey = static_cast<unsigned short>(_touchBeginIndex.y);
				unsigned short targetx = static_cast<unsigned short>(_touchEndIndex.x);
				unsigned short targety = static_cast<unsigned short>(_touchEndIndex.y);
				swapGemStatus move = CGameLogic::getInstance().IsValidMove(scorex,scorey,targetx,targety);
				if (move != NULL_SWAP_STATUS){
					tagGemData *beginData = (tagGemData*)_beginNode->getData();
					tagGemData *endData = (tagGemData*)_endNode->getData();
					tagGemData *data = beginData;
					_beginNode->setData(endData);
					_endNode->setData(data);

					if (_delegator != nullptr)
					{
						_delegator->callBack(CGameLogic::getInstance().getGameStatus(), CGameLogic::getInstance().getGamePlayingStatus(), move);
					}
				}else {
					_beginNode->runAction(MoveTo::create(MoveTime, _endNode->getPosition()));
					_endNode->runAction(MoveTo::create(MoveTime, _beginNode->getPosition()));
					CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_NORMAL);
				}
				_isTouch = false;
				_touchBeginIndex.setPoint(-1, -1);
				_touchEndIndex.setPoint(-1, -1);
			}), NULL));

			CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_SWAP_GEM);
		}else{
			_isTouch = false;
			_touchBeginIndex.setPoint(-1, -1);
			_touchEndIndex.setPoint(-1, -1);
		}
	}
}

void CGameGemLayer::onTouchCancelled(Touch *touch, Event *unused_event) {
	_isTouch = false;
	_touchBeginIndex.setPoint(-1, -1);
	_touchEndIndex.setPoint(-1, -1);
}

void CGameGemLayer::setLayerData(const PBaseData &data){
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int offsetH = (visibleSize.height - ROW_COUNT * 61) / 2 + ROW_COUNT * 61;
	int offserW = (visibleSize.width - COL_COUNT * 61) / 2;

	for (auto pData : data)
	{
		if (pData->childType <= GEM_COLOR_NO /*|| pData->childType >= GEM_COLOR_COUNT*/)
		{
			continue;
		}
		char name[1024] = { 0 };
		if (pData->childType < GEM_COLOR_COUNT)
		{
			snprintf(name, sizeof(name), "res/gameScene/normal/%s.png", GEM_NAME[pData->parentType][pData->childType].c_str());
		}
		else
		{
			int index = pData->childType - GEM_COLOR_COUNT;
			snprintf(name, sizeof(name), "res/gameScene/strumbling/%s.png", GEM_NAME[STUMBLING_BLOCK_PARENT_TYPE][index].c_str());
			pData->parentType = STUMBLING_BLOCK_PARENT_TYPE;
			pData->childType = index;
		}

		tagGemData *gemData = (tagGemData*)pData;
		if (gemData->gemStatus == GEM_STATUS_COLORFUL)
		{
			snprintf(name, sizeof(name), "res/gameScene/normal/%s.png", GEM_NAME[NORMAL_GEM_PARENT_TYPE][0].c_str());
		}

		GemSprite* node = GemSprite::create();
		if (node->initWithData(name, gemData) == false) continue;
		node->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		node->setPosition(offserW + pData->indexY * node->getSpriteNodeWidth() + node->getSpriteNodeWidth() / 2, offsetH - pData->indexX * node->getSpriteNodeHeight() - node->getSpriteNodeHeight() / 2);
		this->addChild(node);

		_data.push_back(node);
	}
}

void CGameGemLayer::removeLayerData(const PBaseData &data) {
	for (auto pData : data)
	{
		std::vector<SpriteNode*>::iterator ite = _data.begin();
		do 
		{
			SpriteNode *sprite = *ite;
			if (sprite->getSpriteNodeIndex().x == pData->indexX && sprite->getSpriteNodeIndex().y == pData->indexY)
			{
				sprite->runAction(Sequence::create(Blink::create(0.3f, 3), CallFunc::create([this, sprite]() {
					this->removeChild(sprite);
					auto ite = std::find(_data.begin(), _data.end(), sprite);
					_data.erase(ite);
				}), nullptr));
				sprite->getData()->childType = GEM_COLOR_NO;
				sprite->setRunAction(true);
			}
			++ite;
		} while (ite != _data.end());
	}
}

//////////////////////////////////////////////////////////////////////////
CGameRoleLayer::CGameRoleLayer() {

	_upStumblingLayer = nullptr;
	_gemLayer = nullptr;
	_downStumblingLayer = nullptr;
}

CGameRoleLayer::~CGameRoleLayer() {

}

bool CGameRoleLayer::init() {
	if (!Layer::init())
	{
		return false;
	}

	_upStumblingLayer = CGameUpStumblingLayer::create();
	this->addChild(_upStumblingLayer, 3);

	_gemLayer = CGameGemLayer::create();
	this->addChild(_gemLayer, 2);

	_downStumblingLayer = CGameDownStumblingLayer::create();
	this->addChild(_downStumblingLayer, 1);

	this->scheduleUpdate();
	return true;
}

void CGameRoleLayer::update(float delta) {
	_upStumblingLayer->update(delta);
	_gemLayer->update(delta);
	_downStumblingLayer->update(delta);
	if (_gemLayer->getRunAction() == false && _downStumblingLayer->getRunAction() == false && _upStumblingLayer->getRunAction() == false)
	{
		_delegator->callBack(CGameLogic::getInstance().getGameStatus(), CGameLogic::getInstance().getGamePlayingStatus(),-1);
	}
}

void CGameRoleLayer::setRoleData(const PBaseData &gemData, const PBaseData &spData) {
	PBaseData newGemData;
	for (auto data : gemData) {
		newGemData.push_back(data);
	}

	PBaseData upData, downData;
	upData.clear();
	downData.clear();
	for (auto data : spData)
	{
		if (((tagBaseData*)data)->childType == STUMBLING_BLOCK_GOLD_POD || ((tagBaseData*)data)->childType == STUMBLING_BLOCK_SILVER_COIN)
		{
			TileBrickData *pTile = CGameLogic::getInstance().currentTileBrickItem(data->indexX, data->indexY);
			pTile->gem.parentType = NORMAL_GEM_PARENT_TYPE;
			pTile->gem.childType = GEM_COLOR_COUNT + data->childType;
			//newGemData.push_back(&(pTile->gem));
		}
		else if (data->childType == STUMBLING_BLOCK_GLASS)
		{
			downData.push_back(data);
		}
		else{
			upData.push_back(data);
		}
	}

	dynamic_cast<CGameUpStumblingLayer*>(_upStumblingLayer)->setLayerData(upData);
	dynamic_cast<CGameGemLayer*>(_gemLayer)->setLayerData(newGemData);
	dynamic_cast<CGameDownStumblingLayer*>(_downStumblingLayer)->setLayerData(downData);
}

void CGameRoleLayer::removeRoleData(const std::vector<cocos2d::Vec2> gemData, const std::vector<cocos2d::Vec2> spData) {
	PBaseData pGemData;
	for (auto data : gemData) {
		tagBaseData *gData = dynamic_cast<CGameGemLayer*>(_gemLayer)->getData(data.x,data.y);
		if (gData != nullptr)
		{
			pGemData.push_back(gData);
		}
	}
	dynamic_cast<CGameGemLayer*>(_gemLayer)->removeLayerData(pGemData);

	PBaseData pUpStumblingData;
	for (auto data : spData) {
		tagBaseData *gData = dynamic_cast<CGameUpStumblingLayer*>(_upStumblingLayer)->getData(data.x, data.y);
		if (gData != nullptr)
		{
			pUpStumblingData.push_back(gData);
		}
	}
	dynamic_cast<CGameUpStumblingLayer*>(_upStumblingLayer)->removeLayerData(pUpStumblingData);

	PBaseData pDownStumblingData;
	for (auto data : spData) {
		tagBaseData *gData = dynamic_cast<CGameDownStumblingLayer*>(_downStumblingLayer)->getData(data.x, data.y);
		if (gData != nullptr)
		{
			pDownStumblingData.push_back(gData);
		}
	}
	dynamic_cast<CGameDownStumblingLayer*>(_downStumblingLayer)->removeLayerData(pDownStumblingData);
}

void CGameRoleLayer::setSpecialRoleData(const std::vector<ObtainSpecialGemInfo> &info) {
	for (auto data : info)
	{
		GemSprite *sprite = (GemSprite*)dynamic_cast<CGameGemLayer*>(_gemLayer)->getSpriteNode(data.point.x, data.point.y);
		if (sprite != nullptr)
		{
			sprite->setGemStatus(data.flag);
		}
	}
}

void CGameRoleLayer::setDelegator(GamingDelegator *delegator) {
	_upStumblingLayer->setDelegator(delegator);
	_gemLayer->setDelegator(delegator);
	_downStumblingLayer->setDelegator(delegator);
	_delegator = delegator;
}
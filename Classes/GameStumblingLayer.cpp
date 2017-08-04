#include "GameStumblingLayer.h"
#include "Resource.h"

USING_NS_CC;

CGameStumblingLayer::CGameStumblingLayer() {

}

CGameStumblingLayer::~CGameStumblingLayer() {

}

bool CGameStumblingLayer::init() {
	if (!CGamingLayer::init())
	{
		return false;
	}

	return true;
}

void CGameStumblingLayer::setLayerData(const PBaseData &data) {

	if (!_data.empty())
	{
		for (auto node : _data)
		{
			this->removeChild(node);
		}
	}
	_data.clear();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int offsetH = (visibleSize.height - ROW_COUNT * 61) / 2 + ROW_COUNT * 61;
	int offserW = (visibleSize.width - COL_COUNT * 61) / 2;

	for (auto pData : data)
	{
		if (pData->childType <= GEM_COLOR_NO || pData->childType >= GEM_COLOR_COUNT)
		{
			continue;
		}
		char name[1024] = { 0 };
		snprintf(name, sizeof(name), "res/gameScene/strumbling/%s.png", GEM_NAME[pData->parentType][pData->childType].c_str());

		if (pData->childType == STUMBLING_BLOCK_WOOD)
		{
			memset(name, 0, sizeof(name));
			tagSpecialBrickData stData = *((tagSpecialBrickData*)pData);
			snprintf(name, sizeof(name), "res/gameScene/strumbling/%s%d.png", GEM_NAME[pData->parentType][pData->childType].c_str(),stData.number);
		}
		else if (pData->childType == STUMBLING_BLOCK_PARTITION)
		{
			memset(name, 0, sizeof(name));
			tagSpecialBrickData stData = *((tagSpecialBrickData*)pData);
			switch (stData.direction)
			{
			case UP_DIRECTION:
				snprintf(name, sizeof(name), "res/gameScene/strumbling/%s%d.png", GEM_NAME[pData->parentType][pData->childType].c_str(), 2);
				break;
			case DOWN_DIRECTION:
				snprintf(name, sizeof(name), "res/gameScene/strumbling/%s%d.png", GEM_NAME[pData->parentType][pData->childType].c_str(), 4);
				break;;
			case LEFT_DIRECTION:
				snprintf(name, sizeof(name), "res/gameScene/strumbling/%s%d.png", GEM_NAME[pData->parentType][pData->childType].c_str(), 1);
				break;
			case RIGHT_DIRECTION:
				snprintf(name, sizeof(name), "res/gameScene/strumbling/%s%d.png", GEM_NAME[pData->parentType][pData->childType].c_str(), 3);
				break;
			default:
				break;
			}
		}
		StumblingSprite* node = StumblingSprite::create();
		if (node->initWithData(name, pData) == false) continue;
		node->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		node->setPosition(offserW + pData->indexY * node->getSpriteNodeWidth() + node->getSpriteNodeWidth() / 2, offsetH - pData->indexX * node->getSpriteNodeHeight() - node->getSpriteNodeHeight() / 2);
		this->addChild(node);

		_data.push_back(node);
	}
}

void CGameStumblingLayer::removeLayerData(const PBaseData &data) {

}

//////////////////////////////////////////////////////////////////////////
CGameDownStumblingLayer::CGameDownStumblingLayer() {

}

CGameDownStumblingLayer::~CGameDownStumblingLayer() {

}

bool CGameDownStumblingLayer::init() {
	if (!CGameStumblingLayer::init())
	{
		return false;
	}

	return true;
}

void CGameDownStumblingLayer::removeLayerData(const PBaseData &data) {

}

//////////////////////////////////////////////////////////////////////////
CGameUpStumblingLayer::CGameUpStumblingLayer() {

}

CGameUpStumblingLayer::~CGameUpStumblingLayer() {

}

bool CGameUpStumblingLayer::init() {
	if (!CGameStumblingLayer::init())
	{
		return false;
	}

	return true;
}

void CGameUpStumblingLayer::removeLayerData(const PBaseData &data) {

}
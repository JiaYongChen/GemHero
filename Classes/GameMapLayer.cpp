#include "GameMapLayer.h"
#include "Resource.h"

using namespace std;
USING_NS_CC;

GameMapLayer::GameMapLayer()
{
}

GameMapLayer::~GameMapLayer()
{
}

bool GameMapLayer::init() {
	if (!CGamingLayer::init())
	{
		return false;
	}

	return true;
}

void GameMapLayer::setLayerData(const PBaseData &data) {
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

	for (auto  item: data)
	{
		tagMapData *pData = (tagMapData*)item;
		if (pData->isShow == false)
		{
			continue;
		}
		char name[1024] = { 0 };
		snprintf(name, sizeof(name), "res/gameScene/%s.png", GEM_NAME[pData->parentType][0].c_str());
		MapSprite* node = MapSprite::create();
		if (node->initWithData(name, pData) == false) continue;
		node->setPosition(offserW + pData->indexY * node->getSpriteNodeWidth()+node->getSpriteNodeWidth()/2, offsetH - pData->indexX * node->getSpriteNodeHeight()-node->getSpriteNodeHeight()/2);
		this->addChild(node);

		_data.push_back(node);
	}
}

void GameMapLayer::removeLayerData(const PBaseData &data) {

}

#include "GamingLayer.h"
#include "Resource.h"

USING_NS_CC;

CGamingLayer::CGamingLayer()
{
	_delegator = nullptr;
	_runAction = false;
}

CGamingLayer::~CGamingLayer()
{

}

bool CGamingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_data.clear();
	return true;
}

void CGamingLayer::setLayerData(const PBaseData &data)
{
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

	for (tagBaseData* baseData : data)
	{
		tagMapData *pData = (tagMapData*)baseData;
		if (pData->isShow == false)
		{
			continue;
		}
		char name[1024] = { 0 };
		snprintf(name, sizeof(name), "res/gameScene/%s.png", GEM_NAME[pData->parentType][0].c_str());
		SpriteNode* node = SpriteNode::create();
		if(node->initWithData(name, pData) == false) continue;
		node->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		node->setPosition(offserW + pData->indexY * node->getSpriteNodeWidth(), offsetH - pData->indexX * node->getSpriteNodeHeight());
		this->addChild(node);

		_data.push_back(node);
	}
}

void CGamingLayer::removeLayerData(const PBaseData &data){

}
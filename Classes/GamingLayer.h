#pragma once
#ifndef _GAMING_LAYER_H_
#define _GAMING_LAYER_H_

#include "SpriteNode.h"

class GamingDelegator {
public:
	virtual void callBack(int type1, int type2, int type3) = 0;
};

class IGamingLayer
{
public:
	virtual  void setLayerData(const PBaseData &data) = 0;
	virtual void removeLayerData(const PBaseData &data) = 0;
};

class CGamingLayer : public cocos2d::Layer, public IGamingLayer
{
public:
	CGamingLayer();
	virtual ~CGamingLayer();
	CREATE_FUNC(CGamingLayer);

public:
	virtual void setLayerData(const PBaseData &data);
	virtual void removeLayerData(const PBaseData &data);

	tagBaseData* getData(int indexX, int indexY);
	SpriteNode* getSpriteNode(int indexX, int indexY);

public:
	void setDelegator(GamingDelegator *delegator) { _delegator = delegator; }

	CC_SYNTHESIZE(bool, _runAction, RunAction);

protected:
	bool init();

protected:
	std::vector<SpriteNode*> _data;

	GamingDelegator *_delegator;
};

#endif
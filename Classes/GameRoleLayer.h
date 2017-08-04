#pragma once
#ifndef _GAME_ROLE_LAYER_H_
#define _GAME_ROLE_LAYER_H_
#include "GamingLayer.h"

class CGameGemLayer : public CGamingLayer
{
public:
	CGameGemLayer();
	~CGameGemLayer();
	CREATE_FUNC(CGameGemLayer);

protected:
	virtual bool init();
	
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

public:
	virtual void update(float delta);
	virtual void setLayerData(const PBaseData &data);
	virtual void removeLayerData(const PBaseData &data);
	tagGemData* getGemData(int indexX, int indexY);
	SpriteNode* getSpriteNode(int indexX, int indexY);

private:
	void setGemMove(int indexX, int indexY, enRunAction action);

private:
	cocos2d::Vec2 _touchBeginIndex;
	cocos2d::Vec2 _touchEndIndex;
	bool _isTouch;
	SpriteNode* _beginNode;
	SpriteNode* _endNode;

public:
	float m_speed;
};

class CGameRoleLayer : public cocos2d::Layer
{
public:
	CGameRoleLayer();
	virtual ~CGameRoleLayer();
	CREATE_FUNC(CGameRoleLayer);

	virtual bool init();
	virtual void update(float delta);
	void setRoleData(const PBaseData &gemData,const PBaseData &spData);
	void removeRoleData(const std::vector<cocos2d::Vec2> gemData, const std::vector<cocos2d::Vec2> spData);
	void setSpecialRoleData(const std::vector<ObtainSpecialGemInfo> &info);

	void setDelegator(GamingDelegator *delegator);

private:
	CGamingLayer *_upStumblingLayer;
	CGamingLayer *_gemLayer;
	CGamingLayer *_downStumblingLayer;

	GamingDelegator *_delegator;
};

#endif // !_GAME_ROLE_LAYER_H_
#pragma once
#ifndef _SPRITE_NODE_H_
#define _SPRITE_NODE_H_

#include "cocos2d.h"
#include "CMD_ThreeEliminate.h"

class SpriteNode : public cocos2d::Node/* , public ISpritNode*/ {
public:
	SpriteNode();
	virtual ~SpriteNode();
	CREATE_FUNC(SpriteNode);

protected:
	virtual bool init();

public:
	virtual bool initWithData(const std::string& name, tagBaseData* data);
	float getSpriteNodeHeight();
	float getSpriteNodeWidth();
	void setData(tagBaseData* data);
	void setIndexText();
	tagBaseData* getData();

	bool containsPointTest(cocos2d::Vec2 point);
	cocos2d::Vec2 getSpriteNodeIndex();

	CC_SYNTHESIZE(bool, _runAction, RunAction);
	CC_SYNTHESIZE(enDirection, _direction, MoveDirection);

protected:
	tagBaseData *_data;
	cocos2d::Sprite *_spriteNode;
	cocos2d::Label *_indexText;
};

class MapSprite: public SpriteNode
{
public:
	MapSprite();
	~MapSprite();
	CREATE_FUNC(MapSprite);

protected:
	virtual bool init();

public:
	virtual bool initWithData(const std::string& name, tagBaseData* data);

private:
	cocos2d::Sprite *_collectSprite;
};

class GemSprite : public SpriteNode
{
public:
	GemSprite();
	~GemSprite();
	CREATE_FUNC(GemSprite);

protected:
	virtual bool init();

public:
	virtual bool initWithData(const std::string& name, tagBaseData* data);
	void setGemStatus(GemStatus status);
	
private:
	std::string getGemStatusString(GemStatus status);

private: 
	GemStatus _gemStatus;
	cocos2d::Label *_gemStatusLable;
};

class StumblingSprite : public SpriteNode
{
public:
	StumblingSprite();
	~StumblingSprite();
	CREATE_FUNC(StumblingSprite);

protected:
	virtual bool init();

public:
	virtual bool initWithData(const std::string& name, tagBaseData* data);
};

#endif

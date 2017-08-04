#pragma once
#ifndef _GAME_PLAYING_SCENE_
#define _GAME_PLAYING_SCENE_

#include "cocos2d.h"

class CGamePlayingScene : public cocos2d::Layer
{
public:
	CGamePlayingScene();
	virtual ~CGamePlayingScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(CGamePlayingScene);

public:
	void clear();

	void setGamePlayingData();

private: 
	void goBackToMainScene(cocos2d::Ref* pSender);
};

#endif
#pragma once
#ifndef _GAME_END_
#define _GAME_END_

#include "cocos2d.h"

class CGameEndNode : public cocos2d::Node
{
public:
	CGameEndNode();
	virtual ~CGameEndNode();
	CREATE_FUNC(CGameEndNode);

private:
	virtual bool init();
	void goBackToMainScene(cocos2d::Ref* pSender);

public:
	void showGame(bool success, int score, int starNumber);
};

#endif // !_GAME_END_
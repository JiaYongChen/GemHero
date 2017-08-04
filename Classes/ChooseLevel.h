#pragma once
#include "cocos2d.h"
#include "CMD_ThreeEliminate.h"

class CChooseLevel: public cocos2d::Node
{
public :
	CChooseLevel();
	~CChooseLevel();

	CREATE_FUNC(CChooseLevel);

public :
	void showChooseLevel(int levelNumber, LevelGameType type);

	void setStartNumber(int number);

protected:
	virtual bool init();

private:
	void onClickClose(cocos2d::Ref*);
	void onClickStartGame(cocos2d::Ref*);

private:
	cocos2d::Node* _chooseLevel;
	int _levelNumber;
};
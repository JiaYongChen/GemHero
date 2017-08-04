#pragma once
#include "cocos2d.h"
#include "MainlevelInfo.h"
#include "ui/CocosGUI.h"

class  CMainLevelLayer:public cocos2d::Layer
{
public:
	 CMainLevelLayer();
	~ CMainLevelLayer();

	CREATE_FUNC(CMainLevelLayer);

public:
	//初始化关卡
	void initLevel(int number);
	//解锁关卡
	void setUnlockLevel(int unlockNumber);
	//设置当前关卡
	void setCurrentLevel(int currentNumber);

protected:
	virtual bool init();

	virtual void onEnter();

public:
	//最大关卡数
	CC_SYNTHESIZE(int, _maxNumber, MaxNumber);
	//当前解锁的关卡数
	CC_SYNTHESIZE(int, _currentUnlockNumber, CurrentUnlockNumber);
	//当前关卡
	CC_SYNTHESIZE(int, _currentLevelNumber, CurrentLevelNumber);

private:
	cocos2d::ui::ScrollView *_scoreView;
	cocos2d::Array _levelInfoList;
};
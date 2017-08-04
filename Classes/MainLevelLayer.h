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
	//��ʼ���ؿ�
	void initLevel(int number);
	//�����ؿ�
	void setUnlockLevel(int unlockNumber);
	//���õ�ǰ�ؿ�
	void setCurrentLevel(int currentNumber);

protected:
	virtual bool init();

	virtual void onEnter();

public:
	//���ؿ���
	CC_SYNTHESIZE(int, _maxNumber, MaxNumber);
	//��ǰ�����Ĺؿ���
	CC_SYNTHESIZE(int, _currentUnlockNumber, CurrentUnlockNumber);
	//��ǰ�ؿ�
	CC_SYNTHESIZE(int, _currentLevelNumber, CurrentLevelNumber);

private:
	cocos2d::ui::ScrollView *_scoreView;
	cocos2d::Array _levelInfoList;
};
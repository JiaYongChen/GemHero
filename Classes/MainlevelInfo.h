#pragma once
#include "cocos2d.h"
#include "MainLevel.h"

class CMainlevelInfo : public cocos2d::Node
{
public:
	static const int LEVELNUMBER = 7;
	static const int LEVELHEIGHT = 400;
	static const cocos2d::Vec2 LEVELPOSITION[LEVELNUMBER];
public:
	CMainlevelInfo();
	~CMainlevelInfo();

	CREATE_FUNC(CMainlevelInfo);

public:
	//��ʼ���ؿ���Ϣ
	void initLevelInfo(int index,int number = LEVELNUMBER);
	//��ȡlevelList
	cocos2d::Array& getLevelList() { return _levelList; }

protected:
	virtual bool init();

private:
	cocos2d::Node* _levelInfo;
	cocos2d::Array _levelList;

	CC_SYNTHESIZE(int, _Index, Index);
};


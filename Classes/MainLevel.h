#pragma once
#include "cocos2d.h"

class CMainLevel : public cocos2d::Node
{
public:
	CMainLevel();
	~CMainLevel();

	CREATE_FUNC(CMainLevel);

	void setLevelNumber(int levelNumber);

	void setStarNumber(unsigned char starNumber);

	void setIsUnlock(bool isUnlock);

protected:
	virtual bool init();

private: 
	void onClick(cocos2d::Ref *);

	void changeLock();

	void changeStar();

	int _levelNumber;

	unsigned char _starNumber;

	bool _IsUnlock;

private:
	cocos2d::Node *_levelNode; //¹Ø¿¨½Úµã
};


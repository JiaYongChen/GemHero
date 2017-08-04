#pragma once
#ifndef _GAME_TARGET_NODE_
#define _GAME_TARGET_NODE_
#include "cocos2d.h"
#include "CMD_ThreeEliminate.h"

class CTarget :public cocos2d::Node {
public:
	CTarget();
	virtual ~CTarget();
	CREATE_FUNC(CTarget);
	virtual bool init();
	//设置目标数据
	void setTarget(const tagTargetData data);
	//显示对号
	void showRight(bool flag = false);
	//改变目标数量
	void changeTargetNumber(int number);
	//获取目标数据
	const tagTargetData getTargetData() { return _targetData; }

private:
	tagTargetData _targetData;
};

class CGameTargetNode : public cocos2d::Node
{
public:
	CGameTargetNode();
	virtual ~CGameTargetNode();
	CREATE_FUNC(CGameTargetNode);

	virtual bool init();
	//清除数据
	void clearData();
	//设置关卡目标数据
	void setTargetData(const TargetData &data);
	//设置关卡ID
	void setlevelNumber(int number);
	//设置剩余步数
	void setSteps(int steps);
	//改变目标数字
	void changeTargetNumber(ParentType type, int index, int number);

private:
	int _levelNumber;
	int _stepsNumber;

	std::vector<CTarget*> _targetList;
};

#endif
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
	//����Ŀ������
	void setTarget(const tagTargetData data);
	//��ʾ�Ժ�
	void showRight(bool flag = false);
	//�ı�Ŀ������
	void changeTargetNumber(int number);
	//��ȡĿ������
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
	//�������
	void clearData();
	//���ùؿ�Ŀ������
	void setTargetData(const TargetData &data);
	//���ùؿ�ID
	void setlevelNumber(int number);
	//����ʣ�ಽ��
	void setSteps(int steps);
	//�ı�Ŀ������
	void changeTargetNumber(ParentType type, int index, int number);

private:
	int _levelNumber;
	int _stepsNumber;

	std::vector<CTarget*> _targetList;
};

#endif
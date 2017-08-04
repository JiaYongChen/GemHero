#include "GameTargetNode.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ImageName.h"
#include "Resource.h"
#include <boost/lexical_cast.hpp>

using namespace std;
USING_NS_CC;

CTarget::CTarget(){

}

CTarget::~CTarget() {

}

bool CTarget::init() {
	if (!Node::init())
	{
		return false;
	}

	auto node = CSLoader::createNode("GameScene/UI/target.csb");
	this->addChild(node, 1, "target");

	return true;
}

void CTarget::setTarget(const tagTargetData data) {

	_targetData = data;

	//获取目标节点
	auto node = this->getChildByName("target");
	//获取目标图片节点
	Sprite* child = dynamic_cast<Sprite*>(node->getChildByName("targetImage"));
	string name = GEM_NAME[data.parentType][data.type];
	name[0] -= 32;
	char path[1024] = {0};
	snprintf(path, sizeof(path), "res/gameScene/small/small%s.png", name.c_str());
	child->initWithSpriteFrameName(path);

	//获取目标数量图片；并设置数据
	ui::TextBMFont* targetNum = dynamic_cast<ui::TextBMFont*>(node->getChildByName("targetNumber"));
	char number[32];
	snprintf(number, sizeof(number), "0/%d",_targetData.number);
	targetNum->setString(number);

	//隐藏对号
	showRight(false);
}

void CTarget::showRight(bool flag /* = false */) {
	auto node = this->getChildByName("target");
	auto child = node->getChildByName("rightImage");
	child->setVisible(flag);
}

void CTarget::changeTargetNumber(int number) {
	auto node = this->getChildByName("target");
	ui::TextBMFont* child = dynamic_cast<ui::TextBMFont*>(node->getChildByName("targetNumber"));
	char numberText[32];
	snprintf(numberText, sizeof(numberText), "%d/%d", number, _targetData.number);
	child->setString(numberText);

	if (number == _targetData.number)
	{
		showRight(true);
	}
}

//////////////////////////////////////////////////////////////////////////
CGameTargetNode::CGameTargetNode()
{
	clearData();
}

CGameTargetNode::~CGameTargetNode()
 {
 }

bool CGameTargetNode::init() {

	if (!Node::init())
	{
		return false;
	}

	auto node = CSLoader::createNode("GameScene/UI/targetNode.csb");
	this->addChild(node, 1, "targetNode");

	return true;
}

void CGameTargetNode::clearData() {
	for  (size_t i = 0; i < _targetList.size(); ++i)
	{
		Node* data = _targetList[i];
		this->removeChild(data);
	}
	_targetList.erase(_targetList.begin(), _targetList.end());

	_levelNumber = 0;
	_stepsNumber = 0;
}

void CGameTargetNode::setTargetData(const TargetData &data) {

	this->clearData();
	int num = 0;
	for (size_t i = 0; i < data.size(); ++i)
	{
		tagTargetData info = data[i];
		CTarget* node = CTarget::create();
		node->setTarget(info);
		node->setPosition(2, 85 - num * 36);
		this->addChild(node, 1);

		num++;
		_targetList.push_back(node);
	}
}

void CGameTargetNode::setlevelNumber(int number) {
	_levelNumber = number;
	auto node = this->getChildByName("targetNode");
	ui::TextBMFont* child = static_cast<ui::TextBMFont*>(node->getChildByName("levelNumber"));
	std::string numText = boost::lexical_cast<std::string>(number);
	child->setString(numText);
}

void CGameTargetNode::setSteps(int steps) {
	_stepsNumber = steps;
	auto node = this->getChildByName("targetNode");
	ui::TextBMFont* child = dynamic_cast<ui::TextBMFont*>(node->getChildByName("stepNumber"));
	child->setVisible(true);
	std::string numText = boost::lexical_cast<std::string>(steps);
	child->setString(numText);
}

void CGameTargetNode::changeTargetNumber(ParentType type, int index, int number) {
	for (size_t i = 0; i < _targetList.size(); ++i)
	{
		CTarget* node = _targetList[i];
		const tagTargetData data = node->getTargetData();
		if (data.parentType == type && data.type == index)
		{
			node->changeTargetNumber(number);
		}
	}
}
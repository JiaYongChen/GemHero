#include "GameEnd.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameMainScene.h"
#include "GameLogic.h"
#include "MainLevelLayer.h"
#include <boost\lexical_cast.hpp>

USING_NS_CC;

CGameEndNode::CGameEndNode() {

}

CGameEndNode::~CGameEndNode() {

}

bool CGameEndNode::init() {

	if (!Node::init())
	{
		return false;
	}

	auto node = CSLoader::createNode("GameScene/gameEnd/gameEnd.csb");
	this->addChild(node, 1, "gameEnd");
	this->setVisible(false);

	auto button = static_cast<ui::Button*>(node->getChildByName("successedNode")->getChildByName("sureBtn"));
	button->addClickEventListener(CC_CALLBACK_1(CGameEndNode::goBackToMainScene, this));

	button = static_cast<ui::Button*>(node->getChildByName("failedNode")->getChildByName("continueBtn"));
	button->addClickEventListener(CC_CALLBACK_1(CGameEndNode::goBackToMainScene, this));

	return true;
}

void CGameEndNode::showGame(bool success, int score, int starNumber) {
	this->setVisible(true);
	if (success)
	{
		auto node = this->getChildByName("gameEnd");
		node->getChildByName("failedNode")->setVisible(false);
		auto success = node->getChildByName("successedNode");
		success->setVisible(true);

		std::string scoreText;
		scoreText = boost::lexical_cast<std::string>(score);
		static_cast<ui::TextBMFont*>(success->getChildByName("scoreText"))->setString(scoreText);

		if (starNumber == 3)
		{
			success->getChildByName("star1")->setVisible(true);
			success->getChildByName("star2")->setVisible(true);
			success->getChildByName("star3")->setVisible(true);
		}
		else if (starNumber == 2)
		{
			success->getChildByName("star1")->setVisible(true);
			success->getChildByName("star2")->setVisible(true);
			success->getChildByName("star3")->setVisible(false);
		}
		else if (starNumber == 1) 
		{
			success->getChildByName("star1")->setVisible(true);
			success->getChildByName("star2")->setVisible(false);
			success->getChildByName("star3")->setVisible(false);
		}
	}
	else
	{
		auto node = this->getChildByName("gameEnd");
		node->getChildByName("failedNode")->setVisible(true);
		node->getChildByName("successedNode")->setVisible(false);
	}
}

void CGameEndNode::goBackToMainScene(Ref* pSender) {
	auto scene = CGameMainScene::createScene();
	scene->setName("gameMainScene");
	auto mainScene = scene->getChildByName("MainSecenLayer");
	auto levelayer = dynamic_cast<CGameMainScene*>(mainScene)->getChildByName("levelLayer");
	dynamic_cast<CMainLevelLayer*>(levelayer)->setCurrentLevelNumber(CGameLogic::getInstance().getCurrentGateID());

	auto transition = CCTransitionCrossFade::create(0.2f, scene);
	Director::getInstance()->replaceScene(transition);
}
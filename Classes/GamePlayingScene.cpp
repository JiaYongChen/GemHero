#include "GamePlayingScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GamePlayingSceneData.h"
#include "GameMainScene.h"
#include "GameTargetNode.h"
#include "ChessboardLayer.h"
#include "GameLogic.h"
#include "MainLevelLayer.h"
#include "GameEnd.h"

using namespace std;
USING_NS_CC;

Scene* CGamePlayingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CGamePlayingScene::create();
	scene->addChild(layer,0, "gameLayer");

	return scene;
}

CGamePlayingScene::CGamePlayingScene()
{
}

 CGamePlayingScene::~CGamePlayingScene()
{
}

 bool CGamePlayingScene::init()
 {
	 if (!Layer::init())
	 {
		 return false;
	 }
	 auto visableSize = Director::getInstance()->getVisibleSize();

	 auto rootNode = CSLoader::createNode("res/GameScene/GameScene.csb");
	 this->addChild(rootNode, 0, "rootNode");

	 auto button = static_cast<ui::Button*>(rootNode->getChildByName("goBack"));
	 button->addClickEventListener(CC_CALLBACK_1(CGamePlayingScene::goBackToMainScene,this));

	 auto target = CGameTargetNode::create();
	 target->setPosition(86, 545);
	 this->addChild(target, 1, "target");

	 auto chessboard = ChessboardLayer::create();
	 this->addChild(chessboard, 2, "chessboard");

	 auto gameEnd = CGameEndNode::create();
	 gameEnd->setPosition(visableSize.width / 2, visableSize.height / 2);
	 this->addChild(gameEnd, 3, "gameEnd");

	 return true;
 }

 void CGamePlayingScene::clear()
 {

 }

 void CGamePlayingScene::setGamePlayingData()
 {
	 auto targetNode = this->getChildByName("target");
	 dynamic_cast<CGameTargetNode*>(targetNode)->setlevelNumber(CGameLogic::getInstance().getCurrentGateID());
	 dynamic_cast<CGameTargetNode*>(targetNode)->setSteps(CGameLogic::getInstance().getCurrentGateStep());
	 const TargetData &data = CGameLogic::getInstance().getSuccessCondition();
	 dynamic_cast<CGameTargetNode*>(targetNode)->setTargetData(data);

	 auto chessboard = this->getChildByName("chessboard");
	 dynamic_cast<ChessboardLayer*>(chessboard)->setMapData(CGameLogic::getInstance().getCurrentMapData());
	 const PBaseData gemData = CGameLogic::getInstance().getCurrentGemData();
	 dynamic_cast<ChessboardLayer*>(chessboard)->setRoleData(CGameLogic::getInstance().getCurrentGemData(),CGameLogic::getInstance().getCurrentSpecialBrickData());
	 CGameLogic::getInstance().setGameStatus(GAME_STATUS_PLAYING);
	 CGameLogic::getInstance().setGamePlayingStatus(GAME_PLAYING_STATUS_NORMAL);
 }

 void CGamePlayingScene::goBackToMainScene(Ref* pSender)
 {
	 this->clear();
 	 auto scene = CGameMainScene::createScene();
	 scene->setName("gameMainScene");
 	 auto mainScene = scene->getChildByName("MainSecenLayer");
 	 auto levelayer = dynamic_cast<CGameMainScene*>(mainScene)->getChildByName("levelLayer");
 	 dynamic_cast<CMainLevelLayer*>(levelayer)->setCurrentLevelNumber(CGameLogic::getInstance().getCurrentGateID());
 
 	 auto transition = CCTransitionCrossFade::create(0.2f, scene);
 	 Director::getInstance()->replaceScene(transition);
 }
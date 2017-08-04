#include "GameMainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainLevelLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

void onClick(Ref*)
{
	CCLOG("CLICK");
}

Scene* CGameMainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CGameMainScene::create();

    // add layer as a child to scene
    scene->addChild(layer,0,"MainSecenLayer");

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CGameMainScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto visibleSize = Director::getInstance()->getVisibleSize();

    auto rootNode = CSLoader::createNode("MainScene/MainScene.csb");
   addChild(rootNode,0,"rootNode");

	auto levelLayer = CMainLevelLayer::create();
	addChild(levelLayer,1,"levelLayer");

	levelLayer->initLevel(100);
	levelLayer->setUnlockLevel(100);

	_chooseLevel = CChooseLevel::create();
	_chooseLevel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_chooseLevel->setVisible(false);
	addChild(_chooseLevel, 2, "chooseLevel");

	scheduleUpdate();
    return true;
}

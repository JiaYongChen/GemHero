#include "ChooseLevel.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GamePlayingScene.h"
#include <boost/lexical_cast.hpp>

USING_NS_CC;

CChooseLevel::CChooseLevel()
{

}

CChooseLevel::~CChooseLevel()
{

}

bool CChooseLevel::init()
{
	if (!Node::init())
	{
		return false;
	}

	_chooseLevel = CSLoader::createNode("MainScene/chessGate/chooseLevel.csb");
	addChild(_chooseLevel, 0, "chooseLevel");

	auto button = static_cast<ui::Button*>(_chooseLevel->getChildByName("closeButton"));
	button->addClickEventListener(CC_CALLBACK_1(CChooseLevel::onClickClose, this));

	button = static_cast<ui::Button*>(_chooseLevel->getChildByName("startGameButton"));
	button->addClickEventListener(CC_CALLBACK_1(CChooseLevel::onClickStartGame, this));

	return true;
}

void CChooseLevel::showChooseLevel(int levelNumber, LevelGameType type)
{
	auto levelNumerText = static_cast<ui::TextBMFont*>(_chooseLevel->getChildByName("levelNumber"));
	std::string cText = boost::lexical_cast<std::string>(levelNumber);
	levelNumerText->setString(cText);
	_levelNumber = levelNumber;

	auto type1 = _chooseLevel->getChildByName("levelType1");
	auto type2 = _chooseLevel->getChildByName("levelType2");
	auto type3 = _chooseLevel->getChildByName("levelType3");
	switch (type)
	{
	case LEVELGAME_NULL:
		break;
	case LEVELGAME_SLYDRIS:
		type1->setVisible(true);
		type2->setVisible(false);
		type3->setVisible(false);
		break;
	case LEVELGAME_COLLECT:
		type1->setVisible(false);
		type2->setVisible(true);
		type3->setVisible(false);
		break;
	case LEVELGAME_TIME:
		type1->setVisible(false);
		type2->setVisible(false);
		type3->setVisible(true);
		break;
	case COUNT_LEVELGAME_TYPE:
		break;
	default:
		break;
	}
}

void CChooseLevel::setStartNumber(int number)
{
	auto star1 = static_cast<Sprite*>(_chooseLevel->getChildByName("star1"));
	auto star2 = static_cast<Sprite*>(_chooseLevel->getChildByName("star2"));
	auto star3 = static_cast<Sprite*>(_chooseLevel->getChildByName("star3"));
	switch (number)
	{
	case 0:
		star1->setVisible(false);
		star2->setVisible(false);
		star3->setVisible(false);
		break;
	case 1:
		star1->setVisible(true);
		star2->setVisible(false);
		star3->setVisible(false);
		break;
	case 2:
		star1->setVisible(true);
		star2->setVisible(true);
		star3->setVisible(false);
		break;
	case 3:
		star1->setVisible(true);
		star2->setVisible(true);
		star3->setVisible(true);
		break;
	default:
		star1->setVisible(false);
		star2->setVisible(false);
		star3->setVisible(false);
		break;
	}
}

void CChooseLevel::onClickClose(cocos2d::Ref* sender)
{
	CCLOG("onClickClose");

	Scene* currentScene = Director::getInstance()->getRunningScene();
	if (strcmp(currentScene->getName().c_str(), "gameMainScene") == 0)
	{
		auto layer = currentScene->getChildByName("MainSecenLayer");
		if (layer != NULL)
		{
			auto node = static_cast<CChooseLevel*>(layer->getChildByName("chooseLevel"));
			if (node != NULL)
			{
				node->setVisible(false);
			}
		}
	}
}

void CChooseLevel::onClickStartGame(cocos2d::Ref* sender)
{
	CCLOG("onClickStartGame");
	auto gameScene = CGamePlayingScene::createScene();
	gameScene->setName("gamePlayingScene");
	dynamic_cast<CGamePlayingScene*>(gameScene->getChildByName("gameLayer"))->setGamePlayingData();

	auto transition = CCTransitionCrossFade::create(0.2f, gameScene);
	Director::getInstance()->replaceScene(transition);
}
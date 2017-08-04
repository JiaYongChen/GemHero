#include "MainLevel.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChooseLevel.h"
#include "GamePlayingSceneData.h"
#include "GameLogic.h"
#include <boost\lexical_cast.hpp>

USING_NS_CC;

CMainLevel::CMainLevel()
{
	_IsUnlock = false;
	_levelNumber = 0;
	_starNumber = 0;
}


CMainLevel::~CMainLevel()
{
}

bool CMainLevel::init()
{
	if (!Node::init())
	{
		return false;
	}

	_levelNode = CSLoader::createNode("MainScene/GateListLayer/Gate.csb");
	addChild(_levelNode,1,"levelNode");

	auto button = static_cast<ui::Button*>(_levelNode->getChildByName("levelButton"));
	button->addClickEventListener(CC_CALLBACK_1(CMainLevel::onClick,this));

	this->scheduleUpdate();
	return true;
}

void CMainLevel::onClick(Ref* sender)
{
	Scene* currentScene = Director::getInstance()->getRunningScene();
	if (strcmp(currentScene->getName().c_str(),"gameMainScene") == 0)
	{
		auto layer = currentScene->getChildByName("MainSecenLayer");
		if (layer != NULL)
		{
			LevelGameType type = LEVELGAME_SLYDRIS;
			auto node = static_cast<CChooseLevel*>(layer->getChildByName("chooseLevel"));
			if (node != NULL)
			{
				node->setVisible(true);

				CGamePlayingSceneData::getInstance().clear();
				CGamePlayingSceneData::getInstance().initialization(_levelNumber);

				type = CGamePlayingSceneData::getInstance().getRulesData().gameType;

				CGameLogic::getInstance().loadLevelDataFromeData(CGamePlayingSceneData::getInstance());
				CGameLogic::getInstance().restartGame();
				CGamePlayingSceneData::getInstance().clear();
			}
			else
			{
				node = CChooseLevel::create();
				node->setVisible(true);
				layer->addChild(node, 2, "chooseLevel");
			}

			node->showChooseLevel(_levelNumber, type);
			node->setStartNumber(0);
		}
	}
}

void CMainLevel::setLevelNumber(int levelNumber)
{
	auto levelText = static_cast<ui::TextBMFont*>(_levelNode->getChildByName("levelNumberLabel"));
	std::string text = boost::lexical_cast<std::string > (levelNumber);
	_levelNumber = levelNumber;
	levelText->setString(text);
}

void CMainLevel::setStarNumber(unsigned char starNumber)
{
	_starNumber = starNumber;
	changeStar();
}

void CMainLevel::setIsUnlock(bool isUnlock)
{
	_IsUnlock = isUnlock;
	changeLock();
}

void CMainLevel::changeLock()
{
	auto button = static_cast<ui::Button*>(_levelNode->getChildByName("levelButton"));
	auto levelText = static_cast<ui::TextBMFont*>(_levelNode->getChildByName("levelNumberLabel"));
	if (_IsUnlock)
	{
		button->setEnabled(true);
		levelText->setVisible(true);
	}
	else
	{
		button->setEnabled(false);
		levelText->setVisible(false);
	}
}

void CMainLevel::changeStar()
{
	auto star1 = static_cast<Sprite*>(_levelNode->getChildByName("star1"));
	auto star2 = static_cast<Sprite*>(_levelNode->getChildByName("star2"));
	auto star3 = static_cast<Sprite*>(_levelNode->getChildByName("star3"));
	switch (_starNumber)
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
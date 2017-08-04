#include "MainlevelInfo.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

const Vec2 CMainlevelInfo::LEVELPOSITION[LEVELNUMBER] = { Vec2(-217, -177),Vec2(-88, -75),Vec2(73, -18),Vec2(205, 67),Vec2(115, 200),Vec2(-20, 114),Vec2(-169, 46) };

CMainlevelInfo::CMainlevelInfo()
{

}


CMainlevelInfo::~CMainlevelInfo()
{
}

bool CMainlevelInfo::init()
{
	if (!Node::init())
	{
		return false;
	}

	_levelInfo = CSLoader::createNode("MainScene/GateListLayer/GateList.csb");
	addChild(_levelInfo, 0, "levelInfo");

	for (int i = 0 ; i < LEVELNUMBER; ++i)
	{
		char cText[256];
		snprintf(cText, sizeof(cText), "road%d", (i + 1));
		std::string str = cText;
		auto node = static_cast<Sprite*>(_levelInfo->getChildByName(str));
		node->setLocalZOrder(1);
	}

	for (int i = 0; i < 3; ++i)
	{
		char cText[256];
		snprintf(cText, sizeof(cText), "cloud%d", (i + 1));
		std::string str = cText;
		auto node = static_cast<Sprite*>(_levelInfo->getChildByName(str));
		node->setLocalZOrder(3);
	}

	_levelList.init();

	this->scheduleUpdate();
	return true;
}

void CMainlevelInfo::initLevelInfo(int index, int number /* = LEVELNUMBER */)
{
	_Index = index;
	for (int i = 0; i < number; ++i)
	{
		auto level = CMainLevel::create();
		level->setPosition(LEVELPOSITION[i]);
		level->setLevelNumber((index-1) *LEVELNUMBER + (i + 1));
		level->setStarNumber(0);
		level->setIsUnlock(false);
		_levelInfo->addChild(level, 2, "level");

		_levelList.addObject(level);
	}

	if (number < 7)
	{
		for (int i = number; i <= LEVELNUMBER; ++i)
		{
			char cText[256];
			snprintf(cText, sizeof(cText), "road%d", i);
			std::string str = cText;
			auto node = static_cast<Sprite*>(_levelInfo->getChildByName(str));
			node->setVisible(false);
		}
	}

	auto cloud1 = static_cast<Sprite*>(_levelInfo->getChildByName("cloud1"));
	auto cloud2 = static_cast<Sprite*>(_levelInfo->getChildByName("cloud2"));
	auto cloud3 = static_cast<Sprite*>(_levelInfo->getChildByName("cloud3"));
	switch (number)
	{
	case 1:
	case 2:
		cloud1->setVisible(true);
		cloud2->setVisible(false);
		cloud3->setVisible(false);
		break;
	case 3:
	case 4:
	case 5:
		cloud1->setVisible(true);
		cloud2->setVisible(true);
		cloud3->setVisible(false);
		break;
	case 6:
	case 7:
		cloud1->setVisible(true);
		cloud2->setVisible(true);
		cloud3->setVisible(true);
		break;
	default:
		cloud1->setVisible(true);
		cloud2->setVisible(true);
		cloud3->setVisible(true);
		break;
	}
}
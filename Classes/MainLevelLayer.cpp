#include "MainLevelLayer.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

CMainLevelLayer::CMainLevelLayer()
{
	_currentLevelNumber = 1;
}

CMainLevelLayer::~CMainLevelLayer()
{

}

bool CMainLevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto layer = CSLoader::createNode("MainScene/GateListLayer/GateListLayer.csb");
	addChild(layer, 0, "scoreViewLayer");

	_scoreView = static_cast<ui::ScrollView*>(layer->getChildByName("ScrollView"));

	_levelInfoList.init();

	return true;
}

void CMainLevelLayer::onEnter()
{
	Layer::onEnter();
	_scoreView->setScrollBarEnabled(false);
	_scoreView->setInertiaScrollEnabled(true);
	_scoreView->setDirection(ui::ScrollView::Direction::VERTICAL);

	int indexNumber = (_maxNumber - 1) / CMainlevelInfo::LEVELNUMBER + 1;
	int currentIndex = (_currentLevelNumber - 1) / CMainlevelInfo::LEVELNUMBER;
	_scoreView->jumpToPercentVertical(100 - ((float)currentIndex / (float)indexNumber) * 100);
}

void CMainLevelLayer::initLevel(int number)
{
	_maxNumber = number;

	int indexNumber = (_maxNumber - 1) / CMainlevelInfo::LEVELNUMBER + 1;
	int height = 0;

	for (int i = indexNumber; i > 0; --i)
	{
		auto levelInfo = CMainlevelInfo::create();
		int infoNumber = number % CMainlevelInfo::LEVELNUMBER;
		infoNumber = infoNumber == 0 ? CMainlevelInfo::LEVELNUMBER : infoNumber;
		levelInfo->initLevelInfo(i, infoNumber);
		levelInfo->setPosition(Vec2(_scoreView->getContentSize().width / 2,(i-1)*CMainlevelInfo::LEVELHEIGHT + 230));
		height = height + CMainlevelInfo::LEVELHEIGHT;

		number = number - infoNumber;
		char cText[256];
		snprintf(cText, sizeof(cText), "levelInfo_%d", i);
		std::string str = cText;
		_scoreView->addChild(levelInfo,i,str);

		_levelInfoList.addObject(levelInfo);
	}
	_scoreView->setInnerContainerSize(Size(_scoreView->getContentSize().width, height));

	if (_maxNumber % CMainlevelInfo::LEVELNUMBER == 0)
	{
		CMainlevelInfo *levelInfo = static_cast<CMainlevelInfo*>(_levelInfoList.getObjectAtIndex(0));
		auto node = static_cast<Sprite*>(levelInfo->getChildByName("levelInfo")->getChildByName("road7"));
		node->setVisible(false);
	}
}

void CMainLevelLayer::setUnlockLevel(int unlockNumber)
{
	_currentUnlockNumber = unlockNumber;
	_currentUnlockNumber = _currentUnlockNumber > _maxNumber ? _maxNumber : _currentUnlockNumber;
	int indexNumber = (_currentUnlockNumber - 1) / CMainlevelInfo::LEVELNUMBER + 1;

	for (int i = indexNumber; i > 0; --i)
	{
		CMainlevelInfo *levelInfo =static_cast<CMainlevelInfo*>(_levelInfoList.getObjectAtIndex(_levelInfoList.count()-i));
		int infoNumber = unlockNumber % CMainlevelInfo::LEVELNUMBER;
		infoNumber = infoNumber == 0 ? CMainlevelInfo::LEVELNUMBER : infoNumber;
		for (int i = 0 ; i < infoNumber; ++i)
		{
			CMainLevel *level = static_cast<CMainLevel*>(levelInfo->getLevelList().getObjectAtIndex(i));
			level->setIsUnlock(true);
		}

		auto cloud1 = static_cast<Sprite*>(levelInfo->getChildByName("levelInfo")->getChildByName("cloud1"));
		auto cloud2 = static_cast<Sprite*>(levelInfo->getChildByName("levelInfo")->getChildByName("cloud2"));
		auto cloud3 = static_cast<Sprite*>(levelInfo->getChildByName("levelInfo")->getChildByName("cloud3"));
		switch (infoNumber)
		{
		case 1:
		case 2:
			cloud1->setVisible(false);
			cloud2->setVisible(true);
			cloud3->setVisible(true);
			break;
		case 3:
		case 4:
		case 5:
			cloud1->setVisible(false);
			cloud2->setVisible(false);
			cloud3->setVisible(true);
			break;
		case 6:
		case 7:
			cloud1->setVisible(false);
			cloud2->setVisible(false);
			cloud3->setVisible(false);
			break;
		default:
			cloud1->setVisible(false);
			cloud2->setVisible(false);
			cloud3->setVisible(false);
			break;
		}

		unlockNumber = unlockNumber - infoNumber;
	}
}
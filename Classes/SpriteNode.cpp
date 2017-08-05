#include "SpriteNode.h"
#include "Resource.h"
#include <boost/lexical_cast.hpp>

USING_NS_CC;

SpriteNode::SpriteNode() {
	_spriteNode = nullptr;
	_runAction = false;
	_direction = CENT_DIRECTION;
	_data = nullptr;
}

SpriteNode::~SpriteNode() {
}

bool SpriteNode::init() {
	if (!Node::init())
	{
		return false;
	}

	_spriteNode = Sprite::create();
	this->addChild(_spriteNode);
	this->setContentSize(_spriteNode->getBoundingBox().size);
	return true;
}

bool SpriteNode::initWithData(const std::string& name, tagBaseData* data) {
	_data = data;

	_spriteNode = Sprite::createWithSpriteFrameName(name);
	if (_spriteNode == NULL)
	{
		return false;
	}

	this->addChild(_spriteNode);

// 	_indexText = Label::createWithSystemFont("", "", 12);
// 	_indexText->setPosition(Vec2(0, 21));
// 	_indexText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
// 	this->addChild(_indexText, 1);
// 
// 	setIndexText();
	return true;
}

void SpriteNode::setData(tagBaseData* data) {
	_data = data;

	//setIndexText();
}

void SpriteNode::setIndexText() {
	char text[32] = { 0 };
	snprintf(text, sizeof(text), "x:%d y:%d", _data->indexX, _data->indexY);
	_indexText->setString(text);
}

float SpriteNode::getSpriteNodeWidth() {
	if (_spriteNode != nullptr)
	{
		return _spriteNode->getBoundingBox().size.width;
	}

	return 0.0f;
}

float SpriteNode::getSpriteNodeHeight() {
	if (_spriteNode != nullptr)
	{
		return _spriteNode->getBoundingBox().size.height;
	}

	return 0.0f;
}

tagBaseData* SpriteNode::getData() {
	return _data;
}

bool SpriteNode::containsPointTest(cocos2d::Vec2 point) {

	auto locationPoint = this->convertToNodeSpace(point);
	Rect rect = _spriteNode->getBoundingBox();
	if (rect.containsPoint(locationPoint))
	{
		return true;
	}

	return false;
}

Vec2 SpriteNode::getSpriteNodeIndex() {

	return Vec2(_data->indexX, _data->indexY);
}

///////////////////////////////////////////////////
MapSprite::MapSprite() {
	_collectSprite = nullptr;
}

MapSprite::~MapSprite() {

}

bool MapSprite::init() {
	if (!SpriteNode::init())
	{
		return false;
	}

	return true;
}

bool MapSprite::initWithData(const std::string& name, tagBaseData* data) {

	tagMapData *mapData = (tagMapData*)data;
	if (SpriteNode::initWithData(name, mapData) == false)
	{
		return false;
	}

	if (mapData->collect)
	{
		char pname[1024] = { 0 };
		snprintf(pname, sizeof(pname), "res/gameScene/%s.png", GEM_NAME[data->parentType][1].c_str());
		_collectSprite = Sprite::createWithSpriteFrameName(pname);
		if (_collectSprite == NULL)
		{
			return false;
		}
		_collectSprite->setPosition(0, -(_spriteNode->getBoundingBox().size.height + _collectSprite->getBoundingBox().size.height)/2);
		this->addChild(_collectSprite);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
GemSprite::GemSprite() {
	_gemStatus = GEM_STATUS_NULL;
	_gemStatusLable = nullptr;
	_indexText = nullptr;
}

GemSprite::~GemSprite() {

}

bool GemSprite::init() {
	if (!SpriteNode::init())
	{
		return false;
	}

	return true;
}

bool GemSprite::initWithData(const std::string& name, tagBaseData* data) {

	tagGemData *mapData = (tagGemData*)data;
	if (SpriteNode::initWithData(name,(mapData)) == false)
	{
		return false;
	}
	_gemStatus = mapData->gemStatus;
	_gemStatusLable = Label::createWithSystemFont("", "", 15);
	_gemStatusLable->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	this->addChild(_gemStatusLable,1);

	if (_gemStatus != GEM_STATUS_NORMAL)
	{
		std::string text = getGemStatusString(_gemStatus);
		_gemStatusLable->setString(text);
	}

	return true;
}

std::string GemSprite::getGemStatusString(GemStatus status) {
	switch (status)
	{
	case GEM_STATUS_NULL:
		return "";
		break;
	case GEM_STATUS_NORMAL:
		return "NORMAL";
		break;
	case GEM_STATUS_VERTICAL:
		return "VERTICAL";
		break;
	case GEM_STATUS_HORIZONTAL:
		return "HORIZONTAL";
		break;
	case GEM_STATUS_HV:
		return "HV";
		break;
	case GEM_STATUS_BOM:
		return "BOM";
		break;
	case GEM_STATUS_COLORFUL:
		return "COLORFUL";
		break;
	case GEM_STATUS_CHANGE:
		return "CHANGE";
		break;
	case GEM_STATUS_TIME_BOM:
		return "TIME_BOM";
		break;
	case GEM_STATUS_ADD_STEP:
		return "ADD_STEP";
		break;
	case GEM_STATUS_TREASURE_CHEST:
		return "TREASURE_CHEST";
		break;
	case GEM_STATUS_COUNT:
		return "";
		break;
	default:
		return "";
		break;
	}

	return "";
}

void GemSprite::setGemStatus(GemStatus status) {
	_gemStatus = status;
	std::string text = getGemStatusString(_gemStatus);
	_gemStatusLable->setString(text);
}

//////////////////////////////////////////////////////////////////////////
StumblingSprite::StumblingSprite() {
	_stumblingAttribute = NULL;
}

StumblingSprite::~StumblingSprite() {

}

bool StumblingSprite::init() {
	if (!SpriteNode::init())
	{
		return false;
	}

	_stumblingAttribute = Label::createWithSystemFont("", "", 15);
	_stumblingAttribute->setWidth(61);
	_stumblingAttribute->setHeight(25);
	_stumblingAttribute->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	_stumblingAttribute->setVisible(false);
	this->addChild(_stumblingAttribute, 1);

	return true;
}

bool StumblingSprite::initWithData(const std::string& name, tagBaseData* data) {
	
	tagSpecialBrickData *mapData = (tagSpecialBrickData*)data;

	if (mapData->childType == STUMBLING_BLOCK_PORTAL)
	{
		std::string attributeText;
		attributeText = boost::lexical_cast<std::string>(unsigned char(mapData->attribute+64));
		_stumblingAttribute->setString(attributeText);
		_stumblingAttribute->setVisible(true);
		if (mapData->direction == UP_DIRECTION)
		{
			_stumblingAttribute->setPosition(Vec2(0, 21));
		}
		else {
			_stumblingAttribute->setPosition(Vec2(0, -21));
		}
		_data = data;
		return true;
	}

	if (SpriteNode::initWithData(name, (mapData)) == false)
	{
		return false;
	}

	return true;
}
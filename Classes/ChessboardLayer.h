#pragma once
#ifndef _CHESSBOARD_LAYER_
#define _CHESSBOARD_LAYER_

#include "cocos2d.h"
#include "GameMapLayer.h"
#include "GameRoleLayer.h"

class ChessboardLayer : public cocos2d::Layer, public GamingDelegator
{
public:
	ChessboardLayer();
	~ChessboardLayer();
	CREATE_FUNC(ChessboardLayer);

	virtual bool init();

public:
	void setMapData(const PBaseData& data);
	void setRoleData(const PBaseData& gemdata, const PBaseData& stumblingdata);

	virtual void callBack(int type1, int type2, int type3);

private:
	void onPlayingGame(GamePlayingStatus playingStatus, int type);
	void onUseProp(enProp propType);
	void onBonusTime();

private:
	void swapGem(swapGemStatus status);
	void connectGem();
	void showGameEnd(gameEndStatus success);

private:
	void sendRemoveData(std::vector<cocos2d::Vec2> &gem, SpecialBrickData &stumbling, std::vector<ObtainSpecialGemInfo> &info);
	void touchSpecialGems();
	void changeTargetNumber(const TargetData &data);

private:
	GameMapLayer *_mapLayer;
	CGameRoleLayer *_roleLayer;
};

#endif
#pragma once
#ifndef _GAME_MAP_LAYER_
#define _GAME_MAP_LAYER_

#include "GamingLayer.h"

class GameMapLayer : public CGamingLayer
{
public:
	GameMapLayer();
	~GameMapLayer();
	CREATE_FUNC(GameMapLayer);

protected:
	virtual bool init();

public:
	virtual void setLayerData(const PBaseData &data);
	virtual void removeLayerData(const PBaseData &data);

};

#endif
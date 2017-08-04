#pragma once
#include "GamingLayer.h"

class CGameStumblingLayer : public CGamingLayer
{
public:
	CGameStumblingLayer();
	virtual ~CGameStumblingLayer();
	CREATE_FUNC(CGameStumblingLayer);

protected:
	virtual bool init();

public:
	virtual void setLayerData(const PBaseData &data);
	virtual void removeLayerData(const PBaseData &data);

};

class CGameDownStumblingLayer : public CGameStumblingLayer
{
public:
	CGameDownStumblingLayer();
	virtual ~CGameDownStumblingLayer();
	CREATE_FUNC(CGameDownStumblingLayer);

protected:
	virtual bool init();

public:
	virtual void removeLayerData(const PBaseData &data);
};

class CGameUpStumblingLayer : public CGameStumblingLayer
{
public:
	CGameUpStumblingLayer();
	virtual ~CGameUpStumblingLayer();
	CREATE_FUNC(CGameUpStumblingLayer);

protected:
	virtual bool init();

public:
	virtual void removeLayerData(const PBaseData &data);
};
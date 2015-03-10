#ifndef __FranticAlien__Planet_H__
#define __FranticAlien__Planet_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "ParallaxForeground.h"
#include "ParallaxBackground.h"
#include "ParallaxTileMap.h"

using namespace cocos2d;

// A planet consists of a single planet surface and several caves that are of the same type
// the planet types are Grass, Snow, Rock, Sand, hence there will be caves of type
// GRASS, SNOW, ROCK and SAND

// Interface
class IPlanet : public Node
{
public:
	IPlanet(){};
	virtual ~IPlanet(){};

	virtual ValueMap getLandingSite() = 0;
	virtual void update(float& delta) = 0;
	virtual void addPlayer(IGameObject* gameObject) = 0;
	virtual PlanetSurface& getPlanetSurface() = 0;
	virtual Cave& getPlanetCave(int index) = 0;
	virtual bool ToCave(IGameObject& gameObject) = 0;
	virtual bool ToSurface(IGameObject& gameObject) = 0;
};

class Planet : public IPlanet
{
	typedef IPlanet super;
	typedef Planet self;

public:		
    Planet(){};
	virtual ~Planet(){};

	/** **/
    virtual ValueMap getLandingSite() override;
    /** **/
	virtual void update(float& delta) override;

	virtual void addPlayer(IGameObject* gameObject) override;

	virtual PlanetSurface& getPlanetSurface() override;
	virtual Cave& getPlanetCave(int index) override;
    
	virtual bool ToCave(IGameObject& gameObject) override;
	virtual bool ToSurface(IGameObject& gameObject) override;

protected:
	PlanetSurface* _planetSurface;
	Cave* _planetCave;

	std::stack<IParallaxTileMap*> planetMaps;
};


class GrassPlanet : public Planet
{
	typedef Planet super;
	typedef GrassPlanet self;
	
public:
    static self* create();
    
	GrassPlanet();
	virtual ~GrassPlanet(){};
};

class SnowPlanet : public Planet
{
	typedef Planet super;
	typedef SnowPlanet self;

public:
    static self* create();
    
	SnowPlanet();
	virtual ~SnowPlanet(){};
};

class SandPlanet : public Planet
{
	typedef Planet super;
	typedef SandPlanet self;

public:
	static self* create();

	SandPlanet();
	virtual ~SandPlanet(){};
};

class DirtPlanet : public Planet
{
	typedef Planet super;
	typedef DirtPlanet self;

public:
	static self* create();

	DirtPlanet();
	virtual ~DirtPlanet(){};
};

#endif /* defined(__FranticAlien__Planet_H__) */

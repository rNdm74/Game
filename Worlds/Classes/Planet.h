#ifndef __FranticAlien__Planet_H__
#define __FranticAlien__Planet_H__

#include "cocos2d.h"
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
};

class Planet : public IPlanet
{
	typedef IPlanet super;
	typedef Planet self;

public:	
    static self* create(){};
	
    Planet(){};
	virtual ~Planet(){};

	/** **/
    virtual ValueMap getLandingSite() override;
    /** **/
	virtual void update(float& delta) override;
    
private:
    IParallaxTileMap* _tileMap;
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


#endif /* defined(__FranticAlien__Planet_H__) */

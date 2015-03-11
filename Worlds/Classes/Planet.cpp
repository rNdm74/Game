#include "Constants.h"
#include "AppGlobal.h"
#include "GameObject.h"
#include "Level.h"
#include "Planet.h"

ValueMap Planet::getLandingSite()
{
    //
    auto layer = _planetSurface->getChildByTag(kTagObjectLayer);
    //
    auto node = layer->getChildByName("LandingSite");
    //
    return static_cast<LandingSite*>(node)->getProperties();
};

void Planet::update(float& delta)
{	
	/** Updates scale creates zoom effect **/
	this->setScale(AppGlobal::getInstance()->getScale());
	/** Update the active map **/

	planetMaps.top()->update(delta);

	/** Keep active map position centered on player **/
	planetMaps.top()->setPositionOnPlayer();
}

void Planet::addPlayer(IGameObject* gameObject)
{
	_planetSurface->addPlayer(gameObject);
};

PlanetSurface& Planet::getPlanetSurface()
{
	return *_planetSurface;
};

Cave& Planet::getPlanetCave(int index)
{
	return *_planetCave;
};

bool Planet::ToCave(IGameObject& gameObject)
{
	_planetSurface->setVisible(false);
	_planetCave->setVisible(true);

	planetMaps.push(_planetCave);

	gameObject.retain();
	gameObject.removeFromParentAndCleanup(true);
	_planetCave->addPlayer(&gameObject);
	gameObject.release();

	ValueMap properties = _planetCave->getToSurfaceProperties();
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();

	gameObject.setPosition(Vec2(x, y));

	while (!AppGlobal::getInstance()->PlayerEvents.empty())
		    AppGlobal::getInstance()->PlayerEvents.pop();

	AppGlobal::getInstance()->PlayerEvents.push(EGameObjectEvent::Stop);

	return true;
};

bool Planet::ToSurface(IGameObject& gameObject)
{
	_planetSurface->setVisible(true);
	_planetCave->setVisible(false);
	planetMaps.pop();

	gameObject.retain();
	gameObject.removeFromParentAndCleanup(true);
	_planetCave->addPlayer(&gameObject);
	gameObject.release();

	ValueMap properties = _planetSurface->getToCaveProperties();
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();

	gameObject.setPosition(Vec2(x, y));

	return true;
};

GrassPlanet* GrassPlanet::create()
{
    // Create an instance of Level
    GrassPlanet* gameObject = new (std::nothrow) GrassPlanet();
    
    if (gameObject && gameObject->init())
    {
        gameObject->autorelease();
        return gameObject;
    }
    
    CC_SAFE_DELETE(gameObject);
    return nullptr;
};

GrassPlanet::GrassPlanet()
{
	/** Create the planets surface**/
	_planetSurface = PlanetSurface::create(GRASS_PNG);
	/** Add it to the maps stack **/
	planetMaps.push(_planetSurface);

	/** Create the planets caves **/ 
	_planetCave = Cave::create(GRASS_PNG);
	/** The caves are not visible yet **/
	_planetCave->setVisible(false);

	/** Add the planets surface and the planets cave to the planet **/
	this->addChild(_planetSurface);	
	this->addChild(_planetCave);

	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	int year = timeinfo->tm_year + 1900; 
	int month = timeinfo->tm_mon + 1;
	int day = timeinfo->tm_mday;

	log("%02d/%02d/%04d", day, month, year);

	int hour = timeinfo->tm_hour;
	int min = timeinfo->tm_min;
	int sec = timeinfo->tm_sec;

	log("%02d:%02d:%02d", hour, min, sec);
}


SnowPlanet* SnowPlanet::create()
{
	// Create an instance of Level
	SnowPlanet* gameObject = new (std::nothrow) SnowPlanet();

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

SnowPlanet::SnowPlanet()
{
	_planetSurface = PlanetSurface::create(SNOW_PNG);
	
	this->addChild(_planetSurface);
}


SandPlanet* SandPlanet::create()
{
	// Create an instance of Level
	SandPlanet* gameObject = new (std::nothrow) SandPlanet();

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

SandPlanet::SandPlanet()
{
	_planetSurface = PlanetSurface::create(SAND_PNG);
	
	this->addChild(_planetSurface);
}


DirtPlanet* DirtPlanet::create()
{
	// Create an instance of Level
	DirtPlanet* gameObject = new (std::nothrow) DirtPlanet();

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

DirtPlanet::DirtPlanet()
{
	_planetSurface = PlanetSurface::create(DIRT_PNG);
	
	this->addChild(_planetSurface);		
}
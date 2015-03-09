#include "Constants.h"
#include "AppGlobal.h"
#include "GameObject.h"
#include "Level.h"
#include "Planet.h"

ValueMap Planet::getLandingSite()
{
    //
    auto layer = _tileMap->getChildByTag(kTagObjectLayer);
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
	_tileMap->update(delta);
	/** Keep active map position centered on player **/
    _tileMap->setPositionOnPlayer();
}

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
	// Load Grass ParallaxBackground
	// Load Grass ParallaxTileMap
	// Load Grass ParallaxForGround
}

SnowPlanet::SnowPlanet()
{
	// Load Grass ParallaxBackground
	// Load Grass ParallaxTileMap
	// Load Grass ParallaxForGround
}
#include "Constants.h"
#include "AppGlobal.h"
#include "Level.h"
#include "Planet.h"

Planet* Planet::create()
{	
}

Planet::Planet()
{	
}

void Planet::update(float& delta)
{	
	/** Updates scale creates zoom effect **/
	this->setScale(AppGlobal::getInstance()->getScale());
	/** Update the active map **/
	activeMap.top()->update(delta);
	/** Keep active map position centered on player **/
	activeMap.top()->setPositionOnPlayer();
}

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
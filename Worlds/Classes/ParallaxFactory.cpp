#include "ParallaxFactory.h"
#include "ParallaxTileMap.h"

IParallaxTileMap* ParallaxFactory::create(std::string className)
{
	IParallaxTileMap* parallaxTileMap = nullptr;

	if (className == "Planet")
	{
		parallaxTileMap = Planet::create(kPlanetTMX);
		parallaxTileMap->setTag(kTagPlanet);
	}
	else if (className == "Cave")
	{
		parallaxTileMap = Cave::create(kCaveTMX);
		parallaxTileMap->setTag(kTagCave);
	}
	
	return parallaxTileMap;
}
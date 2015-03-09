#include "ParallaxFactory.h"
#include "ParallaxTileMap.h"
#include "AppResources.h"

IParallaxTileMap* ParallaxFactory::create(std::string className)
{
	IParallaxTileMap* parallaxTileMap = nullptr;

	if (className == "PlanetSurface")
	{
		parallaxTileMap = PlanetSurface::create(kPlanetSurfaceTmx);
		parallaxTileMap->setName(kPlanetSurfaceTmx);
		parallaxTileMap->setTag(kTagPlanetSurface);
	}
	else if (className == "Cave")
	{
		parallaxTileMap = Cave::create(kCaveTMX);
		parallaxTileMap->setName(kCaveTMX);
		parallaxTileMap->setTag(kTagCave);
	}
	
	return parallaxTileMap;
}
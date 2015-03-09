#include "Constants.h"
#include "Planet.h"
#include "PlanetFactory.h"

IPlanet* PlanetFactory::create(std::string planetType)
{
    IPlanet* planet = nullptr;
    
	if (planetType == "Grass")
    {
        planet = GrassPlanet::create();
    }
	else if (planetType == "Snow")
    {
        planet = SnowPlanet::create();
    }
    
    return planet;
}
#include "Constants.h"
#include "Planet.h"
#include "PlanetFactory.h"

IPlanet* Planet::create(std::string planet)
{
	if (planet == "Grass")
		return new GrassPlanet();
	else if (planet == "Snow")
		return new SnowPlanet();
}
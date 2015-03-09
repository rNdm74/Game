#ifndef __FranticAlien__PlanetFactory_H__
#define __FranticAlien__PlanetFactory_H__

#include "cocos2d.h"
#include "Planet.h"

using namespace cocos2d;

class PlanetFactory
{
public:	
	static IPlanet* create(std::string planet);
};





#endif /* defined(__FranticAlien__PlanetFactory_H__) */

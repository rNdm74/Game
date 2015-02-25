#ifndef __com_dotdat_World__PARALLAX_FACTORY_H__
#define __com_dotdat_World__PARALLAX_FACTORY_H__

#include "cocos2d.h"

using namespace cocos2d;

class IParallaxTileMap;

class IParallaxFactory
{
public:
	IParallaxFactory(){};
	virtual ~IParallaxFactory(){};
	static IParallaxTileMap* create(std::string className);
};

class ParallaxFactory : public IParallaxFactory
{
public:
	static IParallaxTileMap* create(std::string className);
};

#endif /* defined(__com_dotdat_World__PARALLAX_FACTORY_H__) */

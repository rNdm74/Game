#ifndef __com_dotdat_World__PARALLAX_FACTORY_H__
#define __com_dotdat_World__PARALLAX_FACTORY_H__

#include "cocos2d.h"

using namespace cocos2d;

class IParallaxFactory
{
public:
	static Sprite* create(std::string className, Size mapSize);
};

class ParallaxFactory : public IParallaxFactory
{
public:
	static Sprite* create(std::string className, Size mapSize);
};

class Background : public Sprite
{
public:
	Background(Size mapSize);
};

class Mountains : public Sprite
{
public:
	Mountains(Size mapSize);
};

class Hills : public Sprite
{
public:
	Hills(Size mapSize);
};

#endif /* defined(__com_dotdat_World__PARALLAX_FACTORY_H__) */

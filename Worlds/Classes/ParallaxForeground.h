#ifndef __com_dotdat_World__PARALLAX_FOREGROUND_H__
#define __com_dotdat_World__PARALLAX_FOREGROUND_H__

#include "cocos2d.h"
#include <array>


using namespace cocos2d;

class IParallaxForeground : public ParallaxNode
{
public:
	IParallaxForeground(){};
	virtual ~IParallaxForeground(){};
};

class ParallaxForeground : public IParallaxForeground
{
	typedef IParallaxForeground super;
	typedef ParallaxForeground self;

public:		
	ParallaxForeground(float width);
	virtual ~ParallaxForeground(){};

protected:	
	virtual void init(std::string type, float width);
	virtual Sprite* getHill(std::string type);
	virtual Sprite* getShadowForNode(Sprite* sprite);
};

class GrassForeground : public ParallaxForeground
{
	typedef ParallaxForeground super;
	typedef GrassForeground self;

public:
	static self* create(float width);

	GrassForeground(float width);
	virtual ~GrassForeground(){};
};

class SnowForeground : public ParallaxForeground
{
	typedef ParallaxForeground super;
	typedef SnowForeground self;

public:
	static self* create(float width);

	SnowForeground(float width);
	virtual ~SnowForeground(){};
};

class SandForeground : public ParallaxForeground
{
	typedef ParallaxForeground super;
	typedef SandForeground self;

public:
	static self* create(float width);

	SandForeground(float width);
	virtual ~SandForeground(){};
};

class DirtForeground : public ParallaxForeground
{
	typedef ParallaxForeground super;
	typedef DirtForeground self;

public:
	static self* create(float width);

	DirtForeground(float width);
	virtual ~DirtForeground(){};
};

#endif /* defined(__com_dotdat_World__PARALLAX_FOREGROUND_H__) */

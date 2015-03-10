#ifndef __com_dotdat_World__PARALLAX_BACKGROUND_H__
#define __com_dotdat_World__PARALLAX_BACKGROUND_H__

#include "cocos2d.h"
#include <array>

using namespace cocos2d;

typedef std::array<Sprite*, 3> ParallaxBackgroundImages;

class IParallaxBackground : public ParallaxNode
{
public:
	IParallaxBackground(){};
	virtual ~IParallaxBackground(){};
};

class ParallaxBackground : public IParallaxBackground
{
	typedef IParallaxBackground super;
	typedef ParallaxBackground self;

public:
	ParallaxBackground(float width);
	virtual ~ParallaxBackground(){};

protected:
	virtual void initBackground(std::string type, float width);
	virtual void initMountains(std::string type, float width);
	virtual void initHills(std::string type, float width);
	virtual void initRocks(std::string type, float width);
	//
	void update(float delta);

	Sprite* getShadowForNode(Sprite* sprite);

	Sprite* getBackground(std::string type);
	Sprite* getMountain(std::string type);
	Sprite* getHill(std::string type);
	Sprite* getRock(std::string type);
	Sprite* getCloud(std::string type);

private:
	ParallaxBackgroundImages _clouds;
};


class GrassBackground : public ParallaxBackground
{
	typedef ParallaxBackground super;
	typedef GrassBackground self;

public:
	static self* create(float width);

	GrassBackground(float width);
	virtual ~GrassBackground(){};
};

class SnowBackground : public ParallaxBackground
{
	typedef ParallaxBackground super;
	typedef SnowBackground self;

public:
	static self* create(float width);

	SnowBackground(float width);
	virtual ~SnowBackground(){};
};

class SandBackground : public ParallaxBackground
{
	typedef ParallaxBackground super;
	typedef SandBackground self;

public:
	static self* create(float width);

	SandBackground(float width);
	virtual ~SandBackground(){};
};

class DirtBackground : public ParallaxBackground
{
	typedef ParallaxBackground super;
	typedef DirtBackground self;

public:
	static self* create(float width);

	DirtBackground(float width);
	virtual ~DirtBackground(){};
};

#endif /* defined(__com_dotdat_World__PARALLAX_BACKGROUND_H__) */

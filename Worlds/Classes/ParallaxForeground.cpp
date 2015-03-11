#include "Constants.h"
#include "ParallaxForeground.h"


ParallaxForeground::ParallaxForeground(float width)
{
	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->setScale(2.0f);
	this->setTag(kTagPForegroundLayer);
}

void ParallaxForeground::init(std::string type, float width)
{
	Size size = GetSize(type);

	int count = width / size.width;

	for (int i = 0; i < count; ++i)
	{		
		Sprite* hill = getHill(type);
		Sprite* hillShadow = getShadowForNode(hill);

		Vec2 offset = Vec2(size.width * i, 0.0f);

		/** Add the shadow **/
		this->addChild(hillShadow, -3, ForeGroundLayerHillsShadowParallaxRatio, offset);
		/** Add the hill **/
		this->addChild(hill, -2, ForegroundLayerHillsParallaxRatio, offset);
	}
};

Sprite* ParallaxForeground::getHill(std::string type)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName(type);

	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setFlippedX(true);
	sprite->setTag(kTagHill);

	return sprite;
};

Sprite* ParallaxForeground::getShadowForNode(Sprite* sprite)
{
	Sprite* shadow = Sprite::create();
	shadow->setSpriteFrame(sprite->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.005f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);
	shadow->setFlippedX(true);
	return shadow;
}


GrassForeground* GrassForeground::create(float width)
{
	// Create an instance of ParallaxBackground
	GrassForeground* node = new GrassForeground(width);

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
};

GrassForeground::GrassForeground(float width) : super(width)
{
	this->init("groundGrass.png", width);
};


SnowForeground* SnowForeground::create(float width)
{
	// Create an instance of ParallaxBackground
	SnowForeground* node = new SnowForeground(width);

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
};

SnowForeground::SnowForeground(float width) : super(width)
{		
	this->init("groundIce.png", width);
};


SandForeground* SandForeground::create(float width)
{
	// Create an instance of ParallaxBackground
	SandForeground* node = new SandForeground(width);

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
};

SandForeground::SandForeground(float width) : super(width)
{
	this->init("groundDirt.png", width);
};


DirtForeground* DirtForeground::create(float width)
{
	// Create an instance of ParallaxBackground
	DirtForeground* node = new DirtForeground(width);

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
};

DirtForeground::DirtForeground(float width) : super(width)
{
	this->init("groundRock.png", width);
};
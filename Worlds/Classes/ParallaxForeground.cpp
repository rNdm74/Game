#include "Constants.h"
#include "ParallaxForeground.h"
#include "AppGlobal.h"


ParallaxForeground* ParallaxForeground::create(float width)
{
	// Create an instance of ParallaxBackground
	ParallaxForeground* node = new ParallaxForeground(width);

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
}


ParallaxForeground::ParallaxForeground(float width)
{
	Sprite* hill = this->getHill();

	int count = width / hill->getContentSize().width;
    
    for (int i = 0; i < count; ++i)
    {
		hill = this->getHill();

		/** Add the shadow **/
		this->addChild(getShadowForNode(hill), -3, Vec2(1.1025f, 1.0f), Vec2(hill->getContentSize().width * i, 0.0f));
		/** Add the hill **/		
		this->addChild(hill, -2, Vec2(1.1f, 1.0f), Vec2(hill->getContentSize().width * i, 0.0f));
    }

	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->setScale(2.0f);
	this->setTag(kTagPForegroundLayer);
}

ParallaxForeground::~ParallaxForeground()
{
	
}

Sprite* ParallaxForeground::getMountain()
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("rockGrass.png");

	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setScale(rand_0_1() + 0.5f);
	sprite->getTexture()->setAntiAliasTexParameters();
	sprite->setTag(kTagMountain);

	return sprite;
};

Sprite* ParallaxForeground::getHill()
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("groundGrass.png");
	sprite->setFlippedX(true);
	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setTag(kTagHill);
	return sprite;
};

Sprite* ParallaxForeground::getCloud()
{
	std::string cloudType = std::to_string(random(1, 4));
	std::string filename = "cloud" + cloudType + ".png";

	Sprite* cloud = Sprite::createWithSpriteFrameName(filename);

	cloud->setAnchorPoint(Vec2::ZERO);
	cloud->setScale(rand_0_1() + 1.0f);
	cloud->addChild(getShadowForNode(cloud), -99);
	cloud->setTag(kTagCloud);

	return cloud;
};

void ParallaxForeground::update(float delta)
{
	
}


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
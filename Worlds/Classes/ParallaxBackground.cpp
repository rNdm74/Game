#include "Constants.h"
#include "ParallaxBackground.h"
#include "AppGlobal.h"

ParallaxBackground* ParallaxBackground::create(float width)
{
	// Create an instance of ParallaxBackground
	ParallaxBackground* node = new ParallaxBackground(width);

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

ParallaxBackground::ParallaxBackground(float width)
{
	this->setTag(kTagPBackgroundLayer);
	this->setAnchorPoint(Vec2::ZERO);
	this->setScale(1.5f);

    Sprite* background = this->getBackground();	
	int count = width / background->getContentSize().width;
    
	for (int i = 0; i < count; ++i)
	{
		background = this->getBackground();
		this->addChild(background, -6, Vec2(0.6f, 0.3f), Vec2(background->getContentSize().width * i, 0.0f * this->getScale()));
	}

	Sprite* mountain = this->getMountain();

	int rand = random(mountain->getContentSize().width, mountain->getContentSize().width * 3);
	count = width / (mountain->getContentSize().width + rand);
    
	for (int i = 0; i < count; ++i)
	{
		mountain = this->getMountain();		
		
		float offsetX = (mountain->getContentSize().width + rand) * i;
		float offsetY = 10.0f * this->getScale();

		int zindex = random(3, 6);

		this->addChild(getShadowForNode(mountain), -zindex, Vec2(0.703f, 0.51f), Vec2(offsetX + 3.0f, offsetY - 1.0f));

		this->addChild(mountain, -zindex, Vec2(0.7f, 0.5f), Vec2(offsetX, offsetY));
	}

	Sprite* hill = this->getHill();
	count = width / hill->getContentSize().width;
    
    for (int i = 0; i < count; ++i)
    {
		hill = this->getHill();

		float offsetX = hill->getContentSize().width * i;
		float offsetY = 45.0f * this->getScale();

		this->addChild(getShadowForNode(hill), -3, Vec2(0.803f, 0.91f), Vec2(offsetX + 3.0f, offsetY - 1.0f));
		this->addChild(hill, -2, Vec2(0.8f, 0.9f), Vec2(offsetX, offsetY));
    }
    
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < 5; ++i)
	{
		Sprite* cloud = this->getCloud();
		int zindex = random(3, 6);
		float height = random(200, 400);

		this->addChild(cloud, -zindex, Vec2(rand_0_1(), 0.0f), Vec2(origin.x + visibleSize.width, height * this->getScale()));
	}
}


ParallaxBackground::~ParallaxBackground()
{
	
}


Sprite* ParallaxBackground::getBackground()
{
	Sprite* sprite = Sprite::create("bg0.png");
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);	
	return sprite;
};

Sprite* ParallaxBackground::getMountain()
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("rockGrass.png");
	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setScale(rand_0_1() + 0.5f);
	sprite->getTexture()->setAntiAliasTexParameters();	
	return sprite;
};

Sprite* ParallaxBackground::getHill()
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("groundGrass.png");
	sprite->setAnchorPoint(Vec2::ZERO);
	
	return sprite;
};

Sprite* ParallaxBackground::getCloud()
{
	std::string cloudType = std::to_string(random(1, 3));
	std::string filename = "cloud" + cloudType + ".png";

	Sprite* cloud = Sprite::createWithSpriteFrameName(filename);
	cloud->setAnchorPoint(Vec2::ZERO);
	cloud->setScale(rand_0_1() + 1.0f);
	cloud->addChild(getShadowForNode(cloud), -99);

	return cloud;
};

void ParallaxBackground::update(float delta)
{
	
}

Sprite* ParallaxBackground::getShadowForNode(Sprite* sprite)
{
	Sprite* shadow = Sprite::create();
	shadow->setSpriteFrame(sprite->getSpriteFrame());
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);
	shadow->setScale(sprite->getScale());
	shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	return shadow;
}
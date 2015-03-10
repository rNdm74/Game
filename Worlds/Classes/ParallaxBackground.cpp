#include "Constants.h"
#include "ParallaxBackground.h"
#include "AppGlobal.h"

ParallaxBackground::ParallaxBackground(float width)
{
	this->setTag(kTagPBackgroundLayer);
	this->setAnchorPoint(Vec2::ZERO);
	this->setScale(1.5f);
}

void ParallaxBackground::initBackground(std::string type, float width)
{
	Size size = Sprite::create(type)->getContentSize();

	int count = width / size.width;

	for (int i = 0; i < count + 1; ++i)
	{
		Sprite* background = this->getBackground(type);

		Vec2 offset = Vec2(size.width * i, 0.0f);

		this->addChild(background, -6, BackgroundLayerBackgroundParallaxRatio, offset);
	}
};

void ParallaxBackground::initMountains(std::string type, float width)
{
	Size size = Sprite::createWithSpriteFrameName(type)->getContentSize();
	
	int rand = random(size.width, size.width * 3);
	int count = width / (size.width + rand);

	for (int i = 0; i < count; ++i)
	{
		Sprite* sprite = this->getMountain(type);

		float offsetX = (sprite->getContentSize().width + rand) * i;
		float offsetY = 10.0f * this->getScale();

		Vec2 offset = Vec2(offsetX, offsetY);

		int zindex = random(3, 6);

		this->addChild(getShadowForNode(sprite), -zindex, BackgroundLayerMountainsShadowParallaxRatio, Vec2(offsetX + 3.0f, offsetY - 1.0f));

		this->addChild(sprite, -zindex, BackgroundLayerMountainsParallaxRatio, Vec2(offsetX, offsetY));
	}
};

void ParallaxBackground::initHills(std::string type, float width)
{
	Size size = Sprite::createWithSpriteFrameName(type)->getContentSize();

	int count = width / size.width;

	for (int i = 0; i < count; ++i)
	{
		Sprite* hill = this->getHill(type);

		float offsetX = hill->getContentSize().width * i;
		float offsetY = 45.0f * this->getScale();

		Vec2 offset = Vec2(offsetX, offsetY);

		this->addChild(getShadowForNode(hill), -3, BackgroundLayerHillsShadowParallaxRatio, Vec2(offsetX + 3.0f, offsetY - 1.0f));
		this->addChild(hill, -2, BackgroundLayerHillsParallaxRatio, offset);
	}
};

void ParallaxBackground::initRocks(std::string type, float)
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < 5; ++i)
	{
		Sprite* cloud = this->getCloud(type);
		int zindex = random(3, 6);
		float height = random(200, 400);

		this->addChild(cloud, -zindex, Vec2(rand_0_1(), 0.0f), Vec2(origin.x + visibleSize.width, height * this->getScale()));
	}
};

Sprite* ParallaxBackground::getBackground(std::string type)
{
	Sprite* sprite = Sprite::create(type);
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);	
	return sprite;
};

Sprite* ParallaxBackground::getMountain(std::string type)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName(type);
	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setScale(rand_0_1() + 0.5f);
	sprite->getTexture()->setAntiAliasTexParameters();	
	return sprite;
};

Sprite* ParallaxBackground::getHill(std::string type)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName(type);
	sprite->setAnchorPoint(Vec2::ZERO);
	
	return sprite;
};

Sprite* ParallaxBackground::getRock(std::string type)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName(type);
	sprite->setAnchorPoint(Vec2::ZERO);

	return sprite;
};

Sprite* ParallaxBackground::getCloud(std::string type)
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


GrassBackground* GrassBackground::create(float width)
{
	// Create an instance of ParallaxBackground
	GrassBackground* node = new GrassBackground(width);

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

GrassBackground::GrassBackground(float width) : super(width)
{
	this->initBackground("bg0.png", width);
	this->initMountains("rockGrass.png", width);
	this->initHills("groundGrass.png", width);
}


SnowBackground* SnowBackground::create(float width)
{
	// Create an instance of ParallaxBackground
	SnowBackground* node = new SnowBackground(width);

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

SnowBackground::SnowBackground(float width) : super(width)
{
	this->initBackground("bg0.png", width);
	this->initMountains("rockSnow.png", width);
	this->initHills("groundSnow.png", width);
}


SandBackground* SandBackground::create(float width)
{
	// Create an instance of ParallaxBackground
	SandBackground* node = new SandBackground(width);

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

SandBackground::SandBackground(float width) : super(width)
{
	this->initBackground("bg0.png", width);	
	this->initMountains("hill_largeAlt.png", width);
	this->initHills("groundDirt.png", width);
}


DirtBackground* DirtBackground::create(float width)
{
	// Create an instance of ParallaxBackground
	DirtBackground* node = new DirtBackground(width);

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

DirtBackground::DirtBackground(float width) : super(width)
{
	this->initBackground("bg0.png", width);
	this->initMountains("meteorGrey_big1.png", width);
	this->initHills("groundRock.png", width);
}
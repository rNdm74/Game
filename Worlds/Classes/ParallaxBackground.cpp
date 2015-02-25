#include "Constants.h"
#include "ParallaxBackground.h"


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
    Sprite* sprite = Sprite::create("bg0.png");
    
    int count = width / sprite->getContentSize().width;
    
	for (int i = 0; i < count; ++i)
	{
		sprite = Sprite::create("bg0.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		this->addChild(sprite, -6, Vec2(0.5f, 0.5f), Vec2(sprite->getContentSize().width * i, 110.0f));
	}

    sprite = Sprite::createWithSpriteFrameName("rockGrass.png");

	int rand = random(sprite->getContentSize().width, sprite->getContentSize().width * 3);

	count = width / (sprite->getContentSize().width + rand);
    
	for (int i = 0; i < count; ++i)
	{
		sprite = Sprite::createWithSpriteFrameName("rockGrass.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setScale(rand_0_1() + 0.5f);
		sprite->getTexture()->setAntiAliasTexParameters();
		sprite->addChild(getShadowForNode(sprite), -99);

		float offset = (sprite->getContentSize().width + rand) * i;

		int zindex = random(3, 6);

		this->addChild(sprite, -zindex, Vec2(0.6f, 0.6f), Vec2(offset, 105.0f));
	}

    sprite = Sprite::createWithSpriteFrameName("groundGrass.png");
    count = width / sprite->getContentSize().width;
    
    for (int i = 0; i < count; ++i)
    {
        sprite = Sprite::createWithSpriteFrameName("groundGrass.png");
        sprite->setAnchorPoint(Vec2::ZERO);
		sprite->addChild(getShadowForNode(sprite), -99);
        this->addChild(sprite, -2, Vec2(0.7f, 0.7f), Vec2(sprite->getContentSize().width * i, 105.0f));
    }
    
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < _clouds.size(); ++i)
	{
		std::string filename = "cloud" + std::to_string(i+1) + ".png";
		_clouds[i] = Sprite::createWithSpriteFrameName(filename);
		_clouds[i]->setAnchorPoint(Vec2::ZERO);
		_clouds[i]->setScale(rand_0_1() + 1.0f);
		_clouds[i]->setTag(CLOUDS_STARTING_TAG + i);
		_clouds[i]->addChild(getShadowForNode(_clouds[i]), -99);
		int zindex = random(3, 6);
		float height = random(200, 400);
		this->addChild(_clouds[i], -zindex, Vec2(rand_0_1(), 0.0f), Vec2(origin.x + visibleSize.width, height));
	}

	this->setAnchorPoint(Vec2::ZERO);
	this->setScale(1.25f);
}


ParallaxBackground::~ParallaxBackground()
{
	
}


void ParallaxBackground::update(float delta)
{
	
}


Node* ParallaxBackground::getShadowForNode(Node* node)
{
	auto object = static_cast<Sprite*>(node);

	auto shadow = Sprite::create();
	shadow->setSpriteFrame(object->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.05f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);

	return shadow;
}
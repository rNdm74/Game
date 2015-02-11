#include "Constants.h"
#include "ParallaxBackground.h"

/// <summary>
/// Summary for create()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
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


/// <summary>
/// Summary for ParallaxBackground()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
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

		int zindex = random(3, 6);
		float height = random(200, 400);
		this->addChild(_clouds[i], -zindex, Vec2(rand_0_1(), 0.0f), Vec2(origin.x + visibleSize.width, height));
	}

	this->setAnchorPoint(Vec2::ZERO);
	this->setScale(1.25f);
}


/// <summary>
/// Summary for ~ParallaxBackground()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
ParallaxBackground::~ParallaxBackground()
{
	
}

/// <summary>
/// Summary for update(float delta)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxBackground::update(float delta)
{
	/*for (int i = 0; i < _clouds.size(); ++i)
	{
		Vec2 newPosition = _clouds[i]->getPosition();
		newPosition.x -= CLOUDS_SPEED * delta;

		if (newPosition.x < 0)
		{
			log("Cloud off screen");
			newPosition.x = 800.0f;
		}

		_clouds[i]->setPosition(newPosition);
	}*/
}
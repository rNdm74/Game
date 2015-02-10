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
		this->addChild(sprite, -4, Vec2(0.5f, 0.5f), Vec2(sprite->getContentSize().width * i, 110.0f));
	}

    sprite = Sprite::createWithSpriteFrameName("rockGrass.png");
    count = width / sprite->getContentSize().width;
    
	for (int i = 0; i < count; ++i)
	{
		sprite = Sprite::createWithSpriteFrameName("rockGrass.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		this->addChild(sprite, -3, Vec2(0.6f, 0.6f), Vec2(sprite->getContentSize().width * i, 110.0f));
	}

    sprite = Sprite::createWithSpriteFrameName("groundGrass.png");
    count = width / sprite->getContentSize().width;
    
    for (int i = 0; i < count; ++i)
    {
        sprite = Sprite::createWithSpriteFrameName("groundGrass.png");
        sprite->setAnchorPoint(Vec2::ZERO);
        this->addChild(sprite, -2, Vec2(0.7f, 0.7f), Vec2(sprite->getContentSize().width * i, 110.0f));
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
	
}
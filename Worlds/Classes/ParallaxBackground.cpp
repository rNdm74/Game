#include "ParallaxBackground.h"

/// <summary>
/// Summary for create()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
ParallaxBackground* ParallaxBackground::create()
{
	// Create an instance of ParallaxBackground
	ParallaxBackground* node = new ParallaxBackground();

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
ParallaxBackground::ParallaxBackground()
{
	for (int i = 0; i < _background.size(); ++i)
	{
		_background[i] = Sprite::create("bg0.png");
		_background[i]->setAnchorPoint(Vec2::ZERO);
		this->addChild(_background[i], -4, Vec2(0.5f, 0.5f), Vec2(_background[i]->getContentSize().width * i, 100.0f));
	}

	for (int i = 0; i < _midground.size(); ++i)
	{
		_midground[i] = Sprite::createWithSpriteFrameName("groundGrass.png");
		_midground[i]->setAnchorPoint(Vec2::ZERO);
		this->addChild(_midground[i], -3, Vec2(0.6f, 0.6f), Vec2(_midground[i]->getContentSize().width * i, 100.0f));
	}

	for (int i = 0; i < _foreground.size(); ++i)
	{
		_foreground[i] = Sprite::createWithSpriteFrameName("rockGrass.png");
		_foreground[i]->setAnchorPoint(Vec2::ZERO);
		this->addChild(_foreground[i], -2, Vec2(0.7f, 0.7f), Vec2(_foreground[i]->getContentSize().width * i, 100.0f));
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
	Vec2 pos = this->convertToWorldSpaceAR(_background[0]->getPosition());
	if (pos.x + _background[0]->getContentSize().width < 0)
	{
		log("wrap background");

		this->removeChild(_background[0], true);
		this->addChild(_background[0], -4, Vec2(0.5f, 0.5f), Vec2(_background[0]->getContentSize().width, 100.0f));
	}

		

	/*for (int i = 0; i < _midground.size(); ++i)
	{
		_midground[i] = Sprite::createWithSpriteFrameName("groundGrass.png");
		_midground[i]->setAnchorPoint(Vec2::ZERO);
		this->addChild(_midground[i], -3, Vec2(0.6f, 0.6f), Vec2(_midground[i]->getContentSize().width * i, 100.0f));
	}

	for (int i = 0; i < _foreground.size(); ++i)
	{
		_foreground[i] = Sprite::createWithSpriteFrameName("rockGrass.png");
		_foreground[i]->setAnchorPoint(Vec2::ZERO);
		this->addChild(_foreground[i], -2, Vec2(0.7f, 0.7f), Vec2(_foreground[i]->getContentSize().width * i, 100.0f));
	}*/
}
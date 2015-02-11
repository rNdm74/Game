#include "ParallaxFactory.h"

Sprite* ParallaxFactory::create(std::string className, Size mapSize)
{
	if (className == "Background")
	{
		return Background::create();
	}
}

Background::Background(Size mapSize)
{
	this->setContentSize(mapSize);
	this->setAnchorPoint(Vec2::ZERO);
	
	Sprite* sprite = Sprite::create("bg0.png");

	int count = mapSize.width / sprite->getContentSize().width;

	for (int i = 0; i < count; ++i)
	{
		sprite = Sprite::create("bg0.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPositionX(sprite->getContentSize().width * i);
		this->addChild(sprite);
	}
}

Mountains::Mountains(Size mapSize)
{

}

Hills::Hills(Size mapSize)
{

}
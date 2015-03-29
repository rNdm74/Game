#include "GameObject.h"

USING_NS_CC;

IGameObject* GameObjectFactory::create(std::string className, ValueMap& properties)
{
	IGameObject* gameObject = nullptr;

	if (className == "Player")
	{
		//gameObject = Player::create(properties);
	}
	else if (className == "Door")
	{
		//
		gameObject = Door::create(properties);
	}
	else if (className == "Light")
	{
		//
		gameObject = Light::create(properties);
	}

	return gameObject;
}

void GameObject::update(float delta)
{
    
};

Light* Light::create(ValueMap& properties)
{
	// Create an instance of Level
	Light* light = new (std::nothrow) Light(properties);

	if (light && light->init())
	{
		light->autorelease();
		light->setAnchorPoint(Vec2::ZERO);
		return light;
	}

	CC_SAFE_DELETE(light);
	return nullptr;
};

Light::Light(cocos2d::ValueMap& properties)
{

	float x = properties["x"].asFloat();
	float y = std::abs(properties["y"].asFloat());

	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();

	this->setName(properties["name"].asString());
	this->setContentSize(Size(width, height));
	this->setPosition(Vec2(x, height + y));
	this->setTag(TAG_GAMEOBJECT_LIGHT);

	std::string name = properties["name"].asString();
	_sprite = Sprite::create(name);

	this->addChild(_sprite);

	_sprite->setAnchorPoint(Vec2::ZERO);
};

Door* Door::create(ValueMap& properties)
{
	// Create an instance of Level
	Door* door = new (std::nothrow) Door(properties);

	if (door && door->init())
	{
		door->autorelease();
		door->setAnchorPoint(Vec2::ZERO);
		return door;
	}

	CC_SAFE_DELETE(door);
	return nullptr;
};

Door::Door(cocos2d::ValueMap& properties)
{
	
	float x = properties["x"].asFloat();
	float y = std::abs(properties["y"].asFloat());

	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();

	this->setName(properties["name"].asString());
	this->setContentSize(Size(width, height));	
	this->setPosition(Vec2(x, height - y));	
	this->setTag(TAG_GAMEOBJECT_DOOR);
	
	std::string name = properties["name"].asString();
	_sprite = Sprite::create(name);

	this->addChild(_sprite);	

	_sprite->setAnchorPoint(Vec2::ZERO);
};
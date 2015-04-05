#include "GameObject.h"

USING_NS_CC;

GameObject::GameObject(ValueMap& properties)
{
	this->_properties = properties;

	/** **/
	float x = properties["x"].asFloat() * CC_CONTENT_SCALE_FACTOR();
	float y = properties["y"].asFloat() * CC_CONTENT_SCALE_FACTOR();
	float width = properties["width"].asFloat() * CC_CONTENT_SCALE_FACTOR();
	float height = properties["height"].asFloat() * CC_CONTENT_SCALE_FACTOR();

	this->setName(properties["name"].asString());

	this->setObjectSize(Size(width, height));
	this->setPosition(Vec2(x, y));
};


void GameObject::update(float delta)
{
    
};
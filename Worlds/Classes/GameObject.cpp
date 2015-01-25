#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "PhysicsComponent.h"

GameObject::GameObject(ValueMap& properties)
{
	this->setProperties(properties);
	this->setAnchorPoint(Vec2::ZERO);
};

void GameObject::update(Node* node)
{	
	
};

void GameObject::setProperties(ValueMap& properties) 
{
	_properties = properties; 
};

ValueMap GameObject::getProperties()
{ 
	return _properties;
};

Vec2* GameObject::getDirection() 
{ 
	return _direction; 
};

void GameObject::setBearing(EBearing bearing) 
{ 
	_bearing = bearing; 
}

EBearing GameObject::getBearing() 
{ 
	return _bearing; 
};

Size GameObject::getSize()
{ 
	return this->getContentSize(); 
};

Rect GameObject::getBoundingBox()
{ 
	return this->getBoundingBox(); 
};

Rect GameObject::getCollisionBoundingBox()
{ 
	return this->getBoundingBox();
};

//
// CHILD CLASSES
//

Player::Player(ValueMap& properties, MenuComponent* menu, InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics) : super(properties)
{
	_menu = menu;
	_input = input;
	_physics = physics;
	_graphics = graphics;

	_sprite = Sprite::createWithSpriteFrameName(kPlayerFileName);
	_sprite->getTexture()->setAliasTexParameters();
	_sprite->getTexture()->setAntiAliasTexParameters();
	_sprite->setScale(1.0f);
	_sprite->setAnchorPoint(Vec2::ZERO);

	_direction = new Vec2[4];
	_direction[NORTH] = Vec2(0, 1);
	_direction[SOUTH] = Vec2(0, -1);
	_direction[EAST] = Vec2(-1, 0);
	_direction[WEST] = Vec2(1, 0);
	_direction[STOP] = Vec2(0, 0);

	_bearing = STOP;
	
	auto shadow = Sprite::create();
	shadow->setSpriteFrame(_sprite->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.1f, 0.0f)); // position it to the center of the target node
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);
	_sprite->addChild(shadow, -1);

	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = _sprite->getContentSize().width;
	float height = _sprite->getContentSize().height;
		
	this->addChild(_sprite);
	this->setPosition(Vec2(x, y));
};

void Player::update(Node* node)
{		
	//_physics->update(*this);
	_graphics->update(*this);
	_input->update(*this);	
};

Size Player::getSize()
{
	return _sprite->getContentSize();
}

Rect Player::getBoundingBox()
{
	return Rect
	(
		this->getPosition().x, 
		this->getPosition().y, 
		_sprite->getContentSize().width, 
		_sprite->getContentSize().height
	);
}

Rect Player::getCollisionBoundingBox()
{
	Rect collisionBox = getBoundingBox();

	Vec2 diff = desiredPosition - getPosition();

	collisionBox.origin = collisionBox.origin + diff;

	Rect returnBoundingBox = collisionBox;//CGRectOffset(collisionBox, diff.x, diff.y);
	
	return returnBoundingBox;
}
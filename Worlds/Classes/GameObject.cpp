#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"

GameObject::GameObject(ValueMap& properties)
{
	this->setProperties(properties);
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

Vec2 GameObject::getCenterPosition()
{
	return Vec2
	(
		this->getPosition().x + this->getSize().width / 2,
		this->getPosition().y + this->getSize().height / 2
	);
}

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

Player::Player(ValueMap& properties, MenuComponent* menu, InputComponent* input, CollisionComponent* collision, GraphicsComponent* graphics) : super(properties)
{
	_menu = menu;
	_input = input;
	_collision = collision;
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

	isClimbing = false;
	gravity = true;
	
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
    this->setContentSize(Size(width, height));
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2(x, y));

};

void Player::update(Node* node)
{		
	_graphics->update(*this);
	_input->update(*this);	
	_collision->update(*node, *this);
};

Size Player::getSize()
{
	return _sprite->getContentSize();
}

//Vec2 Player::getCenterPosition()
//{
//	return Vec2
//	(
//		this->getPosition().x + getSize().width / 2,
//		this->getPosition().y + getSize().height / 2		
//	);
//}

Sprite* Player::getSprite()
{
	return _sprite;
}

Rect Player::getBoundingBox()
{
	return Rect
	(
		this->getPosition().x, 
		this->getPosition().y, 
		this->getSize().width, 
		this->getSize().height
	);
}

Rect Player::getCollisionBoundingBox()
{
	Rect collisionBox = this->getBoundingBox();

	Vec2 diff = desiredPosition - collisionBox.origin;

	collisionBox.origin = collisionBox.origin + diff;
			
	return collisionBox;
}
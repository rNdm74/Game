#include "AppGlobal.h"
#include "Box2D.h"
#include "Box2dHelper.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "PhysicsComponent.h"

GameObject::GameObject(ValueMap& properties)
{
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->setProperties(properties);
};

void GameObject::update(Node* node)
{	
	Vec2 pos = node->convertToWorldSpace(this->getPosition());
		
	b2Vec2 b2Position = b2Vec2
	(
		(pos.x + _rect.size.width / 2.0f) / kPixelsPerMeter,
		(pos.y + _rect.size.height / 2.0f) / kPixelsPerMeter
	);
	
	_body->SetTransform(b2Position, 0);
};

void GameObject::addBodyToWorld(b2World& world)
{
	_body = Box2dHelper::createBody(world, _bodyDef, _fixtureDef);
};

//
// CHILD CLASSES
//

SolidPlatform::SolidPlatform(ValueMap& properties) : super(properties)
{
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();
		
	// converts world position to box2d world position
	float x_ = (x + width / 2.0f) / kPixelsPerMeter;
	float y_ = (y + height / 2.0f) / kPixelsPerMeter;

	_bodyDef = Box2dHelper::createBodyDef(b2_kinematicBody, x_, y_, this);

	_shape = Box2dHelper::createBoxShape(width, height);

	_rect = Rect(x, y, width, height);
	
	_fixtureDef = Box2dHelper::createFixtureDef
	(
		_shape,
		2.0f,
		0.2f,
		0.0f,
		kFilterCatagory::SOLID_PLATFORM,
		kFilterCatagory::PLAYER | kFilterCatagory::ENEMY,
		false
	);
		
	
	this->setPosition(Vec2(x, y));
};

SolidSlope::SolidSlope(ValueMap& properties) : super(properties)
{
	ValueVector pointsVector = properties["points"].asValueVector();

	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();

	float verticesSize = pointsVector.size() + 1;
	b2Vec2 vertices[30];
	int vindex = 0;

	Vec2 position = Vec2((x) / kPixelsPerMeter, y / kPixelsPerMeter);

	for (Value point : pointsVector)
	{
		vertices[vindex].x = (point.asValueMap()["x"].asFloat() / kPixelsPerMeter + position.x);
		vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / kPixelsPerMeter + position.y);
		vindex++;
	}

	_bodyDef = Box2dHelper::createBodyDef(b2_kinematicBody, 0.0f, 0.0f, this);

	_shape.CreateChain(vertices, vindex);
	
	_fixtureDef = Box2dHelper::createFixtureDef
	(
		_shape,
		2.0f,
		0.2f,
		0.0f,
		kFilterCatagory::SOLID_SLOPE,
		kFilterCatagory::PLAYER | kFilterCatagory::ENEMY,
		false
	);
};

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
	
	float x_ = x / kPixelsPerMeter;
	float y_ = y / kPixelsPerMeter;

	_bodyDef = Box2dHelper::createBodyDef(b2_dynamicBody, x_, y_, this);

	_shape = Box2dHelper::createBoxShape(width * _sprite->getScale(), height * _sprite->getScale());

	_fixtureDef = Box2dHelper::createFixtureDef
	(
		_shape,
		1.0f,
		0.1f,
		0.0f,
		kFilterCatagory::PLAYER,
		kFilterCatagory::SOLID_PLATFORM | kFilterCatagory::SOLID_SLOPE,
		false
	);

	this->addChild(_sprite);
	//this->setPosition(Vec2(x, y));
};

void Player::update(Node* node)
{		
	//_physics->update(*this);
	_graphics->update(*this);
	_input->update(*this);
	
};
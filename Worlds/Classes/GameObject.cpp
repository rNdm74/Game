#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"

/**
* Moveable
* 
*
*/
#pragma region Moveable


/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*
*/
Moveable::Moveable()
{
	path = nullptr;

	onGround = false;

	canMove = false;
	canMoveUp = false;
	canMoveDown = false;
	canMoveLeft = false;
	canMoveRight = false;

	isMovingUp = false;
	isMovingDown = false;
	isMovingLeft = false;
	isMovingRight = false;

	isClimbing = false;
	gravity = true;

	velocity = Vec2::ZERO;
	desiredPosition = Vec2::ZERO;
};


#pragma endregion Moveable


/**
* GameObjectNode
* 
*
*/
#pragma region GameObjectNode


/**
* Create a new GameObjectNode
*
* @param properties The ValueMap that contains information about the gameObject
*/
GameObjectNode::GameObjectNode(ValueMap& properties)
{
	this->setProperties(properties);
};


/**
* Sets the properties of a GameObject
*
* @param properties The ValueMap that contains information about the gameObject
*/
void GameObjectNode::setProperties(ValueMap& properties)
{
	_properties = properties;
};


/**
* Get the GameObject property ValueMap information
*
* @return The ValueMap reference of the GameObjects properties
*/
ValueMap GameObjectNode::getProperties()
{
	return _properties;
};


#pragma endregion GameObjectNode


/**
* GameObject
*
*
*/
#pragma region GameObject


/**
* Create a new GameObject
*
* @param properties The ValueMap that contains information about the gameObject
*/
GameObject::GameObject(ValueMap& properties) : super(properties)
{
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();

	this->setContentSize(Size(width, height));
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2(x, y));
};


/**
* Updates a GameObject
*
* @param node The Node that contains the gameObject
*/
void GameObject::update(Node* node)
{	
	
};


/**
* Get the GameObject content size
*
* @return The Size of the GameObject
*/
Size GameObject::getSize()
{ 
	return this->getContentSize(); 
};


/**
* Get the GameObject center Vec2
*
* @return The Vec2 of the GameObject center position
*/
Vec2 GameObject::getCenterPosition()
{
	float x = this->getPosition().x + this->getSize().width / 2;
	float y = this->getPosition().y + this->getSize().height / 2;
	
	return Vec2(x, y);
};


/**
* Get the GameObject bounding box
*
* @return The Rect of the GameObject bounding box
*/
Rect GameObject::getBoundingBox()
{ 
	return Rect
	(
		this->getPosition().x,
		this->getPosition().y,
		this->getSize().width,
		this->getSize().height
	);
};


/**
* Get the GameObject collision bounding box, overridden by child classes
*
* @return The Rect of the GameObject collision || bounding box
*/
Rect GameObject::getCollisionBoundingBox()
{ 
	return this->getBoundingBox();
};


#pragma endregion GameObject


/**
* Player
*
*
*/
#pragma region Player


/**
* Create a Player GameObject, in the constructor, its properties are set
* All components that are passed in are set, the sprite that contains the image of the player is created and added, 
* A shadow sprite is created and added, finally is position is set
*
* @param properties The ValueMap that contains information about the gameObject
* @param menu The MenuComponent that contains information about the gameObject
* @param input The InputComponent that contains information about the gameObject
* @param collision The CollisionComponent that contains information about the gameObject
* @param graphics The GraphicsComponent that contains information about the gameObject
*/
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
	
	auto shadow = Sprite::create();
	shadow->setSpriteFrame(_sprite->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.1f, 0.0f)); // position it to the center of the target node
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);
	_sprite->addChild(shadow, -1);

	float width = _sprite->getContentSize().width;
	float height = _sprite->getContentSize().height;
	
	this->setContentSize(Size(width, height));
	this->addChild(_sprite);
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
};


Sprite* Player::getSprite()
{
	return _sprite;
};


Rect Player::getCollisionBoundingBox()
{
	Rect collisionBox = this->getBoundingBox();

	Vec2 diff = desiredPosition - collisionBox.origin;

	collisionBox.origin = collisionBox.origin + diff;
			
	return collisionBox;
};


#pragma endregion Player


/**
* Left
*
*
*/
#pragma region Left


/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*
*/
Left::Left(ValueMap& properties, CollisionComponent* collision) : super(properties)
{
	_collision = collision;
};


/**
* Updates a Left
*
* @param node The Node that contains the gameObject
*/
void Left::update(Node* node)
{
	_collision->update(*node, *this);
};


#pragma endregion Left


/**
* Right
*
*
*/
#pragma region Right


/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*
*/
Right::Right(ValueMap& properties, CollisionComponent* collision) : super(properties)
{
	_collision = collision;
};


/**
* Updates a Right
*
* @param node The Node that contains the gameObject
*/
void Right::update(Node* node)
{
	_collision->update(*node, *this);
};


#pragma endregion Right


/**
* Enter
*
*
*/
#pragma region Enter


/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*
*/
Enter::Enter(ValueMap& properties, CollisionComponent* collision) : super(properties)
{
	_collision = collision;
};


/**
* Updates a Enter
*
* @param node The Node that contains the gameObject
*/
void Enter::update(Node* node)
{
	_collision->update(*node, *this);
};


#pragma endregion Enter


/**
* Exit
*
*
*/
#pragma region Exit


/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*
*/
Exit::Exit(ValueMap& properties, CollisionComponent* collision) : super(properties)
{
	_collision = collision;
};


/**
* Updates a Exit
*
* @param node The Node that contains the gameObject
*/
void Exit::update(Node* node)
{
	_collision->update(*node, *this);
};


#pragma endregion Exit
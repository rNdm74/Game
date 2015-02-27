#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFsm.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"

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
GameObject::GameObject(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics)
{	
	_collision = collision;
	_graphics = graphics;
		
	_path = nullptr;

	for (int i = 0; i < 4; ++i)
	{
		_canMove[i] = false;
		_isMoving[i] = false;		
	}

	_onGround = false;
	_isClimbing = false;
		
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();

	this->setProperties(properties);
	this->setName(properties["name"].asString());
	this->setContentSize(Size(width, height));
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2(x, y));

	this->CurrentState = EGameObjectState::Stop;
};


/**
* Updates a GameObject
*
* @param node The Node that contains the gameObject
*/
void GameObject::update(Node* node)
{
	_collision->update(*node, *this);
};


bool GameObject::containsPoint(Vec2 point)
{
	Rect rect = this->getBoundingBox();

	return rect.containsPoint(point);
};

/** Getters **/
#pragma region Getters

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
		this->getSize().width
	);
};

/**
* Get the GameObject collision bounding box, overridden by child classes
*
* @return The Rect of the GameObject collision || bounding box
*/
Rect GameObject::getCollisionBox()
{
	return this->getBoundingBox();
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

bool GameObject::getClimbing()
{
	return _isClimbing;
};

Vec2 GameObject::getDesiredPosition()
{
	return _desiredPosition;
};

Vec2 GameObject::getDirection()
{
	return _direction;
};

Vec2 GameObject::getMapTransition()
{
	return _mapTransition;
};

bool GameObject::getOnGround()
{
	return _onGround;
};

IPath* GameObject::getPath()
{
	return _path;
};

/**
* Get the GameObject property ValueMap information
*
* @return The ValueMap reference of the GameObjects properties
*/
ValueMap GameObject::getProperties()
{
	return _properties; 
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

Vec2 GameObject::getVelocity()
{
	return _velocity;
};

CanMove GameObject::getCanMove()
{
	return _canMove;
};

IsMoving GameObject::getIsMoving()
{
	return _isMoving;
};


#pragma endregion Getters

/** Setters **/
#pragma region Setters

void GameObject::setBoundingBox(Rect boundingBox)
{

};

void GameObject::setCollisionBox(Rect collisionBox)
{

};

void GameObject::setClimbing(bool climbing)
{
	_isClimbing = climbing;
};

void GameObject::setDesiredPosition(Vec2 desiredPosition)
{
	_desiredPosition = desiredPosition;
};

void GameObject::setDesiredPositionX(float x)
{
	_desiredPosition.x = x;
};

void GameObject::setDesiredPositionY(float y)
{
	_desiredPosition.y = y;
};

void GameObject::setDirection(Vec2 direction)
{
	_direction = direction;
};

void GameObject::setMapTransition(Vec2 mapTransition)
{
	_mapTransition = mapTransition;
};

void GameObject::setOnGround(bool onGround)
{
	_onGround = onGround;
};

void GameObject::setPath(IPath* path)
{
	_path = path;
};

/**
* Sets the properties of a GameObject
*
* @param properties The ValueMap that contains information about the gameObject
*/
void GameObject::setProperties(ValueMap& properties)
{
	_properties = properties;
};

void GameObject::setSize(Vec2 size)
{
};

void GameObject::setVelocity(Vec2 velocity)
{
	_velocity = velocity;
};

void GameObject::setCanMove(CanMove canMove)
{
	_canMove = canMove;
};

void GameObject::setIsMoving(IsMoving isMoving)
{
	_isMoving = isMoving;
};

#pragma endregion Setters

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
Player::Player(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics, IMenuComponent* menu, IInputComponent* input) : super(properties, collision, graphics)
{
	_menu = menu;
	_input = input;

	_fsm = new GameObjectFsm(this);

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
	
	this->setContentSize(Size(width, width));
	this->setTag(kTagPlayer);
	this->addChild(_sprite);

	_isLoaded = true;

	_desiredPosition = this->getPosition();	
};


void Player::update(Node* node)
{	
	void(GameObjectFsm:: *ptrs[])() =
	{
		&GameObjectFsm::CheckCanClimbUp,
		&GameObjectFsm::CheckCanClimbDown,
		&GameObjectFsm::CheckCanWalkLeft,
		&GameObjectFsm::CheckCanWalkRight,
		&GameObjectFsm::Stop,
		&GameObjectFsm::LoadNextMap,
		&GameObjectFsm::LoadPreviousMap
	};

	EGameObjectState state = AppGlobal::getInstance()->gameObjectState;

	(_fsm->*ptrs[state])();

	_collision->update(*node, *this);

	this->setPosition(this->getDesiredPosition());
};

void Player::ClimbUp()
{
	// Run climbing animation
	_graphics->ClimbUp(*this);
	_input->ClimbUp(*this);
};

void Player::ClimbDown()
{
	// Run climbing animation
	_graphics->ClimbDown(*this);
	_input->ClimbDown(*this);
};

void Player::WalkLeft()
{
	// Run walking animation
	_graphics->WalkLeft(*this);
	_input->WalkLeft(*this);
};

void Player::WalkRight()
{
	// Run walking animation
	_graphics->WalkRight(*this);
	_input->WalkRight(*this);
};

void Player::Stop()
{
	// Run stop animation
	_graphics->Stop(*this);
	_input->Stop(*this);
};

Size Player::getSize()
{
	return _sprite->getContentSize();
};

Sprite* Player::getSprite()
{
	return _sprite;
};

Rect Player::getCollisionBox()
{
	Rect collisionBox = this->getBoundingBox();

	Vec2 diff = _desiredPosition - collisionBox.origin;

	collisionBox.origin = collisionBox.origin + diff;
			
	return collisionBox;
};

bool Player::IsLoaded()
{
	return _isLoaded;
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
Left::Left(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics) : super(properties, collision, graphics)
{
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
Right::Right(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics) : super(properties, collision, graphics)
{	
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
Enter::Enter(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics) : super(properties, collision, graphics)
{			
	auto _particle = ParticleGalaxy::createWithTotalParticles(900);
	_particle->setAutoRemoveOnFinish(true);
	_particle->setAnchorPoint(Vec2::ZERO);

	SpriteFrame* _particleFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("fire07.png");
	_particle->setTextureWithRect(_particleFrame->getTexture(), _particleFrame->getRect());	
	_particle->setEmitterMode(kCCParticleModeGravity);

	//The code below we can use both in 2.x and 3.x
	_particle->setDuration(-1);
	_particle->setGravity(Point(0, 240));
	_particle->setAngle(90);
	_particle->setAngleVar(180);
	_particle->setRadialAccel(50);
	_particle->setRadialAccelVar(0);
	_particle->setTangentialAccel(30);
	_particle->setTangentialAccelVar(0);
	_particle->setPosition(Vec2(35.0f, 35.0f));
	_particle->setLife(0.6);
	_particle->setLifeVar(0.6);
	_particle->setStartSpin(30);
	_particle->setStartSpinVar(60);
	_particle->setEndSpin(60);
	_particle->setEndSpinVar(60);
	_particle->setStartSize(15.0f);
	_particle->setStartSizeVar(0);
	_particle->setEndSize(50.0f);
	_particle->setEndSizeVar(0);
	_particle->setEmissionRate(100);
	this->addChild(_particle, 10);
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
Exit::Exit(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics) : super(properties, collision, graphics)
{
};


#pragma endregion Exit
#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFsm.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"
#include "ParallaxTileMap.h"

/**
* GameObject
*
*
*/
#pragma region GameObject

GameObject* GameObject::create(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics)
{
	// Create an instance of Level
	GameObject* gameObject = new (std::nothrow) GameObject(properties, collision, graphics);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setAnchorPoint(Vec2::ZERO);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

/**
* Create a new GameObject
*
* @param properties The ValueMap that contains information about the gameObject
*/
GameObject::GameObject(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics)
{	
	_collision = collision;
	_graphics = graphics;
	_desiredPosition.x = properties["x"].asFloat();
	_desiredPosition.y = properties["y"].asFloat();

	_path = nullptr;
		
	_onGround = false;
	_isClimbing = false;
		
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();

	this->setProperties(properties);
	this->setName(properties["name"].asString());
	this->setContentSize(Size(width, height));
	this->setPosition(Vec2(x, y));
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
* Get the GameObject currentState
*
* @return enum EGameObjectState
*/
EGameObjectState GameObject::getCurrentState()
{
	return CurrentState;
};
/**
* Get the GameObject finite state machine
*
* @return the pointer to the finite state machine
*/
IGameObjectFsm* GameObject::getFsm()
{
	return _fsm;
};
/**
* Get the path of the gameObject
*
* @return the pointer to the finite state machine
*/
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
	float x = this->getPosition().x + this->getContentSize().width / 2;
	float y = this->getPosition().y + this->getContentSize().height / 2;

	return Vec2(x, y);
};
/**
* Get the GameObject desiredPosition
*
* @return The Vec2 of the GameObject desired position
*/
Vec2 GameObject::getDesiredPosition()
{
	return _desiredPosition;
};
/**
* Get the GameObject's direction
*
* @return The Vec2 of the GameObject moving direction
*/
Vec2 GameObject::getDirection()
{
	return _direction;
};
/**
* Get the GameObject's velocity
*
* @return The Vec2 of the GameObject moving velocity
*/
Vec2 GameObject::getVelocity()
{
	return _velocity;
};
/**
* Get the is GameObject climbing
*
* @return The bool of the GameObject isClimbing
*/
bool GameObject::getClimbing()
{
	return _isClimbing;
};
/**
* Get the GameObject content size
*
* @return The Size of the GameObject
*/
bool GameObject::getOnGround()
{
	return _onGround;
};


#pragma endregion Getters

/** Setters **/
#pragma region Setters

/****/
void GameObject::setCurrentState(EGameObjectState newState)
{
    this->CurrentState = newState;
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

void GameObject::setDirection(Vec2 direction)
{
	_direction = direction;
};

void GameObject::setDesiredPosition(Vec2 desiredPosition)
{
	_desiredPosition = desiredPosition;
};

void GameObject::setVelocity(Vec2 velocity)
{
	_velocity = velocity;
};

void GameObject::setDesiredPositionX(float x)
{
	_desiredPosition.x = x;
};

void GameObject::setDesiredPositionY(float y)
{
	_desiredPosition.y = y;
};

void GameObject::setClimbing(bool climbing)
{
	_isClimbing = climbing;
};

void GameObject::setOnGround(bool onGround)
{
	_onGround = onGround;
};

#pragma endregion Setters

#pragma endregion GameObject


/**
* Player
*
*
*/
#pragma region Player


Player* Player::create(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics, IMenuComponent* menu, IInputComponent* input)
{
	// Create an instance of Level
	Player* player = new (std::nothrow) Player(properties, collision, graphics, menu, input);

	if (player && player->init())
	{
		player->autorelease();
		player->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(kPlayerFileName));
		player->getTexture()->setAliasTexParameters();
		player->getTexture()->setAntiAliasTexParameters();
		player->setAnchorPoint(Vec2::ZERO);

		return player;
	}

	CC_SAFE_DELETE(player);
	return nullptr;
};

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
	_fsm = new GameObjectFsm(this);

	_menu = menu;
	_input = input;
		
	this->setTag(kTagPlayer);

	_shadow = Sprite::create();
	_shadow->setSpriteFrame(this->getSpriteFrame());
	_shadow->setAnchorPoint(Vec2(-0.1f, 0.0f)); // position it to the center of the target node
	_shadow->setColor(Color3B(0, 0, 0));
	_shadow->setOpacity(50);

	this->addChild(_shadow, -1);
		
	this->CurrentState = EGameObjectState::Stop;

	activeMap = AppGlobal::getInstance()->activeMap;
};

void Player::update(Node* node)
{		
	void(IGameObjectFsm:: *ptrs[])() =
	{
		&IGameObjectFsm::CheckCanClimbUp,
		&IGameObjectFsm::CheckCanClimbDown,
		&IGameObjectFsm::CheckCanWalkLeft,
		&IGameObjectFsm::CheckCanWalkRight,
		&IGameObjectFsm::Stop,
		&IGameObjectFsm::LoadNextMap,
		&IGameObjectFsm::LoadPreviousMap,
		&IGameObjectFsm::OnGround
	};
		
	(_fsm->*ptrs[this->getCurrentState()])();

	_input->update(*node, *this);
	_collision->update(*node, *this);
		
	this->setPosition(this->getDesiredPosition());

	_shadow->setSpriteFrame(this->getSpriteFrame());
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

void Player::Idle()
{
    _graphics->Idle(*this);
    _input->Stop(*this);
};

void Player::Crouch()
{
    _graphics->Crouch(*this);
    _input->Stop(*this);
};

void Player::Jump()
{
};

void Player::Die()
{
};

void Player::Hurt()
{
};

void Player::ThrowGem()
{
};

void Player::PickUpGem()
{
};

void Player::Talk()
{
};

Rect Player::getCollisionBox()
{
	Rect collisionBox = this->getBoundingBox();

	Vec2 diff = _desiredPosition - collisionBox.origin;

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
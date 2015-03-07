#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"

#include "FsmComponent.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"

/**
* GameObject
*/
GameObject* GameObject::create(ValueMap& properties)
{
	// Create an instance of Level
	GameObject* gameObject = new (std::nothrow) GameObject(properties);

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
* @param 
*/
GameObject::GameObject(ValueMap& properties)
{
	_properties = properties;
				
	_desiredPosition.x = properties["x"].asFloat();
	_desiredPosition.y = properties["y"].asFloat();
							
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();
		
	this->setName(properties["name"].asString());
	this->setContentSize(Size(width, height));
	this->setPosition(Vec2(x, y));

	_sprite = Sprite::create();
	_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_sprite->setPositionX(width / 2);
	this->addChild(_sprite);
};

GameObject::~GameObject(){};

/**
* Updates a GameObject
*
* @param node The Node that contains the gameObject
*/
void GameObject::update(Node* node)
{
	//_collision->update(*node);
    //_graphics->update(*node);
};

ValueMap GameObject::getProperties()
{
	return _properties;
};

Rect GameObject::getCollisionBox()
{
    float x = _properties["x"].asFloat();
    float y = _properties["y"].asFloat();
    float width = _properties["width"].asFloat();
    float height = _properties["height"].asFloat();
    
    return Rect(x,y,width,height);
};

Vec2 GameObject::getCenterPosition()
{
	float x = this->getPosition().x + this->getContentSize().width / 2;
	float y = this->getPosition().y + this->getContentSize().height / 2;

	return Vec2(x, y);
};

Vec2 GameObject::getDesiredPosition()
{
	return _desiredPosition;
};

Vec2 GameObject::getDirection()
{
	return _direction;
};

Vec2 GameObject::getVelocity()
{
	return _velocity;
};

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

void GameObject::setSpriteFrame(SpriteFrame* spriteFrame)
{
	_sprite->setSpriteFrame(spriteFrame);
};

void GameObject::setFlippedX(bool flippedX)
{
	_sprite->setFlippedX(flippedX);
};


/**
* Player
*/
Player* Player::create(ValueMap& properties)
{
	// Create an instance of Level
	Player* player = new (std::nothrow) Player(properties);

	if (player && player->init())
	{
		player->autorelease();
		player->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(kPlayerFileName));
		player->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
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
Player::Player(ValueMap& properties) : super(properties)
{
	_fsm = new (std::nothrow) PlayerFsmComponent(*this);
	_menu = new (std::nothrow) PlayerMenuComponent(*this);
	_input = new (std::nothrow) PlayerInputComponent(*this);
	_graphics = new (std::nothrow) PlayerGraphicsComponent(*this);
	_collision = new (std::nothrow) PlayerCollisionComponent(*this);
	
	this->setTag(kTagPlayer);	
};

Player::~Player()
{
	delete _fsm;
	delete _menu;
	delete _input;
	delete _graphics;
	delete _collision;
};

void Player::update(Node* node)
{		
	_graphics->update(*node);

	_fsm->update();
	_input->update();
	_collision->update(*node);	

	if (OnGround && JumpRequest)
	{
		AppGlobal::getInstance()->EventStack.push(EGameObjectEvent::Jump);
	}
};

void Player::Up()
{
	_input->Up();
	_graphics->Up();	
};

void Player::Down()
{
	_input->Down();
	_graphics->Down();
	
};

void Player::Left()
{
	_input->Left();
	_graphics->Left();
};

void Player::Right()
{
	_input->Right(); 
	_graphics->Right();
};

void Player::Stop()
{
	_input->Stop();
	_graphics->Stop();
};

void Player::Gravity()
{
};

void Player::Idle()
{
    _graphics->Idle();
    _input->Stop();
};

void Player::Crouch()
{
    _input->Stop();
	_graphics->Crouch();
};

void Player::Jump()
{
	_graphics->Jump();
	_input->Jump();	
};

void Player::Die()
{
};

void Player::Hurt()
{
    _graphics->Hurt();
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

void Player::HitWall()
{
	_input->HitWall();
	_graphics->Hurt();
};

Rect Player::getCollisionBox()
{	
	Rect collisionBox = getBoundingBox();
	
	Vec2 diff = _desiredPosition - collisionBox.origin;

	collisionBox.origin.add(diff);
			
	return collisionBox;
};

/**
 * Moveable gameObject Variables,
 * Initializes the varaiables to their default state
 *
 */
ShowCave::ShowCave(ValueMap& properties) : super(properties)
{
};

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Left::Left(ValueMap& properties) : super(properties)
{
};

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Right::Right(ValueMap& properties) : super(properties)
{	
};

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Enter::Enter(ValueMap& properties) : super(properties)
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

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Exit::Exit(ValueMap& properties) : super(properties)
{
};
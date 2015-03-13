#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "ParallaxTileMap.h"

#include "AiComponent.h"
#include "FsmComponent.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"
#include "PathfindingComponent.h"


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


Player* Player::create(ValueMap& properties)
{
	// Create an instance of Level
	Player* gameObject = new (std::nothrow) Player(properties);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(kPlayerFileName));
		gameObject->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

Player::Player(ValueMap& properties) : super(properties)
{
	_fsm = new (std::nothrow) PlayerFsmComponent(*this);
	_menu = new (std::nothrow) PlayerMenuComponent(*this);
	_input = new (std::nothrow) PlayerInputComponent(*this);
	_graphics = new (std::nothrow) PlayerGraphicsComponent(*this);
	_collision = new (std::nothrow) PlayerCollisionComponent(*this);
    _pathfinding = new (std::nothrow) PlayerPathfindingComponent(*this);
    
	this->setTag(kTagPlayer);	

	// Player is in the stop state and it is pushed onto the event stack
	events.push(EGameObjectEvent::Stop);
};

void Player::update(Node* node)
{                
	_graphics->update(*node);

    _pathfinding->update();
    
	_fsm->update();
	_input->update();
	_collision->update(*node);	

	if (OnGround && JumpRequest)
	{
		AppGlobal::getInstance()->PlayerEvents.push(EGameObjectEvent::Jump);
	}
};

void Player::Up()
{
	AppGlobal::getInstance()->zoomOut();

	_input->Up();
	_graphics->Up();	
};

void Player::Down()
{
	AppGlobal::getInstance()->zoomOut();

	_input->Down();
	_graphics->Down();
	
};

void Player::Left()
{
	AppGlobal::getInstance()->zoomOut();

	_input->Left();
	_graphics->Left();
};

void Player::Right()
{
	AppGlobal::getInstance()->zoomOut();

	_input->Right(); 
	_graphics->Right();
};

void Player::Stop()
{
	AppGlobal::getInstance()->zoomIn();

	_input->Stop();
	_graphics->Idle();
};

void Player::Gravity()
{
};

void Player::Idle()
{
	AppGlobal::getInstance()->zoomIn();

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
	_graphics->Stop();
};

Rect Player::getCollisionBox()
{	
	Rect collisionBox = getBoundingBox();
	
	Vec2 diff = _desiredPosition - collisionBox.origin;

	collisionBox.origin.add(diff);
			
	return collisionBox;
};



Npc* Npc::create(ValueMap& properties)
{
	// Create an instance of Level
	Npc* gameObject = new (std::nothrow) Npc(properties);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(kPlayerFileName));
		gameObject->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

Npc::Npc(ValueMap& properties) : super(properties)
{
    _ai = new (std::nothrow) NpcAiComponent(*this);
	_fsm = new (std::nothrow) NpcFsmComponent(*this);
	_input = new (std::nothrow) NpcInputComponent(*this);
	_graphics = new (std::nothrow) NpcGraphicsComponent(*this);
	_collision = new (std::nothrow) NpcCollisionComponent(*this);

	this->setTag(kTagNpc);

	events.push(EGameObjectEvent::Stop);
	npcEvents.push(ENpcEvent::Decision);

	GrowFactor = random(0.00005f, 0.0003f);
	age = kBornAge;
	_sprite->setScale(kAdultAge);
};

void Npc::update(Node* node)
{
	_graphics->update(*node);

    _ai->update();
	_fsm->update();
	_input->update();
	_collision->update(*node);
};

void Npc::Up()
{
	_input->Up();
	_graphics->Up();
};

void Npc::Down()
{
	_input->Down();
	_graphics->Down();

};

void Npc::Left()
{
	_input->Left();
	_graphics->Left();
};

void Npc::Right()
{
	_input->Right();
	_graphics->Right();
};

void Npc::Stop()
{
	_input->Stop();
	_graphics->Idle();
};


void Npc::Idle()
{
	_graphics->Idle();
	_input->Stop();
};

void Npc::Talk()
{
};


Rect Npc::getCollisionBox()
{
	Rect collisionBox = getBoundingBox();

	Vec2 diff = _desiredPosition - collisionBox.origin;

	collisionBox.origin.add(diff);

	return collisionBox;
};












ShowCave* ShowCave::create(ValueMap& properties)
{
	// Create an instance of Level
	ShowCave* gameObject = new (std::nothrow) ShowCave(properties);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

ShowCave::ShowCave(ValueMap& properties) : super(properties)
{
	_collision = new (std::nothrow) ShowCaveCollisionComponent(*this);
};

ShowCave::~ShowCave()
{
	delete _collision;
};

void ShowCave::update(Node* node)
{
	_collision->update(*node);
};


ToSurface* ToSurface::create(ValueMap& properties)
{
	// Create an instance of Level
	ToSurface* gameObject = new (std::nothrow) ToSurface(properties);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

ToSurface::ToSurface(ValueMap& properties) : super(properties)
{
	this->setTag(kTagToSurface);
	_collision = new (std::nothrow) ToSurfaceCollisionComponent(*this);
};

void ToSurface::update(Node* node)
{
	_collision->update(*node);
};


ToCave* ToCave::create(ValueMap& properties)
{
	// Create an instance of Level
	ToCave* gameObject = new (std::nothrow) ToCave(properties);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

ToCave::ToCave(ValueMap& properties) : super(properties)
{	
	this->setTag(kTagToCave);
	_collision = new (std::nothrow) ToCaveCollisionComponent(*this);
};

void ToCave::update(Node* node)
{
	_collision->update(*node);
};

LandingSite* LandingSite::create(ValueMap& properties)
{
	// Create an instance of Level
	LandingSite* gameObject = new (std::nothrow) LandingSite(properties);

	if (gameObject && gameObject->init())
	{
		gameObject->autorelease();
		gameObject->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		return gameObject;
	}

	CC_SAFE_DELETE(gameObject);
	return nullptr;
};

LandingSite::LandingSite(ValueMap& properties) : super(properties)
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
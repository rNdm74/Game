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
GameObject* GameObject::create
(
	ValueMap& properties, 
	ICollisionComponent* collision, 
	IGraphicsComponent* graphics
)
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
* @param 
*/
GameObject::GameObject
(
	ValueMap& properties, 
	ICollisionComponent* collision, 
	IGraphicsComponent* graphics
)
{	
	_collision = collision;
	_graphics = graphics;
	_desiredPosition.x = properties["x"].asFloat();
	_desiredPosition.y = properties["y"].asFloat();
							
	float x = properties["x"].asFloat();
	float y = properties["y"].asFloat();
	float width = properties["width"].asFloat();
	float height = properties["height"].asFloat();

	this->setProperties(properties);
	this->setName(properties["name"].asString());
	this->setContentSize(Size(width, height));
	this->setPosition(Vec2(x, y));
};

GameObject::~GameObject()
{
	delete _collision;
	delete _graphics;
};

/**
* Updates a GameObject
*
* @param node The Node that contains the gameObject
*/
void GameObject::update(Node* node)
{
	_collision->update(*node, *this);
    _graphics->update(*node, *this);
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

/**
* Player
*/
Player* Player::create
(
	ValueMap& properties, 
	ICollisionComponent* collision, 
	IGraphicsComponent* graphics, 
	IMenuComponent* menu, 
	IInputComponent* input, 
	IFsmComponent* fsm
)
{
	// Create an instance of Level
	Player* player = new (std::nothrow) Player(properties, collision, graphics, menu, input, fsm);

	if (player && player->init())
	{
		player->autorelease();
		player->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(kPlayerFileName));
		player->getTexture()->setAliasTexParameters();
		player->getTexture()->setAntiAliasTexParameters();
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
Player::Player
(
	ValueMap& properties, 
	ICollisionComponent* collision, 
	IGraphicsComponent* graphics, 
	IMenuComponent* menu, 
	IInputComponent* input, 
	IFsmComponent* fsm
) : super(properties, collision, graphics)
{
	_fsm = fsm;
	_menu = menu;
	_input = input;
		
    _fsm->gameObject = this;
    
	this->setTag(kTagPlayer);	
};

Player::~Player()
{
	delete _fsm;
	delete _menu;
	delete _input;
};

void Player::update(Node* node)
{	
	_graphics->update(*node, *this);

	_fsm->update(*node, *this);
	_input->update(*node, *this);
	_collision->update(*node, *this);	

	
};

void Player::Up()
{
	_input->Up(*this);
	_graphics->Up(*this);	
};

void Player::Down()
{
	_input->Down(*this);
	_graphics->Down(*this);
	
};

void Player::Left()
{
	_input->Left(*this);
	_graphics->Left(*this);
};

void Player::Right()
{
	_input->Right(*this); 
	_graphics->Right(*this);
};

void Player::Stop()
{
	_input->Stop(*this);
	_graphics->Stop(*this);
};

void Player::Gravity()
{
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
	_graphics->Jump(*this);
	_input->Jump(*this);	
};

void Player::Die()
{
};

void Player::Hurt()
{
    _graphics->Hurt(*this);
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
	_input->HitWall(*this);
	_graphics->Hurt(*this);
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
ShowCave::ShowCave
(
  ValueMap& properties,
  ICollisionComponent* collision,
  IGraphicsComponent* graphics
)
: super(properties, collision, graphics)
{
};

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Left::Left
(
	ValueMap& properties,
	ICollisionComponent* collision,
	IGraphicsComponent* graphics
)
: super(properties, collision, graphics)
{
};

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Right::Right
(
ValueMap& properties,
ICollisionComponent* collision,
IGraphicsComponent* graphics
)
: super(properties, collision, graphics)
{	
};

/**
* Moveable gameObject Variables,
* Initializes the varaiables to their default state
*/
Enter::Enter
(
ValueMap& properties,
ICollisionComponent* collision,
IGraphicsComponent* graphics
)
: super(properties, collision, graphics)
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
Exit::Exit
(
ValueMap& properties,
ICollisionComponent* collision,
IGraphicsComponent* graphics
)
: super(properties, collision, graphics)
{
};
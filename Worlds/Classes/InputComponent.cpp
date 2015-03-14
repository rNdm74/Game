#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "ParallaxTileMap.h"
#include "Path.h"
#include "Utils.h"


void InputComponent::Up()
{
	//
	Vec2 velocity = _gameObject->getVelocity();
	//
	Vec2 movementStep = _movement * kUpdateInterval;
	velocity.y += movementStep.y;
	//
	_gameObject->setVelocity(velocity);
};

void InputComponent::Down()
{
	//
	Vec2 velocity = _gameObject->getVelocity();
	//
	Vec2 movementStep = _movement * kUpdateInterval;
	velocity.y -= movementStep.y;
	//
	_gameObject->setVelocity(velocity);
};

void InputComponent::Left()
{
	//
	Vec2 velocity = _gameObject->getVelocity();
	//
	Vec2 movementStep = _movement * kUpdateInterval;
	velocity.x -= movementStep.x;
	//
	_gameObject->setVelocity(velocity);
};

void InputComponent::Right()
{
	//
	Vec2 velocity = _gameObject->getVelocity();
	//
	Vec2 movementStep = _movement * kUpdateInterval;
	velocity.x += movementStep.x;
	//	
	_gameObject->setVelocity(velocity);
};

void InputComponent::Stop()
{
	Vec2 velocity = _gameObject->getVelocity();
	
	/** Dampens velocity **/
	velocity.x *= pow(0.0000001, kUpdateInterval);	

	_gameObject->setVelocity(velocity);
};

void InputComponent::Jump()
{
	Vec2 velocity = _gameObject->getVelocity();
	//
	velocity = velocity + _jumpForce;
	//
	_gameObject->setVelocity(velocity);
};

void InputComponent::ApplyGravity()
{
	Vec2 velocity = _gameObject->getVelocity();
 	//
	if (velocity.y > _jumpLimit)
	{
		velocity = Vec2(velocity.x, _jumpLimit);
	}
	//
	velocity.y += _gravity.y * kUpdateInterval;
	//
	_gameObject->setVelocity(velocity);
};

void InputComponent::HitWall()
{
	/** Immediate stop **/
	_gameObject->setVelocity(Vec2::ZERO);
};


PlayerInputComponent::PlayerInputComponent(IGameObject& gameObject) : super(gameObject)
{
	_jumpLimit = 850.0f;
	_jumpForce = Vec2(0.0f, 2800.0f);
	_gravity = Vec2(0.0, -1850.0f);
	_movement = Vec2(1800.0f, 1800.0f);
	_minMovement = Vec2(-320.0f, -850.0f);
	_maxMovement = Vec2(320.0f, 520.0f);
};

void PlayerInputComponent::update()
{
	/** Apply gravity **/
	if (_gameObject->OnGround == false && _gameObject->OnLadder == false)
		this->ApplyGravity();

	Vec2 velocity = _gameObject->getVelocity();
	//
	velocity.clamp(_minMovement, _maxMovement);
	//
	Vec2 stepVelocity = velocity * kUpdateInterval;
	//
	_gameObject->setDesiredPosition(_gameObject->getPosition() + stepVelocity);
}


NpcInputComponent::NpcInputComponent(IGameObject& gameObject) : super(gameObject)
{
	_jumpLimit = 850.0f;
	_jumpForce = Vec2(0.0f, 2800.0f);
	_gravity = Vec2(0.0, -1850.0f);

	_movement = Vec2(random(500.0f, 1000.0f), 1800.0f);
	
	float range = random(110.0f, 220.0f);

	_minMovement = Vec2(-range, -850.0f);
	_maxMovement = Vec2(range, 520.0f);
};

void NpcInputComponent::update()
{
	/** Apply gravity **/
	if (_gameObject->OnGround == false && _gameObject->OnLadder == false)
		this->ApplyGravity();

	Vec2 velocity = _gameObject->getVelocity();
	//
	velocity.clamp(_minMovement, _maxMovement);
	//
	Vec2 stepVelocity = velocity * kUpdateInterval;
	//
	_gameObject->setDesiredPosition(_gameObject->getPosition() + stepVelocity);
}
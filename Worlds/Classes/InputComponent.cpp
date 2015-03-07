#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "ParallaxTileMap.h"
#include "Path.h"
#include "Utils.h"


void InputComponent::Up()
{
};

void InputComponent::Down()
{
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
	velocity.x *= pow(0.0001, kUpdateInterval);	

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

void PlayerInputComponent::update()
{
	/** Apply gravity **/
	if (_gameObject->OnGround == false)
		this->ApplyGravity();

	Vec2 velocity = _gameObject->getVelocity();
	//
	velocity.clamp(_minMovement, _maxMovement);
	//
	Vec2 stepVelocity = velocity * kUpdateInterval;
	//
	_gameObject->setDesiredPosition(_gameObject->getPosition() + stepVelocity);
}
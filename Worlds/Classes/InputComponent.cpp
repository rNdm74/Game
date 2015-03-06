#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "ParallaxTileMap.h"
#include "Path.h"
#include "Utils.h"


void InputComponent::Up(IGameObject& gameObject)
{
};

void InputComponent::Down(IGameObject& gameObject)
{
};

void InputComponent::Left(IGameObject& gameObject)
{
	//
	Vec2 velocity = gameObject.getVelocity();
	//
	Vec2 movementStep = _movement * kUpdateInterval;
	velocity.x -= movementStep.x;
	//
	gameObject.setVelocity(velocity);	
};

void InputComponent::Right(IGameObject& gameObject)
{
	//
	Vec2 velocity = gameObject.getVelocity();
	//
	Vec2 movementStep = _movement * kUpdateInterval;
	velocity.x += movementStep.x;
	//	
	gameObject.setVelocity(velocity);
};

void InputComponent::Stop(IGameObject& gameObject)
{
	Vec2 velocity = gameObject.getVelocity();
	
	/** Dampens velocity **/
	velocity.x *= pow(0.0001, kUpdateInterval);	

	gameObject.setVelocity(velocity);
};

void InputComponent::Jump(IGameObject& gameObject)
{
	Vec2 velocity = gameObject.getVelocity();
	//
	velocity = velocity + _jumpForce;
	//
	gameObject.setVelocity(velocity);
};

void InputComponent::Gravity(IGameObject& gameObject)
{
	Vec2 velocity = gameObject.getVelocity();
 	//
	if (velocity.y > _jumpLimit)
	{
		velocity = Vec2(velocity.x, _jumpLimit);
	}
	//
	velocity.y += _gravity.y * kUpdateInterval;
	//
	gameObject.setVelocity(velocity);
};

void InputComponent::HitWall(IGameObject& gameObject)
{
	/** Immediate stop **/
	gameObject.setVelocity(Vec2::ZERO);
};

void PlayerInputComponent::update(Node& node, IGameObject& gameObject)
{
	/** Apply gravity **/
	this->Gravity(gameObject);

	Vec2 velocity = gameObject.getVelocity();	
	//
	velocity.clamp(_minMovement, _maxMovement);
	//
	Vec2 stepVelocity = velocity * kUpdateInterval;
	//
	gameObject.setDesiredPosition(gameObject.getPosition() + stepVelocity);
}
#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"

void PlayerInputComponent::update(GameObject& gameObject)
{
	auto global = AppGlobal::getInstance();

	auto body = gameObject.getBody();

	auto vel = body->GetLinearVelocity();

	float desiredVelX = 0.0f;

	float impulse = 0.0f;
	
	if (global->states.LEFT)
	{
		desiredVelX = b2Max(vel.x - kMinVelocityX, -kMaxVelocityX);

	}

	if (global->states.RIGHT)
	{
		desiredVelX = b2Min(vel.x + kMinVelocityX, kMaxVelocityX);
	}

	if (global->states.JUMP)
	{
		float impulse = body->GetMass() * 3;
		body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter(), true);
	}

	if (global->states.STOP)
	{
		desiredVelX = vel.x * kStopVelocity;
	}
	
	float velChangeX = desiredVelX - vel.x;	
	float impulseX = body->GetMass() * velChangeX;
	body->ApplyLinearImpulse(b2Vec2(impulseX, impulse), body->GetWorldCenter(), true);
	
}
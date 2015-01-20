#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"

void PlayerInputComponent::update(GameObject& gameObject)
{
	auto global = AppGlobal::getInstance();		
	auto body = gameObject.getBody();
	
	b2Vec2 velocity = body->GetLinearVelocity();
	b2Vec2 desiredVel = b2Vec2(0.0f, 0.0f);

	if (global->states.LEFT)
	{
		desiredVel.x = b2Max(velocity.x - kMinVelocityX, -kMaxVelocityX);
	}

	if (global->states.RIGHT)
	{
		desiredVel.x = b2Min(velocity.x + kMinVelocityX, kMaxVelocityX);
	}

	if (global->states.DOWN)
	{
		desiredVel.y = b2Max(velocity.y - kMinVelocityX, -kMaxVelocityX);
	}

	if (global->states.UP)
	{
		desiredVel.y = b2Min(velocity.y + kMinVelocityX, kMaxVelocityX);
	}

	if (global->states.STOP)
	{
		desiredVel.x = velocity.x * kStopVelocity;
		desiredVel.y = velocity.y * kStopVelocity;
	}
	
	b2Vec2 velChange = b2Vec2
	(
		desiredVel.x - velocity.x,
		desiredVel.y - velocity.y
	);

	b2Vec2 impulse = b2Vec2
	(
		body->GetMass() * velChange.x, //disregard time factor
		body->GetMass() * velChange.y
	);

	body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);


}
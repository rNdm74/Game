#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"

void PlayerInputComponent::update(GameObject& gameObject, float& delta)
{
	auto global = AppGlobal::getInstance();

	auto body = gameObject.getBody();

	auto vel = body->GetLinearVelocity();

	float desiredVelX = 0.0f;
	float desiredVelY = 0.0f;
		
	switch (global->state)
	{
		case STATE_LEFT : 
			desiredVelX = b2Max(vel.x - kMinVelocityX, -kMaxVelocityX);
			break;

		case STATE_STOP :  
			desiredVelX = vel.x * kStopVelocity;	
			desiredVelY = vel.y * kStopVelocity;
			break;

		case STATE_RIGHT : 
			desiredVelX = b2Min(vel.x + kMinVelocityX, kMaxVelocityX);
			break;
				
		case STATE_UP:
			desiredVelY = b2Min(vel.y + kMinVelocityX, kMaxVelocityX);
		break;

		case STATE_DOWN:
			desiredVelY = b2Max(vel.y - kMinVelocityX, -kMaxVelocityX);
		break;
	}

	float velChangeX = desiredVelX - vel.x;
	float velChangeY = desiredVelY - vel.y;
	
	float impulseX = body->GetMass() * velChangeX;
	float impulseY = body->GetMass() * velChangeY;
	
	if (global->state == STATE_LEFT || global->state == STATE_RIGHT || global->state == STATE_STOP)
	{
		if (gameObject.sensor != nullptr && global->state != STATE_STOP)
		{
			auto sensorCenter = gameObject.sensor->GetBody()->GetWorldCenter();

			float diff = body->GetWorldCenter().y - body->GetWorldPoint(b2Vec2(0, -1)).y;

			body->SetTransform(b2Vec2(body->GetPosition().x, sensorCenter.y - (diff / 2) + 0.13), 0);
		}

		body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), body->GetWorldCenter(), true);
	}

	
	
	
	if (gameObject.ladder != nullptr && gameObject.setTouchLadder)
	{
		if ((global->state == STATE_UP || global->state == STATE_DOWN))
		{

			auto ladderCenter = gameObject.ladder->GetBody()->GetWorldCenter();
			body->SetTransform(b2Vec2(ladderCenter.x, body->GetPosition().y), 0);

			body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), body->GetWorldCenter(), true);
		}
	}
	
}
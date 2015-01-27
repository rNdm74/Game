#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"

void PlayerInputComponent::update(GameObject& gameObject)
{
	auto global = AppGlobal::getInstance();

	float direction = 0;

	if (global->states.LEFT)
	{
		gameObject.move = true; 
		direction = -1;
		//gameObject.setBearing(EAST);
	}
		
	if (global->states.RIGHT)
	{
		gameObject.move = true; 
		direction = 1;
		//gameObject.setBearing(WEST);
	}
			
	if (global->states.DOWN)
		gameObject.setBearing(SOUTH);
	if (global->states.UP)
		gameObject.setBearing(NORTH);
	if (global->states.STOP)
		gameObject.move = false;//gameObject.setBearing(STOP);
	if (global->states.JUMP)
		gameObject.canJump = true;

	

	Vec2 jumpForce = Vec2(0.0, 610.0);
	float jumpCutoff = 300.0;

	if (gameObject.canJump && gameObject.onGround) 
	{
		gameObject.velocity = gameObject.velocity + jumpForce;
	}
	else if (!gameObject.canJump && gameObject.velocity.y > jumpCutoff) 
	{
		gameObject.velocity = Vec2(gameObject.velocity.x, jumpCutoff);
	}

	//
	Vec2 gravity = Vec2(0.0, -850.0);
	Vec2 gravityStep = gravity *  kUpdateInterval;
	//
	Vec2 forwardMove = Vec2(1600.0, 0.0);
	Vec2 forwardStep = forwardMove * kUpdateInterval;

	// 
	gameObject.velocity = gameObject.velocity + gravityStep;
	gameObject.velocity = Vec2(gameObject.velocity.x * 0.90, gameObject.velocity.y);

	if (gameObject.move) 
	{
		gameObject.velocity = gameObject.velocity + forwardStep * direction;
	}

	Vec2 minMovement = Vec2(-240.0, -850.0);
	Vec2 maxMovement = Vec2(240.0, 500.0);

	gameObject.velocity.clamp(minMovement, maxMovement);

	Vec2 stepVelocity = gameObject.velocity * kUpdateInterval;

	// 
	gameObject.desiredPosition = gameObject.getPosition() + stepVelocity;
}
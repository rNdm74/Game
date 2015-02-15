#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"

void PlayerInputComponent::update(GameObject& gameObject)
{
	auto global = AppGlobal::getInstance();

	Vec2 direction = Vec2::ZERO;
	
	if (global->states.LEFT && gameObject.canMoveLeft)
	{		
		gameObject.isMovingLeft = true;
		gameObject.move = true; 
		direction.x = -1.0f;
	}
		
	if (global->states.RIGHT && gameObject.canMoveRight)
	{		
		gameObject.isMovingRight = true;
		gameObject.move = true; 
		direction.x = 1.0f;
	}
		
	if (global->states.UP && gameObject.canMoveUp)
	{
		gameObject.isMovingUp = true;
		gameObject.move = true;
		direction.y = 1.0f;
	}

	if (global->states.DOWN && gameObject.canMoveDown)
	{
		gameObject.isMovingDown = true;
		gameObject.move = true;
		direction.y = -1.0f;
	}
				
	if (global->states.STOP)
	{	
		gameObject.isMovingUp = false;
		gameObject.isMovingDown = false;
		gameObject.isMovingLeft = false;
		gameObject.isMovingRight = false;
		gameObject.move = false;
        gameObject.velocity = Vec2::ZERO;
	}
	
//    if(gameObject.path.size() > -1)
//    {
//        Rect dest = gameObject.path.front();
//        gameObject.path.pop();
//    }
    
	//
	Vec2 gravity = Vec2(0.0, -850.0);
    Vec2 gravityStep = gravity *  kUpdateInterval;
	//
	Vec2 forwardMove = Vec2(1600.0, 0.0);
	Vec2 forwardStep = forwardMove * kUpdateInterval;

	Vec2 climb = Vec2(0.0, 1600.0);
	Vec2 climbStep = climb * kUpdateInterval;
		
	if (gameObject.move)
	{
		gameObject.velocity.x = gameObject.velocity.x + forwardStep.x * direction.x;
        gameObject.velocity.y = gameObject.velocity.y + climbStep.y * direction.y;
	}

	if (gameObject.gravity)
	{
		//gameObject.velocity = gameObject.velocity + gravityStep;
	}

	

	Vec2 minMovement = Vec2(-340.0, -340.0);
	Vec2 maxMovement = Vec2(340.0, 340.0);

	gameObject.velocity.clamp(minMovement, maxMovement);

	Vec2 stepVelocity = gameObject.velocity * kUpdateInterval;
	
	// 
	gameObject.desiredPosition = gameObject.getPosition() + stepVelocity;
}
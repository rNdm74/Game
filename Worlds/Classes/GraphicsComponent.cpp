#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GraphicsComponent.h"

void GraphicsComponent::runAction(std::string frameName)
{
	
};

PlayerGraphicsComponent::PlayerGraphicsComponent()
{ 	
	activeState = EAnimationStates::IDLE;
	currentFrame = 0; 
	frameTime = 0.0f; 
	idleTime = 0l;
	maxIdleTime = 50l;
			
	std::string initFrames[5][2] =
	{
		{ climbingFileName(1), climbingFileName(2) },
		{ walkingFileName(1), walkingFileName(2) },
		{ "alienBeige.png", "alienBeige.png" },
		{ "alienBeige_hurt.png", "alienBeige_hurt.png" },
		{ "alienBeige_duck.png", "alienBeige_duck.png" }
	};

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			animationFrames[i][j] = initFrames[i][j];
		}
	}			
};

void PlayerGraphicsComponent::update(IGameObject& gameObject)
{
	/** Reset the currentFrame to init frame **/
	currentFrame %= 2;
	
	/** Set the sprite frame **/
	gameObject.setSpriteFrame(frameCache(animationFrames[activeState][currentFrame]));

	/** Add delay so animation effect is realisitic **/
	if (frameTime > kFrameDelay /**  **/)
	{
		frameTime = 0.0f;
		currentFrame++;
	}

	//
	Vec2 v = gameObject.getVelocity();
	float velocityFactor = std::abs((v.x + v.y) / kFrameTimeFactor);
	//log("velocityFactor: %f", velocityFactor);

	frameTime += velocityFactor;

	if (velocityFactor < 1.0f && activeState != EAnimationStates::IDLE)
	{
		gameObject.setSpriteFrame(frameCache("alienBeige_stand.png"));
	}
}


void PlayerGraphicsComponent::ClimbUp(IGameObject& gameObject)
{
	this->activeState = EAnimationStates::CLIMBING;
	this->update(gameObject);
};

void PlayerGraphicsComponent::ClimbDown(IGameObject& gameObject)
{	
	this->activeState = EAnimationStates::CLIMBING;
	this->update(gameObject);
};

void PlayerGraphicsComponent::WalkLeft(IGameObject& gameObject)
{
	this->activeState = EAnimationStates::WALKING;
	this->update(gameObject);	

	gameObject.setFlippedX(true);
};

void PlayerGraphicsComponent::WalkRight(IGameObject& gameObject)
{
	this->activeState = EAnimationStates::WALKING;

	this->update(gameObject);	

	gameObject.setFlippedX(false);
};

void PlayerGraphicsComponent::Stop(IGameObject& gameObject)
{
	this->update(gameObject);
};

void PlayerGraphicsComponent::Idle(IGameObject &gameObject)
{
	this->activeState = EAnimationStates::IDLE;
		
	if (idleTime > maxIdleTime)
	{
		idleTime = 0l;
		maxIdleTime = random(30l, 75l);

		switch (random(0, 5))
		{
			case 0:
				this->lookLeft(gameObject);
				break;
			case 1:
				this->lookRight(gameObject);
				break;
			case 2:
				this->lookUp(gameObject);
				break;
			case 3:
				this->lookDown(gameObject);
				break;
			case 4:
				this->lookForward(gameObject);
				break;
		}
	}

	idleTime++;
};

void PlayerGraphicsComponent::Hurt(IGameObject& gameObject)
{
	this->activeState = EAnimationStates::HURT;
	this->update(gameObject);
};

void PlayerGraphicsComponent::Crouch(IGameObject& gameObject)
{
	this->activeState = EAnimationStates::CROUCH;
	this->update(gameObject);
};

/**
* Private functions
*/
void PlayerGraphicsComponent::lookLeft(IGameObject& gameObject)
{
	gameObject.setFlippedX(true);
};

void PlayerGraphicsComponent::lookRight(IGameObject& gameObject)
{
	gameObject.setFlippedX(false);
};

void PlayerGraphicsComponent::lookUp(IGameObject& gameObject)
{
	gameObject.setSpriteFrame(frameCache("alienBeige_stand.png"));
};

void PlayerGraphicsComponent::lookDown(IGameObject& gameObject)
{
	gameObject.setSpriteFrame(frameCache("alienBeige_stand.png"));
};

void PlayerGraphicsComponent::lookForward(IGameObject& gameObject)
{
	gameObject.setSpriteFrame(frameCache("alienBeige.png"));
};
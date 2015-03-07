#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "ParallaxTileMap.h"

void GraphicsComponent::update(Node& node)
{
    Rect r = _gameObject->getCollisionBox();
    static_cast<IParallaxTileMap&>(node).drawDebugRect(r, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
};

PlayerGraphicsComponent::PlayerGraphicsComponent(IGameObject& gameObject)
{ 	
	_gameObject = &gameObject;

	activeState = EAnimationStates::IDLE;
	currentFrame = 0; 
	frameTime = 0.0f; 
	idleTime = 0l;
	maxIdleTime = 50l;
			
	std::string initFrames[6][2] =
	{
		{ climbingFileName(1), climbingFileName(2) },
		{ walkingFileName(1), walkingFileName(2) },
		{ "alienBeige.png", "alienBeige.png" },
		{ "alienBeige_hurt.png", "alienBeige_hurt.png" },
		{ "alienBeige_duck.png", "alienBeige_duck.png" },
		{ "alienBeige_jump.png", "alienBeige_jump.png" }
	};

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			animationFrames[i][j] = initFrames[i][j];
		}
	}

	//_shadow = Sprite::create();
	//_shadow->setSpriteFrame(this->getSpriteFrame());
	//_shadow->setAnchorPoint(Vec2(-0.1f, 0.0f)); // position it to the center of the target node
	//_shadow->setColor(Color3B(0, 0, 0));
	//_shadow->setOpacity(50);
};

void PlayerGraphicsComponent::update(Node& node)
{
	/** Reset the currentFrame to init frame **/
	currentFrame %= 2;
	
	/** Set the sprite frame **/
	_gameObject->setSpriteFrame(frameCache(animationFrames[activeState][currentFrame]));

	/** Add delay so animation effect is realisitic **/
	if (frameTime > kFrameDelay /**  **/)
	{
		frameTime = 0.0f;
		currentFrame++;
	}

	//
	Vec2 v = _gameObject->getVelocity();
	float velocityFactor = std::abs((v.x + v.y) / kFrameTimeFactor);
	//log("velocityFactor: %f", velocityFactor);

	frameTime += velocityFactor;

	if (velocityFactor < 1.0f && activeState != EAnimationStates::IDLE)
	{
		_gameObject->setSpriteFrame(frameCache("alienBeige_stand.png"));
	}
}


void PlayerGraphicsComponent::Up()
{
	this->activeState = EAnimationStates::CLIMBING;
};

void PlayerGraphicsComponent::Down()
{	
	this->activeState = EAnimationStates::CLIMBING;
};

void PlayerGraphicsComponent::Left()
{
	this->activeState = EAnimationStates::WALKING;
	_gameObject->setFlippedX(true);
};

void PlayerGraphicsComponent::Right()
{
	this->activeState = EAnimationStates::WALKING;
	_gameObject->setFlippedX(false);
};

void PlayerGraphicsComponent::Stop()
{
};

void PlayerGraphicsComponent::Idle()
{
	this->activeState = EAnimationStates::IDLE;
		
	if (idleTime > maxIdleTime)
	{
		idleTime = 0l;
		maxIdleTime = random(30l, 75l);

		switch (random(0, 5))
		{
			case 0:
				this->lookLeft();
				break;
			case 1:
				this->lookRight();
				break;
			case 2:
				this->lookUp();
				break;
			case 3:
				this->lookDown();
				break;
			case 4:
				this->lookForward();
				break;
		}
	}

	idleTime++;
};

void PlayerGraphicsComponent::Hurt()
{
	this->activeState = EAnimationStates::HURT;
};

void PlayerGraphicsComponent::Crouch()
{
	this->activeState = EAnimationStates::CROUCH;
};

void PlayerGraphicsComponent::Jump()
{
	this->activeState = EAnimationStates::JUMP;
};

/**
* Private functions
*/
void PlayerGraphicsComponent::lookLeft()
{
	_gameObject->setFlippedX(true);
};

void PlayerGraphicsComponent::lookRight()
{
	_gameObject->setFlippedX(false);
};

void PlayerGraphicsComponent::lookUp()
{
	_gameObject->setSpriteFrame(frameCache("alienBeige_stand.png"));
};

void PlayerGraphicsComponent::lookDown()
{
	_gameObject->setSpriteFrame(frameCache("alienBeige_stand.png"));
};

void PlayerGraphicsComponent::lookForward()
{
	_gameObject->setSpriteFrame(frameCache("alienBeige.png"));
};
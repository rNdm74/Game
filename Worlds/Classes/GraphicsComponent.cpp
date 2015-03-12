#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "ParallaxTileMap.h"
#include "Utils.h"

GraphicsComponent::GraphicsComponent(IGameObject& gameObject)
{
	_gameObject = &gameObject;

	idleTime = 0l;
	maxIdleTime = 50l;
	currentFrame = 0;
	frameTime = 0.0f;
};

void GraphicsComponent::update(Node& node)
{
    Rect r = _gameObject->getCollisionBox();
    static_cast<IParallaxTileMap&>(node).drawDebugRect(r, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
};

void GraphicsComponent::updateFrame()
{

	/** Reset the currentFrame to init frame **/
	currentFrame %= 2;

	/** Set the sprite frame **/
	_gameObject->setSpriteFrame(frameCache(frames[_gameObject->events.top()][currentFrame]));

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

	if (velocityFactor < 1.0f && _gameObject->events.top() != EGameObjectEvent::Stop)
	{
		_gameObject->setSpriteFrame(frameCache(frames[StopAnimation][currentFrame]));
	}
};

void GraphicsComponent::Left()
{
	_gameObject->setFlippedX(true);
};

void GraphicsComponent::Right()
{
	_gameObject->setFlippedX(false);
};


AnimationFrames PlayerGraphicsComponent::frames = ANIMATION_FRAMES("Green");

PlayerGraphicsComponent::PlayerGraphicsComponent(IGameObject& gameObject) : super(gameObject)
{	
};


void PlayerGraphicsComponent::update(Node& node)
{
	this->updateFrame();
}

void PlayerGraphicsComponent::Idle()
{		
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
	_gameObject->setSpriteFrame(frameCache(frames[StopAnimation][currentFrame]));
};


void PlayerGraphicsComponent::lookDown()
{
	_gameObject->setSpriteFrame(frameCache(frames[StopAnimation][currentFrame]));
};


void PlayerGraphicsComponent::lookForward()
{
	_gameObject->setSpriteFrame(frameCache(frames[IdleAnimation][currentFrame]));
};



NpcGraphicsComponent::NpcGraphicsComponent(IGameObject& gameObject) : super(gameObject)
{
};
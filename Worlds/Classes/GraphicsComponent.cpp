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

Frames GraphicsComponent::getFramesForHero()
{
	Frames frames;
	std::vector<std::string> state;

	/** Climb Frames **/
	state.clear();
	for (int i = 1; i <= 6; i++) state.push_back("climb (" + std::to_string(i) + ").png");
	frames.push_back(state);
	state.clear();

	for (int i = 1; i <= 6; i++) state.push_back("climb (" + std::to_string(i) + ").png");
	frames.push_back(state);

	/** Walk Frames **/
	state.clear();
	for (int i = 1; i <= 8; i++) state.push_back("walk (" + std::to_string(i) + ").png");
	frames.push_back(state);

	state.clear();
	for (int i = 1; i <= 8; i++) state.push_back("walk (" + std::to_string(i) + ").png");
	frames.push_back(state);

	/** Stand Frames **/
	state.push_back("idle.png");
	frames.push_back(state);

	/** Jump Frames **/
	state.clear();
	for (int i = 1; i <= 3; i++) state.push_back("jump (" + std::to_string(i) + ").png");
	frames.push_back(state);

	/**  Stand Frames **/
	state.clear();
	state.push_back("idle.png");
	frames.push_back(state);	
	
	return frames;
};

Frames GraphicsComponent::getFramesFor(std::string type)
{
    return
    {
        { "alien" + type + "_climb1.png", "alien" + type + "_climb2.png" },
        { "alien" + type + "_climb1.png", "alien" + type + "_climb2.png" },
        { "alien" + type + "_walk1.png",  "alien" + type + "_walk2.png" },
        { "alien" + type + "_walk1.png",  "alien" + type + "_walk2.png" },
        { "alien" + type + "_stand.png" },
        { "alien" + type + "_jump.png" },
        { "alien" + type + ".png" }
    };
};

void GraphicsComponent::update(Node& node)
{
    Rect r = _gameObject->getCollisionBox();
#if DEBUG_ENABLE
    static_cast<IParallaxTileMap&>(node).drawDebugRect(r, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
#endif // DEBUG_ENABLE
	this->updateFrame();
};

void GraphicsComponent::updateFrame()
{
	EGameObjectEvent currentEvent = _gameObject->events.top();

	/** Reset the currentFrame to init frame **/
	currentFrame %= frames[currentEvent].size();

	/** Set the sprite frame **/
	_gameObject->setSpriteFrame(frameCache(frames[currentEvent][currentFrame]));

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

	if (velocityFactor < 1.0f && currentEvent != EGameObjectEvent::Stop)
	{
		currentFrame = 0;
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


void GraphicsComponent::Idle()
{		
	/*Sprite& sprite = _gameObject->getSprite();

	float scale = sprite.getScaleY();

	scale += 0.001f * breath;

	if (scale < 0.99f)
	{
		breath = 1;

		scale = 0.99f;
	}
	if (scale > 1.0f)
	{
		breath = -1;
		scale = 1.0f; 
	}

	sprite.setScaleY(scale);*/

	
};

void GraphicsComponent::lookLeft()
{
	_gameObject->setFlippedX(true);
};

void GraphicsComponent::lookRight()
{
	_gameObject->setFlippedX(false);
};

void GraphicsComponent::lookUp()
{
	currentFrame = 0;
	_gameObject->setSpriteFrame(frameCache(frames[StopAnimation][currentFrame]));
};

void GraphicsComponent::lookDown()
{
	currentFrame = 0;
	_gameObject->setSpriteFrame(frameCache(frames[StopAnimation][currentFrame]));
};

void GraphicsComponent::lookForward()
{
	currentFrame = 0;
	_gameObject->setSpriteFrame(frameCache(frames[IdleAnimation][currentFrame]));
};



PlayerGraphicsComponent::PlayerGraphicsComponent(IGameObject& gameObject) : super(gameObject)
{
	frames = getFramesForHero();
};

NpcGraphicsComponent::NpcGraphicsComponent(IGameObject& gameObject) : super(gameObject)
{
	int index = random(0, static_cast<int>(alienTypes.size()-1));

	frames = getFramesFor(alienTypes[index]);
};
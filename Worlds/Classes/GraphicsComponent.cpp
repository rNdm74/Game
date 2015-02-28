#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GraphicsComponent.h"

void GraphicsComponent::runAction(std::string frameName)
{
	
};

void PlayerGraphicsComponent::update(IGameObject& gameObject)
{
	
}


void PlayerGraphicsComponent::ClimbUp(IGameObject& gameObject)
{
	Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
	sprite->stopActionByTag(kTagClimbDown);
	sprite->stopActionByTag(kTagWalkLeft);
	sprite->stopActionByTag(kTagWalkRight);
	//
	auto action = sprite->getActionByTag(kTagClimbUp);
	if (action != nullptr)
		return;

	//  Create animation
	auto _anim = Animation::create();
	_anim->setDelayPerUnit(0.1f);
	//
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = _PREFIX _CLIMB + std::to_string(i) + _SUFFIX;
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		_anim->addSpriteFrame(spriteFrame);
	}
	//
	//
	Action* climb = RepeatForever::create(Animate::create(_anim));
	climb->setTag(kTagClimbUp);
	//
	sprite->runAction(climb);
};

void PlayerGraphicsComponent::ClimbDown(IGameObject& gameObject)
{
	Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
	sprite->stopActionByTag(kTagClimbUp);
	sprite->stopActionByTag(kTagWalkLeft);
	sprite->stopActionByTag(kTagWalkRight);
	//
	auto action = sprite->getActionByTag(kTagClimbDown);
	if (action != nullptr)
		return;

	//  Create animation
	auto _anim = Animation::create();
	_anim->setDelayPerUnit(0.1f);
	//
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = _PREFIX _CLIMB + std::to_string(i) + _SUFFIX;
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		_anim->addSpriteFrame(spriteFrame);
	}
	//
	//
	Action* climb = RepeatForever::create(Animate::create(_anim));
	climb->setTag(kTagClimbDown);
	//
	sprite->runAction(climb);
};

void PlayerGraphicsComponent::WalkLeft(IGameObject& gameObject)
{
    Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
    //
    
    if (frameRate > kFrameRate /**  **/)
    {
        frameRate = 0.0f;
        index++;
    }
    
    if (index > 2) index = 1;
    
    sprite->setSpriteFrame(GetSpriteFrame(getFilename(index)));
    
    //
    Vec2 v = gameObject.getVelocity();
    float velocityFactor = std::abs((v.x + v.y) / kFrameRateFactor);
    //log("velocityFactor: %f", velocityFactor);
    frameRate += velocityFactor;
    //log("frameRate: %f", frameRate);
    
    sprite->setFlippedX(true);
};

void PlayerGraphicsComponent::WalkRight(IGameObject& gameObject)
{
	Sprite* sprite = static_cast<Player&>(gameObject).getSprite();	
	//
    
    if (frameRate > kFrameRate /**  **/)
    {
        frameRate = 0.0f;
        index++;
    }
    
    if (index > 2) index = 1;
    
    sprite->setSpriteFrame(GetSpriteFrame(getFilename(index)));
    
    //
	Vec2 v = gameObject.getVelocity();
	float velocityFactor = std::abs((v.x + v.y) / kFrameRateFactor);
	//log("velocityFactor: %f", velocityFactor);
	frameRate += velocityFactor;
	//log("frameRate: %f", frameRate);

    sprite->setFlippedX(false);
};

void PlayerGraphicsComponent::Stop(IGameObject& gameObject)
{
	Sprite* sprite = static_cast<Player&>(gameObject).getSprite();

    if (frameRate > kFrameRate /**  **/)
    {
        frameRate = 0.0f;
        index++;
    }
    
    if (index > 2) index = 1;
    
    sprite->setSpriteFrame(GetSpriteFrame(getFilename(index)));
    
    //
    Vec2 v = gameObject.getVelocity();
    float velocityFactor = std::abs((v.x + v.y) / kFrameRateFactor);
    //log("velocityFactor: %f", velocityFactor);
    
    frameRate += velocityFactor;
    
    if(velocityFactor == 0.0f)
    {
        sprite->setSpriteFrame(GetSpriteFrame("alienBeige_stand.png"));
    }
};

void PlayerGraphicsComponent::Idle(IGameObject &gameObject)
{
    Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
    sprite->setSpriteFrame(GetSpriteFrame("alienBeige.png"));
};

void PlayerGraphicsComponent::Hurt(IGameObject& gameObject)
{
    Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
    sprite->setSpriteFrame(GetSpriteFrame("alienBeige_hurt.png"));
};

void PlayerGraphicsComponent::Crouch(IGameObject& gameObject)
{
    Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
    sprite->setSpriteFrame(GetSpriteFrame("alienBeige_duck.png"));
};
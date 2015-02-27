#ifndef __FranticAlien__GRAPHICS_COMPONENT_H__
#define __FranticAlien__GRAPHICS_COMPONENT_H__

#include "cocos2d.h"

#define getFilename(index) ( _PREFIX _WALK + std::to_string(index) + _SUFFIX )
#define GetSpriteFrame(frameName) ( SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName) )

class GameObject;

using namespace cocos2d;

class IGraphicsComponent
{
public:  
	//
	IGraphicsComponent(){};
	virtual ~IGraphicsComponent() {};
	virtual void update(IGameObject& gameObject) = 0;

	virtual void ClimbUp(IGameObject& gameObject) = 0;
	virtual void ClimbDown(IGameObject& gameObject) = 0;
	virtual void WalkLeft(IGameObject& gameObject) = 0;
	virtual void WalkRight(IGameObject& gameObject) = 0;
	virtual void Stop(IGameObject& gameObject) = 0;
	virtual void Hurt(IGameObject& gameObject) = 0;

protected:
	virtual void runAction(std::string frameName) = 0;

};

class GraphicsComponent : public IGraphicsComponent
{
public:
	GraphicsComponent(){};
	virtual ~GraphicsComponent() {};
	virtual void update(IGameObject& gameObject){};

	virtual void ClimbUp(IGameObject& gameObject){};
	virtual void ClimbDown(IGameObject& gameObject){};
	virtual void WalkLeft(IGameObject& gameObject){};
	virtual void WalkRight(IGameObject& gameObject){};
	virtual void Stop(IGameObject& gameObject){};
	virtual void Hurt(IGameObject& gameObject){};

protected:
	virtual void runAction(std::string frameName);
};

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
	PlayerGraphicsComponent(){ animation = nullptr;  index = 1; frameRate = 0.0f; };
	virtual ~PlayerGraphicsComponent() {};

	virtual void update(IGameObject& gameObject) override;

	virtual void ClimbUp(IGameObject& gameObject) override;
	virtual void ClimbDown(IGameObject& gameObject) override;
	virtual void WalkLeft(IGameObject& gameObject) override;
	virtual void WalkRight(IGameObject& gameObject) override;
	virtual void Stop(IGameObject& gameObject) override;
	virtual void Hurt(IGameObject& gameObject) override;

private:
	Animation* animation;
	Animate* animate;

	int index;
	float frameRate;
};

#endif /* defined(__FranticAlien__GraphicsComponent__) */

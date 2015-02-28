#ifndef __FranticAlien__GRAPHICS_COMPONENT_H__
#define __FranticAlien__GRAPHICS_COMPONENT_H__

#include "cocos2d.h"

#define climbingFileName(index) ( _PREFIX _CLIMB + std::to_string(index) + _SUFFIX )
#define walkingFileName(index) ( _PREFIX _WALK + std::to_string(index) + _SUFFIX )
#define frameCache(frameName) ( SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName) )

class GameObject;

using namespace cocos2d;

class IGraphicsComponent
{
public:  
	//
	IGraphicsComponent(){};
	virtual ~IGraphicsComponent() {};
	
	virtual void ClimbUp(IGameObject& gameObject) = 0;
	virtual void ClimbDown(IGameObject& gameObject) = 0;
	virtual void WalkLeft(IGameObject& gameObject) = 0;
	virtual void WalkRight(IGameObject& gameObject) = 0;
	virtual void Stop(IGameObject& gameObject) = 0;
    virtual void Idle(IGameObject& gameObject) = 0;
	virtual void Hurt(IGameObject& gameObject) = 0;
    virtual void Crouch(IGameObject& gameObject) = 0;

protected:
	virtual void update(IGameObject& gameObject) = 0;
	virtual void runAction(std::string frameName) = 0;

};

class GraphicsComponent : public IGraphicsComponent
{
public:
	GraphicsComponent(){};
	virtual ~GraphicsComponent() {};
	
	virtual void ClimbUp(IGameObject& gameObject){};
	virtual void ClimbDown(IGameObject& gameObject){};
	virtual void WalkLeft(IGameObject& gameObject){};
	virtual void WalkRight(IGameObject& gameObject){};
	virtual void Stop(IGameObject& gameObject){};
    virtual void Idle(IGameObject& gameObject){};
	virtual void Hurt(IGameObject& gameObject){};
    virtual void Crouch(IGameObject& gameObject){};

protected:
	virtual void update(IGameObject& gameObject){};
	virtual void runAction(std::string frameName);
};

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
	PlayerGraphicsComponent();
	virtual ~PlayerGraphicsComponent() {};
		
	virtual void ClimbUp(IGameObject& gameObject) override;
	virtual void ClimbDown(IGameObject& gameObject) override;
	virtual void WalkLeft(IGameObject& gameObject) override;
	virtual void WalkRight(IGameObject& gameObject) override;
	virtual void Stop(IGameObject& gameObject) override;
    virtual void Idle(IGameObject& gameObject) override;
	virtual void Hurt(IGameObject& gameObject) override;
    virtual void Crouch(IGameObject& gameObject) override;

private:
	virtual void update(IGameObject& gameObject) override;

	void lookLeft(IGameObject& gameObject);
	void lookRight(IGameObject& gameObject);
	void lookUp(IGameObject& gameObject);
	void lookDown(IGameObject& gameObject);
	void lookForward(IGameObject& gameObject);
	

	std::string animationFrames[5][2];
	EAnimationStates activeState;

	int currentFrame;
	float frameTime;

	long idleTime;
	long maxIdleTime;
};

#endif /* defined(__FranticAlien__GraphicsComponent__) */

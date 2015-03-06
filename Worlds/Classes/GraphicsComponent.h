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
	virtual ~IGraphicsComponent(){};
	
	virtual void update(Node& node, IGameObject& gameObject) = 0;

	virtual void Up(IGameObject& gameObject) = 0;
	virtual void Down(IGameObject& gameObject) = 0;
	virtual void Left(IGameObject& gameObject) = 0;
	virtual void Right(IGameObject& gameObject) = 0;
	virtual void Stop(IGameObject& gameObject) = 0;
    virtual void Idle(IGameObject& gameObject) = 0;
	virtual void Hurt(IGameObject& gameObject) = 0;
    virtual void Crouch(IGameObject& gameObject) = 0;	
	virtual void Jump(IGameObject& gameObject) = 0;
};

class GraphicsComponent : public IGraphicsComponent
{
public:
	GraphicsComponent(){};
	virtual ~GraphicsComponent(){};
	
	virtual void update(Node& node, IGameObject& gameObject);

	virtual void Up(IGameObject& gameObject){};
	virtual void Down(IGameObject& gameObject){};
	virtual void Left(IGameObject& gameObject){};
	virtual void Right(IGameObject& gameObject){};
	virtual void Stop(IGameObject& gameObject){};
    virtual void Idle(IGameObject& gameObject){};
	virtual void Hurt(IGameObject& gameObject){};
    virtual void Crouch(IGameObject& gameObject){};	
	virtual void Jump(IGameObject& gameObject){};
};

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
	PlayerGraphicsComponent();
	virtual ~PlayerGraphicsComponent(){};
		
	virtual void update(Node& node, IGameObject& gameObject) override;

	virtual void Up(IGameObject& gameObject) override;
	virtual void Down(IGameObject& gameObject) override;
	virtual void Left(IGameObject& gameObject) override;
	virtual void Right(IGameObject& gameObject) override;
	virtual void Stop(IGameObject& gameObject) override;
    virtual void Idle(IGameObject& gameObject) override;
	virtual void Hurt(IGameObject& gameObject) override;
    virtual void Crouch(IGameObject& gameObject) override;	
	virtual void Jump(IGameObject& gameObject) override;

private:
	void lookLeft(IGameObject& gameObject);
	void lookRight(IGameObject& gameObject);
	void lookUp(IGameObject& gameObject);
	void lookDown(IGameObject& gameObject);
	void lookForward(IGameObject& gameObject);
	
	Sprite* _shadow;

	AnimationFrames animationFrames;
	EAnimationStates activeState;

	int currentFrame;
	float frameTime;

	long idleTime;
	long maxIdleTime;
};

#endif /* defined(__FranticAlien__GraphicsComponent__) */

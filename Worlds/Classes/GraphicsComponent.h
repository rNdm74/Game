#ifndef __FranticAlien__GRAPHICS_COMPONENT_H__
#define __FranticAlien__GRAPHICS_COMPONENT_H__


#define climbingFileName(index) ( _PREFIX _CLIMB + std::to_string(index) + _SUFFIX )
#define walkingFileName(index) ( _PREFIX _WALK + std::to_string(index) + _SUFFIX )
#define frameCache(frameName) ( SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName) )

class GameObject;

class IGraphicsComponent
{
public:  
	//
	IGraphicsComponent(){};
	virtual ~IGraphicsComponent(){};
	
	virtual void update(Node& node) = 0;

	virtual void Up() = 0;
	virtual void Down() = 0;
	virtual void Left() = 0;
	virtual void Right() = 0;
	virtual void Stop() = 0;
	virtual void Jump() = 0;
    virtual void Idle() = 0;
	virtual void Hurt() = 0;
    virtual void Crouch() = 0;
};

class GraphicsComponent : public IGraphicsComponent
{
public:
	GraphicsComponent(){};
	virtual ~GraphicsComponent(){};
	
	virtual void update(Node& node);

	virtual void Up(){};
	virtual void Down(){};
	virtual void Left(){};
	virtual void Right(){};
	virtual void Stop(){};
    virtual void Idle(){};
	virtual void Hurt(){};
    virtual void Crouch(){};	
	virtual void Jump(){};

protected:
	IGameObject* _gameObject;
};

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
	PlayerGraphicsComponent(IGameObject& gameObject);
	virtual ~PlayerGraphicsComponent(){};
		
	virtual void update(Node& node) override;

	virtual void Up() override;
	virtual void Down() override;
	virtual void Left() override;
	virtual void Right() override;
	virtual void Stop() override;
    virtual void Idle() override;
	virtual void Hurt() override;
    virtual void Crouch() override;	
	virtual void Jump() override;

private:
	void lookLeft();
	void lookRight();
	void lookUp();
	void lookDown();
	void lookForward();
	
	Sprite* _shadow;

	AnimationFrames animationFrames;
	EAnimationStates activeState;

	int currentFrame;
	float frameTime;

	long idleTime;
	long maxIdleTime;
};

#endif /* defined(__FranticAlien__GraphicsComponent__) */

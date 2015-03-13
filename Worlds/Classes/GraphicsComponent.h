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
	virtual void updateFrame() = 0;

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
	typedef IGraphicsComponent super;
	typedef GraphicsComponent self;

public:
	GraphicsComponent(IGameObject& gameObject);
	virtual ~GraphicsComponent(){};
	
	virtual void update(Node& node);
	virtual void updateFrame();

	virtual void Up(){};
	virtual void Down(){};
	virtual void Left();
	virtual void Right();
	virtual void Stop(){};
    virtual void Idle() override;
	virtual void Hurt(){};
    virtual void Crouch(){};	
	virtual void Jump(){};

protected:
	void lookLeft();
	void lookRight();
	void lookUp();
	void lookDown();
	void lookForward();

protected: /** Variables **/
	Frames getFramesForHero();
    Frames getFramesFor(std::string type);
    
	IGameObject* _gameObject;
			
	int currentFrame;
	float frameTime;

	long idleTime;
	long maxIdleTime;

	Frames frames;

	int breath = -1;
};


class PlayerGraphicsComponent : public GraphicsComponent
{
	typedef GraphicsComponent super;
	typedef PlayerGraphicsComponent self;

public:
	PlayerGraphicsComponent(IGameObject& gameObject);
	virtual ~PlayerGraphicsComponent(){};
};


class NpcGraphicsComponent : public GraphicsComponent
{
	typedef GraphicsComponent super;
	typedef NpcGraphicsComponent self;

public:
	NpcGraphicsComponent(IGameObject& gameObject);
	virtual ~NpcGraphicsComponent(){};
};


#endif /* defined(__FranticAlien__GraphicsComponent__) */

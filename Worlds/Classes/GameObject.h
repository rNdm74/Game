#ifndef __Worlds__GAME_OBJECT_H__
#define __Worlds__GAME_OBJECT_H__

#include "cocos2d.h"
#include "Constants.h"

class IMenuComponent;
class IInputComponent;
class ICollisionComponent;
class IGraphicsComponent;
class IPath;

using namespace cocos2d;

/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class IGameObject : public Node
{
public:
	/** Constructor and the Destructor **/
	IGameObject(){};
	virtual ~IGameObject(){};			
	
	/** **/
	EGameObjectState CurrentState;

	/** Update the gameObject **/
	virtual void update(Node* node) = 0;

	/** Action methods **/
	virtual void ClimbUp() = 0;
	virtual void ClimbDown() = 0;
	virtual void WalkLeft() = 0;
	virtual void WalkRight() = 0;
	virtual void Stop() = 0;
    virtual void Idle() = 0;
    virtual void Crouch() = 0;

	/** **/
	virtual bool containsPoint(Vec2 point) = 0;

	/** Getters **/
    virtual EGameObjectState getCurrentState() = 0;
	virtual Rect getBoundingBox() = 0;
	virtual Rect getCollisionBox() = 0;	
	virtual Vec2 getCenterPosition() = 0;
	virtual bool getClimbing() = 0;
	virtual Vec2 getDesiredPosition() = 0;
	virtual Vec2 getDirection() = 0;
	virtual Vec2 getMapTransition() = 0;
	virtual bool getOnGround() = 0;
	virtual IPath* getPath() = 0;
	virtual ValueMap getProperties() = 0;
	virtual Size getSize() = 0;
	virtual Vec2 getVelocity() = 0;
	virtual CanMove getCanMove() = 0;
	virtual IsMoving getIsMoving() = 0;
    virtual bool isOnGround() = 0;

	/** Setters **/
    virtual void setCurrentState(EGameObjectState newState) = 0;
	virtual void setBoundingBox(Rect boundingBox) = 0;
	virtual void setCollisionBox(Rect collisionBox) = 0;
	virtual void setClimbing(bool climbing) = 0;
	virtual void setDesiredPosition(Vec2 desiredPosition) = 0;
	virtual void setDesiredPositionX(float x) = 0;
	virtual void setDesiredPositionY(float y) = 0;
	virtual void setDirection(Vec2 direction) = 0;
	virtual void setMapTransition(Vec2 mapTransition) = 0;
	virtual void setOnGround(bool onGround) = 0;
	virtual void setPath(IPath* path) = 0;
	virtual void setProperties(ValueMap& properties) = 0;
	virtual void setSize(Vec2 size) = 0;
	virtual void setVelocity(Vec2 velocity) = 0;
	virtual void setCanMove(CanMove canMove) = 0;
	virtual void setIsMoving(IsMoving isMoving) = 0;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class GameObject : public IGameObject
{
	typedef IGameObject super;
	typedef GameObject self;
    
public:
	/** Constructor and the Destructor **/
	GameObject(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics);
	virtual ~GameObject(){};
    
	/** **/
	EGameObjectState CurrentState;

	/** Update the gameObject **/
	virtual void update(Node* node);

	/** Action methods **/	
	virtual void ClimbUp(){};
	virtual void ClimbDown(){};
	virtual void WalkLeft(){};
	virtual void WalkRight(){};
	virtual void Stop(){};
    virtual void Idle(){};
    virtual void Crouch(){};
    
	/** **/
	virtual bool containsPoint(Vec2 point);

	/** Getters **/
    virtual EGameObjectState getCurrentState();
	virtual Rect getBoundingBox();
	virtual Rect getCollisionBox();
	virtual bool getClimbing();
	virtual Vec2 getCenterPosition();
	virtual Vec2 getDesiredPosition();
	virtual Vec2 getDirection();
	virtual Vec2 getMapTransition();
	virtual bool getOnGround();
	virtual IPath* getPath();
	virtual ValueMap getProperties();
	virtual Size getSize();
	virtual Vec2 getVelocity();
	virtual CanMove getCanMove();
	virtual IsMoving getIsMoving();
    virtual bool isOnGround();

	/** Setters **/
    virtual void setCurrentState(EGameObjectState newState);
	virtual void setBoundingBox(Rect boundingBox);
	virtual void setCollisionBox(Rect collisionBox);
	virtual void setClimbing(bool climbing);
	virtual void setDesiredPosition(Vec2 desiredPosition);
	virtual void setDesiredPositionX(float x);
	virtual void setDesiredPositionY(float y);
	virtual void setDirection(Vec2 direction);
	virtual void setMapTransition(Vec2 mapTransition);
	virtual void setOnGround(bool onGround);
	virtual void setPath(IPath* path);
	virtual void setProperties(ValueMap& properties);
	virtual void setSize(Vec2 size);
	virtual void setVelocity(Vec2 velocity);
	virtual void setCanMove(CanMove canMove);
	virtual void setIsMoving(IsMoving isMoving);

protected:	
	/** **/
	ICollisionComponent* _collision;
	/** **/
	IGraphicsComponent* _graphics;
	/** **/
	IPath* _path;
	/** **/
	Sprite* _sprite;
	/** **/
	Vec2 _desiredPosition;
	/** **/
	Vec2 _direction;
	/** **/
	Vec2 _mapTransition;	
	/** **/
	ValueMap _properties;
	/** **/
	Vec2 _velocity;	
	/** **/
	CanMove _canMove;
	/** **/
	IsMoving _isMoving;
	/** **/
	bool _onGround;
	/** **/
	bool _isClimbing;	
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Player : public GameObject
{
	typedef GameObject super;
	typedef Player self;

public:
	Player(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics, IMenuComponent* menu, IInputComponent* input);
	~Player(){};

	/** Update the gameObject overridden **/
	virtual void update(Node* node) override;

	/** Action methods overridden **/	
	virtual void ClimbUp() override;
	virtual void ClimbDown() override;
	virtual void WalkLeft() override;
	virtual void WalkRight() override;
	virtual void Stop() override;
    virtual void Idle() override;
    virtual void Crouch() override;

	/** Getters Overridden **/
	virtual Size getSize() override;
	virtual Rect getCollisionBox() override;	
	virtual Sprite* getSprite();
		
	bool IsLoaded();

private:	
	/** **/
	IMenuComponent* _menu;
	/** **/
	IInputComponent* _input;	
	/****/
	bool _isLoaded;

	class GameObjectFsm* _fsm;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Left : public GameObject
{
	typedef GameObject super;
	typedef Left self;

public:
	Left(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics);
	virtual ~Left(){};

private:
	/** **/
	
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Right : public GameObject
{
	typedef GameObject super;
	typedef Right self;

public:
	Right(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics);
	virtual ~Right(){};

private:
	/** **/
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Enter : public GameObject
{
	typedef GameObject super;
	typedef Enter self;

public:
	Enter(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics);
	virtual ~Enter(){};

private:
	/** **/
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Exit : public GameObject
{
	typedef GameObject super;
	typedef Exit self;

public:
	Exit(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics);
	virtual ~Exit(){};

private:
	/** **/
};


#endif /* defined(__Worlds__GAME_OBJECT_H__) */

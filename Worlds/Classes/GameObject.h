#ifndef __Worlds__GAME_OBJECT_H__
#define __Worlds__GAME_OBJECT_H__

#include "cocos2d.h"
#include "Constants.h"

class IMenuComponent;
class IInputComponent;
class ICollisionComponent;
class IGraphicsComponent;
class IFsmComponent;
class IPath;
class IGameObjectFsm;

using namespace cocos2d;

/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class IGameObject : public Sprite
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
	virtual void Jump() = 0;
	virtual void Die() = 0;
	virtual void Hurt() = 0;
	virtual void ThrowGem() = 0;
	virtual void PickUpGem() = 0;
	virtual void Talk() = 0;
	/** Action methods end **/

	/** **/
	virtual bool containsPoint(Vec2 point) = 0;
	virtual void updatePosition() = 0;

	/** Getters **/
#pragma region Getters
	virtual EGameObjectState getCurrentState() = 0;

	virtual IGameObjectFsm* getFsm() = 0;
	virtual IPath* getPath() = 0;

	virtual ValueMap getProperties() = 0;
		
	virtual Rect getCollisionBox() = 0;	
		
	virtual Vec2 getCenterPosition() = 0;		
	virtual Vec2 getDesiredPosition() = 0;
	virtual Vec2 getDirection() = 0; 
	virtual Vec2 getVelocity() = 0;
				
	virtual bool getClimbing() = 0;
	virtual bool getOnGround() = 0;

#pragma endregion Getters

	/** Setters **/
#pragma region Setters

	virtual void setCurrentState(EGameObjectState newState) = 0;

	virtual void setPath(IPath* path) = 0;

	virtual void setProperties(ValueMap& properties) = 0;
	
	virtual void setDirection(Vec2 direction) = 0;
	
	virtual void setVelocity(Vec2 velocity) = 0;
		
	virtual void setDesiredPosition(Vec2 desiredPosition) = 0;
	virtual void setDesiredPositionX(float x) = 0;
	virtual void setDesiredPositionY(float y) = 0;
	
	virtual void setClimbing(bool climbing) = 0;
	virtual void setOnGround(bool onGround) = 0;

#pragma endregion Setters

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
	static GameObject* create(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics);

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
	virtual void Jump(){};
	virtual void Die(){};
	virtual void Hurt(){};
	virtual void ThrowGem(){};
	virtual void PickUpGem(){};
	virtual void Talk(){};
    
	/** **/
	virtual bool containsPoint(Vec2 point);
	virtual void updatePosition();

	/** Getters **/
#pragma region Getters 
	virtual EGameObjectState getCurrentState();

	virtual IGameObjectFsm* getFsm();
	virtual IPath* getPath();

	virtual ValueMap getProperties();
		
	virtual Rect getCollisionBox();
		
	virtual Vec2 getCenterPosition();
	virtual Vec2 getDesiredPosition();
	virtual Vec2 getDirection();
	virtual Vec2 getVelocity();
				
	virtual bool getClimbing();
	virtual bool getOnGround();

#pragma endregion Getters

	/** Setters **/
#pragma region Setters	
	virtual void setCurrentState(EGameObjectState newState);

	virtual void setPath(IPath* path);

	virtual void setProperties(ValueMap& properties);		
	virtual void setDirection(Vec2 direction);	
	virtual void setVelocity(Vec2 velocity);
		
	virtual void setDesiredPosition(Vec2 desiredPosition);
	virtual void setDesiredPositionX(float x);
	virtual void setDesiredPositionY(float y);
		
	virtual void setClimbing(bool climbing);
	virtual void setOnGround(bool onGround);

#pragma endregion Setters

protected:	
	/** **/
	ICollisionComponent* _collision;
	/** **/
	IGraphicsComponent* _graphics;
	/** **/
	IPath* _path;	

	/** **/
	ValueMap _properties;

	/** **/
	Vec2 _desiredPosition;
	/** **/
	Vec2 _direction;
	/** **/
	Vec2 _velocity;
			
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
	static Player* create(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics, IMenuComponent* menu, IInputComponent* input, IFsmComponent* fsm);

	Player(ValueMap& properties, ICollisionComponent* collision, IGraphicsComponent* graphics, IMenuComponent* menu, IInputComponent* input, IFsmComponent* fsm);
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
	virtual void Jump() override;
	virtual void Die() override;
	virtual void Hurt() override;
	virtual void ThrowGem() override;
	virtual void PickUpGem() override;
	virtual void Talk() override;
	
	/** Getters Overridden **/
	virtual Rect getCollisionBox() override;	
	
private:	
	/** **/
	IFsmComponent* _fsm;
	/** **/
	IMenuComponent* _menu;
	/** **/
	IInputComponent* _input;
	
	/** **/
	

	ActiveTileMap activeMap;
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

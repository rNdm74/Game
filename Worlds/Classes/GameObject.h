#ifndef __Worlds__GAME_OBJECT_H__
#define __Worlds__GAME_OBJECT_H__

#include "cocos2d.h"
#include "Constants.h"

class MenuComponent;
class InputComponent;
class CollisionComponent;
class GraphicsComponent;
class Path;

using namespace cocos2d;


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class IGameObject
{
public:
	virtual void update(Node* node) = 0;
	virtual Rect getBoundingBox() = 0;
	virtual Rect getCollisionBoundingBox() = 0;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Moveable
{
public:
	Moveable();
	virtual ~Moveable(){};

    Vec2 mapTransition;
    
	/** **/
	Path* path;
	/** **/
	bool onGround;
	/** **/
	bool canMove;
	bool canMoveUp;
	bool canMoveDown;
	bool canMoveLeft;
	bool canMoveRight;
	/** **/
	bool isMovingUp;
	bool isMovingDown;
	bool isMovingLeft;
	bool isMovingRight;
	/** **/
	bool isClimbing;
	/** **/
	bool gravity;
	/** **/
	Vec2 velocity;
	/** **/
	Vec2 desiredPosition;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class GameObjectNode : public Node
{
public:
	// If applicable, then Constructors and the Destructor
	GameObjectNode(ValueMap& properties);
	virtual ~GameObjectNode(){};

	// The gets and sets
	virtual void setProperties(ValueMap& properties);
	virtual ValueMap getProperties();

protected:
	/** **/
	ValueMap _properties;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class GameObject : public GameObjectNode, public IGameObject
{
	typedef GameObjectNode super;
	typedef GameObject self;
    
public:
	/** If applicable, then Constructors and the Destructor **/
	GameObject(ValueMap& properties);
	virtual ~GameObject(){};
    
	/** Then the init methods **/
	
	/** Then methods of the instance **/
	virtual void update(Node* node);
	virtual Rect getBoundingBox();
	virtual Rect getCollisionBoundingBox();

	/** Then the overrides **/
	virtual Size getSize();
	virtual Vec2 getCenterPosition();
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Player : public GameObject, public Moveable
{
	typedef GameObject super;
	typedef Player self;

public:
	Player(ValueMap& properties, MenuComponent* menu, InputComponent* input, CollisionComponent* collision, GraphicsComponent* graphics);
	~Player(){};

	virtual void update(Node* node) override;

	virtual Size getSize() override;
	virtual Rect getCollisionBoundingBox() override;
	
	virtual Sprite* getSprite();
	
private:
	/** **/
	Sprite* _sprite;
	/** **/
	MenuComponent* _menu;
	/** **/
	InputComponent* _input;
	/** **/
	CollisionComponent* _collision;
	/** **/
	GraphicsComponent* _graphics;		
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
	Left(ValueMap& properties, CollisionComponent* collision);
	virtual ~Left(){};

	virtual void update(Node* node) override;

private:
	/** **/
	CollisionComponent* _collision;
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
	Right(ValueMap& properties, CollisionComponent* collision);
	virtual ~Right(){};

	virtual void update(Node* node) override;

private:
	/** **/
	CollisionComponent* _collision;
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
	Enter(ValueMap& properties, CollisionComponent* collision);
	virtual ~Enter(){};

	virtual void update(Node* node) override;

private:
	/** **/
	CollisionComponent* _collision;
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
	Exit(ValueMap& properties, CollisionComponent* collision);
	virtual ~Exit(){};

	virtual void update(Node* node) override;

private:
	/** **/
	CollisionComponent* _collision;
};


#endif /* defined(__Worlds__GAME_OBJECT_H__) */

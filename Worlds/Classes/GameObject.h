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
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class MoveableNode
{
public:
	MoveableNode();
	virtual ~MoveableNode(){};

	void initMoveableNode();

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
	Vec2 direction;
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
class GameObjectNode : public Node, public IGameObject
{
public:
	// If applicable, then Constructors and the Destructor
	GameObjectNode(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics);
	virtual ~GameObjectNode(){};
	
	// The gets and sets
	virtual void setProperties(ValueMap& properties);
	virtual ValueMap getProperties();

	virtual void update(Node* node);

protected:
	/** **/
	ValueMap _properties;
	/** **/
	CollisionComponent* _collision;
	/** **/
	GraphicsComponent* _graphics;
	/** **/
	Sprite* _sprite;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class GameObject : public GameObjectNode, public MoveableNode
{
	typedef GameObjectNode super;
	typedef GameObject self;
    
public:
	/** If applicable, then Constructors and the Destructor **/
	GameObject(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics);
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
class Player : public GameObject
{
	typedef GameObject super;
	typedef Player self;

public:
	Player(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics, MenuComponent* menu, InputComponent* input);
	~Player(){};

	virtual void update(Node* node) override;

	virtual Size getSize() override;
	virtual Rect getCollisionBoundingBox() override;
	
	virtual Sprite* getSprite();
	
private:	
	/** **/
	MenuComponent* _menu;
	/** **/
	InputComponent* _input;				
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Left : public GameObjectNode
{
	typedef GameObjectNode super;
	typedef Left self;

public:
	Left(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics);
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
class Right : public GameObjectNode
{
	typedef GameObjectNode super;
	typedef Right self;

public:
	Right(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics);
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
class Enter : public GameObjectNode
{
	typedef GameObjectNode super;
	typedef Enter self;

public:
	Enter(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics);
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
class Exit : public GameObjectNode
{
	typedef GameObjectNode super;
	typedef Exit self;

public:
	Exit(ValueMap& properties, CollisionComponent* collision, GraphicsComponent* graphics);
	virtual ~Exit(){};

private:
	/** **/
};


#endif /* defined(__Worlds__GAME_OBJECT_H__) */

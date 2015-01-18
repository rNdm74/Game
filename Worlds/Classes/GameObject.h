#ifndef __Worlds__GAME_OBJECT_H__
#define __Worlds__GAME_OBJECT_H__

#include "cocos2d.h"
#include "Box2D.h"
#include "Constants.h"

class Box2dHelper;
class MenuComponent;
class InputComponent;
class PhysicsComponent;
class GraphicsComponent;

using namespace cocos2d;

class IGameObject
{
public:
	virtual void update(Node* node) = 0;
	virtual void addBodyToWorld(b2World& world) = 0;
};

/// <summary>
/// Summary for Constructor
///	
/// PRE-CONDITION:	Must provide the type of the stack
/// POST-CONDITION: The head and tail are assigned nullptr's 
/// </summary>
class GameObject : public IGameObject, public Node
{
	typedef Node super;
	typedef GameObject self;
    
public:
    // If applicable, then Constructors and the Destructor
	GameObject(ValueMap& properties);
	virtual ~GameObject(){};
    
    // Then the init methods
	
    // Then methods of the instance
	virtual void update(Node* node);
	virtual void addBodyToWorld(b2World& world);
	
    // Then the overrides

	//
	virtual void setProperties(ValueMap& properties) { _properties = properties; }

	virtual ValueMap getProperties(){ return _properties;  };
	virtual b2Body* getBody() { return _body; }

protected: 
	b2Body* _body;
	b2BodyDef _bodyDef;	
	b2FixtureDef _fixtureDef;

	Rect _rect;

private:
	ValueMap _properties;
};

/// <summary>
/// Summary for SolidPlatform
///	
/// PRE-CONDITION:	Must provide the type of the stack
/// POST-CONDITION: The head and tail are assigned nullptr's 
/// </summary>
class SolidPlatform : public GameObject
{
	typedef GameObject super;
	typedef SolidPlatform self;

public:
	SolidPlatform(ValueMap& properties);
	~SolidPlatform(){};

private:
	b2PolygonShape _shape;
};

/// <summary>
/// Summary for SolidSlope
///	
/// PRE-CONDITION:	Must provide the type of the stack
/// POST-CONDITION: The head and tail are assigned nullptr's 
/// </summary>
class SolidSlope : public GameObject
{
	typedef GameObject super;
	typedef SolidSlope self;

public:
	SolidSlope(ValueMap& properties);
	~SolidSlope(){};

private:
	b2ChainShape _shape;
};

/// <summary>
/// Summary for Player
///	
/// PRE-CONDITION:	Must provide the type of the stack
/// POST-CONDITION: The head and tail are assigned nullptr's 
/// </summary>
class Player : public GameObject
{
	typedef GameObject super;
	typedef Player self;

public:
	Player(ValueMap& properties, MenuComponent* menu, InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics);
	~Player(){};

	virtual void update(Node* node) override;

private:
	Sprite* _sprite;

	b2PolygonShape _shape;

	MenuComponent* _menu;
	InputComponent* _input;
	PhysicsComponent* _physics;
	GraphicsComponent* _graphics;	
};

#endif /* defined(__Worlds__GAME_OBJECT_H__) */

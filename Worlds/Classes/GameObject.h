#ifndef __Worlds__GAME_OBJECT_H__
#define __Worlds__GAME_OBJECT_H__

#include "cocos2d.h"
#include "Constants.h"

class MenuComponent;
class InputComponent;
class PhysicsComponent;
class GraphicsComponent;

using namespace cocos2d;

class IGameObject
{
public:
	virtual void update(Node* node) = 0;
	virtual Rect getBoundingBox() = 0;
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
	
    // Then the overrides

	//
	virtual void setProperties(ValueMap& properties) { _properties = properties; }
	virtual ValueMap getProperties(){ return _properties;  };
	virtual Vec2* getDirection() { return _direction; };
	virtual void setBearing(EBearing bearing) { _bearing = bearing; }
	virtual EBearing getBearing() { return _bearing;  };
	virtual Size getSize(){ return this->getContentSize();  };
	virtual Rect getBoundingBox(){ return this->getBoundingBox(); };

	bool onGround;
	Vec2 velocity;
	Vec2 desiredPosition;

protected: 
	Vec2* _direction;
	EBearing _bearing;

private:
	ValueMap _properties;
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
	virtual Size getSize() override { return _sprite->getContentSize(); };

	virtual Rect getBoundingBox() override { return Rect(this->getPosition().x, this->getPosition().y, _sprite->getContentSize().width, _sprite->getContentSize().height); };

private:
	Sprite* _sprite;

	MenuComponent* _menu;
	InputComponent* _input;
	PhysicsComponent* _physics;
	GraphicsComponent* _graphics;		
};

#endif /* defined(__Worlds__GAME_OBJECT_H__) */

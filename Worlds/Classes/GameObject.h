#ifndef __Worlds__GAME_OBJECT_H__
#define __Worlds__GAME_OBJECT_H__

#include "Constants.h"

class IAiComponent;
class IFsmComponent;
class IMenuComponent;
class IInputComponent;
class IGraphicsComponent;
class ICollisionComponent;
class IPathfindingComponent;

/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class IGameObject : public cocos2d::Node
{
public:
	/** Constructor and the Destructor **/
	IGameObject(){};
	virtual ~IGameObject(){};			
		
	/** Update the gameObject **/
	virtual void update(Node* node) = 0;

	/** Action methods **/
	virtual void Up() = 0;
	virtual void Down() = 0;
	virtual void Left() = 0;
	virtual void Right() = 0;
	virtual void Stop() = 0;	
	virtual void Gravity() = 0;
    virtual void Idle() = 0;
    virtual void Crouch() = 0;
	virtual void Jump() = 0;
	virtual void Die() = 0;
	virtual void Hurt() = 0;
	virtual void ThrowGem() = 0;
	virtual void PickUpGem() = 0;
	virtual void Talk() = 0;
	virtual void HitWall() = 0;
	/** Action methods end **/
	
	/** Getters **/
	virtual cocos2d::ValueMap getProperties() = 0;
	virtual cocos2d::Rect getCollisionBox() = 0;
	virtual cocos2d::Vec2 getCenterPosition() = 0;
	virtual cocos2d::Vec2 getDesiredPosition() = 0;
	virtual cocos2d::Vec2 getDirection() = 0;
	virtual cocos2d::Vec2 getVelocity() = 0;
	virtual cocos2d::Sprite& getSprite() = 0;
	/** Getters end **/

	/** Setters **/	
	virtual void setProperties(cocos2d::ValueMap& properties) = 0;
	virtual void setVelocity(cocos2d::Vec2 velocity) = 0;
	virtual void setDesiredPosition(cocos2d::Vec2 desiredPosition) = 0;
	virtual void setDesiredPositionX(float x) = 0;
	virtual void setDesiredPositionY(float y) = 0;

	virtual void setSpriteFrame(cocos2d::SpriteFrame* spriteFrame) = 0;
	virtual void setFlippedX(bool flippedX) = 0;	
	/** Setters end **/

public: /** Variables **/
	cocos2d::Vec2 LadderOrigin = cocos2d::Vec2::ZERO;

	bool OnGround = true;
	bool OnLadder = false;
	bool JumpRequest = false;

	long Stamina = 1000l;
	float GrowFactor;

	Events events;
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
	static GameObject* create(cocos2d::ValueMap& properties);

	/** Constructor and the Destructor **/
	GameObject(cocos2d::ValueMap& properties);
    virtual ~GameObject()
    {
    };

	/** Update the gameObject **/
	virtual void update(Node* node){};

	/** Action methods **/
	virtual void Up(){};
	virtual void Down(){};
	virtual void Left(){};
	virtual void Right(){};
	virtual void Stop(){};
	virtual void Gravity(){};
	virtual void Idle(){};
	virtual void Crouch(){};
	virtual void Jump(){};
	virtual void Die(){};
	virtual void Hurt(){};
	virtual void ThrowGem(){};
	virtual void PickUpGem(){};
	virtual void Talk(){};
	virtual void HitWall(){};
	/** Action methods end **/

	/** Getters **/
	virtual cocos2d::ValueMap getProperties();
	virtual cocos2d::Rect getCollisionBox();
	virtual cocos2d::Vec2 getCenterPosition();
	virtual cocos2d::Vec2 getDesiredPosition();
	virtual cocos2d::Vec2 getDirection();
	virtual cocos2d::Vec2 getVelocity();
	virtual cocos2d::Sprite& getSprite() { return *_sprite; };
	/** Getters end **/

	/** Setters **/	
	virtual void setProperties(cocos2d::ValueMap& properties);
	virtual void setDirection(cocos2d::Vec2 direction);
	virtual void setVelocity(cocos2d::Vec2 velocity);
	virtual void setDesiredPosition(cocos2d::Vec2 desiredPosition);
	virtual void setDesiredPositionX(float x);
	virtual void setDesiredPositionY(float y);
	virtual void setSpriteFrame(cocos2d::SpriteFrame* spriteFrame);
	virtual void setFlippedX(bool flippedX);
	/** Setters end **/

protected: /** **/
	cocos2d::Sprite* _sprite;
	
	cocos2d::ValueMap _properties;
	
	cocos2d::Vec2 _desiredPosition;
	cocos2d::Vec2 _direction;
	cocos2d::Vec2 _velocity;
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
	static self* create(cocos2d::ValueMap& properties);

	Player(cocos2d::ValueMap& properties);
    virtual ~Player(){};

	/** Update the gameObject overridden **/
	virtual void update(Node* node) override;

	/** Action methods overridden **/	
	virtual void Up() override;
	virtual void Down() override;
	virtual void Left() override;
	virtual void Right() override;
	virtual void Stop() override;	 
	virtual void Gravity() override;
	virtual void Idle() override;
	virtual void Crouch() override;
	virtual void Jump() override;
	virtual void Die() override;
	virtual void Hurt() override;
	virtual void ThrowGem() override;
	virtual void PickUpGem() override;
	virtual void Talk() override;
	virtual void HitWall() override;
	/** Action methods end **/

	/** Getters Overridden **/
	virtual cocos2d::Rect getCollisionBox() override;
	
private: /** **/
	IFsmComponent* _fsm;
	IMenuComponent* _menu;
	IInputComponent* _input;
	IGraphicsComponent* _graphics;
	ICollisionComponent* _collision;
    IPathfindingComponent* _pathfinding;
};


class Npc : public GameObject
{
	typedef GameObject super;
	typedef Npc self;

public:
	static self* create(cocos2d::ValueMap& properties);

	Npc(cocos2d::ValueMap& properties);
    virtual ~Npc(){};

	/** Update the gameObject overridden **/
	virtual void update(Node* node) override;

	/** Action methods overridden **/
	virtual void Up() override;
	virtual void Down() override;
	virtual void Left() override;
	virtual void Right() override;
	virtual void Stop() override;
	virtual void Idle() override;
	virtual void Talk() override;
	/** Action methods end **/

	/** Getters Overridden **/
	virtual cocos2d::Rect getCollisionBox() override;

public: /** Variables **/
	NpcEvents npcEvents;

	float age;
	

private: /** **/
    IAiComponent* _ai;
	IFsmComponent* _fsm;
	IMenuComponent* _menu;
	IInputComponent* _input;
	IGraphicsComponent* _graphics;
	ICollisionComponent* _collision;
};

/**
 * A path determined by some path finding algorithm. A series of steps from
 * the starting location to the target location. This includes a step for the
 * initial location.
 *
 * @author Adam Charlton
 */
class ShowCave : public GameObject
{
    typedef GameObject super;
    typedef ShowCave self;
    
public:
	static self* create(cocos2d::ValueMap& properties);

	ShowCave(cocos2d::ValueMap& properties);
    virtual ~ShowCave();   

	virtual void update(Node* node) override;

public: /** Variables **/
	bool DetectedPlayer = false;

private: /** Variables **/
	ICollisionComponent* _collision;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class ToSurface : public GameObject
{
	typedef GameObject super;
	typedef ToSurface self;

public:
	static self* create(cocos2d::ValueMap& properties);

	ToSurface(cocos2d::ValueMap& properties);
	virtual ~ToSurface(){};

	virtual void update(Node* node) override;

private:
	/** **/
	ICollisionComponent* _collision;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class ToCave : public GameObject
{
	typedef GameObject super;
	typedef ToCave self;

public:
	static self* create(cocos2d::ValueMap& properties);

	ToCave(cocos2d::ValueMap& properties);
	virtual ~ToCave(){};

	virtual void update(Node* node) override;

private:
	/** **/
	ICollisionComponent* _collision;
};


/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class LandingSite : public GameObject
{
	typedef GameObject super;
	typedef LandingSite self;

public:
	static self* create(cocos2d::ValueMap& properties);
	LandingSite(cocos2d::ValueMap& properties);
	virtual ~LandingSite(){};
};

#endif /* defined(__Worlds__GAME_OBJECT_H__) */

#ifndef __FranticAlien__PHYSICS_COMPONENT_H__
#define __FranticAlien__PHYSICS_COMPONENT_H__

#include "cocos2d.h"
#include "Constants.h"

using namespace cocos2d;

#define TileMap(n) ( static_cast<IParallaxTileMap&>(n) )

class IGameObject;

class ICollisionComponent
{
public:
	ICollisionComponent(){};
	virtual ~ICollisionComponent(){};
	virtual void update(Node& node, IGameObject& gameObject) = 0;        
	virtual void checkTileCollision(Node& node, IGameObject& gameObject) = 0;
	virtual void isLadderCollision(Node& node, IGameObject& gameObject) = 0;
};

class CollisionComponent : public ICollisionComponent
{
public:
	virtual void update(Node& node, IGameObject& gameObjectNode);    
	virtual void checkTileCollision(Node& node, IGameObject& gameObject){};
	virtual void isLadderCollision(Node& node, IGameObject& gameObject){};	
};

class PlayerCollisionComponent : public CollisionComponent
{
public:
	virtual void update(Node& node, IGameObject& gameObject) override;   
	virtual void checkTileCollision(Node& node, IGameObject& gameObject) override;
	virtual void isLadderCollision(Node& node, IGameObject& gameObject) override;
	
};

class ShowCaveCollisionComponent : public CollisionComponent
{
    virtual void update(Node& node, IGameObject& gameObject) override;
};

#endif /* defined(__FranticAlien__PhysicsComponent__) */

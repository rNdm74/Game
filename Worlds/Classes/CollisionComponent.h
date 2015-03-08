#ifndef __FranticAlien__PHYSICS_COMPONENT_H__
#define __FranticAlien__PHYSICS_COMPONENT_H__

#include "Constants.h"

#define TileMap(n) ( static_cast<IParallaxTileMap&>(n) )

class IGameObject;

class ICollisionComponent
{
public:
	ICollisionComponent(){};
	virtual ~ICollisionComponent(){};

	virtual void update(Node& node) = 0;        
	virtual void checkTileCollision(Node& node) = 0;
	virtual void checkLadderCollision(Node& node) = 0;

protected:
	IGameObject* _gameObject;
};

class CollisionComponent : public ICollisionComponent
{
public:
	virtual void update(Node& node);    
	virtual void checkTileCollision(Node& node){};
	virtual void checkLadderCollision(Node& node){};
};

class PlayerCollisionComponent : public CollisionComponent
{
public:
	PlayerCollisionComponent(IGameObject& gameObject){ _gameObject = &gameObject; };
	virtual ~PlayerCollisionComponent(){};

	virtual void update(Node& node) override;   
	virtual void checkTileCollision(Node& node) override;
	virtual void checkLadderCollision(Node& node) override;
	
};

class ShowCaveCollisionComponent : public CollisionComponent
{
public:
	ShowCaveCollisionComponent(IGameObject& gameObject){ _gameObject = &gameObject; };
	virtual ~ShowCaveCollisionComponent(){};

    virtual void update(Node& node) override;

};

#endif /* defined(__FranticAlien__PhysicsComponent__) */

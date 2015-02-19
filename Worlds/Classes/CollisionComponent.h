#ifndef __FranticAlien__PHYSICS_COMPONENT_H__
#define __FranticAlien__PHYSICS_COMPONENT_H__

#include "cocos2d.h"
#include "Constants.h"

using namespace cocos2d;


class IGameObject;

class ICollisionComponent
{
public:
	ICollisionComponent(){};
	virtual ~ICollisionComponent(){};
	virtual void update(Node& node, IGameObject& gameObject) = 0;

protected:
	virtual void checkCollision(Node& node, IGameObject& gameObject) = 0;
	virtual void solidTileCollision(Node& node, IGameObject& gameObject) = 0;
	virtual void ladderTileCollision(Node& node, IGameObject& gameObject) = 0;
};


class CollisionComponent : public ICollisionComponent
{
public:
	virtual void update(Node& node, IGameObject& gameObjectNode);

protected:
	virtual void checkCollision(Node& node, IGameObject& gameObject);
	virtual void solidTileCollision(Node& node, IGameObject& gameObject);
	virtual void ladderTileCollision(Node& node, IGameObject& gameObject);
};


class PlayerCollisionComponent : public CollisionComponent
{
public:
	virtual void update(Node& node, IGameObject& gameObject) override;
};

#endif /* defined(__FranticAlien__PhysicsComponent__) */

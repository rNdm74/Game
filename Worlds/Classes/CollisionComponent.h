#ifndef __FranticAlien__PHYSICS_COMPONENT_H__
#define __FranticAlien__PHYSICS_COMPONENT_H__

#include "cocos2d.h"
#include <array>

using namespace cocos2d;

class GameObject;
class Player;
class ParallaxTileMap;


class ICollisionComponent
{
public:
	virtual void update(Node& node, GameObject& gameObject) = 0;

protected:
	virtual void solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void wrap(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void nextLevel(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void pathfinding(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
};


class CollisionComponent : public ICollisionComponent
{
public:
	virtual void update(Node& node, GameObject& gameObject);

protected:
	virtual void solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject){};
	virtual void ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject){};
	virtual void wrap(ParallaxTileMap& parallaxTileMap, GameObject& gameObject){};
	virtual void nextLevel(ParallaxTileMap& parallaxTileMap, GameObject& gameObject){};
	virtual void pathfinding(ParallaxTileMap& parallaxTileMap, GameObject& gameObject){};
};


class PlayerCollisionComponent : public CollisionComponent
{
public:
	void update(Node& node, GameObject& gameObject);

private:
	void solidTileCollision(ParallaxTileMap& parallaxTileMap, Player& player);
	void ladderTileCollision(ParallaxTileMap& parallaxTileMap, Player& player);
	void wrap(ParallaxTileMap& parallaxTileMap, Player& player);
	void nextLevel(ParallaxTileMap& parallaxTileMap, Player& player);
	void pathfinding(ParallaxTileMap& parallaxTileMap, Player& player);
};

#endif /* defined(__FranticAlien__PhysicsComponent__) */

#ifndef __FranticAlien__PHYSICS_COMPONENT_H__
#define __FranticAlien__PHYSICS_COMPONENT_H__

#include "cocos2d.h"
#include <array>

using namespace cocos2d;

class GameObject;
class ParallaxTileMap;

class CollisionComponent
{
public:
	virtual ~CollisionComponent(){};
	virtual void update(Node& node, GameObject &gameObject) = 0;

protected:
	virtual void solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void wrap(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void nextLevel(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
	virtual void pathfinding(ParallaxTileMap& parallaxTileMap, GameObject& gameObject) = 0;
};

class GameObjectCollisionComponent : public CollisionComponent
{
public:
	void update(Node& node, GameObject &gameObject);

private:
	void solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject);
	void ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject);
	void wrap(ParallaxTileMap& parallaxTileMap, GameObject& gameObject);
	void nextLevel(ParallaxTileMap& parallaxTileMap, GameObject& gameObject);
	void pathfinding(ParallaxTileMap& parallaxTileMap, GameObject& gameObject);
};




















#endif /* defined(__FranticAlien__PhysicsComponent__) */

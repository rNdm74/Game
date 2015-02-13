#ifndef __FranticAlien__PHYSICS_COMPONENT_H__
#define __FranticAlien__PHYSICS_COMPONENT_H__

#include "cocos2d.h"

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


class AStarHeuristic
{
public:
    virtual float getCost(Node& map, GameObject& mover, Vec2 evaluatedTile, Vec2 targetLocation) = 0;
};


class ClosestHeuristic : public AStarHeuristic
{
public:
    float getCost(Node& map, GameObject& mover, Vec2 evaluatedTile, Vec2 targetLocation);
};


class SearchGraphNode
{
public:
    SearchGraphNode(Vec2 coordinate);
    virtual ~SearchGraphNode();
    
    int setParent(SearchGraphNode& parent);
    int compareTo(SearchGraphNode& node);
    
private:
    Vec2 coordinate;
    float cost;
    SearchGraphNode* parent;
    float heuristic;
    int depth;
};


class PathFinder
{
	virtual void findPath(GameObject& gameObject, Vec2 startLocation, Vec2 targetLocation) = 0;
};


class AStarPathFinder : public PathFinder
{
public:
	AStarPathFinder(Node& node, int maxSearchDistance, bool allowDiagMovement, ClosestHeuristic heuristic);
	virtual ~AStarPathFinder();

	void findPath(GameObject& gameObject, Vec2 startLocation, Vec2 targetLocation);
private:
	std::vector<Vec2> closed;
	std::vector<Vec2> open;

	Node node;

	ClosestHeuristic heuristic;

	int maxSearchDistance;
	bool allowDiagMovement;
};

class Path
{
public:
	Path();
	virtual ~Path();

	int getLength();
	
	Vec2 getStep(int index);
	
	float getX(int index);
	float getY(int index);
	
	void appendStep(Vec2 step);
	void prependStep(Vec2 step);

	bool contains(Vec2 step);

private:
	std::vector<Vec2> steps;
};



#endif /* defined(__FranticAlien__PhysicsComponent__) */

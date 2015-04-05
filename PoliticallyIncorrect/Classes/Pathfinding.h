#ifndef __PoliticallyIncorrect__Pathfinding_H__
#define __PoliticallyIncorrect__Pathfinding_H__

#include "cocos2d.h"

class Pathfinding
{
	typedef Pathfinding self;

public:
	Pathfinding();
	~Pathfinding();
};

class IAStarHeuristic
{
public:
	IAStarHeuristic(){};
	virtual ~IAStarHeuristic(){};
	virtual float getCost(cocos2d::Vec2 evaluatedTile, cocos2d::Vec2 targetLocation) = 0;
};

class AStarHeuristic : public IAStarHeuristic
{
public:
	virtual float getCost(cocos2d::Vec2 evaluatedTile, cocos2d::Vec2 targetLocation);
};

class ClosestHeuristic : public AStarHeuristic
{
public:
	virtual float getCost(cocos2d::Vec2 evaluatedTile, cocos2d::Vec2 targetLocation) override;
};

class IPath
{
public:
	IPath(){};
	virtual ~IPath(){};

	virtual int getLength() = 0;
	virtual cocos2d::Vec2 getStep(unsigned int index) = 0;
	virtual float getX(unsigned int index) = 0;
	virtual float getY(unsigned int index) = 0;

	virtual bool contains(Vec2 step) = 0;

	virtual cocos2d::Vec2 peek_front() = 0;
	virtual cocos2d::Vec2 peek_back() = 0;
	virtual cocos2d::Vec2 pop_front() = 0;
	virtual cocos2d::Vec2 pop_back() = 0;
	virtual void push_back(cocos2d::Vec2 step) = 0;
	virtual void push_front(cocos2d::Vec2 step) = 0;
	virtual void add(IPath* path) = 0;
};

class Path : public IPath
{
public:

	/**
	* Create an empty path
	*/
	Path();
	virtual ~Path(){}

	virtual int getLength();

	virtual cocos2d::Vec2 getStep(unsigned int index);
	virtual float getX(unsigned int index);
	virtual float getY(unsigned int index);

	virtual bool contains(Vec2 step);

	virtual cocos2d::Vec2 peek_front();
	virtual cocos2d::Vec2 peek_back();
	virtual cocos2d::Vec2 pop_front();
	virtual cocos2d::Vec2 pop_back();
	virtual void push_back(cocos2d::Vec2 step);
	virtual void push_front(cocos2d::Vec2 step);
	virtual void add(IPath* path) override;

private:
	/** The list of steps building up this path */
	std::list<cocos2d::Vec2> steps;
};


class ISearchGraphNode
{
public:
	ISearchGraphNode(){};
	virtual ~ISearchGraphNode(){};

	virtual int setParent(ISearchGraphNode* parent) = 0;
	virtual int compareTo(ISearchGraphNode* searchGraphNode) = 0;

	/**
	* Variables
	*/
	ISearchGraphNode* parent;
	cocos2d::Vec2 coordinate;
	float heuristic;
	float cost;
	int depth;
};


/**
* A single node in the search graph
*/
class SearchGraphNode : public ISearchGraphNode
{
public:
	SearchGraphNode(cocos2d::Vec2 coordinate);
	virtual ~SearchGraphNode(){};

	virtual int setParent(ISearchGraphNode* parent) override;
	virtual int compareTo(ISearchGraphNode* searchGraphNode) override;
};


class IPathFinder
{
public:
	IPathFinder(){};
	virtual ~IPathFinder(){};
	virtual IPath* findPath(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation) = 0;
};

class AStarPathFinder : public IPathFinder, public Ref
{
public:
	// 
	static AStarPathFinder* create(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement);
	static AStarPathFinder* create(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement, IAStarHeuristic* heuristic);

	AStarPathFinder(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement);
	AStarPathFinder(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement, IAStarHeuristic* heuristic);
	virtual ~AStarPathFinder(){};

	IPath* findPath(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation);
	float getMovementCost(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation);
	float getHeuristicCost(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation);

	void setMap(IParallaxTileMap* map);

protected:
	ISearchGraphNode* getFirstInOpen();
	void addToOpen(ISearchGraphNode* searchGraphNode);
	bool inOpenList(ISearchGraphNode* searchGraphNode);
	void removeFromOpen(ISearchGraphNode* searchGraphNode);
	void addToClosed(ISearchGraphNode* searchGraphNode);
	bool inClosedList(ISearchGraphNode* searchGraphNode);
	void removeFromClosed(ISearchGraphNode* searchGraphNode);

	bool isValidLocation(cocos2d::Vec2 startingLocation, cocos2d::Vec2 checkLocation);

private:
	/** The set of nodes that have been searched through */
	std::vector<ISearchGraphNode*> _closed;
	/** The set of nodes that we do not yet consider fully searched */
	std::vector<ISearchGraphNode*> _open;
	/** The complete set of nodes across the map */
	std::vector<ISearchGraphNode*> _nodes;

	/** The ParallaxTileMap being searched */
	IParallaxTileMap* _map;

	/** The maximum depth of search we're willing to accept before giving up */
	int _maxSearchDistance;
	/** True if we allow diaganol movement */
	bool _allowDiagMovement;
	/** The heuristic we're applying to determine which nodes to search first */
	IAStarHeuristic* _heuristic;
};

#endif // __PoliticallyIncorrect__Pathfinding_H__


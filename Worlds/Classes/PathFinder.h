#ifndef __com_dotdat_World__PATHFINDER_H__
#define __com_dotdat_World__PATHFINDER_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameObject;
class ParallaxTileMap;
class SearchGraphNode;
class AStarHeuristic;
class Path;

/**
* Interface
*
* @see TileBasedMap
* @author Adam Charlton
*/
class IPathFinder
{
	virtual Path* findPath(Vec2 startLocation, Vec2 targetLocation) = 0;
};

/**
* A description of an implementation that can find a path from one
* location on a tile map to another based on information provided
* by that tile map.
*
* @see TileBasedMap
* @author Adam Charlton
*/
class PathFinder : public IPathFinder
{
	virtual Path* findPath(Vec2 startLocation, Vec2 targetLocation);
};

/**
* A path finder implementation that uses the AStar heuristic based algorithm
* to determine a path.
*
* @author Adam Charlton
*/
class AStarPathFinder : public IPathFinder, public Ref
{
public:
	// 
	static AStarPathFinder* create(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement);
	static AStarPathFinder* create(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement, AStarHeuristic* heuristic);

	AStarPathFinder(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement);
	AStarPathFinder(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement, AStarHeuristic* heuristic);
	virtual ~AStarPathFinder(){}

	Path* findPath(Vec2 startLocation, Vec2 targetLocation);
	float getMovementCost(Vec2 startLocation, Vec2 targetLocation);
	float getHeuristicCost(Vec2 startLocation, Vec2 targetLocation);
		
protected:
	SearchGraphNode* getFirstInOpen();
	void addToOpen(SearchGraphNode* searchGraphNode);
	bool inOpenList(SearchGraphNode* searchGraphNode);
	void removeFromOpen(SearchGraphNode* searchGraphNode);
	void addToClosed(SearchGraphNode* searchGraphNode);
	bool inClosedList(SearchGraphNode* searchGraphNode);
	void removeFromClosed(SearchGraphNode* searchGraphNode);

	bool isValidLocation(Vec2 startingLocation, Vec2 checkLocation);
	
private:
	/** The set of nodes that have been searched through */
	std::vector<SearchGraphNode*> closed;
	/** The set of nodes that we do not yet consider fully searched */
	std::vector<SearchGraphNode*> open;
	/** The complete set of nodes across the map */
	std::vector<SearchGraphNode*> nodes;

	/** The ParallaxTileMap being searched */
	ParallaxTileMap* parallaxTileMap;
		
	/** The maximum depth of search we're willing to accept before giving up */
	int maxSearchDistance;
	/** True if we allow diaganol movement */
	bool allowDiagMovement;
	/** The heuristic we're applying to determine which nodes to search first */
	AStarHeuristic* heuristic;
};

#endif /* defined(__com_dotdat_World__PATHFINDER_H__) */

#ifndef __com_dotdat_World__PATHFINDER_H__
#define __com_dotdat_World__PATHFINDER_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameObject;
class ParallaxTileMap;
class SearchGraphNode;
class AStarHeuristic;
class IPath;

/**
* Interface
*
* @see TileBasedMap
* @author Adam Charlton
*/
class IPathFinder
{
public:
	IPathFinder(){};
	virtual ~IPathFinder(){};
	virtual IPath* findPath(Vec2 startLocation, Vec2 targetLocation) = 0;
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
	virtual ~AStarPathFinder(){};

	IPath* findPath(Vec2 startLocation, Vec2 targetLocation);
	float getMovementCost(Vec2 startLocation, Vec2 targetLocation);
	float getHeuristicCost(Vec2 startLocation, Vec2 targetLocation);

	void setParallaxTileMap(ParallaxTileMap* parallaxTileMap);
		
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
	std::vector<SearchGraphNode*> _closed;
	/** The set of nodes that we do not yet consider fully searched */
	std::vector<SearchGraphNode*> _open;
	/** The complete set of nodes across the map */
	std::vector<SearchGraphNode*> _nodes;

	/** The ParallaxTileMap being searched */
	ParallaxTileMap* _parallaxTileMap;
		
	/** The maximum depth of search we're willing to accept before giving up */
	int _maxSearchDistance;
	/** True if we allow diaganol movement */
	bool _allowDiagMovement;
	/** The heuristic we're applying to determine which nodes to search first */
	AStarHeuristic* _heuristic;
};

#endif /* defined(__com_dotdat_World__PATHFINDER_H__) */

#ifndef __com_dotdat_World__PATHFINDER_H__
#define __com_dotdat_World__PATHFINDER_H__

#include "cocos2d.h"

using namespace cocos2d;

class IPath;
class IAStarHeuristic;
class IGameObject;
class IParallaxTileMap;
class ISearchGraphNode;



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
	static AStarPathFinder* create(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement);
	static AStarPathFinder* create(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement, IAStarHeuristic* heuristic);

	AStarPathFinder(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement);
	AStarPathFinder(IParallaxTileMap* map, int maxSearchDistance, bool allowDiagMovement, IAStarHeuristic* heuristic);
	virtual ~AStarPathFinder(){};

	IPath* findPath(Vec2 startLocation, Vec2 targetLocation);
	float getMovementCost(Vec2 startLocation, Vec2 targetLocation);
	float getHeuristicCost(Vec2 startLocation, Vec2 targetLocation);

	void setMap(IParallaxTileMap* map);
		
protected:
	ISearchGraphNode* getFirstInOpen();
	void addToOpen(ISearchGraphNode* searchGraphNode);
	bool inOpenList(ISearchGraphNode* searchGraphNode);
	void removeFromOpen(ISearchGraphNode* searchGraphNode);
	void addToClosed(ISearchGraphNode* searchGraphNode);
	bool inClosedList(ISearchGraphNode* searchGraphNode);
	void removeFromClosed(ISearchGraphNode* searchGraphNode);

	bool isValidLocation(Vec2 startingLocation, Vec2 checkLocation);
	
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

#endif /* defined(__com_dotdat_World__PATHFINDER_H__) */

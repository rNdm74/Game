#ifndef __com_dotdat_World__ASTAR_HEURISTIC_H__
#define __com_dotdat_World__ASTAR_HEURISTIC_H__

#include "cocos2d.h"

using namespace cocos2d;

class ParallaxTileMap;
class GameObject;

/**
* Interface
*
* @author Adam Charlton
*/
class IAStarHeuristic
{
public:
	virtual float getCost(Vec2 evaluatedTile, Vec2 targetLocation) = 0;
};

/**
* The description of a class providing a cost for a given tile based
* on a target location and entity being moved. This heuristic controls
* what priority is placed on different tiles during the search for a path
*
* @author Adam Charlton
*/
class AStarHeuristic : public IAStarHeuristic
{
public:
	virtual float getCost(Vec2 evaluatedTile, Vec2 targetLocation){ return 0.0f; };
};

/**
* A heuristic that uses the tile that is closest to the target
* as the next best tile.
*
* @author Adam Charlton
*/
class ClosestHeuristic : public AStarHeuristic
{
public:
	float getCost(Vec2 evaluatedTile, Vec2 targetLocation);
};

#endif /* defined(__com_dotdat_World__ASTAR_HEURISTIC_H__) */

#include "AStarHeuristic.h"
#include "ParallaxTileMap.h"


/**
* Get the additional heuristic cost of the given tile. This controls the
* order in which tiles are searched while attempting to find a path to the
* target location. The lower the cost the more likely the tile will
* be searched.
*
* @param evaluatedTile The Vec2 coordinate of the tile being evaluated
* @param targetLocation The Vec2 coordinate of the target location
* @return The Manhattan Distance cost associated with the given tile
*/
float AStarHeuristic::getCost(Vec2 evaluatedTile, Vec2 targetLocation)
{
	float dx = std::abs(targetLocation.x - evaluatedTile.x);
	float dy = std::abs(targetLocation.y - evaluatedTile.y);

	return dx + dy;
}

/**
* Get the additional heuristic cost of the given tile. This controls the
* order in which tiles are searched while attempting to find a path to the
* target location. The lower the cost the more likely the tile will
* be searched.
*
* @param evaluatedTile The Vec2 coordinate of the tile being evaluated
* @param targetLocation The Vec2 coordinate of the target location
* @return The Euclidean Distance cost associated with the given tile
*/
float ClosestHeuristic::getCost(Vec2 evaluatedTile, Vec2 targetLocation)
{
	Vec2 delta = targetLocation - evaluatedTile;
	return static_cast<float>(std::sqrt((delta.x * delta.x) + (delta.y * delta.y)));
}

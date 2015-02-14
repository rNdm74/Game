#include "AStarHeuristic.h"
#include "Path.h"
#include "PathFinder.h"
#include "ParallaxTileMap.h"
#include "SearchGraphNode.h"

/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
AStarPathFinder::AStarPathFinder(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement)
{
	this->heuristic = new ClosestHeuristic();
	this->parallaxTileMap = parallaxTileMap;
	this->maxSearchDistance = maxSearchDistance;
	this->allowDiagMovement = allowDiagMovement;

	Size tileMapSize = this->parallaxTileMap->getMapSize();

	for (int row = 0; row < tileMapSize.height; ++row)
	{
		for (int col = 0; col < tileMapSize.width; ++col)
		{
			nodes.push_back(new SearchGraphNode(Vec2(col, row)));
		}
	}
}

/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
* @param heuristic The heuristic used to determine the search order of the ParallaxTileMap
*/
AStarPathFinder::AStarPathFinder(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement, AStarHeuristic* heuristic)
{
	this->heuristic = heuristic;
	this->parallaxTileMap = parallaxTileMap;
	this->maxSearchDistance = maxSearchDistance;
	this->allowDiagMovement = allowDiagMovement;

	Size tileMapSize = this->parallaxTileMap->getMapSize();

	for (int row = 0; row < tileMapSize.height; ++row)
	{
		for (int col = 0; col < tileMapSize.width; ++col)
		{
			nodes.push_back(new SearchGraphNode(Vec2(col, row)));
		}
	}
}


/**
* Find a path from the starting location provided (Vec2 startLocation) to the target
* location (Vec2 targetLocation) avoiding blockages and attempting to honour costs
* provided by the tile map.
*
* @param gameObject The GameObject that will be moving along the path. This provides
* a place to pass context information about the game entity doing the moving, e.g.
* can it fly? can it swim etc.
*
* @param startLocation The Vec2 coordinate of the start location
* @param targetLocation The Vec2 coordinate of the target location
* @return The path found from start to end, or null if no path can be found.
*/
Path* AStarPathFinder::findPath(GameObject& gameObject, Vec2 startLocation, Vec2 targetLocation)
{
	//
	Size mapSize = parallaxTileMap->getMapSize();
	int startIndex = startLocation.y * mapSize.width + startLocation.x;
	int targetIndex = targetLocation.y * mapSize.width + targetLocation.x;

	// easy first check, if the destination is blocked, we can't get there
	if (parallaxTileMap->blocked(gameObject, targetLocation)) 
	{
		return nullptr;
	}

	// initial state for A*. The closed group is empty. Only the starting

	// tile is in the open list and it'e're already there
	nodes[startIndex]->cost = 0;
	nodes[startIndex]->depth = 0;
	closed.clear();
	open.clear();
	open.push_back(nodes[startIndex]);

	//
	nodes[targetIndex]->parent = nullptr;

	// while we haven't exceeded our max search depth
	int maxDepth = 0;

	while ((maxDepth < maxSearchDistance) && (open.size() != 0)) 
	{
		// pull out the first node in our open list, this is determined to 

		// be the most likely to be the next step based on our heuristic

		SearchGraphNode* current = this->getFirstInOpen();
		if (current == nodes[targetIndex]) 
		{
			break;
		}

		removeFromOpen(current);
		addToClosed(current);

		// search through all the neighbours of the current node evaluating

		// them as next steps
		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				// not a neighbour, its the current tile

				if ((x == 0) && (y == 0))
				{
					continue;
				}

				// if we're not allowing diagonal movement then only 

				// one of x or y can be set

				if (allowDiagMovement == false)
				{
					if ((x != 0) && (y != 0))
					{
						continue;
					}
				}

				// determine the location of the neighbour and evaluate it
				Vec2 neighbourLocation = Vec2
				(
					x + current->coordinate.x,
					y + current->coordinate.y
				);

				/*float xp = x + current->coordinate.x;
				float yp = y + current->coordinate.y;*/

				if (this->isValidLocation(gameObject, startLocation, neighbourLocation)) {
					// the cost to get to this node is cost the current plus the movement

					// cost to reach this node. Note that the heursitic value is only used

					// in the sorted open list

					float nextStepCost = current->cost + this->getMovementCost(gameObject, current->coordinate, neighbourLocation);
					int neighbourIndex = neighbourLocation.y * mapSize.width + neighbourLocation.x;
					SearchGraphNode* neighbour = nodes[neighbourIndex];
					parallaxTileMap->pathFinderVisited(neighbourLocation);

					// if the new cost we've determined for this node is lower than 

					// it has been previously makes sure the node hasn'e've
					// determined that there might have been a better path to get to

					// this node so it needs to be re-evaluated

					if (nextStepCost < neighbour->cost)
					{
						if (inOpenList(neighbour))
						{
							removeFromOpen(neighbour);
						}

						if (inClosedList(neighbour))
						{
							removeFromClosed(neighbour);
						}
					}

					// if the node hasn't already been processed and discarded then

					// reset it's cost to our current cost and add it as a next possible

					// step (i.e. to the open list)

					if (inOpenList(neighbour) == false && inClosedList(neighbour) == false)
					{
						neighbour->cost = nextStepCost;
						neighbour->heuristic = this->getHeuristicCost(gameObject, neighbourLocation, targetLocation);
						maxDepth = std::max(maxDepth, neighbour->setParent(current));
						addToOpen(neighbour);
					}
				}
			}
		}
	}

	// since we'e've run out of search 
	// there was no path. Just return null

	if (nodes[targetIndex]->parent == nullptr)
	{
		return nullptr;
	}

	// At this point we've definitely found a path so we can uses the parent

	// references of the nodes to find out way from the target location back

	// to the start recording the nodes on the way.

	Path* path = new Path();
	SearchGraphNode* target = nodes[targetIndex];
	while (target != nodes[startIndex])
	{
		path->prependStep(target->coordinate);
		target = target->parent;
	}

	path->prependStep(startLocation);

	// thats it, we have our path 

	return path;
}


/**
* Get the cost to move through a given location
*
* @param gameObject The GameObject that is being moved
* @param startLocation The Vec2 coordinate of the tile whose cost is being determined
* @param targetLocation The Vec2 coordinate of the target location
* @return The cost of movement through the given tile
*/
float AStarPathFinder::getMovementCost(GameObject& gameObject, Vec2 startLocation, Vec2 targetLocation)
{
	return parallaxTileMap->getCost(gameObject, startLocation, targetLocation);
}


/**
* Get the heuristic cost for the given location. This determines in which
* order the locations are processed.
*
* @param gameObject The GameObject that is being moved
* @param startLocation The Vec2 coordinate of the tile whose cost is being determined
* @param targetLocation The Vec2 coordinate of the target location
* @return The heuristic cost assigned to the tile
*/
float AStarPathFinder::getHeuristicCost(GameObject& gameObject, Vec2 startLocation, Vec2 targetLocation)
{
	return heuristic->getCost(startLocation, targetLocation);
}


/**
* Get the first element from the open list. This is the next
* one to be searched.
*
* @return The first element in the open list
*/
SearchGraphNode* AStarPathFinder::getFirstInOpen()
{
	return open.front();
}


/**
* Add a SearchGraphNode to the open list
*
* @param searchGraphNode The SearchGraphNode to be added to the open list
*/
void AStarPathFinder::addToOpen(SearchGraphNode* searchGraphNode)
{
	open.push_back(searchGraphNode);
}


/**
* Check if a SearchGraphNode is in the open list
*
* @param searchGraphNode The SearchGraphNode to check for
* @return True if the SearchGraphNode given is in the open list
*/
bool AStarPathFinder::inOpenList(SearchGraphNode* searchGraphNode)
{
	return (std::find(open.begin(), open.end(), searchGraphNode) != open.end());
}


/**
* Remove a SearchGraphNode from the open list
*
* @param searchGraphNode The SearchGraphNode to remove from the open list
*/
void AStarPathFinder::removeFromOpen(SearchGraphNode* searchGraphNode)
{
	int index = std::find(open.begin(), open.end(), searchGraphNode) - open.begin();
	open.erase(open.begin() + index);
}


/**
* Add a SearchGraphNode to the closed list
*
* @param searchGraphNode The SearchGraphNode to add to the closed list
*/
void AStarPathFinder::addToClosed(SearchGraphNode* searchGraphNode)
{
	closed.push_back(searchGraphNode);
}


/**
* Check if the SearchGraphNode supplied is in the closed list
*
* @param searchGraphNode The SearchGraphNode to search for
* @return True if the SearchGraphNode specified is in the closed list
*/
bool AStarPathFinder::inClosedList(SearchGraphNode* searchGraphNode)
{
	return (std::find(closed.begin(), closed.end(), searchGraphNode) != closed.end());
}


/**
* Remove a node from the closed list
*
* @param searchGraphNode The SearchGraphNode to remove from the closed list
*/
void AStarPathFinder::removeFromClosed(SearchGraphNode* searchGraphNode)
{
	int index = std::find(closed.begin(), closed.end(), searchGraphNode) - closed.begin();
	closed.erase(closed.begin() + index);
}


/**
* Check if a given location is valid for the supplied mover
*
* @param gameObject The gameObject that would hold a given location
* @param startingLocation The starting Vec2 coordinate
* @param checkLocation The Vec2 coordinate of the location to check
* @return True if the location is valid for the given gameObject
*/
bool AStarPathFinder::isValidLocation(GameObject& gameObject, Vec2 startingLocation, Vec2 checkLocation)
{
	Size tileMapSize = this->parallaxTileMap->getMapSize();

	bool invalid = (checkLocation.x < 0) || (checkLocation.y < 0) || (checkLocation.x >= tileMapSize.width) || (checkLocation.y >= tileMapSize.height);

	if ((invalid == false) && ((startingLocation.x != checkLocation.x) || (startingLocation.y != checkLocation.y)))
	{
		invalid = parallaxTileMap->blocked(gameObject, checkLocation);
	}

	return !invalid;
}
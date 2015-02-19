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
AStarPathFinder* AStarPathFinder::create(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement)
{
	// Create an instance of Level
	AStarPathFinder* node = new AStarPathFinder(parallaxTileMap, maxSearchDistance, allowDiagMovement);

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
AStarPathFinder* AStarPathFinder::create(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement, AStarHeuristic* heuristic)
{
	// Create an instance of Level
	AStarPathFinder* node = new AStarPathFinder(parallaxTileMap, maxSearchDistance, allowDiagMovement, heuristic);

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
AStarPathFinder::AStarPathFinder(ParallaxTileMap* parallaxTileMap, int maxSearchDistance, bool allowDiagMovement)
{
	_heuristic = new AStarHeuristic();
	_parallaxTileMap = parallaxTileMap;
	_maxSearchDistance = maxSearchDistance;
	_allowDiagMovement = allowDiagMovement;

	Size mapSize = _parallaxTileMap->getMapSize();

	for (int i = 0; i < mapSize.width * mapSize.height; ++i)
	{
		int col = i % static_cast<int>(mapSize.width);
		int row = static_cast<int>(i / mapSize.width);
		
		_nodes.push_back(new SearchGraphNode(Vec2(col, row)));
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
	_heuristic = heuristic;
	_parallaxTileMap = parallaxTileMap;
	_maxSearchDistance = maxSearchDistance;
	_allowDiagMovement = allowDiagMovement;

	Size mapSize = _parallaxTileMap->getMapSize();

	for (int i = 0; i < mapSize.width * mapSize.height; ++i)
	{
		int col = i % static_cast<int>(mapSize.width);
		int row = static_cast<int>(i / mapSize.width);
		_nodes.push_back(new SearchGraphNode(Vec2(col, row)));
	}
}


/**
* Find a path from the starting location provided (Vec2 startLocation) to the target
* location (Vec2 targetLocation) avoiding blockages and attempting to honour costs
* provided by the tile map.
*
* @param startLocation The Vec2 coordinate of the start location
* @param targetLocation The Vec2 coordinate of the target location
* @return The path found from start to end, or null if no path can be found.
*/
IPath* AStarPathFinder::findPath(Vec2 startLocation, Vec2 targetLocation)
{
	//
	Size mapSize = _parallaxTileMap->getMapSize();
	int startIndex = startLocation.y * mapSize.width + startLocation.x;
	int targetIndex = targetLocation.y * mapSize.width + targetLocation.x;

	// easy first check, if the destination is blocked, we can't get there
	if (_parallaxTileMap->isBlocked(targetLocation) || startIndex > _nodes.size() || targetIndex > _nodes.size())
	{
		return nullptr;
	}

	// initial state for A*. The closed group is empty. Only the starting

	// tile is in the open list and it'e're already there
	_nodes[startIndex]->cost = 0;
	_nodes[startIndex]->depth = 0;
	_closed.clear();
	_open.clear();
	_open.push_back(_nodes[startIndex]);

	//
	_nodes[targetIndex]->parent = nullptr;

	// while we haven't exceeded our max search depth
	int maxDepth = 0;

	while ((maxDepth < _maxSearchDistance) && (_open.size() != 0)) 
	{
		// pull out the first node in our open list, this is determined to 

		// be the most likely to be the next step based on our heuristic

		SearchGraphNode* current = this->getFirstInOpen();
		if (current == _nodes[targetIndex])
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

				if (_allowDiagMovement == false)
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

				//
				if (this->isValidLocation(startLocation, neighbourLocation))
                {
					// the cost to get to this node is cost the current plus the movement

					// cost to reach this node. Note that the heursitic value is only used

					// in the sorted open list

					float nextStepCost = current->cost + this->getMovementCost(current->coordinate, neighbourLocation);
					int neighbourIndex = neighbourLocation.y * mapSize.width + neighbourLocation.x;
					SearchGraphNode* neighbour = _nodes[neighbourIndex];
					_parallaxTileMap->pathFinderVisited(neighbourLocation);

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
						neighbour->heuristic = this->getHeuristicCost(neighbourLocation, targetLocation);
						maxDepth = std::max(maxDepth, neighbour->setParent(current));
						addToOpen(neighbour);
					}
				}
			}
		}
	}

	// since we'e've run out of search 
	// there was no path. Just return null

	if (_nodes[targetIndex]->parent == nullptr)
	{
		return nullptr;
	}

	// At this point we've definitely found a path so we can uses the parent

	// references of the nodes to find out way from the target location back

	// to the start recording the nodes on the way.

	IPath* path = new Path();
	SearchGraphNode* target = _nodes[targetIndex];
	while (target != _nodes[startIndex])
	{
		path->push_front(target->coordinate);
		target = target->parent;
	}

	path->push_front(startLocation);

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
float AStarPathFinder::getMovementCost(Vec2 startLocation, Vec2 targetLocation)
{
	return _parallaxTileMap->getCost(startLocation, targetLocation);
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
float AStarPathFinder::getHeuristicCost(Vec2 startLocation, Vec2 targetLocation)
{
	return _heuristic->getCost(startLocation, targetLocation);
}


/**
* Get the first element from the open list. This is the next
* one to be searched.
*
* @return The first element in the open list
*/
SearchGraphNode* AStarPathFinder::getFirstInOpen()
{
	return _open.front();
}


/**
* Add a SearchGraphNode to the open list
*
* @param searchGraphNode The SearchGraphNode to be added to the open list
*/
void AStarPathFinder::addToOpen(SearchGraphNode* searchGraphNode)
{
	_open.push_back(searchGraphNode);
}


/**
* Check if a SearchGraphNode is in the open list
*
* @param searchGraphNode The SearchGraphNode to check for
* @return True if the SearchGraphNode given is in the open list
*/
bool AStarPathFinder::inOpenList(SearchGraphNode* searchGraphNode)
{
	return (std::find(_open.begin(), _open.end(), searchGraphNode) != _open.end());
}


/**
* Remove a SearchGraphNode from the open list
*
* @param searchGraphNode The SearchGraphNode to remove from the open list
*/
void AStarPathFinder::removeFromOpen(SearchGraphNode* searchGraphNode)
{
	int index = std::find(_open.begin(), _open.end(), searchGraphNode) - _open.begin();
	_open.erase(_open.begin() + index);
}


/**
* Add a SearchGraphNode to the closed list
*
* @param searchGraphNode The SearchGraphNode to add to the closed list
*/
void AStarPathFinder::addToClosed(SearchGraphNode* searchGraphNode)
{
	_closed.push_back(searchGraphNode);
}


/**
* Check if the SearchGraphNode supplied is in the closed list
*
* @param searchGraphNode The SearchGraphNode to search for
* @return True if the SearchGraphNode specified is in the closed list
*/
bool AStarPathFinder::inClosedList(SearchGraphNode* searchGraphNode)
{
	return (std::find(_closed.begin(), _closed.end(), searchGraphNode) != _closed.end());
}


/**
* Remove a node from the closed list
*
* @param searchGraphNode The SearchGraphNode to remove from the closed list
*/
void AStarPathFinder::removeFromClosed(SearchGraphNode* searchGraphNode)
{
	int index = std::find(_closed.begin(), _closed.end(), searchGraphNode) - _closed.begin();
	_closed.erase(_closed.begin() + index);
}


/**
* Check if a given location is valid for the supplied mover
*
* @param gameObject The gameObject that would hold a given location
* @param startingLocation The starting Vec2 coordinate
* @param checkLocation The Vec2 coordinate of the location to check
* @return True if the location is valid for the given gameObject
*/
bool AStarPathFinder::isValidLocation(Vec2 startingLocation, Vec2 checkLocation)
{
	Size tileMapSize = _parallaxTileMap->getMapSize();

	bool invalid = (checkLocation.x < 0) || (checkLocation.y < 0) || (checkLocation.x >= tileMapSize.width) || (checkLocation.y >= tileMapSize.height);

	if ((invalid == false) && ((startingLocation.x != checkLocation.x) || (startingLocation.y != checkLocation.y)))
	{
		invalid = _parallaxTileMap->isBlocked(checkLocation);
	}

	return !invalid;
}

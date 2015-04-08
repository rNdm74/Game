#include "Pathfinding.h"
#include "Components.h"
#include "GameObject.h"

USING_NS_CC;

Pathfinding::Pathfinding()
{
	
};


Pathfinding::~Pathfinding()
{
};

float AStarHeuristic::getCost(Vec2 evaluatedTile, Vec2 targetLocation)
{
	float dx = std::abs(targetLocation.x - evaluatedTile.x);
	float dy = std::abs(targetLocation.y - evaluatedTile.y);

	return dx + dy;
};

float ClosestHeuristic::getCost(Vec2 evaluatedTile, Vec2 targetLocation)
{
	Vec2 delta = targetLocation - evaluatedTile;
	return static_cast<float>(std::sqrt((delta.x * delta.x) + (delta.y * delta.y)));
};

Path::Path()
{
};

int Path::getLength()
{
	return steps.size();
};

Vec2 Path::getStep(unsigned int index)
{
	Vec2 step = Vec2::ZERO;

	if (steps.size() > index)
	{
		std::list<Vec2>::iterator it = std::next(steps.begin(), index);
		step = *it;
	}

	return step;
};

float Path::getX(unsigned int index)
{
	Vec2 step = Vec2::ZERO;

	if (steps.size() > index)
	{
		std::list<Vec2>::iterator it = std::next(steps.begin(), index);
		step = *it;
	}

	return step.x;
};

float Path::getY(unsigned int index)
{
	Vec2 step = Vec2::ZERO;

	if (steps.size() > index)
	{
		std::list<Vec2>::iterator it = std::next(steps.begin(), index);
		step = *it;
	}

	return step.y;
};

void Path::push_back(Vec2 step)
{
	steps.push_back(step);
};

void Path::push_front(Vec2 step)
{
	steps.push_front(step);
};

Vec2 Path::peek_front()
{
	return steps.front();
};

Vec2 Path::peek_back()
{
	return steps.back();
};

Vec2 Path::pop_front()
{
	Vec2 front = steps.front();
	steps.pop_front();
	return front;
};

Vec2 Path::pop_back()
{
	Vec2 back = steps.back();
	steps.pop_back();
	return back;
};

bool Path::contains(Vec2 step)
{
	return (std::find(steps.begin(), steps.end(), step) != steps.end());
};

void Path::add(IPath* path)
{
	if (path != nullptr)
	{
		for (int i = 0; i < path->getLength(); ++i)
		{
			this->steps.push_back(path->getStep(i));
		}
	}
};

/**
* Create a new SearchGraphNode
*
* @param coordinate The Vec2 coordinate of the SearchGraphNode
*/
SearchGraphNode::SearchGraphNode(Vec2 coordinate)
{
	this->coordinate = coordinate;
}


/**
* Set the parent of this SearchGraphNode
*
* @param parent The parent SearchGraphNode which lead us to this SearchGraphNode
* @return The depth we have no reached in searching
*/
int SearchGraphNode::setParent(ISearchGraphNode* parent)
{
	this->depth = parent->depth + 1;
	this->parent = parent;

	return depth;
}


/**
* Compare the heuristic cost between two SearchGraphNodes
*
* @param searchGraphNode The SearchGraphNode which is being compared
* @return The -1 if cost is less else 1 if costs is greater else cost is the same 0
*/
int SearchGraphNode::compareTo(ISearchGraphNode* searchGraphNode)
{
	ISearchGraphNode* o = searchGraphNode;

	float f = this->heuristic + this->cost;
	float of = o->heuristic + o->cost;

	if (f < of)
	{
		return -1;
	}
	else if (f > of)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
AStarPathFinder::AStarPathFinder(ExtendedTMXTiledMap* map, int maxSearchDistance, bool allowDiagMovement)
{
	_map = map;
	_heuristic = new AStarHeuristic();
	_maxSearchDistance = maxSearchDistance;
	_allowDiagMovement = allowDiagMovement;

	Size mapSize = _map->getMapSize();

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
AStarPathFinder::AStarPathFinder(ExtendedTMXTiledMap* map, int maxSearchDistance, bool allowDiagMovement, IAStarHeuristic* heuristic)
{
	_map = map;
	_heuristic = heuristic;
	_maxSearchDistance = maxSearchDistance;
	_allowDiagMovement = allowDiagMovement;

	Size mapSize = _map->getMapSize();

	for (int i = 0; i < mapSize.width * mapSize.height; ++i)
	{
		int col = i % static_cast<int>(mapSize.width);
		int row = static_cast<int>(i / mapSize.width);
		_nodes.push_back(new SearchGraphNode(Vec2(col, row)));
	}
}

IPath* AStarPathFinder::findPathFromPositions(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation)
{
	// Convert locations to tile coordinates
	startLocation = _map->getTileCoordFrom(startLocation);
	targetLocation = _map->getTileCoordFrom(targetLocation);

	return findPath(startLocation, targetLocation);
};

IPath* AStarPathFinder::findPathForPlayer(IGameObject* player, cocos2d::Vec2 targetLocation)
{
	Vec2 pos = _map->convertToWorldSpace(player->getPosition());
	Vec2 startLocation = _map->getTileCoordFrom(pos);
	targetLocation = _map->getTileCoordFrom(targetLocation);

	return findPath(startLocation, targetLocation);
};
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
	Size mapSize = _map->getMapSize();
	int startIndex = startLocation.y * mapSize.width + startLocation.x;
	int targetIndex = targetLocation.y * mapSize.width + targetLocation.x;

	bool isBlocked = _map->isBlocked(targetLocation);
	// easy first check, if the destination is blocked, we can't get there
	if (isBlocked || startIndex > _nodes.size() || targetIndex > _nodes.size())
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

		ISearchGraphNode* current = this->getFirstInOpen();
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
				Vec2 neighbourLocation = Vec2(x + current->coordinate.x, y + current->coordinate.y);
				
				// Is the neighbour location in the non diagonal area??
				// This is so we will not cut the corners
				
				if (_map->isTileCoordValid(neighbourLocation))
				{
					auto nonDiagonalArea = _map->getLayer("movementCost");

					if (nonDiagonalArea)
					{
						bool isInNonDiagonal = nonDiagonalArea->tileAt(neighbourLocation);

						if (isInNonDiagonal)
						{
							if ((x != 0) && (y != 0))
							{
								continue;
							}
						}
					}
				};
				
				//
				if (this->isValidLocation(startLocation, neighbourLocation))
				{	
					// the cost to get to this node is cost the current plus the movement
					// cost to reach this node. Note that the heursitic value is only used
					// in the sorted open list

					float nextStepCost = current->cost + this->getMovementCost(current->coordinate, neighbourLocation);
					int neighbourIndex = neighbourLocation.y * mapSize.width + neighbourLocation.x;
					ISearchGraphNode* neighbour = _nodes[neighbourIndex];

					// if the new cost we've determined for this node is lower than 
					// it has been previously make sure the node hasn't determined that there might have been a better path to get to
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
	ISearchGraphNode* target = _nodes[targetIndex];
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
float AStarPathFinder::getMovementCost(Vec2 startLocation, Vec2 neighbourCoordinate)
{
	return _map->getCost(startLocation, neighbourCoordinate);
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
ISearchGraphNode* AStarPathFinder::getFirstInOpen()
{
	return _open.front();
}


/**
* Add a SearchGraphNode to the open list
*
* @param searchGraphNode The SearchGraphNode to be added to the open list
*/
void AStarPathFinder::addToOpen(ISearchGraphNode* searchGraphNode)
{
	_open.push_back(searchGraphNode);
}


/**
* Check if a SearchGraphNode is in the open list
*
* @param searchGraphNode The SearchGraphNode to check for
* @return True if the SearchGraphNode given is in the open list
*/
bool AStarPathFinder::inOpenList(ISearchGraphNode* searchGraphNode)
{
	return (std::find(_open.begin(), _open.end(), searchGraphNode) != _open.end());
}


/**
* Remove a SearchGraphNode from the open list
*
* @param searchGraphNode The SearchGraphNode to remove from the open list
*/
void AStarPathFinder::removeFromOpen(ISearchGraphNode* searchGraphNode)
{
	int index = std::find(_open.begin(), _open.end(), searchGraphNode) - _open.begin();
	_open.erase(_open.begin() + index);
}


/**
* Add a SearchGraphNode to the closed list
*
* @param searchGraphNode The SearchGraphNode to add to the closed list
*/
void AStarPathFinder::addToClosed(ISearchGraphNode* searchGraphNode)
{
	_closed.push_back(searchGraphNode);
}


/**
* Check if the SearchGraphNode supplied is in the closed list
*
* @param searchGraphNode The SearchGraphNode to search for
* @return True if the SearchGraphNode specified is in the closed list
*/
bool AStarPathFinder::inClosedList(ISearchGraphNode* searchGraphNode)
{
	return (std::find(_closed.begin(), _closed.end(), searchGraphNode) != _closed.end());
}


/**
* Remove a node from the closed list
*
* @param searchGraphNode The SearchGraphNode to remove from the closed list
*/
void AStarPathFinder::removeFromClosed(ISearchGraphNode* searchGraphNode)
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
	Size tileMapSize = _map->getMapSize();

	bool invalid = (checkLocation.x < 0) || (checkLocation.y < 0) || (checkLocation.x >= tileMapSize.width) || (checkLocation.y >= tileMapSize.height);

	if ((invalid == false) && ((startingLocation.x != checkLocation.x) || (startingLocation.y != checkLocation.y)))
	{
		invalid = _map->isBlocked(checkLocation);
	}

	return !invalid;
}
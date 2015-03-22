#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"

#include "Path.h"
#include "PathFinder.h"
#include "PathfindingComponent.h"

#include "ParallaxTileMap.h"

PathfindingComponent::PathfindingComponent(IGameObject& gameObject)
{
    _planetSurface = &AppGlobal::getInstance()->PlanetInstance->getPlanetSurface();
    _gameObject = &gameObject;
    _path = nullptr;
};

void PathfindingComponent::update()
{		
	// If there is a path we can follow
	if (_path == nullptr) return;
	
	// We have arrived remove the front of the path
	if (_path->getLength() > 0 && reachedPathCoords())
	{
		_path->pop_front();
	
		_gameObject->removeMovementEvent();
		// Whats the next direction we need to move
		Vec2 direction = getDirection();
		_gameObject->addMovementEvent(direction);
	}
										
#if DEBUG_ENABLE
	for (int i = 0; i < _path->getLength(); i++)
		_planetSurface->drawDebugRectAtTile(_path->getStep(i), Color4F(1.0f, 1.0f, 0.0f, 0.5f));
#endif // DEBUG_ENABLE

	// We have finished moving there is no path anymore
	if (_path->getLength() == 0)
	{
		_path = nullptr;
	}
};

void PathfindingComponent::setPath(IPath* newPath)
{
	_path = newPath;
};

bool PathfindingComponent::isPathActive()
{
	return (_path != nullptr);
};

Vec2 PathfindingComponent::getDistance()
{
	if (_path->getLength() == 0)
		return Vec2::ZERO;

	Vec2 origin = _gameObject->getCenterPosition();

	Vec2 front = _path->peek_front();
	Vec2 pathOrigin = getPathOrigin(front);

	return (pathOrigin - origin);
};

Vec2 PathfindingComponent::getDirection()
{
	if (_path->getLength() == 0)
		return Vec2::ZERO;

	Vec2 distance = getDistance();
	Vec2 n = distance.getNormalized();

	return Vec2(std::round(n.x), std::round(n.y));
}

Vec2 PathfindingComponent::getNextDirection()
{	
	if (_path->getLength() == 0)
		return Vec2::ZERO;
		
	//Vec2 back = _path->peek_back();
	
	// We have reached the last path coord so should stop
	//if (front.equals(back))
	//	return Vec2::ZERO;

	Vec2 front = _path->peek_front();
	Vec2 next = _path->getStep(NEXT_IN_PATH);
	
	Vec2 pathOrigin = getPathOrigin(front);
	Vec2 nextPathOrigin = getPathOrigin(next);

	Vec2 nextDistance = nextPathOrigin - pathOrigin;
	Vec2 n = nextDistance.getNormalized();
	
	return Vec2(std::round(n.x), std::round(n.y));
};

Vec2 PathfindingComponent::getPathOrigin(Vec2 pathCoords)
{
	Rect pathRect = _planetSurface->getTileRectFrom(pathCoords);
	
	return Vec2(pathRect.getMidX(), pathRect.getMidY());
};

bool PathfindingComponent::reachedPathCoords()
{
	Vec2 distance = getDistance();

	bool reachedX = std::abs(distance.x) < 35.0f;
	bool reachedY = std::abs(distance.y) < 35.0f;

	return (reachedX  && reachedY);
};

PlayerPathfindingComponent::PlayerPathfindingComponent(IGameObject& gameObject) : super(gameObject)
{
};

NpcPathfindingComponent::NpcPathfindingComponent(IGameObject& gameObject) : super(gameObject)
{
};
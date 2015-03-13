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
    _pathFinder = new AStarPathFinder(_planetSurface, 100, false);
    _path = nullptr;
};

void PathfindingComponent::update()
{
    Vec2 startLocation = _gameObject->getCenterPosition();
    Vec2 targetLocation = _planetSurface->convertToNodeSpaceAR(AppGlobal::getInstance()->MovePoint);
    
    if(targetLocation != Vec2::ZERO)
    {
        startLocation.x = startLocation.x / 
        IPath* path = _pathFinder->findPath(startLocation, targetLocation);
        _path = path;
    }
};

PlayerPathfindingComponent::PlayerPathfindingComponent(IGameObject& gameObject) : super(gameObject)
{
};

NpcPathfindingComponent::NpcPathfindingComponent(IGameObject& gameObject) : super(gameObject)
{
};
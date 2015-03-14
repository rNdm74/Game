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
	// Get the selected gameobject we want to move towards
	IGameObject* selectedNpc = AppGlobal::getInstance()->SelectedNpc;
    
	// If the gameObject exists 
	if (selectedNpc != nullptr)
    {
		// Get the players position
		Vec2 startLocation = _gameObject->getCenterPosition();
		// Get the selected gameObjects position
		Vec2 targetLocation = selectedNpc->getCenterPosition();

		// Find the best path towards the selected gameObject
        IPath* path = _pathFinder->findPath(startLocation, targetLocation);
		
		// If there is a path we can follow
		if (path != nullptr)
		{		
			// Get the rectangle of the 1st path Vec2
			Rect pathRect = _planetSurface->getTileRectFrom(path->peek_front());
			
			// If the players location is inside that rect
			if (pathRect.containsPoint(startLocation))
			{	
				// Move to the next path element				
				path->pop_front();
				
				// 
				pathRect = _planetSurface->getTileRectFrom(path->peek_front());
			}

			Vec2 v1 = startLocation;
			Vec2 v2 = Vec2(pathRect.getMidX(), pathRect.getMidY());
									
			Vec2 n = Vec2(v2 - v1).getNormalized();
			Vec2 d = Vec2(std::round(n.x), std::round(n.y));
									
			if (d.x > 0)
			{
				_gameObject->addMovementEvent(EMovementEvent::Right);
			}
			else if (d.x < 0)
			{
				_gameObject->addMovementEvent(EMovementEvent::Left);
			}

#if DEBUG_ENABLE
			for (int i = 0; i < path->getLength(); i++)
				_planetSurface->drawDebugRectAtTile(path->getStep(i), Color4F(1.0f, 1.0f, 0.0f, 0.5f));
#endif // DEBUG_ENABLE

		}		
	}
	else
	{
		_gameObject->addMovementEvent(EMovementEvent::Stop);
	}
};

PlayerPathfindingComponent::PlayerPathfindingComponent(IGameObject& gameObject) : super(gameObject)
{
};

NpcPathfindingComponent::NpcPathfindingComponent(IGameObject& gameObject) : super(gameObject)
{
};
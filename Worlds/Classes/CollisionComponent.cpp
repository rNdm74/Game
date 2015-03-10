#include "AppGlobal.h"
#include "CollisionComponent.h"
#include "Constants.h"
#include "GameObject.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "Utils.h"
#include "Path.h"


/**
* Update the collision component
*
* @param node The Node that contains the gameObjects
* @param gameObject The GameObject associated with being updated
*/
void CollisionComponent::update(Node& node)
{
	
};


void ShowCaveCollisionComponent::update(Node& node)
{ 	
};

void ToCaveCollisionComponent::update(Node& node)
{
	if (AppGlobal::getInstance()->PlayerEvents.top() == EGameObjectEvent::MoveToCave)
		return;

	IParallaxTileMap& map = static_cast<IParallaxTileMap&>(node);

	Rect r1 = map.getPlayer()->getCollisionBox();
	Rect r2 = _gameObject->getCollisionBox();
	
	if (r1.origin.y < r2.origin.y)
	{
		while (AppGlobal::getInstance()->PlayerEvents.top() != EGameObjectEvent::Stop )
		{
			AppGlobal::getInstance()->PlayerEvents.pop();
		}
					
		AppGlobal::getInstance()->PlayerEvents.push(EGameObjectEvent::MoveToCave);
	}
};

void ToSurfaceCollisionComponent::update(Node& node)
{
	//if (AppGlobal::getInstance()->PlayerEvents.top() == EGameObjectEvent::MoveToSurface)
	//	return;

	IParallaxTileMap& map = static_cast<IParallaxTileMap&>(node);

	Rect r1 = map.getPlayer()->getCollisionBox();
	Rect r2 = _gameObject->getCollisionBox();

	if (r1.origin.y > r2.origin.y + r2.size.height)
	{
		while (AppGlobal::getInstance()->PlayerEvents.top() != EGameObjectEvent::Stop)
		{
			AppGlobal::getInstance()->PlayerEvents.pop();
		}

		AppGlobal::getInstance()->PlayerEvents.push(EGameObjectEvent::MoveToSurface);
	}
};


void PlayerCollisionComponent::update(Node& node)
{
	this->checkTileCollision(node);
	this->checkLadderCollision(node);
}

void PlayerCollisionComponent::checkTileCollision(Node& node)
{		
	/** Game objects desired position **/
	Vec2 desiredPosition = _gameObject->getDesiredPosition();
	/** Game objects center position **/
	Vec2 centerPosition = _gameObject->getCenterPosition();
	/** Game objects velocity **/
	Vec2 velocity = _gameObject->getVelocity();
	/** Game objects collision bounding box **/
	Rect collisionBox = _gameObject->getCollisionBox();
	
	_gameObject->OnGround = false;

	/** Get array of tiles surrounding the gameobject **/
	TileDataArray tileDataArray = TileMap(node).getCollisionDataAt(centerPosition);
		    
	for (unsigned int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
	{ 	
		Rect& tileRect = tileDataArray[tileIndex].tileRect;

#if DEBUG_ENABLE
		/** debug draw tile rect RED **/
		TileMap(node).drawDebugRect(tileRect, Color4F(1.0f, 0.3f, 0.3f, 0.5f));
#endif // DEBUG_ENABLE

		if (tileRect.intersectsRect(collisionBox) /** Intersection has occurred **/)
		{	
			/** Get the intersecting rect **/
			Rect intersection = Utils::getRectIntersection(collisionBox, tileRect);
						
			if (tileIndex == ETileGrid::BOTTOM /** Tile is below gameObject **/)
			{				
				desiredPosition.y = desiredPosition.y + intersection.size.height;
				velocity.y = 0.0f;	
				_gameObject->OnGround = true;
			}
			else if (tileIndex == ETileGrid::TOP /** Tile is above gameObject **/)
			{				
				desiredPosition.y = desiredPosition.y - intersection.size.height;
				velocity.y = 0.0f;
			}
			else if (tileIndex == ETileGrid::LEFT /** Tile is left of gameObject **/)
			{
				desiredPosition.x = desiredPosition.x + intersection.size.width;
				velocity.x = 0.0f;
				
			}
			else if (tileIndex == ETileGrid::RIGHT /** Tile is right of gameObject **/) 
			{
				desiredPosition.x = desiredPosition.x - intersection.size.width;
				velocity.x = 0.0f;
			}
			else
			{
				if (intersection.size.width > intersection.size.height)
				{
					/** Tile is diagonal, but resolving collision vertically **/
					velocity.y = 0.0f;
					
					float resolutionHeight;

					if (tileIndex > ETileGrid::TOP_RIGHT)
					{
						resolutionHeight = intersection.size.height;
						_gameObject->OnGround = true;
					}
					else
					{
						resolutionHeight = -intersection.size.height;
					}
										
					desiredPosition.y = desiredPosition.y + resolutionHeight;
				}
				else
				{
					/** Tile is diagonal, but resolving collision horizontally **/ 
					
					float resolutionWidth;
					
					if (tileIndex == ETileGrid::TOP_LEFT || tileIndex == ETileGrid::BOTTOM_LEFT)
					{
						resolutionWidth = intersection.size.width;
					}
					else
					{
						resolutionWidth = -intersection.size.width;
					}

					desiredPosition.x = desiredPosition.x + resolutionWidth;
				}
			}
		}
	}

	/** Update gameObject with updated desiredPosition and setPosition **/	
	_gameObject->setDesiredPosition(desiredPosition);
	_gameObject->setPosition(desiredPosition);
	/** Update gameObject with updated velocity and if object is on ground flag **/
	_gameObject->setVelocity(velocity);
}

void PlayerCollisionComponent::checkLadderCollision(Node& node)
{
	ParallaxTileMap& map = static_cast<ParallaxTileMap&>(node);

	TileDataArray tileDataArray = map.getLadderDataAt(_gameObject->getCenterPosition());
	
	_gameObject->OnLadder = false;

	if (tileDataArray[ETileGrid::CENTER].GID)
	{
		Rect r = tileDataArray[ETileGrid::CENTER].tileRect;
		map.drawDebugRect(r, Color4F(0.5f, 1.0f, 0.0f, 0.5f));

		_gameObject->LadderOrigin = r.origin;
		_gameObject->OnLadder = true;
	}
	if (tileDataArray[ETileGrid::BOTTOM].GID)
	{
		Rect r = tileDataArray[ETileGrid::BOTTOM].tileRect;
		map.drawDebugRect(r, Color4F(0.5f, 1.0f, 0.0f, 0.5f));

		_gameObject->LadderOrigin = r.origin;
		_gameObject->OnLadder = true;
	}
};


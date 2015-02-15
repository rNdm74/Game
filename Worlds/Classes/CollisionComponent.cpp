#include "AppGlobal.h"
#include "CollisionComponent.h"
#include "Constants.h"
#include "GameObject.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "Utils.h"
#include "Path.h"

void GameObjectCollisionComponent::update(Node& node, GameObject &gameObject)
{
	// Set flags
	gameObject.onGround = false;
	gameObject.canJump = false;
	
	//
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);

	this->pathfinding(parallaxTileMap, gameObject);

	// run checks	
	this->solidTileCollision(parallaxTileMap, gameObject);
	this->ladderTileCollision(parallaxTileMap, gameObject);
	this->wrap(parallaxTileMap, gameObject);

	gameObject.setPosition(gameObject.desiredPosition);
}


void GameObjectCollisionComponent::solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject)
{
	// get gameobjects center
	Vec2 gameObjectCenterPosition = gameObject.getCenterPosition();
	Vec2 gameObjectNewPosition = gameObject.desiredPosition;
	Vec2 gameObjectNewVelocity = gameObject.velocity;

	// get the gameobject bounding box
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();
	// debug draw gameobject rect GREEN
	parallaxTileMap.drawDebugRect(gameObjectBoundingBox, Color4F(0.3f, 1.0f, 0.3f, 0.5f));
	
	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromCollisionLayerAt(gameObjectCenterPosition);
		
	// loop through tiles array	    
	for (unsigned int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
	{
		TileData& tileData = tileDataArray[tileIndex];
		Rect& tileRect = tileData.tileRect;

		// debug draw tile rect RED
		parallaxTileMap.drawDebugRect(tileRect, Color4F(1.0f, 0.3f, 0.3f, 0.5f));
		
		// check if the tiledata is valid
		if (tileRect.intersectsRect(gameObjectBoundingBox))
		{	
			// get the intersecting rect
			Rect intersection = Utils::getRectIntersection(gameObjectBoundingBox, tileRect);

			//
			if (tileIndex == ETileGrid::BOTTOM) // tile is below gameobject
			{				
				gameObjectNewPosition.y += intersection.size.height;
				gameObjectNewVelocity.y = 0.0f;
				gameObject.onGround = true;
			}
			else if (tileIndex == ETileGrid::TOP) // top tile
			{
				gameObjectNewPosition.y += -intersection.size.height;
				gameObjectNewVelocity.y = 0.0f;
			}
			else if (tileIndex == ETileGrid::LEFT) // left tile
			{
				gameObjectNewPosition.x += intersection.size.width;
				gameObjectNewVelocity.x = 0.0f;
			}
			else if (tileIndex == ETileGrid::RIGHT) // right tile
			{
				gameObjectNewPosition.x += -intersection.size.width;
				gameObjectNewVelocity.x = 0.0f;
			}
			else
			{
				if (intersection.size.width > intersection.size.height)
				{
					//tile is diagonal, but resolving collision vertically
					gameObjectNewVelocity.y = 0.0f;

					float resolutionHeight;

					if (tileIndex > ETileGrid::TOP_RIGHT)
					{
						resolutionHeight = intersection.size.height;
						gameObject.onGround = true;
					}
					else
					{
						resolutionHeight = -intersection.size.height;
					}

					gameObjectNewPosition.y += resolutionHeight;
				}
				else
				{
					//tile is diagonal, but resolving collision horizontally                                                
					float resolutionWidth;

					if (tileIndex == ETileGrid::TOP_LEFT || tileIndex == ETileGrid::BOTTOM_LEFT)
					{
						resolutionWidth = intersection.size.width;
					}
					else
					{
						resolutionWidth = -intersection.size.width;
					}

					gameObjectNewPosition.x += resolutionWidth;
				}
			}
		}
	}

	//
	gameObject.desiredPosition = gameObjectNewPosition;
	gameObject.velocity = gameObjectNewVelocity;
}


void GameObjectCollisionComponent::ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
{			
	// Variables	
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();
	Vec2 gameObjectCenterPosition = gameObject.getCenterPosition();
	Vec2 gameObjectNewPosition = gameObject.desiredPosition;
	Vec2 gameObjectNewVelocity = gameObject.velocity;
					
	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromLadderLayerAt(gameObjectCenterPosition);
	

	// pseudo code ladder collisions

	// bool flags
	bool isLadderTop = false;
	bool isLadderMid = false;
	bool isLadderBottom = false;

	bool canMoveUp = false;
	bool canMoveDown = false;
	bool canMoveLeft = false;
	bool canMoveRight = false;

	bool isClimbing = gameObject.isClimbing;
	bool isMovingUp = gameObject.isMovingUp;
	bool isMovingDown = gameObject.isMovingDown;
	bool isMovingLeft = gameObject.isMovingLeft;
	bool isMovingRight = gameObject.isMovingRight;	

	bool gravity = gameObject.gravity;
	

	// states

	//
	if (tileDataArray[ETileGrid::LEFT].GID == false && isClimbing == false)
	{
		canMoveLeft = true;
	}

	//
	if (tileDataArray[ETileGrid::RIGHT].GID == false && isClimbing == false)
	{
		canMoveRight = true;
	}

	//
	if (tileDataArray[ETileGrid::BOTTOM_LEFT].GID && isClimbing)
	{
		Rect tileRect = tileDataArray[ETileGrid::BOTTOM_LEFT].tileRect;
	
		float maxOffset = gameObjectBoundingBox.getMinY() + 5.0f;
		float minOffset = gameObjectBoundingBox.getMinY() - 15.0f;
		
		if (tileRect.getMaxY() < maxOffset && tileRect.getMaxY() > minOffset)
		{
			canMoveLeft = true;
		}		
	}

	//
	if (tileDataArray[ETileGrid::BOTTOM_RIGHT].GID && isClimbing)
	{
		Rect tileRect = tileDataArray[ETileGrid::BOTTOM_RIGHT].tileRect;

		float maxOffset = gameObjectBoundingBox.getMinY() + 5.0f;
		float minOffset = gameObjectBoundingBox.getMinY() - 15.0f;

		if (tileRect.getMaxY() < maxOffset && tileRect.getMaxY() > minOffset)
		{
			canMoveRight = true;
		}
		
	}
			
	// gameobject moves onto a ladder top section
	if (tileDataArray[ETileGrid::CENTER].GID == false && tileDataArray[ETileGrid::BOTTOM].GID)
	{
		isLadderTop = true;
	}
	
	// gameobject moves onto a ladder mid section
	if (tileDataArray[ETileGrid::CENTER].GID && tileDataArray[ETileGrid::BOTTOM].GID)
	{
		isLadderMid = true;
	}	
	
	// gameobject moves onto a ladder mid section when section is bottom of the complete ladder
	if (tileDataArray[ETileGrid::CENTER].GID && tileDataArray[ETileGrid::BOTTOM].GID == false)
	{
		isLadderBottom = true;
	}

	// gameobject moves down ladder when on a ladder top section
	if (isLadderTop)
	{
		// get the top ladder rect
		Rect tileRect = tileDataArray[ETileGrid::BOTTOM].tileRect;
						
		// when gameobject is not intersecting
		if (gameObjectBoundingBox.intersectsRect(tileRect) == false)
		{
			// gameObject is not climbing
			isClimbing = false;
			gravity = false;
			
			// clamp gameObject to top of ladder
			gameObjectNewPosition.y = tileRect.getMaxY();
			gameObjectNewVelocity.y = 0.0f;					
		}
		else
		{			
			// set flags
			if (gameObjectBoundingBox.getMinX() >= tileRect.getMinX() &&
				gameObjectBoundingBox.getMaxX() <= tileRect.getMaxX())
			{
				canMoveDown = true;
				canMoveUp = true;
			}
		}
	}

	// gameObject is on ladder top section and climbing down
	if (isLadderTop && isMovingDown)
	{
		isClimbing = true;

		// clamp gameobject to center of ladder
		float tileMidX = tileDataArray[ETileGrid::BOTTOM].tileRect.getMidX();
		float gameObjectMidX = gameObjectBoundingBox.size.width / 2;

		gameObjectNewPosition.x = tileMidX - gameObjectMidX;
		gameObjectNewVelocity.x = 0.0f;
	}

	// gameObject is on ladder mid section
	if (isLadderMid)
	{
		// get the top ladder rect
		Rect tileRect = tileDataArray[ETileGrid::CENTER].tileRect;

		if (gameObjectBoundingBox.getMinX() >= tileRect.getMinX() &&
			gameObjectBoundingBox.getMaxX() <= tileRect.getMaxX())
		{
			canMoveDown = true;
			canMoveUp = true;
		}
	}
		
	// gameobject moves up or down when on a ladder mid section
	if (isLadderMid && (isMovingUp || isMovingDown))
	{
		isClimbing = true;		

		// clamp gameobject to center of ladder
		float tileMidX = tileDataArray[ETileGrid::CENTER].tileRect.getMidX();
		float gameObjectMidX = gameObjectBoundingBox.size.width / 2;

		gameObjectNewPosition.x = tileMidX - gameObjectMidX;
		gameObjectNewVelocity.x = 0.0f;
	}

	// gameobject moves left or right from ladder mid section to stationary platform
	if (isLadderMid && (isMovingLeft || isMovingRight) && (canMoveLeft || canMoveRight))
	{
		isClimbing = false;

		gameObjectNewPosition.y = tileDataArray[ETileGrid::BOTTOM].tileRect.getMaxY();
		gameObjectNewVelocity.y = 0.0f;
	}

	// gameObject is on ladder bottom section
	if (isLadderBottom)
	{
		Rect tileRect = tileDataArray[ETileGrid::CENTER].tileRect;

		if (gameObjectBoundingBox.getMinX() >= tileRect.getMinX() &&
			gameObjectBoundingBox.getMaxX() <= tileRect.getMaxX())
		{
			canMoveUp = true;
			canMoveDown = true;
		}
	}

	// gameObject is on ladder bottom section and climbing up
	if (isLadderBottom && isMovingUp)
	{
		isClimbing = true;
				
		// clamp gameobject to center of ladder
		float tileMidX = tileDataArray[ETileGrid::CENTER].tileRect.getMidX();
		float gameObjectMidX = gameObjectBoundingBox.size.width / 2;

		gameObjectNewPosition.x = tileMidX - gameObjectMidX;
		gameObjectNewVelocity.x = 0.0f;
	}

	// gameObject has stopped climbing down
	if (isLadderBottom && gameObject.onGround)
	{
		isClimbing = false;
		gravity = false;
	}
	
	//debug
	for (TileData tileData : tileDataArray)
	{
		Rect& tileRect = tileData.tileRect;

		// debug draw tile (PURPLE)
		if (isClimbing)		
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.5f, 1.0f, 0.5f));
		else
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));
	}
	
	//
	gameObject.desiredPosition = gameObjectNewPosition;
	gameObject.velocity = gameObjectNewVelocity;

	// gameObject flags
	gameObject.isClimbing = isClimbing;
	gameObject.canMoveUp = canMoveUp;
	gameObject.canMoveDown = canMoveDown;
	gameObject.canMoveLeft = canMoveLeft;
	gameObject.canMoveRight = canMoveRight;

	gameObject.gravity = gravity;
}


void GameObjectCollisionComponent::wrap(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
{
	// Get active mapsize and tilesize
	Size mapSize = parallaxTileMap.getMapSize();
	Size tileSize = parallaxTileMap.getTileSize();

	// get the gameobject bounding box
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();

	// bounds check gameobject
	if (gameObjectBoundingBox.getMaxX() < 0)
		gameObject.desiredPosition = Vec2((mapSize.width * tileSize.width) - gameObjectBoundingBox.size.width, gameObject.desiredPosition.y);

	if (gameObjectBoundingBox.getMinX() > mapSize.width * tileSize.width)
		gameObject.desiredPosition = Vec2(0, gameObject.desiredPosition.y);	
}


void GameObjectCollisionComponent::nextLevel(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
{
}


void GameObjectCollisionComponent::pathfinding(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
{
	Vec2 direction = Vec2::ZERO;

    //
    Vec2 cursor = parallaxTileMap.convertToNodeSpaceAR(AppGlobal::getInstance()->cursorLocation);
    parallaxTileMap.drawDebugRectAt(cursor, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
    
    if( AppGlobal::getInstance()->leftMouseButton )
    {
        Vec2 startLocation = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
        Vec2 targetLocation = parallaxTileMap.getTileCoordinatesFor(cursor);

		if (gameObject.path == nullptr)
		{
			gameObject.path = parallaxTileMap.getPath(startLocation, targetLocation);
		}			
		else
		{
			Path* newPath = parallaxTileMap.getPath(gameObject.path->peek_back(), targetLocation);
			gameObject.path->addPath(newPath);
		}
    }
	else if ( AppGlobal::getInstance()->rightMouseButton )
	{
		gameObject.path = nullptr;
		gameObject.velocity = Vec2::ZERO;
		direction = Vec2::ZERO;
	}
    	
	

	if ( gameObject.path != nullptr && gameObject.path->getLength() > 0 )
    {	
		for (int i = 0; i < gameObject.path->getLength(); ++i)
		{
			parallaxTileMap.drawDebugRectAtTile(gameObject.path->getStep(i), Color4F(0.3f, 1.0f, 0.3f, 0.5f));
		}

		Rect rect = parallaxTileMap.getTileRectFrom(gameObject.path->peek_front());
		
		Vec2 tileCenter = Vec2
		(
			rect.getMidX(), 
			rect.getMinY()
		);
        
		Vec2 gameObjectCenter = Vec2
		(
			gameObject.getCenterPosition().x,
			gameObject.getPositionY()
		);
        
		Vec2 normalized = Vec2(tileCenter - gameObjectCenter).getNormalized();  

		direction = Vec2(std::round(normalized.x), std::round(normalized.y));
		
		float distance = gameObjectCenter.distance(tileCenter);

        if( distance <= 17.0f )
        {
			gameObject.path->pop_front();
            
			if (gameObject.path->getLength() < 1)
            {
				gameObject.path = nullptr;

				gameObject.velocity = Vec2::ZERO;
				direction = Vec2::ZERO;				
            }
        }
        
		//gameObject.desiredPosition.x += 150 * kUpdateInterval * dx;
		//gameObject.desiredPosition.y += 150 * kUpdateInterval * dy;

		if (direction.y > 0 && gameObject.canMoveUp)
		{
			gameObject.isMovingUp = true;
			gameObject.move = true;
		}

		if (direction.y < 0 && gameObject.canMoveDown)
		{
			gameObject.isMovingDown = true;
			gameObject.move = true;
		}

		if (direction.x < 0 && gameObject.canMoveLeft)
		{
			gameObject.isMovingLeft = true;
			gameObject.move = true;
		}

		if (direction.x > 0 && gameObject.canMoveRight)
		{
			gameObject.isMovingRight = true;
			gameObject.move = true;
		}

		if (direction == Vec2::ZERO)
		{
			gameObject.isMovingUp = false;
			gameObject.isMovingDown = false;
			gameObject.isMovingLeft = false;
			gameObject.isMovingRight = false;
			gameObject.move = false;
			gameObject.velocity = Vec2::ZERO;
		}
	}

	Vec2 move = Vec2(1600.0, 1600.0);
	Vec2 step = move * kUpdateInterval;

	if (gameObject.move)
	{
		gameObject.velocity.x = gameObject.velocity.x + step.x * direction.x;
		gameObject.velocity.y = gameObject.velocity.y + step.y * direction.y;
	}

	Vec2 minMovement = Vec2(-340.0, -340.0);
	Vec2 maxMovement = Vec2(340.0, 340.0);

	gameObject.velocity.clamp(minMovement, maxMovement);
		
	Vec2 stepVelocity = gameObject.velocity * kUpdateInterval;
	// 
	gameObject.desiredPosition = gameObject.getPosition() + stepVelocity;
}
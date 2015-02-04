#include "Box2D.h"
#include "Constants.h"
#include "GameObject.h"
#include "Level.h"
#include "CollisionComponent.h"
#include "ParallaxTileMap.h"
#include "Utils.h"

void GameObjectCollisionComponent::update(Node& node, GameObject &gameObject)
{
	// Set flags
	gameObject.onGround = false;
	gameObject.canJump = false;
	
	//
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);

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
	for (int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
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
	// Flags
	bool isLadderTop = false;
	bool canClimb = false;
		
	// Variables	
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();
	Vec2 gameObjectCenterPosition = gameObject.getCenterPosition();
	Vec2 gameObjectNewPosition = gameObject.desiredPosition;
	Vec2 gameObjectNewVelocity = gameObject.velocity;
					
	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromLadderLayerAt(gameObjectCenterPosition);
	
	if (tileDataArray[ETileGrid::BOTTOM].GID && tileDataArray[ETileGrid::CENTER].GID == false)
	{
		isLadderTop = true;
		gameObject.onGround = true;
	}

	// debug loop through tiles array draw ladder rect PURPLE    
	for (int tileIndex = ETileGrid::BOTTOM; tileIndex < 3; tileIndex++)
	{
		// Get the tileData
		TileData& tileData = tileDataArray[tileIndex];
		Rect& tileRect = tileData.tileRect;

		// debug draw tile (PURPLE)
		parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));
		
		// Do not check if tile is empty
		if (tileData.GID == false) continue;

		// There is a tile so we can climb
		if (gameObjectBoundingBox.getMinX() >= tileRect.getMinX() &&
			gameObjectBoundingBox.getMaxX() <= tileRect.getMaxX())
		{
			canClimb = true;
		}

		//
		if (tileIndex == ETileGrid::BOTTOM)
		{
			// 
			if (isLadderTop && gameObjectBoundingBox.intersectsRect(tileRect) == false)
			{
				gameObjectNewPosition.y = tileRect.getMaxY();
				gameObjectNewVelocity.y = 0.0f;					
			}			
		}
		else if (tileIndex == ETileGrid::CENTER)
		{
			//
			if (gameObject.onGround) continue;

			// clamp gameobject to center tile bottom when moving left or right
			if (gameObject.canMoveLeft || gameObject.canMoveRight)
			{				
				gameObjectNewPosition.y = tileRect.getMinY();
			}			
		}
		
		// clamp gameObject to ladder when gameobject needs to climb
		if ( gameObject.isClimbingLadder )
		{			
			float tileMidX = tileRect.getMidX();
			float gameObjectMidX = gameObjectBoundingBox.size.width / 2;

			gameObjectNewPosition.x = tileMidX - gameObjectMidX;
			gameObjectNewVelocity.x = 0.0f;
		}
	}
	
	//
	gameObject.desiredPosition = gameObjectNewPosition;
	gameObject.velocity = gameObjectNewVelocity;

	// gameObject flags
	gameObject.canClimb = canClimb;
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
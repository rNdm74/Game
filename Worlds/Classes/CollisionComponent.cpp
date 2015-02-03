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
	bool canClimbUpLadder = false;
	bool canClimbDownLadder = false;
		
	// Variables		
	Vec2 gameObjectCenterPosition = gameObject.getCenterPosition();
	Vec2 gameObjectNewPosition = gameObject.desiredPosition;
	Vec2 gameObjectNewVelocity = gameObject.velocity;

	//
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();
					
	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromLadderLayerAt(gameObjectCenterPosition);
	
	// standing on top of tile
	if (tileDataArray[ETileGrid::BOTTOM].GID && tileDataArray[ETileGrid::CENTER].GID == false)
	{
		Rect& tileRect = tileDataArray[ETileGrid::BOTTOM].tileRect;
				
		if (gameObjectBoundingBox.intersectsRect(tileRect) == false)
		{
			gameObjectNewPosition.y = tileRect.getMaxY();
			gameObjectNewVelocity.y = 0.0f;

			gameObject.onGround = true;
		}			
	}

	// standing is at bottom of a ladder
	if (tileDataArray[ETileGrid::CENTER].GID && tileDataArray[ETileGrid::BOTTOM].GID == false) {}

	//
	if (tileDataArray[ETileGrid::LEFT].GID == false && gameObject.canMoveLeft && gameObject.onGround == false)
	{
		Rect& tileRect = tileDataArray[ETileGrid::CENTER].tileRect;
		gameObjectNewPosition.y = tileRect.getMinY();
	}

	//
	if (tileDataArray[ETileGrid::RIGHT].GID == false && gameObject.canMoveRight && gameObject.onGround == false)
	{
		Rect& tileRect = tileDataArray[ETileGrid::CENTER].tileRect;
		gameObjectNewPosition.y = tileRect.getMinY();
	}

	// gameObject can climb up ladder
	if (tileDataArray[ETileGrid::CENTER].GID)
	{
		canClimbUpLadder = true;
	}

	// gameObject can climb down ladder
	if (tileDataArray[ETileGrid::BOTTOM].GID)
	{
		canClimbDownLadder = true;
	}
		
	// clamp gameObject to ladder when gameobject needs to climb
	if (tileDataArray[ETileGrid::CENTER].GID && gameObject.isClimbingLadder && gameObject.onGround == false)
	{
		float tileMidX = tileDataArray[ETileGrid::CENTER].tileRect.getMidX();
		float gameObjectMidX = gameObjectBoundingBox.size.width / 2;

		gameObjectNewPosition.x = tileMidX - gameObjectMidX;
		gameObjectNewVelocity.x = 0.0f;
	}

	// debug loop through tiles array draw ladder rect PURPLE    
	for (int tileIndex = ETileGrid::BOTTOM; tileIndex < 5; tileIndex++)
		parallaxTileMap.drawDebugRect(tileDataArray[tileIndex].tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));
	
	//
	gameObject.desiredPosition = gameObjectNewPosition;
	gameObject.velocity = gameObjectNewVelocity;

	// gameObject flags
	gameObject.canClimbUpLadder = canClimbUpLadder;
	gameObject.canClimbDownLadder = canClimbDownLadder;	
}
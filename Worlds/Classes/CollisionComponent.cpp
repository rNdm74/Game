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
		
		// check if the tiledata is valid
		if (tileRect.intersectsRect(gameObjectBoundingBox))
		{			
			// debug draw tile rect RED
			parallaxTileMap.drawDebugRect(tileRect, Color4F(1.0f, 0.3f, 0.3f, 0.5f));

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
	bool isTileLadder = false;
	bool isTileLadderTop = false;
	bool isTileLadderMid = false;
	bool checkLadderMiddle = false;

	// Variables
	Rect tileRect;
	TileData tileLadder;
	ValueMap tileProperties;
	Vec2 gameObjectPosition;

	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();

	Vec2 newPosition = gameObject.getCenterPosition();

	//
	gameObjectPosition = gameObject.getPosition();
	gameObjectPosition.x = gameObjectPosition.x + gameObject.getSize().width / 2;

	//
	//tileLadder = parallaxTileMap.getTileDataFromLadderLayerAtPosition(newPosition);

	//// When there is a valid ladder tile
	//if (tileLadder.GID)
	//{
	//	// Get the ladder tile type
	//	tileProperties = parallaxTileMap.getPropertiesForGID(tileLadder.GID).asValueMap();

	//	// Set flags
	//	isTileLadderTop = tileProperties["LadderTop"].asBool();
	//	isTileLadderMid = tileProperties["LadderMid"].asBool();
	//	isTileLadder = isTileLadderTop || isTileLadderMid;
	//}

	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromLadderLayerAt(newPosition);

	// loop through tiles array	    
	for (int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
	{
		TileData& tileData = tileDataArray[tileIndex];
		Rect& tileRect = tileData.tileRect;
		
		// debug draw ladder rect PURPLE
		if (tileRect.intersectsRect(gameObjectBoundingBox))
		{
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));
		}	
	}
	
	if (parallaxTileMap.isTileLadder(newPosition))
	{
		bool isGameObjectTouchingGround = gameObject.onGround;
	}
	

	//else
	//{
	//	//
	//	tileLadder = parallaxTileMap.getTileDataFromLadderLayerAtPosition(newPosition);
	//}

	// debug draw ladder rect PURPLE
	//parallaxTileMap.drawDebugRect(tileLadder.tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));

	//// When gameObject is on ground or not colliding with a ladder set flags to false
	//if (gameObject.onGround || gameObject.collideLadder == false)
	//{
	//	gameObject.climbLadder = false;
	//	gameObject.disableLadderTopCollision = false;
	//}

	//// if we fall onto the top of a ladder, we start climbing it
	//if (isTileLadder && gameObject.onGround == false)
	//{
	//	gameObject.climbLadder = true;
	//	// play landing animation
	//}

	//// if we are touching a ladder tile
	//if (isTileLadder)
	//{
	//	// Flag - gameObject is colliding ladder
	//	gameObject.collideLadder = true;

	//	// 1.0 - Check if gameObject has moved past the top of the ladder
	//	if (isTileLadderTop && gameObjectBoundingBox.getMinY() > tileLadder.tileRect.getMaxY())
	//	{
	//		// 1.1 - Clamp gameObject to top of ladder
	//		gameObject.desiredPosition = Vec2(gameObject.desiredPosition.x, tileLadder.tileRect.getMaxY());

	//		// 1.2 - Flag :: gameObject is not climbing the ladder anymore
	//		gameObject.climbLadder = false;
	//	}

	//	// 2.0 - Check if gameObject is colliding with the top of a ladder tile
	//	if (isTileLadderTop && gameObject.disableLadderTopCollision == false)
	//	{
	//		// 2.1 - Check if gameObject is on the top of the ladder top tile
	//		gameObject.onLadderTop = gameObjectBoundingBox.getMinY() <= tileLadder.tileRect.getMaxY();

	//		// 2.2 - Keep gameObject on top of the ladder until, movement down ladder is required
	//		if (gameObject.onLadderTop)
	//		{
	//			// 2.2.3 - Clamp gameObject to top of ladder (doesnt fall down)
	//			gameObject.desiredPosition = Vec2(gameObject.desiredPosition.x, tileLadder.tileRect.getMaxY());
	//		}
	//		else
	//		{
	//			// 2.2.4 - Check if gameObject is climbing
	//			checkLadderMiddle = true;
	//		}
	//	}
	//	else
	//	{
	//		// 2.3 - Check if gameObject is climbing
	//		checkLadderMiddle = true;
	//	}

	//	// 3.0 - Check if gameObject is climbing a ladder tile
	//	if (isTileLadderMid && checkLadderMiddle)
	//	{
	//		// 3.1 - Check if gameObject is colliding with the middle of a ladder tile (top or mid tile)
	//		if (tileLadder.tileRect.containsPoint(gameObject.getPosition()))
	//		{
	//			// 3.1.1 - Check if gameObject is climbing ladder
	//			if (gameObject.climbLadder)
	//			{
	//				// 3.1.1.1 - Clamp gameObject in centre of ladder
	//				gameObject.desiredPosition = Vec2
	//				(
	//					tileLadder.tileRect.getMidX() - gameObjectBoundingBox.size.width / 2,
	//					gameObject.desiredPosition.y
	//				);
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	gameObject.collideLadder = false;
	//}
}
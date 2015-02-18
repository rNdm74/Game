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
void CollisionComponent::update(Node& node, GameObjectNode& gameObjectNode)
{
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
	this->checkCollision(parallaxTileMap, gameObjectNode);
};


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void PlayerCollisionComponent::update(Node& node, GameObjectNode& gameObjectNode)
{		
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
	Player& player = static_cast<Player&>(gameObjectNode);
		
	this->solidTileCollision(parallaxTileMap, player);
	this->ladderTileCollision(parallaxTileMap, player);
	//this->wrap(parallaxTileMap, player);
		
	//
	player.setPosition(player.desiredPosition);
}


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void CollisionComponent::solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject)
{
	// get gameobjects center
	Vec2 gameObjectCenterPosition = gameObject.getCenterPosition();
	Vec2 gameObjectNewPosition = gameObject.desiredPosition;
	Vec2 gameObjectNewVelocity = gameObject.velocity;

	// get the gameobject bounding box
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();

#if DEBUG_ENABLE
	// debug draw gameobject rect GREEN
	parallaxTileMap.drawDebugRect(gameObjectBoundingBox, Color4F(0.3f, 1.0f, 0.3f, 0.5f));
#endif // DEBUG_ENABLE

	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromCollisionLayerAt(gameObjectCenterPosition);
		
	// loop through tiles array	    
	for (unsigned int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
	{
		TileData& tileData = tileDataArray[tileIndex];
		Rect& tileRect = tileData.tileRect;

#if DEBUG_ENABLE
		// debug draw tile rect RED
		parallaxTileMap.drawDebugRect(tileRect, Color4F(1.0f, 0.3f, 0.3f, 0.5f));
#endif // DEBUG_ENABLE

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


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void CollisionComponent::ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
{			
	// Variables	
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();
	Vec2 gameObjectCenterPosition = gameObject.getCenterPosition();
	Vec2 gameObjectNewPosition = gameObject.desiredPosition;
	Vec2 gameObjectNewVelocity = gameObject.velocity;
					
	// get array of tiles surrounding the gameobject
	TileDataArray tileDataArray = parallaxTileMap.getTileDataArrayFromLadderLayerAt(gameObjectCenterPosition);
		
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

    
#if DEBUG_ENABLE
	//debug
	for (TileData tileData : tileDataArray)
	{
		Rect& tileRect = tileData.tileRect;

		// debug draw tile (PURPLE)
        if (isClimbing)
        {
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.5f, 1.0f, 0.5f));
        }
        else
        {
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));
        }

	}
#endif // DEBUG_ENABLE
	

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


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
//void PlayerCollisionComponent::wrap(ParallaxTileMap& parallaxTileMap, Player& gameObject)
//{
//	// Get active mapsize and tilesize
//	Size mapSize = parallaxTileMap.getMapSize();
//	Size tileSize = parallaxTileMap.getTileSize();
//
//	// get the gameobject bounding box
//	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();
//
//	// bounds check gameobject
//	if (gameObjectBoundingBox.getMaxX() < 0)
//		gameObject.desiredPosition = Vec2((mapSize.width * tileSize.width) - gameObjectBoundingBox.size.width, gameObject.desiredPosition.y);
//
//	if (gameObjectBoundingBox.getMinX() > mapSize.width * tileSize.width)
//		gameObject.desiredPosition = Vec2(0, gameObject.desiredPosition.y);	
//}



void CollisionComponent::checkCollision(ParallaxTileMap& parallaxTileMap, GameObjectNode& gameObjectNode)
{	
	Player& player = *parallaxTileMap.getPlayer();

	Rect r1 = player.getBoundingBox();
	Rect r2 = gameObjectNode.getBoundingBox();

	if (r1.intersectsRect(r2) == false)	return;

	std::string type = gameObjectNode.getName();

	Vec2 v1 = Vec2::ZERO;
	Vec2 v2 = Vec2::ZERO;

	//
	if (type == "Left" && player.isMovingLeft)
	{
		v1 = Vec2(r1.getMaxX(), r1.origin.y);
		v2 = r2.origin;
	}
	else if (type == "Right" && player.isMovingRight)
	{
		v1 = r1.origin;
		v2 = Vec2(r2.getMaxX(), r2.origin.y);
	}
	else if (type == "Enter" && player.isMovingDown)
	{
		v1 = Vec2(r1.getMidX(), r1.getMaxY());
		v2 = Vec2(r2.getMidX(), r2.origin.y);
	}
	else if (type == "Exit" && player.isMovingUp)
	{
		v1 = Vec2(r1.getMidX(), r1.getMinY());
		v2 = Vec2(r2.getMidX(), r2.getMaxY());
	}

	Vec2 n = Vec2(v2 - v1).getNormalized();
	Vec2 direction = Vec2(std::round(n.x), std::round(n.y));

	player.mapTransition = direction;
}
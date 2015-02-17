#include "AppGlobal.h"
#include "CollisionComponent.h"
#include "Constants.h"
#include "GameObject.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "Utils.h"
#include "Path.h"


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void CollisionComponent::update(Node& node, GameObject& gameObject)
{
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
	Player& player = *parallaxTileMap.getPlayer();
	
	Rect r1 = player.getBoundingBox();
	Rect r2 = gameObject.getBoundingBox();
		
	if (r1.intersectsRect(r2) == false)	return;
	
	//
	ValueMap properties = gameObject.getProperties();
	std::string type = properties["name"].asString().c_str();
	
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
};


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void PlayerCollisionComponent::update(Node& node, GameObject &gameObject)
{
	//
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
	//
	Player& player = static_cast<Player&>(gameObject);
		
	// run checks	
	this->pathfinding(parallaxTileMap, player);
	this->solidTileCollision(parallaxTileMap, player);
	this->ladderTileCollision(parallaxTileMap, player);
	this->wrap(parallaxTileMap, player);

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
void PlayerCollisionComponent::solidTileCollision(ParallaxTileMap& parallaxTileMap, Player& gameObject)
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
void PlayerCollisionComponent::ladderTileCollision(ParallaxTileMap& parallaxTileMap, Player &gameObject)
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

#if DEBUG_ENABLE
		// debug draw tile (PURPLE)
		if (isClimbing)		
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.5f, 1.0f, 0.5f));
		else
			parallaxTileMap.drawDebugRect(tileRect, Color4F(0.5f, 0.3f, 1.0f, 0.5f));
#endif // DEBUG_ENABLE

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


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void PlayerCollisionComponent::wrap(ParallaxTileMap& parallaxTileMap, Player& gameObject)
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


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void PlayerCollisionComponent::nextLevel(ParallaxTileMap& parallaxTileMap, Player& gameObject)
{
}


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void PlayerCollisionComponent::pathfinding(ParallaxTileMap& parallaxTileMap, Player& gameObject)
{
	Vec2 cursorLocation = parallaxTileMap.convertToNodeSpaceAR(AppGlobal::getInstance()->cursorLocation);
	
#if DEBUG_ENABLE
	parallaxTileMap.drawDebugRectAt(cursorLocation, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
#endif // DEBUG_ENABLE

	gameObject.direction = Vec2::ZERO;
	//
	if (AppGlobal::getInstance()->leftMouseButton)
	{
		//
		Vec2 v1 = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
		Vec2 v2 = parallaxTileMap.getTileCoordinatesFor(cursorLocation);
		//
		if (gameObject.path == nullptr)
		{
			gameObject.path = parallaxTileMap.getPath(v1, v2);
		}
		else
		{
			Path* newPath = parallaxTileMap.getPath(gameObject.path->peek_back(), v2);
			gameObject.path->addPath(newPath);
		}
	}
	else if (AppGlobal::getInstance()->mouseToggle)
	{
		//
		Vec2 v1 = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
		Vec2 v2 = parallaxTileMap.getTileCoordinatesFor(cursorLocation);
		//
		if ( gameObject.path == nullptr && AppGlobal::getInstance()->leftMouseButton )
		{
			gameObject.path = parallaxTileMap.getPath(v1, v2);
		}
	}
	else
	{
		gameObject.path = nullptr;
		gameObject.velocity = Vec2::ZERO;
		gameObject.direction = Vec2::ZERO;
	}    
	//
	if ( gameObject.path != nullptr && gameObject.path->getLength() > 0 )
    {	

#if DEBUG_ENABLE
		for (int i = 0; i < gameObject.path->getLength(); ++i)
		{
			parallaxTileMap.drawDebugRectAtTile(gameObject.path->getStep(i), Color4F(0.3f, 1.0f, 0.3f, 0.5f));
		}
#endif // DEBUG_ENABLE

		//
		Rect r1 = gameObject.getCollisionBoundingBox();
		Rect r2 = parallaxTileMap.getTileRectFrom(gameObject.path->peek_front());
		//
		Vec2 v1 = Vec2(r1.getMidX(), r1.getMinY());
		Vec2 v2 = Vec2(r2.getMidX(), r2.getMinY());
        //		        
		Vec2 n = Vec2(v2 - v1).getNormalized();
		gameObject.direction = Vec2(std::round(n.x), std::round(n.y));
		//
		float distance = v1.distance(v2);
		log("distance: %f", distance);
		//
        if( distance <= 17.0f )
        {
			gameObject.path->pop_front();
            
			if (gameObject.path->getLength() < 1)
            {
				AppGlobal::getInstance()->mouseToggle = false;

				gameObject.path = nullptr;
				gameObject.velocity = Vec2::ZERO;
				gameObject.direction = Vec2::ZERO;
            }
        }
        
		//
		gameObject.isMovingUp = false;
		gameObject.isMovingDown = false;
		gameObject.isMovingLeft = false;
		gameObject.isMovingRight = false;

		if (gameObject.direction.y > 0 && gameObject.canMoveUp)
		{
			gameObject.isMovingUp = true;
			gameObject.canMove = true;
		}
		else if (gameObject.direction.y < 0 && gameObject.canMoveDown)
		{
			gameObject.isMovingDown = true;
			gameObject.canMove = true;
		}
		else if (gameObject.direction.x < 0 && gameObject.canMoveLeft)
		{
			gameObject.isMovingLeft = true;
			gameObject.canMove = true;
		}
		else if (gameObject.direction.x > 0 && gameObject.canMoveRight)
		{
			gameObject.isMovingRight = true;
			gameObject.canMove = true;
		}
		else if (gameObject.direction == Vec2::ZERO)
		{
			gameObject.isMovingUp = false;
			gameObject.isMovingDown = false;
			gameObject.isMovingLeft = false;
			gameObject.isMovingRight = false;
			gameObject.canMove = false;
			gameObject.velocity = Vec2::ZERO;
		}
	}

	Vec2 move = Vec2(1600.0, 1600.0);
	Vec2 step = move * kUpdateInterval;

	if (gameObject.canMove)
	{
		gameObject.velocity.x = gameObject.velocity.x + step.x * gameObject.direction.x;
		gameObject.velocity.y = gameObject.velocity.y + step.y * gameObject.direction.y;
	}

	Vec2 minMovement = Vec2(-340.0, -340.0);
	Vec2 maxMovement = Vec2(340.0, 340.0);

	gameObject.velocity.clamp(minMovement, maxMovement);
		
	Vec2 stepVelocity = gameObject.velocity * kUpdateInterval;
	// 
	gameObject.desiredPosition = gameObject.getPosition() + stepVelocity;
}
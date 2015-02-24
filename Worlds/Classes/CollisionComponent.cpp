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
void CollisionComponent::update(Node& node, IGameObject& gameObject)
{
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
	this->checkCollision(parallaxTileMap, gameObject);
};


/**
* Create a path finder
*
* @param parallaxTileMap The ParallaxTileMap to be searched
* @param maxSearchDistance The maximum depth we'll search before giving up
* @param allowDiagMovement True if the search should try diaganol movement
*/
void PlayerCollisionComponent::update(Node& node, IGameObject& gameObject)
{	

#if DEBUG_ENABLE
	// debug draw gameobject rect GREEN
	static_cast<ParallaxTileMap&>(node).drawDebugRect(gameObject.getCollisionBox(), Color4F(0.3f, 1.0f, 0.3f, 0.5f));
#endif // DEBUG_ENABLE

	this->checkTileCollision(node, gameObject);
	this->checkLadderCollision(node, gameObject);		
	//
	
}

/**
* Check if a gameObject has a tile collision
* The velocity and desiredPosition of the game object will be modified depending on collisions found 
*
* @param node The Node containing the gameObject
* @param gameObject The IGameObject that is checking collisions
*/
void CollisionComponent::checkTileCollision(Node& node, IGameObject& gameObject)
{
	/** Game objects desired position **/
	Vec2 desiredPosition = gameObject.getDesiredPosition();
	/** Game objects center position **/
	Vec2 centerPosition = gameObject.getCenterPosition();	
	/** Game objects velocity **/
	Vec2 velocity = gameObject.getVelocity();
	/** Game objects collision bounding box **/
	Rect collisionBox = gameObject.getCollisionBox();
	
	/** Get array of tiles surrounding the gameobject **/
	TileDataArray tileDataArray = static_cast<ParallaxTileMap&>(node).getTileDataArrayFromCollisionLayerAt(centerPosition);
		    
	for (unsigned int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
	{ 	
		Rect& tileRect = tileDataArray[tileIndex].tileRect;

#if DEBUG_ENABLE
		/** debug draw tile rect RED **/
		static_cast<ParallaxTileMap&>(node).drawDebugRect(tileRect, Color4F(1.0f, 0.3f, 0.3f, 0.5f));
#endif // DEBUG_ENABLE

		if (tileRect.intersectsRect(collisionBox) /** Intersection has occurred **/)
		{	
			/** Get the intersecting rect **/
			Rect intersection = Utils::getRectIntersection(collisionBox, tileRect);
						
			if (tileIndex == ETileGrid::BOTTOM /** Tile is below gameObject **/)
			{				
				desiredPosition.y += intersection.size.height;
				velocity.y = 0.0f;
				gameObject.setOnGround(true);
			}
			else if (tileIndex == ETileGrid::TOP /** Tile is above gameObject **/)
			{
				desiredPosition.y += -intersection.size.height;
				velocity.y = 0.0f;
			}
			else if (tileIndex == ETileGrid::LEFT /** Tile is left of gameObject **/)
			{
				desiredPosition.x += intersection.size.width;
				velocity.x = 0.0f;
			}
			else if (tileIndex == ETileGrid::RIGHT /** Tile is right of gameObject **/) 
			{
				desiredPosition.x += -intersection.size.width;
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
						gameObject.setOnGround(true);
					}
					else
					{
						resolutionHeight = -intersection.size.height;
					}

					desiredPosition.y += resolutionHeight;
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

					desiredPosition.x += resolutionWidth;
				}
			}
		}
	}

	/** Update gameObject with updated velocity and desiredPosition **/
	gameObject.setVelocity(velocity);
	gameObject.setDesiredPosition(desiredPosition);
}

/**
* Check if a gameObject has a ladder collision
* The velocity and desiredPosition of the game object will be modified depending on collisions found
*
* @param node The Node containing the gameObject
* @param gameObject The IGameObject that is checking collisions
*/
void CollisionComponent::checkLadderCollision(Node& node, IGameObject& gameObject)
{
	/** Game objects desired position **/
	Vec2 desiredPosition = gameObject.getDesiredPosition();
	/** Game objects center position **/
	Vec2 centerPosition = gameObject.getCenterPosition();
	/** Game objects velocity **/
	Vec2 velocity = gameObject.getVelocity();
	/** Game objects collision bounding box **/
	Rect collisionBox = gameObject.getCollisionBox();

	/** Get array of tiles surrounding the gameobject **/
	TileDataArray tileDataArray = static_cast<ParallaxTileMap&>(node).getTileDataArrayFromLadderLayerAt(centerPosition);
		
	for (unsigned int tileIndex = ETileGrid::BOTTOM; tileIndex < tileDataArray.size(); tileIndex++)
	{
		{
			//if (tileDataArray[tileIndex].GID == false)
			//	continue;

			Rect tileRect = tileDataArray[tileIndex].tileRect;

			if (tileDataArray[tileIndex].GID && tileIndex == ETileGrid::BOTTOM /** Tile is below gameObject **/)
			{
				if (collisionBox.intersectsRect(tileRect))
				{
					// clamp gameobject to center of ladder
					float tileMidX = tileRect.getMidX();
					float gameObjectMidX = collisionBox.size.width / 2;
					desiredPosition.x = tileMidX - gameObjectMidX;

					velocity.y += 0.1f;
					velocity.x = 0.0f;
				}
			}
			
			static_cast<ParallaxTileMap&>(node).drawDebugRect(tileRect, Color4F(0.0f, 0.5f, 0.5f, 0.5f));
		}
		
		/** Update gameObject with updated velocity and desiredPosition **/
		gameObject.setVelocity(velocity);
		gameObject.setDesiredPosition(desiredPosition);
	}
};

	/**
	* Create a path finder
	*
	* @param parallaxTileMap The ParallaxTileMap to be searched
	* @param maxSearchDistance The maximum depth we'll search before giving up
	* @param allowDiagMovement True if the search should try diaganol movement
	*/
	void CollisionComponent::ladderTileCollision(Node& node, IGameObject& gameObject)
	{
		// Variables	
		Rect collisionBox = gameObject.getCollisionBox();
		Vec2 centerPosition = gameObject.getCenterPosition();
		Vec2 newPosition = gameObject.getDesiredPosition();
		Vec2 velocity = gameObject.getVelocity();

		// get array of tiles surrounding the gameobject
		TileDataArray tileDataArray = static_cast<ParallaxTileMap&>(node).getTileDataArrayFromLadderLayerAt(centerPosition);

		CanMove canMove = gameObject.getCanMove();
		IsMoving isMoving = gameObject.getIsMoving();

		bool isOnGround = gameObject.getOnGround();
		bool isClimbing = gameObject.getClimbing();

		// bool flags
		bool isLadderTop = false;
		bool isLadderMid = false;
		bool isLadderBottom = false;

		//
		if (tileDataArray[ETileGrid::LEFT].GID == false && isClimbing == false)
		{
			canMove[STATE_LEFT] = true;
		}

		//
		if (tileDataArray[ETileGrid::RIGHT].GID == false && isClimbing == false)
		{
			canMove[STATE_RIGHT] = true;
		}

		//
		if (tileDataArray[ETileGrid::BOTTOM_LEFT].GID && isClimbing)
		{
			Rect tileRect = tileDataArray[ETileGrid::BOTTOM_LEFT].tileRect;

			float maxOffset = collisionBox.getMinY() + 5.0f;
			float minOffset = collisionBox.getMinY() - 15.0f;

			if (tileRect.getMaxY() < maxOffset && tileRect.getMaxY() > minOffset)
			{
				canMove[STATE_LEFT] = true;
			}
		}

		//
		if (tileDataArray[ETileGrid::BOTTOM_RIGHT].GID && isClimbing)
		{
			Rect tileRect = tileDataArray[ETileGrid::BOTTOM_RIGHT].tileRect;

			float maxOffset = collisionBox.getMinY() + 5.0f;
			float minOffset = collisionBox.getMinY() - 15.0f;

			if (tileRect.getMaxY() < maxOffset && tileRect.getMaxY() > minOffset)
			{
				canMove[STATE_RIGHT] = true;
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
			if (collisionBox.intersectsRect(tileRect) == false)
			{
				// gameObject is not climbing
				isClimbing = false;

				// clamp gameObject to top of ladder
				newPosition.y = tileRect.getMaxY();
				velocity.y = 0.0f;
			}
			else
			{
				// set flags
				if (collisionBox.getMinX() >= tileRect.getMinX() &&
					collisionBox.getMaxX() <= tileRect.getMaxX())
				{
					canMove[STATE_UP] = true;
					canMove[STATE_DOWN] = true;
				}
			}
		}

		// gameObject is on ladder top section and climbing down
		if (isLadderTop && isMoving[STATE_DOWN])
		{
			isClimbing = true;

			// clamp gameobject to center of ladder
			float tileMidX = tileDataArray[ETileGrid::BOTTOM].tileRect.getMidX();
			float gameObjectMidX = collisionBox.size.width / 2;

			newPosition.x = tileMidX - gameObjectMidX;
			velocity.x = 0.0f;
		}

		// gameObject is on ladder mid section
		if (isLadderMid)
		{
			// get the top ladder rect
			Rect tileRect = tileDataArray[ETileGrid::CENTER].tileRect;

			if (collisionBox.getMinX() >= tileRect.getMinX() &&
				collisionBox.getMaxX() <= tileRect.getMaxX())
			{
				canMove[STATE_UP] = true;
				canMove[STATE_DOWN] = true;
			}
		}

		// gameobject moves up or down when on a ladder mid section
		if (isLadderMid && (isMoving[STATE_UP] || isMoving[STATE_DOWN]))
		{
			isClimbing = true;

			// clamp gameobject to center of ladder
			float tileMidX = tileDataArray[ETileGrid::CENTER].tileRect.getMidX();
			float gameObjectMidX = collisionBox.size.width / 2;

			newPosition.x = tileMidX - gameObjectMidX;
			velocity.x = 0.0f;
		}

		// gameobject moves left or right from ladder mid section to stationary platform
		if (isLadderMid && (isMoving[STATE_LEFT] || isMoving[STATE_RIGHT]) && (canMove[STATE_LEFT] || canMove[STATE_RIGHT]))
		{
			isClimbing = false;

			newPosition.y = tileDataArray[ETileGrid::BOTTOM].tileRect.getMaxY();
			velocity.y = 0.0f;
		}

		// gameObject is on ladder bottom section
		if (isLadderBottom)
		{
			Rect tileRect = tileDataArray[ETileGrid::CENTER].tileRect;

			if (collisionBox.getMinX() >= tileRect.getMinX() &&
				collisionBox.getMaxX() <= tileRect.getMaxX())
			{
				canMove[STATE_UP] = true;
				canMove[STATE_DOWN] = true;
			}
		}

		// gameObject is on ladder bottom section and climbing up
		if (isLadderBottom && isMoving[STATE_UP])
		{
			isClimbing = true;

			// clamp gameobject to center of ladder
			float tileMidX = tileDataArray[ETileGrid::CENTER].tileRect.getMidX();
			float gameObjectMidX = collisionBox.size.width / 2;

			newPosition.x = tileMidX - gameObjectMidX;
			velocity.x = 0.0f;
		}

		// gameObject has stopped climbing down
		if (isLadderBottom && isOnGround)
		{
			isClimbing = false;
		}


#if DEBUG_ENABLE
		for (TileData tileData : tileDataArray)
		{
			// debug draw tile (PURPLE)
			Color4F color = (isClimbing) ? Color4F(0.5f, 0.5f, 1.0f, 0.5f) : Color4F(0.5f, 0.3f, 1.0f, 0.5f);
			static_cast<ParallaxTileMap&>(node).drawDebugRect(tileData.tileRect, color);
		}
#endif // DEBUG_ENABLE


		//
		gameObject.setDesiredPosition(newPosition);
		gameObject.setVelocity(velocity);

		// gameObject flags
		gameObject.setClimbing(isClimbing);
		gameObject.setIsMoving(isMoving);
		gameObject.setCanMove(canMove);
	}

//
void CollisionComponent::checkCollision(Node& node, IGameObject& gameObject)
{	
	IGameObject* player = static_cast<ParallaxTileMap&>(node).getPlayer();

	Rect r1 = player->getBoundingBox();
	Rect r2 = gameObject.getBoundingBox();

	if (r1.intersectsRect(r2) == false)	return;

	std::string type = gameObject.getName();

	Vec2 v1 = Vec2::ZERO;
	Vec2 v2 = Vec2::ZERO;

	IsMoving isMoving = player->getIsMoving();

	//
	if (type == "Left" && isMoving[STATE_LEFT])
	{
		v1 = Vec2(r1.getMaxX(), r1.origin.y);
		v2 = r2.origin;
	}
	else if (type == "Right" && isMoving[STATE_RIGHT])
	{
		v1 = r1.origin;
		v2 = Vec2(r2.getMaxX(), r2.origin.y);
	}
	else if (type == "Enter" && isMoving[STATE_DOWN])
	{
		v1 = Vec2(r1.getMidX(), r1.getMaxY());
		v2 = Vec2(r2.getMidX(), r2.origin.y);
	}
	else if (type == "Exit" && isMoving[STATE_UP])
	{
		v1 = Vec2(r1.getMidX(), r1.getMinY());
		v2 = Vec2(r2.getMidX(), r2.getMaxY());
	}

	Vec2 n = Vec2(v2 - v1).getNormalized();
	Vec2 direction = Vec2(std::round(n.x), std::round(n.y));


	player->setMapTransition(direction);
}
#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "Utils.h"

Level* Level::create(std::string mapName)
{
	// Create an instance of Level
	Level* node = new Level();

	if (node) 
	{
		// Add it to autorelease pool
		node->autorelease();
		node->loadMap(mapName);
		node->loadPlayer();
	}
	else 
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}


Level::Level()
{
	global = AppGlobal::getInstance();
	winSize = Director::getInstance()->getWinSize();
	size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + size.width / 2, (origin.y + size.height / 2));
}


Level::~Level(){}


void Level::loadMap(std::string mapname)
{	
	this->removeAllChildrenWithCleanup(true);
	parallaxTileMap = ParallaxTileMap::create(mapname);
	this->addChild(parallaxTileMap);

	mapSize = parallaxTileMap->getMapSize();
	tileSize = parallaxTileMap->getTileSize();
}


void Level::loadPlayer()
{	
	gameObjectList.clear();

	player = parallaxTileMap->getPlayer();	
	gameObjectList.push_back(player);

	this->setViewPointCenter(player->getCenterPosition());
}


void Level::setViewPointCenter(Vec2 position)
{	
	winSize = winSize / this->getScale();
	
    float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (mapSize.width * tileSize.width) - winSize.width / 2);
	y = MIN(y, (mapSize.height * tileSize.height) - winSize.height / 2);
    
	Vec2 actualPosition = Vec2(x, y);    
	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
	parallaxTileMap->setPosition(viewPoint);
}


void Level::update(float& delta)
{	
	// debug
	parallaxTileMap->clearDebugDraw();

	// updates scale creates zoom effect
	this->setScale(global->scale);
		
	// update gameobjects
	//for (auto& gameObject : gameObjectList)
	//	gameObject->update(parallaxTileMap);
    parallaxTileMap->update(delta);
    
	//
	this->checkCollisions();

	// player moves to next map
	this->checkNextMap(player);

	// centre viewport on player
	this->setViewPointCenter(player->getCenterPosition());
    
	Vec2 cursor = parallaxTileMap->convertToNodeSpaceAR(global->cursorLocation);
    Vec2 cursorPrevious = parallaxTileMap->convertToNodeSpaceAR(global->cursorDownLocation);
		
	if (global->leftMouseButton)
	{
		Vec2 cursorPosition = global->cursorLocation;
		//log("cursorPosition - x:%f, y:%f", cursorPosition.x, cursorPosition.y);

		Vec2 position = this->getPosition();
		Vec2 parentPosition = this->getParent()->getPosition();
		Vec2 diff = position - parentPosition;
		//log("diff - x:%f, y:%f", diff.x, diff.y);

		//
		Vec2 scrollStep = cursor - cursorPrevious;
		//log("scrollStep - x:%f, y:%f", scrollStep.x, scrollStep.y);

		Vec2 distance = diff + scrollStep;
		log("distance - x:%f, y:%f", distance.x, distance.y);
				
		this->setPosition(scrollStep);

		parallaxTileMap->drawDebugRectAt(cursor, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
	}
	else
	{
		parallaxTileMap->drawDebugRectAt(cursor, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
	}
}


void Level::checkCollisions()
{
	for (auto& gameObject : gameObjectList)
	{
		// Set flags
		gameObject->onGround = false;
		gameObject->canJump = false;

		// run checks	
		this->solidTileCollision(*parallaxTileMap, *gameObject);
		this->ladderTileCollision(*parallaxTileMap, *gameObject);
		this->wrap(*parallaxTileMap, *gameObject);

		gameObject->setPosition(gameObject->desiredPosition);
	}
}


void Level::solidTileCollision(ParallaxTileMap& parallaxTileMap, GameObject& gameObject)
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


void Level::ladderTileCollision(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
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
		if (gameObject.isClimbingLadder)
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


void Level::wrap(ParallaxTileMap& parallaxTileMap, GameObject &gameObject)
{
	// get the gameobject bounding box
	Rect gameObjectBoundingBox = gameObject.getCollisionBoundingBox();

	// bounds check gameobject
	if (gameObjectBoundingBox.getMaxX() < 0)
		gameObject.desiredPosition = Vec2((mapSize.width * tileSize.width) - gameObjectBoundingBox.size.width, gameObject.desiredPosition.y);

	if (gameObjectBoundingBox.getMinX() > mapSize.width * tileSize.width)
		gameObject.desiredPosition = Vec2(0, gameObject.desiredPosition.y);
}


void Level::checkNextMap(GameObject* gameObject)
{	
	if (gameObject->getCollisionBoundingBox().getMinY() < 0)
	{
		loadMap("planet1.tmx");
		loadPlayer();
	}
}
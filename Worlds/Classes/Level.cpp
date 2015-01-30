#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "Utils.h"

Level::Level(){}

Level::~Level(){}

void Level::loadMap(std::string mapname)
{
	global = AppGlobal::getInstance();
	size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + size.width / 2, (origin.y + size.height / 2));
	
	parallaxTileMap = ParallaxTileMap::create(mapname);
	parallaxTileMap->addObjects();

	drawNode = DrawNode::create();
	parallaxTileMap->getObjectLayer()->addChild(drawNode, 99);

	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(parallaxTileMap);

	//auto testMap = ParallaxTileMap::create("planet1.tmx");
	
	//this->addChild(testMap);

	//testMap->setAnchorPoint(Vec2(0.5, 1));
	//testMap->setPositionY(70);

	//this->runAction(MoveBy::create(5.0f, Vec2(0.0, size.height/2)));
}

void Level::followPlayer()
{	
	gameObjectList.clear();

	player = parallaxTileMap->getPlayer();	
	gameObjectList.push_back(player);

	this->setViewPointCenter(player->getPosition());
}

void Level::setViewPointCenter(Vec2 position)
{
	Size mapSize = parallaxTileMap->getMapSize();
	Size tileSize = parallaxTileMap->getTileSize();
	Size winSize = Director::getInstance()->getWinSize() / this->getScale();
	
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
	// updates scale creates zoom effect
	this->setScale(global->scale);

	player->update(this);
	checkForAndResolveCollisions(player);

	// update gameobjects
	//for (auto& gameObject : gameObjectList) 
 //   {
 //       gameObject->update(this);

 //       //
 //       checkForAndResolveCollisions(gameObject);
 //   }
    
	// centre view port on player
	this->setViewPointCenter(player->getPosition());
}

void Level::checkForAndResolveCollisions(GameObject* gameObject)
{	
	// Get active mapsize and tilesize
	Size mapSize = parallaxTileMap->getMapSize();
	Size tileSize = parallaxTileMap->getTileSize();
			
	// get gameobjects center
	Vec2 newPosition = gameObject->getPosition();
	newPosition.x = newPosition.x + gameObject->getSize().width / 2;
	newPosition.y = newPosition.y + gameObject->getSize().height / 2;

	// get array of tiles surronding the gameobject
	TileDataArray tiles = Utils::getSurroundingTilesAtPosition(newPosition, *parallaxTileMap->getForegroundLayer(), mapSize, tileSize);
		
	//
	gameObject->onGround = false;
	gameObject->canJump = false;

	// get the gameobject bounding box
	Rect gameObjectBoundingBox = gameObject->getCollisionBoundingBox();
		
	// debug
    drawNode->clear();
	drawNode->drawSolidRect
	(
		gameObjectBoundingBox.origin,
		Vec2(gameObjectBoundingBox.getMaxX(), gameObjectBoundingBox.getMaxY()),
		Color4F(0.3f, 1.0f, 0.3f, 0.5f)
	);

	// loop through tiles array	    
	for (int tileIndex = ETileGrid::BOTTOM; tileIndex < tiles.size(); tileIndex++)
	{
		// get the tile data
		TileData tileData = tiles[tileIndex];
				
		// check if the tiledata is valid
		if (tileData.gid > 0)
		{	
			//
			Rect tileRect = tileData.tile;
						
			// debug
			drawNode->drawSolidRect
            (
				tileRect.origin,
                Vec2(tileRect.getMaxX(), tileRect.getMaxY()),
                Color4F(1.0f, 0.3f, 0.3f, 0.5f)
            );
            
			// check if the the gameobject intersects with the tilesRect
			if (Utils::RectIntersectsRect(gameObjectBoundingBox, tileRect))
			{
				// get the intersecting rect
				Rect intersection = Utils::RectIntersection(gameObjectBoundingBox, tileRect);
				
				// debug
				drawNode->drawSolidRect
				(
					intersection.origin,
					Vec2(intersection.getMaxX(), intersection.getMaxY()),
					Color4F(0.3f, 0.3f, 1.0f, 0.5f)
				);
			
				if (tileIndex == ETileGrid::BOTTOM) // tile is below gameobject
				{					
					// check tile type
					ValueMap properties = parallaxTileMap->getPropertiesForGID(tileData.gid).asValueMap();
					
					if (properties["Solid"].asBool())
					{
						gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y + intersection.size.height);
						gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);
						gameObject->onGround = true;
					}					
										
					//if (properties["Ladder"].asBool() && AppGlobal::getInstance()->states.DOWN)
					//{
					//	gameObject->desiredPosition = Vec2(tileRect.getMidX() - gameObjectBoundingBox.size.width / 2, gameObject->desiredPosition.y);
					//	//gameObject->velocity = Vec2(gameObject->velocity.x, gameObject->velocity.y);
					//	gameObject->onLadder = true;
					//}
					//else
					//{
					//	gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y + intersection.size.height);
					//	gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);
					//}
					
				}
				else if (tileIndex == ETileGrid::TOP) // top tile
                {
                    gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y - intersection.size.height);
                    gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);
                }
				else if (tileIndex == ETileGrid::LEFT) // left tile
                {
                    gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x + intersection.size.width, gameObject->desiredPosition.y);
                    gameObject->velocity = Vec2(0.0f, gameObject->velocity.y);
                }
				else if (tileIndex == ETileGrid::RIGHT) // right tile
                {
                    gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x - intersection.size.width, gameObject->desiredPosition.y);
                    gameObject->velocity = Vec2(0.0f, gameObject->velocity.y);
                }
				else
				{
					if (intersection.size.width > intersection.size.height) 
					{
						//tile is diagonal, but resolving collision vertically
						gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);

						float resolutionHeight;
						
						if (tileIndex > ETileGrid::TOP_RIGHT)
						{
							resolutionHeight = intersection.size.height;
							gameObject->onGround = true;
						}
						else 
						{
							resolutionHeight = -intersection.size.height;
						}

						gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y + resolutionHeight);
					}
					else 
					{	
						//tile is diagonal, but resolving collision horizontally
                        //gameObject->velocity = Vec2(0.0f, gameObject->velocity.y);
                        
						float resolutionWidth;

						if (tileIndex == ETileGrid::TOP_LEFT || tileIndex == ETileGrid::BOTTOM_LEFT)
						{
							resolutionWidth = intersection.size.width;
						}
						else 
						{
							resolutionWidth = -intersection.size.width;
						}

						gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x + resolutionWidth, gameObject->desiredPosition.y);
					}
				}
			}
		}
	}

	// bounds check gameobject
	if (gameObjectBoundingBox.getMaxX() < 0)
		gameObject->desiredPosition = Vec2((mapSize.width * tileSize.width) - gameObjectBoundingBox.size.width, gameObject->desiredPosition.y);
	if (gameObjectBoundingBox.getMinX() > mapSize.width * tileSize.width)
		gameObject->desiredPosition = Vec2(0, gameObject->desiredPosition.y);
	if (gameObjectBoundingBox.getMinY() < 0)
	{
		this->removeAllChildrenWithCleanup(true);

		parallaxTileMap = ParallaxTileMap::create("planet1.tmx");
        parallaxTileMap->addObjects();
		this->addChild(parallaxTileMap);

		drawNode = DrawNode::create();
		parallaxTileMap->getObjectLayer()->addChild(drawNode, 99);

		followPlayer();
	}
	

	Vec2 gameObjectPosition = gameObject->getPosition();
	gameObjectPosition.x = gameObjectPosition.x + gameObject->getSize().width / 2;
	
	TileData ladder = Utils::getTileAtPosition(gameObjectPosition, *parallaxTileMap->getLadderLayer(), mapSize, tileSize);

	/*gameObject->isClimbing = false;
	gameObject->onLadderTop = false;*/

	//if (gameObject->collideLadder == false || gameObject->onGround)
	//{
	//	gameObject->climbLadder = false;
	//}

	//// if we fall onto the top of a ladder, we start climbing it
	//if (gameObject->prevCollideLadder == false && Map.IsTileLadder(m_collideLadder) && gameObject->onGround == false)
	//{
	//	gameObject->climbLadder = true;
	//	// gameobject animation
	//}

	//if (gameObject->onGround == false && m_OnGroundLast && m_vel.m_y > 0)
	//{
	//	m_animationController.PlayOnce(kFallAnim);
	//}


	if (ladder.gid > 0)
	{
		//
		ValueMap tileProperties = parallaxTileMap->getPropertiesForGID(ladder.gid).asValueMap();
		Rect tileRect = ladder.tile;

		drawNode->drawSolidRect
		(
			tileRect.origin,
			Vec2(tileRect.getMaxX(), tileRect.getMaxY()),
			Color4F(0.5f, 0.3f, 1.0f, 0.5f)
		);

		if (tileProperties["LadderTop"].asBool())
		{
			float diff = gameObjectPosition.y - tileRect.getMaxY();

			if (AppGlobal::getInstance()->states.UP)
			{
				gameObject->desiredPosition = Vec2(tileRect.getMidX() - gameObjectBoundingBox.size.width / 2, gameObject->desiredPosition.y + 1.0f);
				gameObject->isClimbing = true;
			}
			else if (AppGlobal::getInstance()->states.DOWN)
			{
				gameObject->desiredPosition = Vec2(tileRect.getMidX() - gameObjectBoundingBox.size.width / 2, gameObject->desiredPosition.y - 1.0f);
				gameObject->isClimbing = true;
			}
			else
			{
				gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y + diff);
				gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);
				gameObject->onLadderTop = true;
				gameObject->onGround = true;
				gameObject->isClimbing = false;

				AppGlobal::getInstance()->states.UP = false;
				AppGlobal::getInstance()->states.DOWN = false;
			}			
		}

		if (tileProperties["LadderMid"].asBool())
		{			
			if (AppGlobal::getInstance()->states.UP)
			{
				gameObject->desiredPosition = Vec2(tileRect.getMidX() - gameObjectBoundingBox.size.width / 2, gameObject->desiredPosition.y + 1.0f);
				gameObject->isClimbing = true;
			}
			else if (AppGlobal::getInstance()->states.DOWN)
			{
				gameObject->desiredPosition = Vec2(tileRect.getMidX() - gameObjectBoundingBox.size.width / 2, gameObject->desiredPosition.y - 1.0f);
				gameObject->isClimbing = true;
			}
		}
	}
	
	// update gameobject position
	gameObject->setPosition(gameObject->desiredPosition);
}
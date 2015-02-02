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

	
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(parallaxTileMap);
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
	parallaxTileMap->clearDebugDraw();

	// updates scale creates zoom effect
	this->setScale(global->scale);

	player->update(parallaxTileMap);
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
			
	// get the gameobject bounding box
	Rect gameObjectBoundingBox = gameObject->getCollisionBoundingBox();
	
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

		followPlayer();
	}
}
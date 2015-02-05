#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "Utils.h"

Level* Level::create(std::string mapName)
{
	// Create an instance of InfiniteParallaxNode
	Level* node = new Level();

	if (node) {
		// Add it to autorelease pool
		node->autorelease();
		node->loadMap(mapName);
		node->loadPlayer();
	}
	else {
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}

Level::Level()
{
	global = AppGlobal::getInstance();
	size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + size.width / 2, (origin.y + size.height / 2));
}

Level::~Level(){}

void Level::loadMap(std::string mapname)
{	
	this->removeAllChildrenWithCleanup(true);
	parallaxTileMap = ParallaxTileMap::create(mapname);
	parallaxTileMap->addObjects();
	this->addChild(parallaxTileMap);
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
	//this->setPosition(Vec2::ZERO);

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

void Level::scrollLevelMap(Vec2 position)
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

	this->setPosition(viewPoint);
}

void Level::update(float& delta)
{	
	// debug
	parallaxTileMap->clearDebugDraw();

	// updates scale creates zoom effect
	this->setScale(global->scale);
		
	// update gameobjects
	for (auto& gameObject : gameObjectList) 
		gameObject->update(parallaxTileMap);
    
	// player moves to next map
	this->checkNextMap(player);
	// centre viewport on player
	this->setViewPointCenter(player->getCenterPosition());
    
	Vec2 cursor = parallaxTileMap->convertToNodeSpaceAR(global->cursorLocation);
    Vec2 cursorPrevious = parallaxTileMap->convertToNodeSpaceAR(global->cursorDownLocation);

	if (global->leftMouseButton)
	{
		Vec2 scroll = cursorPrevious - cursor;
        Vec2 scrollStep = scroll * kUpdateInterval;

		Vec2 minMovement = Vec2(-440.0, -440.0);
		Vec2 maxMovement = Vec2(440.0, 440.0);
        //
		//scroll.clamp(minMovement, maxMovement);

        
		this->setPosition(this->getPosition() - scrollStep);

		parallaxTileMap->drawDebugRectAt(cursor, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
	}
	else
	{
		parallaxTileMap->drawDebugRectAt(cursor, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
	}
}

void Level::checkNextMap(GameObject* gameObject)
{	
	if (gameObject->getCollisionBoundingBox().getMinY() < 0)
	{
		loadMap("planet1.tmx");
		loadPlayer();
	}
}
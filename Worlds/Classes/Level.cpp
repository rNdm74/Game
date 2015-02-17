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
	winSize = Director::getInstance()->getWinSize();	
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
	center = Vec2(origin.x + visibleSize.width / 2, (origin.y + visibleSize.height / 2));
}


Level::~Level(){}


void Level::loadMap(std::string mapname)
{	
	this->removeAllChildrenWithCleanup(true);

	parallaxTileMap = ParallaxTileMap::create(mapname);
	parallaxTileMap->isLoaded = false;
	this->addChild(parallaxTileMap);

	mapSize = parallaxTileMap->getMapSize();
	tileSize = parallaxTileMap->getTileSize();
}


void Level::loadPlayer()
{	
	if (parallaxTileMap->isLoaded == false)
	{		
		player = parallaxTileMap->getPlayer();		
	}
	else
	{
		parallaxTileMap->getObjectLayer()->addChild(player);
		player->setPosition(parallaxTileMap->getMapTransition(player->mapTransition));
		player->mapTransition = Vec2::ZERO;
		player->path = nullptr;
	}
		
	this->setViewPointCenter(player->getCenterPosition());
}


void Level::setViewPointCenter(Vec2 position)
{	
	winSize = winSize / AppGlobal::getInstance()->scale;
	
    float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (mapSize.width * tileSize.width) - winSize.width / 2);
	y = MIN(y, (mapSize.height * tileSize.height) - winSize.height / 2);
    
	Vec2 actualPosition = Vec2(x, y);    
	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
	this->parallaxTileMap->setPosition(viewPoint);
}


void Level::update(float& delta)
{
	// updates scale creates zoom effect
	this->setScale( AppGlobal::getInstance()->scale );
		    
	//
    this->parallaxTileMap->update(delta);
    	
	// player moves to next map
	this->checkNextMap(player);

	// centre viewport on player
	this->setViewPointCenter(player->getCenterPosition());
}


void Level::checkNextMap(GameObject* gameObject)
{
    
    if ( player->mapTransition.y < 0 && player->isMovingDown)
	{
		player->retain();
        player->removeFromParent();
		player->release();
		                
		loadMap("planet1.tmx");

		parallaxTileMap->isLoaded = true;

		loadPlayer();  


	}
	else if (player->mapTransition.y > 0 && player->isMovingUp)
	{
		player->retain();
		player->removeFromParent();
		player->release();
		
		loadMap(kLevelTMX);

		parallaxTileMap->getObjectLayer()->removeChildByName("Player", true);
		parallaxTileMap->isLoaded = true;

		loadPlayer();
	}


}
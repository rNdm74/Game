#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "PathFinder.h"

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
	_origin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();
	_center = Vec2(_origin.x + _visibleSize.width / 2, (_origin.y + _visibleSize.height / 2));	
	
	_fsm = new GameObjectFsm();
}


Level::~Level(){}


void Level::loadMap(std::string mapname)
{	
	this->removeAllChildrenWithCleanup(true);

	_parallaxTileMap = ParallaxTileMap::create(mapname);
	this->addChild(_parallaxTileMap);

	_mapSize = _parallaxTileMap->getMapSize();
	_tileSize = _parallaxTileMap->getTileSize();

	_pathFinder = new AStarPathFinder(_parallaxTileMap, 500, false);
}


void Level::loadPlayer()
{	
	if (_parallaxTileMap->isPlayerLoaded == false)
	{		
		//player = _parallaxTileMap->getPlayer();
	}
	else
	{
		//_parallaxTileMap->getObjectLayer()->addChild(&player);
		//Vec2 transition = _parallaxTileMap->getMapTransition(player.getMapTransition());

		//player.setPositionX(transition.x - player.getContentSize().width / 2);
		//player.setPositionY(transition.y);
	}
		
	this->setViewPointCenter();
}


void Level::setViewPointCenter()
{	
	Vec2 position = _parallaxTileMap->getPlayer().getCenterPosition();

	Size _winSize = Director::getInstance()->getWinSize() / this->getScale();
	
	float x = MAX(position.x, _winSize.width / 2);
	float y = MAX(position.y, _winSize.height / 2);
	x = MIN(x, (_mapSize.width * _tileSize.width) - _winSize.width / 2);
	y = MIN(y, (_mapSize.height * _tileSize.height) - _winSize.height / 2);
    
	Vec2 actualPosition = Vec2(x, y);    
	Vec2 centerOfView = Vec2(_winSize.width / 2, _winSize.height / 2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
	this->_parallaxTileMap->setPosition(viewPoint);
}


void Level::update(float& delta)
{
	// updates scale creates zoom effect
	this->setScale(AppGlobal::getInstance()->scale);
	//
	this->updateState();
	//
	_parallaxTileMap->update(delta);
	// player moves to next map
	this->checkNextMap();
	// centre viewport on player
	this->setViewPointCenter();
}


void Level::updateState()
{
	void(GameObjectFsm:: *ptrs[])() =
	{
		&GameObjectFsm::CheckCanClimbUp,
		&GameObjectFsm::CheckCanClimbDown,
		&GameObjectFsm::CheckCanWalkLeft,
		&GameObjectFsm::CheckCanWalkRight,
		&GameObjectFsm::Stop,
		&GameObjectFsm::LoadNextMap,
		&GameObjectFsm::LoadPreviousMap
	};

	(_fsm->*ptrs[AppGlobal::getInstance()->gameObjectStates])();
};


void Level::checkNextMap()
{
	IGameObject& player = _parallaxTileMap->getPlayer();

	Vec2 transition = player.getMapTransition();
	IsMoving isMoving = player.getIsMoving();

	if (transition.y < 0 && isMoving[STATE_DOWN])
	{		
		player.removeFromParent();
		                
		loadMap("planet1.tmx");

		_parallaxTileMap->isPlayerLoaded = true;

		loadPlayer();  
	}
	else if (transition.y > 0 && isMoving[STATE_UP])
	{
		player.removeFromParent();
		
		loadMap(kLevelTMX);

		_parallaxTileMap->getObjectLayer()->removeChildByName("Player", true);
		_parallaxTileMap->isPlayerLoaded = true;

		loadPlayer();
	}
}
#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Level.h"
#include "ParallaxTileMap.h"
#include "ParallaxFactory.h"
#include "PathFinder.h"
#include "Utils.h"

Level* Level::create()
{
	// Create an instance of Level
	Level* node = new Level();

	if (node) 
	{
		// Add it to autorelease pool
		node->autorelease();
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
	//
	activeMap = AppGlobal::getInstance()->activeMap;
	activeMap.push(ParallaxFactory::create("Planet"));
	//
	this->addChild(activeMap.top());

	this->loadPlayer();
}

void Level::loadMap()
{
	//
	if (activeMap.size() > 0)
	{
		//
		if (activeMap.top()->getPlayer() != nullptr)
			activeMap.top()->getPlayer()->removeFromParent();
		//
		this->removeChild(activeMap.top());
	}	
	//
	activeMap.push(ParallaxFactory::create("Cave"));
	//
	this->addChild(activeMap.top());
}

void Level::loadPlayer()
{
	ValueMap properties = activeMap.top()->getInitialProperties();	

	activeMap.top()->addPlayer(GameObjectFactory::create("Player", properties));
}

void Level::update(float& delta)
{
	/** Updates scale creates zoom effect **/
	this->setScale(AppGlobal::getInstance()->getScale());	
	/** Update the active map **/
	activeMap.top()->update(delta);
	/** Keep active map position centered on player **/
	activeMap.top()->setPositionOnPlayer();
}

void Level::checkNextMap()
{
	/*IGameObject* player = _tileMap->getPlayer();

	Vec2 transition = player->getMapTransition();
	IsMoving isMoving = player->getIsMoving();

	if (transition.y < 0 && isMoving[STATE_DOWN])
	{		
		player->removeFromParent();
		                
		loadMap();
				
		loadPlayer();  
	}
	else if (transition.y > 0 && isMoving[STATE_UP])
	{
		player->removeFromParent();
		
		loadMap();
		
		loadPlayer();
	}*/
}
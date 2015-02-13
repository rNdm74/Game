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
	this->addChild(parallaxTileMap);

	mapSize = parallaxTileMap->getMapSize();
	tileSize = parallaxTileMap->getTileSize();
}


void Level::loadPlayer()
{	
	player = parallaxTileMap->getPlayer();
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
    
	this->parallaxTileMap->setPosition(viewPoint);
}

void Level::renderViewPort()
{
	for (auto& child : this->parallaxTileMap->getChildren())
	{
		TMXLayer* layer = static_cast<TMXLayer*>(child);

		Size s = layer->getLayerSize();

		for (int col = 0; col < s.width; ++col)
		{
			for (int row = 0; row < s.height; ++row)
			{
				Sprite* tile = layer->getTileAt(Vec2(col, row));

				if (tile)
				{
					Vec2 tileOrigin = parallaxTileMap->convertToWorldSpaceAR(tile->getPosition());

					if (tileOrigin.x < origin.x + visibleSize.width && tileOrigin.x > -tileSize.width &&
						tileOrigin.y < origin.y + visibleSize.height && tileOrigin.y > -tileSize.height)
					{
						tile->setVisible(true);
					}
					else
					{
						tile->setVisible(false);
					}
				}
			}
		}
	}
}

void Level::update(float& delta)
{
	// updates scale creates zoom effect
	this->setScale( AppGlobal::getInstance()->scale );
		
	// debug
	this->parallaxTileMap->clearDebugDraw();

	//
    this->parallaxTileMap->update(delta);
    	
	// player moves to next map
	this->checkNextMap(player);

	// centre viewport on player
	this->setViewPointCenter(player->getCenterPosition());

	//	
	Vec2 cursor = parallaxTileMap->convertToNodeSpaceAR(AppGlobal::getInstance()->cursorLocation);
    parallaxTileMap->drawDebugRectAt(cursor, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
    
    if(AppGlobal::getInstance()->mouseDown)
    {
        
    }
    
	//Vec2 cursorPrevious = parallaxTileMap->convertToNodeSpaceAR(global->cursorDownLocation);
	//	
	//if (global->leftMouseButton)
	//{
	//	Vec2 cursorPosition = global->cursorLocation;
	//	//log("cursorPosition - x:%f, y:%f", cursorPosition.x, cursorPosition.y);

	//	Vec2 position = this->getPosition();
	//	Vec2 parentPosition = this->getParent()->getPosition();
	//	Vec2 diff = position - parentPosition;
	//	//log("diff - x:%f, y:%f", diff.x, diff.y);

	//	
	//	Vec2 scrollStep = cursor - cursorPrevious;
	//	//log("scrollStep - x:%f, y:%f", scrollStep.x, scrollStep.y);

	//	Vec2 distance = diff + scrollStep;
	//	//log("distance - x:%f, y:%f", distance.x, distance.y);
	//			
	//	this->setPosition(scrollStep);

    
	//}
	//else
	//{
	//	parallaxTileMap->drawDebugRectAt(cursor, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
	//}
}


void Level::checkNextMap(GameObject* gameObject)
{	
	if ( gameObject->getCollisionBoundingBox().getMinY() < 0 )
	{
		loadMap("planet1.tmx");
		loadPlayer();
	}
}
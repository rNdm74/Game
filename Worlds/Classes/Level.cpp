#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Level.h"

void Level::loadMap(std::string mapname)
{
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	
	global = AppGlobal::getInstance();
	size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + size.width / 2, (origin.y + size.height / 2));
		
	map = TMXTiledMap::create(mapname);
	map->retain();

	mapInfo = TMXMapInfo::formatWithTMXFile(mapname.c_str());
	tileProperties = mapInfo->getTileProperties();

	this->load();

	drawNode = DrawNode::create();
	this->addChild(drawNode, 99);
}

void Level::load()
{
	parallaxNode = ParallaxNode::create();
	parallaxNode->setContentSize(map->getContentSize());
	parallaxNode->setAnchorPoint(Vec2::ZERO);
	this->addChild(parallaxNode);
		
	backgroundLayer = map->getLayer("background");
	backgroundLayer->retain();
	backgroundLayer->removeFromParentAndCleanup(false);
	backgroundLayer->setScale(1.0f);

	parallaxNode->addChild(backgroundLayer, -1, Vec2(0.4f, 0.4f), Vec2(0, 0));
	backgroundLayer->getTexture()->setAntiAliasTexParameters();
	backgroundLayer->release();

	float top = backgroundLayer->getBoundingBox().getMaxY();

	unsigned int rocksQuantity = 3;
	for (unsigned int i = 0; i < rocksQuantity; i++)
	{
		Sprite* rockGrassDown = Sprite::createWithSpriteFrameName("rockDown.png");
		rockGrassDown->setAnchorPoint(Vec2(0.5, 1));
		rockGrassDown->setScale(RAND(0.45f, 0.65f));
		rockGrassDown->setTag(1000 + i);

		Size s = rockGrassDown->getContentSize();

		auto shadow = getShadowForNode(rockGrassDown);
		shadow->setAnchorPoint(Vec2(-0.05f, 0.01f));
		shadow->setPosition(rockGrassDown->getPosition());
		rockGrassDown->addChild(shadow, -1);

		//parallaxNode->addChild
		//	(
		//	rockGrassDown,
		//	// Set z-index of rock
		//	3,
		//	// Set ration (rocks don't move)
		//	Vec2(0.0f, 0.4f),
		//	// Set position with random component
		//	Vec2(((size.width / 2) / rocksQuantity) * (i)+RAND(0, s.width / 2), top)
		//	);

		rockGrassDown->getTexture()->setAntiAliasTexParameters();
	}

	for (unsigned int i = 0; i < rocksQuantity; i++)
	{
		Sprite* rockGrassDown = Sprite::createWithSpriteFrameName("rockDown.png");
		rockGrassDown->setAnchorPoint(Vec2(0.5, 1));
		rockGrassDown->setScale(RAND(0.45f, 0.65f));
		rockGrassDown->setTag(1000 + i);

		Size s = rockGrassDown->getContentSize();

		auto shadow = getShadowForNode(rockGrassDown);
		shadow->setAnchorPoint(Vec2(-0.05f, 0.01f));
		shadow->setPosition(rockGrassDown->getPosition());
		rockGrassDown->addChild(shadow, -1);

		//parallaxNode->addChild
		//	(
		//	rockGrassDown,
		//	// Set z-index of rock
		//	3,
		//	// Set ration (rocks don't move)
		//	Vec2(0.0f, 0.4f),
		//	// Set position with random component
		//	Vec2
		//	(
		//	(size.width / 1.6) + ((size.width / 2) / rocksQuantity) * (i)+RAND(0, s.width / 2),
		//	top
		//	)
		//	);

		rockGrassDown->getTexture()->setAntiAliasTexParameters();
	}

	Sprite* groundGrass = Sprite::createWithSpriteFrameName("groundDirt.png");
	groundGrass->setAnchorPoint(Vec2(0, 0));
	auto shadow = getShadowForNode(groundGrass);
	shadow->setAnchorPoint(Vec2(-0.005f, 0.01f));
	shadow->setPosition(groundGrass->getPosition());
	groundGrass->addChild(shadow, -1);
	//parallaxNode->addChild(groundGrass, 3, Vec2(0.0f, 0.4f), Vec2(0, 0));
	groundGrass->getTexture()->setAntiAliasTexParameters();
	groundGrass = Sprite::createWithSpriteFrameName("groundDirt.png");
	groundGrass->setAnchorPoint(Vec2(0, 0));
	//parallaxNode->addChild(groundGrass, 3, Vec2(0.0f, 0.4f), Vec2(groundGrass->getContentSize().width, 0));

	foregroundLayer = map->getLayer("foreground");
	foregroundLayer->retain();
	foregroundLayer->removeFromParentAndCleanup(false);
	parallaxNode->addChild(foregroundLayer, 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	foregroundLayer->release();

	Node* shadowLayer = Node::create();
	parallaxNode->addChild(shadowLayer, 0, Vec2(1.0f, 1.0f), Vec2::ZERO);

	Size layerSize = foregroundLayer->getLayerSize();
	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			auto tileSprite = foregroundLayer->getTileAt(Point(x, y));

			if (tileSprite)
			{
				auto shadow = getShadowForNode(tileSprite);
				shadow->setPosition(shadowLayer->convertToWorldSpace(tileSprite->getPosition()));
				shadowLayer->addChild(shadow);
			}
		}
	}

	collisionLayer = Node::create();
	parallaxNode->addChild(collisionLayer, 2, Vec2(1.0f, 1.0f), Vec2::ZERO);
}

void Level::followPlayer()
{	
	player = static_cast<Player*>(collisionLayer->getChildByName("Player"));

	gameObjectList.push_back(player);

    this->setViewPointCenter(player->getPosition());
}

void Level::setAliasTexParameters(TMXLayer* layer)
{
	Size layerSize = layer->getLayerSize();

	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			auto tileSprite = layer->getTileAt(Point(x, y));

			if (tileSprite)
			{
				tileSprite->getTexture()->setAliasTexParameters();
			}
		}
	}
}

Node* Level::getShadowForNode(Node* node)
{
	auto shadow = Sprite::create();

	auto object = static_cast<Sprite*>(node);

	shadow->setSpriteFrame(object->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.05f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);
	
	return shadow;
}

Level::Level()
{

}

Level::~Level(void)
{
	map->release();
}

void Level::addObjects()
{   
	// loop over the object groups in this tmx file
	auto objectGroups = map->getObjectGroups();
    
    for (auto& objectGroup : objectGroups)
	{
		auto objects = objectGroup->getObjects();
        
        for (auto& object : objects)
		{
			auto properties = object.asValueMap();
			auto type = properties.at("type");

			if (!type.isNull())
            {
				this->addObject(type.asString().c_str(), properties);
				this->objectCount++;
			}
		}
	}	
}

GameObject* Level::addObject(std::string className, ValueMap& properties)
{		
	// create the object
	GameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{
		o->setName(className);
		
		collisionLayer->addChild(o);
	}
		
	//
	return o;
}

void Level::setViewPointCenter(Vec2 position)
{
    Size mapSize = map->getMapSize();
    Size tileSize = map->getTileSize();
	Size winSize = Director::getInstance()->getWinSize() / this->getScale();
	
    float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (mapSize.width * tileSize.width) - winSize.width / 2);
	y = MIN(y, (mapSize.height * tileSize.height) - winSize.height / 2);
    
	Vec2 actualPosition = Vec2(x, y);
    
	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
	parallaxNode->setPosition(viewPoint);
}

void Level::update(float& delta)
{	
	// updates scale creates zoom effect
	this->setScale(global->scale);

	// update gameobjects
	for (auto& gameObject : gameObjectList) 
    {
        gameObject->update(this);

        checkForAndResolveCollisions(gameObject);
    }
    
	// check collisions in level
	//checkCollisions();
    
	// centre view port on player
	this->setViewPointCenter(player->getPosition());
}

Vec2 Level::tileCoordForPosition(Vec2 position)
{
	Size tileSize = map->getTileSize();
	Size mapSize = map->getMapSize();

	float x = floor(position.x / tileSize.width);

	float levelHeightInPixels = mapSize.height * tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / tileSize.height);

	return Vec2(x, y - 1);
}

Rect Level::tileRectFromTileCoords(Vec2 tileCoords)
{
	Size tileSize = map->getTileSize();
	Size mapSize = map->getMapSize();

	float levelHeightInPixels = mapSize.height * tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * tileSize.height));

	return Rect(origin.x, origin.y, tileSize.width, tileSize.height);
}

Rect Level::RectIntersection(Rect r1, Rect r2)
{
	Rect intersection;

	intersection = Rect(std::max(r1.getMinX(), r2.getMinX()), std::max(r1.getMinY(), r2.getMinY()), 0, 0);
	intersection.size.width = std::min(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = std::min(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();

	return intersection;
}

std::vector<TileData*> Level::getSurroundingTilesAtPosition(Vec2 position, TMXLayer* layer)
{
	Vec2 gameObjectPosition = tileCoordForPosition(position);
    
	std::vector<TileData*> gids;

	for (int i = 0; i < 9; i++) 
	{
		int column = i % 3;
		int row = (int)(i / 3); 

		Vec2 tilePos = Vec2(gameObjectPosition.x + (column - 1), gameObjectPosition.y + (row - 1));

		// if its a valid tilepos for layer
        if(tilePos.x < map->getMapSize().width && tilePos.x >= 0 &&  tilePos.y < map->getMapSize().height && tilePos.y >= 0)
        {
			// 
            int tgid = layer->getTileGIDAt(tilePos);
            
			if (tgid)
			{
				Rect tileRect = tileRectFromTileCoords(tilePos);

				TileData* tileData = new TileData();
				tileData->gid = tgid;
				tileData->x = tileRect.origin.x;
				tileData->y = tileRect.origin.y;
				tileData->pos = tilePos;

				gids.push_back(tileData);
            }
			else
			{
                gids.push_back(nullptr);
			}			
        }
	}
    
	// delete the center tile
	gids.erase(gids.begin() + 4);

    // swap bottom tile to 0
    auto temp = gids[0];
    gids[0] = gids[6];
    gids[6] = temp;
    
    temp = gids[2];
    gids[2] = gids[3]; // swap 3 -> 2
    gids[3] = gids[4]; // swap 5 -> 3
	gids[4] = gids[6]; // swap 2 -> 5
	gids[6] = gids[5];
	gids[5] = temp;

    /* 
	 * OLD | NEW
	 * --- + ---
     * 012 | 415
     * 3 4 | 2 3
     * 567 | 607
     */
    
	return gids;
}

void Level::checkForAndResolveCollisions(GameObject* gameObject)
{
	std::vector<TileData*> tiles = getSurroundingTilesAtPosition(gameObject->getPosition(), foregroundLayer);
		
	gameObject->onGround = false;
	gameObject->canJump = false;

	Rect gameObjectBoundingBox = gameObject->getCollisionBoundingBox();
	// draw gameObject bounding box

    drawNode->clear();
    
	for (int tileIndex = ETileGrid::BOTTOM; tileIndex < tiles.size(); tileIndex++)
	{
		TileData* tileData = tiles[tileIndex];

		if (tileData)
		{	
			Rect tileRect = Rect(tileData->x, tileData->y, map->getTileSize().width, map->getTileSize().height);

            drawNode->drawRect
            (
                foregroundLayer->convertToWorldSpace(tileRect.origin),
                Vec2(tileRect.origin.x + 70, tileRect.origin.y + 70),
                Color4F(1.0f, 0.3f, 0.3f, 1)
            );
            
			if (tileRect.intersectsRect(gameObjectBoundingBox))
			{
				Rect intersection = RectIntersection(gameObjectBoundingBox, tileRect);
			
				if (tileIndex == ETileGrid::BOTTOM) // tile is below gameobject
				{					
					gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y + intersection.size.height);
					gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);					
					gameObject->onGround = true;
				}
				else if (tileIndex == ETileGrid::TOP) // top tile
                {
                    gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y - intersection.size.height);
                    gameObject->velocity = Vec2(gameObject->velocity.x, 0.0f);
                }
				else if (tileIndex == ETileGrid::LEFT) // left tile
                {
                    gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x + intersection.size.width, gameObject->desiredPosition.y);
                }
				else if (tileIndex == ETileGrid::RIGHT) // right tile
                {
                    gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x - intersection.size.width, gameObject->desiredPosition.y);
                }
				else
				{
					//if (intersection.size.width > intersection.size.height) 
					//{
					//	//tile is diagonal, but resolving collision vertically
					//	gameObject->velocity = Vec2(gameObject->velocity.x, 0.0);

					//	float resolutionHeight;
					//	
					//	if (i == 6 || i ==7)
					//	{
					//		resolutionHeight = -intersection.size.height;
					//		gameObject->onGround = true;
					//	}
					//	else 
					//	{
					//		resolutionHeight = intersection.size.height;
					//	}

					//	gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x, gameObject->desiredPosition.y + resolutionHeight);
					//}
					//else 
					//{
					//	//tile is diagonal, but resolving collision horizontally
					//	gameObject->velocity = Vec2(0.0, gameObject->velocity.y);

					//	float resolutionWidth;

					//	if (i == 4 || i == 5) 
					//	{
					//		resolutionWidth = intersection.size.width;
					//	}
					//	else 
					//	{
					//		resolutionWidth = -intersection.size.width;
					//	}

					//	gameObject->desiredPosition = Vec2(gameObject->desiredPosition.x + resolutionWidth, gameObject->desiredPosition.y);

					//}
				}
			}
		}
	}

	gameObject->setPosition(gameObject->desiredPosition);
}
#include "Components.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Pathfinding.h"

USING_NS_CC;

ExtendedTMXTiledMap* ExtendedTMXTiledMap::create(const std::string& tmxFile)
{
	ExtendedTMXTiledMap *ret = new (std::nothrow) ExtendedTMXTiledMap();
	if (ret->initWithTMXFile(tmxFile))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

ExtendedTMXTiledMap::ExtendedTMXTiledMap()
{
    source = Vec2::ZERO;
    destination = Vec2::ZERO;	
};

ExtendedTMXTiledMap::~ExtendedTMXTiledMap()
{
	floorLayer->release();
    
    //delete pathfinder;
};

void ExtendedTMXTiledMap::update(float delta)
{
	if (playerInstance == nullptr)
		return;

	//this->setPositionOnPlayer(playerInstance->getBoundingBox());
};

void ExtendedTMXTiledMap::setPositionOnPlayer(Rect collisionBox)
{
	//Vec2 v = this->getPlayer()->getVelocity();
	Rect r = collisionBox; // this->getPlayer()->getCollisionBox();
	/** Bottom middle of sprite **/
	Vec2 p = Vec2(r.origin.x + (r.size.width / 2), r.origin.y);
	//p.x += v.x * kUpdateInterval;

	float s = this->getParent()->getScale();// *CC_CONTENT_SCALE_FACTOR();

	Size m = _mapSize;
	Size t = _tileSize;
	Size w = Director::getInstance()->getWinSize() * CC_CONTENT_SCALE_FACTOR();

	float x = MAX(p.x, w.width / 2);
	float y = MAX(p.y, w.height / 2);
	x = MIN(x, (m.width * t.width) - w.width / 2);
	y = MIN(y, (m.height * t.height) - w.height / 2);

	Vec2 ap = Vec2(x, y);
	Vec2 cov = Vec2(w.width / 2, w.height / 2);
	Vec2 vp = cov - ap;

	this->setPosition(vp);
};

void ExtendedTMXTiledMap::initGameObjects()
{
	floorLayer = this->getLayer("floor");
	floorLayer->retain();
	
	// loop over the object groups in this tmx file
	for (auto& objectGroup : this->getObjectGroups())
	{
		for (auto& object : objectGroup->getObjects())
		{
			auto properties = object.asValueMap();
			auto type = properties.at("type");

			if (type.isNull() == false)
			{
				if (initGameObject(type.asString(), properties))
				{
					_objectCount++;
				}
			}
		}
	}

	playerInstance = static_cast<IGameObject*>(this->getChildByTag(TAG_PLAYER));
	log("Loaded objects \n player instance is now loaded");
};

bool ExtendedTMXTiledMap::initGameObject(std::string className, ValueMap& properties)
{
	// create the object
	IGameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{		
		Vec2 tileCoord = this->getTileCoordFrom(o);				
		Vec2 tilePos = floorLayer->getPositionAt(tileCoord);
						
		o->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		o->setPosition(tilePos);
						
		this->addChild(o);		
		this->setChildZOrder(o, tileCoord);
        
		return true;
	}

	return false;
};

void ExtendedTMXTiledMap::setChildZOrder(IGameObject* gameObject, Vec2 tileCoord)
{
	float lowestZ = _mapSize.width + _mapSize.height;
	float currentZ = tileCoord.x + tileCoord.y;
	int zOrderDecided = lowestZ + currentZ - 1;
	this->reorderChild(gameObject, zOrderDecided);
};

void ExtendedTMXTiledMap::selectTile(cocos2d::Vec2 coord)
{
	Sprite* tile = floorLayer->getTileAt(coord);

	if (tile)
	{
		tile->setColor(ccc3(100, 100, 100));
	}

};

void ExtendedTMXTiledMap::deselectTile(cocos2d::Vec2 coord)
{
	Sprite* tile = floorLayer->getTileAt(coord);

	if (tile)
	{
		tile->setColor(ccc3(255, 255, 255));
	}

};

Vec2 ExtendedTMXTiledMap::getTileCoordFrom(IGameObject* gameObject)
{	
	Vec2 position = gameObject->getPosition();
	Size size = gameObject->getObjectSize();

	// Calculate the map height in pixels
	float mapHeightPixels = _mapSize.height *_tileSize.height;

	// Convert the y position to tiled position
	float flippedY = (mapHeightPixels - position.y) - _tileSize.height;

	// Calculate the x and y coordinates - 1 for zero based coordinate
	float x = floor(position.x / size.width) - 1;
	float y = floor(flippedY / size.height) - 1;

	return Vec2(x,y);
};

Vec2 ExtendedTMXTiledMap::getTileCoordFrom(Vec2 position)
{
	Vec2 pos = position;
	float halfMapWidth = _mapSize.width * 0.5;
	float mapHeight = _mapSize.height;
	float tileWidth = _tileSize.width;
	float tileHeight = _tileSize.height;

	Vec2 tilePosDiv = Vec2(pos.x / tileWidth, pos.y / tileHeight);
	float invereseTileY = mapHeight - tilePosDiv.y;

	// Cast int to make sure that result is in whole numbers

	float posX = (int)(invereseTileY + tilePosDiv.x - halfMapWidth);
	float posY = (int)(invereseTileY - tilePosDiv.x + halfMapWidth);

	return Vec2(posX, posY);
};

bool ExtendedTMXTiledMap::isBlocked(Vec2 coordinate)
{
    TMXLayer* layer = this->getLayer("collision");
    
    Sprite* tile = layer->getTileAt(coordinate);
    
    return (tile != nullptr);
}

float ExtendedTMXTiledMap::getCost(Vec2 startLocation, Vec2 targetLocation)
{
    return 1;
}

IPath* ExtendedTMXTiledMap::getPath(Vec2 startLocation, Vec2 targetLocation)
{
	pathfinder = new AStarPathFinder(this, 100, false, new ClosestHeuristic());

    return pathfinder->findPath(startLocation, targetLocation);
};

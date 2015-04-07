#include "Components.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Player.h"
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
    _debugLayer = DrawNode::create();
    this->addChild(_debugLayer, 99);
};

ExtendedTMXTiledMap::~ExtendedTMXTiledMap()
{
	//floorLayer->release();    
    //delete pathfinder;
};

void ExtendedTMXTiledMap::update(float delta)
{
	if (playerInstance == nullptr)
		return;

	for (auto child : this->getChildren())
	{
		child->update(delta);
	}

	//this->setPositionOnPlayer();
};

void ExtendedTMXTiledMap::setPositionOnPlayer()
{	
	Vec2 playerPosition = playerInstance->getPosition();
	Size playerSize = playerInstance->getObjectSize() / 2;
	Vec2 p = this->convertToWorldSpace(Vec2(playerPosition.x + playerSize.width, playerPosition.y + playerSize.height));
		
	Size m = _mapSize;
	Size t = _tileSize;
	Size w = Director::getInstance()->getWinSize();

	float x = MAX(p.x, w.width / 2);
	float y = MAX(p.y, w.height / 2);
	x = MIN(x, (m.width * t.width) - w.width / 2);
	y = MIN(y, (m.height * t.height) - w.height / 2);

	Vec2 ap = Vec2(x, y);
	Vec2 cov = Vec2(w.width / 2, w.height / 2);
	Vec2 vp = cov - ap;

	log("Map - x:%f, y:%f", this->getPositionX(), this->getPositionY());
	this->setPosition(vp);
};

void ExtendedTMXTiledMap::initGameObjects()
{
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

	playerInstance = static_cast<Player*>(this->getChildByTag(TAG_PLAYER));
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
		Vec2 tilePos = this->getLayer("ground")->getPositionAt(tileCoord);
						
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
    Sprite* tile = this->getLayer("ground")->getTileAt(coord);

	if (tile)
	{
		this->drawRect(tile->getBoundingBox());
		//tile->setColor(ccc3(100, 100, 100));
	}

};

void ExtendedTMXTiledMap::deselectTile(cocos2d::Vec2 coord)
{
	_debugLayer->clear();

	Sprite* tile = this->getLayer("ground")->getTileAt(coord);

	if (tile)
	{		
		//tile->setColor(ccc3(255, 255, 255));
	}

};

void ExtendedTMXTiledMap::movePlayerAlongPath(IPath* path)
{
	if (path)
	{
		playerInstance->moveTo(path->pop_back());
	}	
};

Vec2 ExtendedTMXTiledMap::getPlayerPosition()
{ 
	return this->convertToWorldSpace(playerInstance->getCenterPosition()); 
};

void ExtendedTMXTiledMap::selectPlayer() 
{ 
	playerInstance->Selected = true; 
	playerInstance->setColor(ccc3(100, 100, 100));
};
void ExtendedTMXTiledMap::deselectPlayer() 
{ 
	playerInstance->Selected = false; 
	playerInstance->setColor(ccc3(255, 255, 255));
};

bool ExtendedTMXTiledMap::playerIsSelected() 
{ 
	return playerInstance->Selected; 
};

void ExtendedTMXTiledMap::playerSetPath()
{ 
	//playerInstance->ActivePath = true; 
};
void ExtendedTMXTiledMap::playerUnSetPath()
{ 
	//playerInstance->ActivePath = false; 
};
bool ExtendedTMXTiledMap::playerHasActivePath()
{ 
	return false;
	//return playerInstance->ActivePath; 
};

Vec2 ExtendedTMXTiledMap::getTileCoordFrom(IGameObject* gameObject)
{	
	Vec2 position = gameObject->getPosition() - this->getPosition();
	Size size = gameObject->getObjectSize();

	// Calculate the map height in pixels
	float mapHeightPixels = _mapSize.height *_tileSize.height;

	// Convert the y position to tiled position
	float flippedY = (mapHeightPixels - position.y) - _tileSize.height;

	// Calculate the x and y coordinates - 1 for zero based coordinate
	float x = floor(position.x / size.width);
	float y = floor(flippedY / size.height);

	return Vec2(x,y);
};

Vec2 ExtendedTMXTiledMap::getTileCoordFrom(Vec2 position)
{
	Vec2 pos = position - this->getPosition();

	float halfMapWidth = _mapSize.width * 0.5;
	float mapHeight = _mapSize.height;
	float tileWidth = _tileSize.width;
	float tileHeight = _tileSize.height;

	Vec2 tilePosDiv = Vec2(pos.x / tileWidth, pos.y / tileHeight);
	float invereseTileY = mapHeight - tilePosDiv.y;

	// Cast int to make sure that result is in whole numbers

	float coordX = (int)(invereseTileY + tilePosDiv.x - halfMapWidth);
	float coordY = (int)(invereseTileY - tilePosDiv.x + halfMapWidth);

	return Vec2(coordX, coordY);
};

bool ExtendedTMXTiledMap::isBlocked(Vec2 coordinate)
{       
	return (this->getLayer("collision")->getTileAt(coordinate) != nullptr);
}

float ExtendedTMXTiledMap::getCost(Vec2 startLocation, Vec2 targetLocation)
{
    return 1;
}

IPath* ExtendedTMXTiledMap::findPath(Vec2 sourceCoordinate, Vec2 targetCoordinate)
{
	pathfinder = new AStarPathFinder(this, 100, false, new ClosestHeuristic());
				
	return pathfinder->findPath(sourceCoordinate, targetCoordinate);
};

bool ExtendedTMXTiledMap::isTileCoordValid(Vec2 coord)
{
	return (coord.y < _mapSize.height && coord.y >= 0 && coord.x < _mapSize.width && coord.x >= 0);
};

void ExtendedTMXTiledMap::drawRect(Rect r)
{
	Vec2* points = new Vec2[5];
		
	// left
	points[0] = Vec2(r.getMinX(), r.getMidY());
	points[1] = Vec2(r.getMidX(), r.getMinY());
	_debugLayer->drawLine(points[0], points[1], DEBUG_RECT_COLOR);

	// bottom	
	points[2] = Vec2(r.getMaxX(), r.getMidY());
	_debugLayer->drawLine(points[1], points[2], DEBUG_RECT_COLOR);
		
	// right
	points[3] = Vec2(r.getMidX(), r.getMaxY());	
	_debugLayer->drawLine(points[2], points[3], DEBUG_RECT_COLOR);

	// top
	points[4] = Vec2(r.getMinX(), r.getMidY());
	_debugLayer->drawLine(points[3], points[4], DEBUG_RECT_COLOR);

	_debugLayer->drawSolidPoly(points, 5, DEBUG_RECT_COLOR);

	_debugLayer->drawPoint(Vec2(r.getMidX(), r.getMidY()), 2.0f, DEBUG_RECT_COLOR);
};

Rect ExtendedTMXTiledMap::getTileRectFrom(Vec2 coord)
{
	return this->getLayer("ground")->getTileAt(coord)->getBoundingBox();
};
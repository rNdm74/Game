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
	playerPath = nullptr;
};

void ExtendedTMXTiledMap::update(float delta)
{	
	for (auto child : this->getChildren())
	{
		child->update(delta);
	}	
};

void ExtendedTMXTiledMap::setPositionOnPlayer()
{	
	Vec2 p1 = playerInstance->getCenterPosition();
	Vec2 p2 = this->convertToWorldSpace(p1);
	Vec2 p = p2 - this->getPosition();
		
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
		this->drawRect(tile->getBoundingBox(), DEBUG_RECT_COLOR);
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

void ExtendedTMXTiledMap::movePlayerAlongPath(IPath& path)
{
	playerInstance->moveAlong(path);
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

float ExtendedTMXTiledMap::getCost(Vec2 startLocation, Vec2 neighbourCoordinate)
{	
	// Initial cost
	float cost = 10.0f;

	// Diagonal tile
	float tx = abs(neighbourCoordinate.x - startLocation.x);
	float ty = abs(neighbourCoordinate.y - startLocation.y);
				
	// Diagonal tile and there are no collision tiles surrounding it
	if (tx == 1 && ty == 1)
	{
		cost = 14.0f; // higher cost		
	}	

	//log("tx: %f, ty:%f", tx, ty);	
	//log("cost: %f", cost);
	
	return cost;
}

int ExtendedTMXTiledMap::calculateCostFactor(TileDataArray tileDataArray)
{
	int costFactor = 0;

	for (auto t : tileDataArray)
		if (t.GID > 0) costFactor++;

	return costFactor;
};

IPath* ExtendedTMXTiledMap::findPath(Vec2 sourceCoordinate, Vec2 targetCoordinate)
{
	pathfinder = new AStarPathFinder(this, 100, true, new ClosestHeuristic());
				
	return pathfinder->findPath(sourceCoordinate, targetCoordinate);
};

bool ExtendedTMXTiledMap::isTileCoordValid(Vec2 coord)
{
	return (coord.y < _mapSize.height && coord.y >= 0 && coord.x < _mapSize.width && coord.x >= 0);
};

void ExtendedTMXTiledMap::drawRect(Rect r, Color4F color)
{
	Vec2* points = new Vec2[5];
		
	// left
	points[0] = Vec2(r.getMinX(), r.getMidY());
	points[1] = Vec2(r.getMidX(), r.getMinY());
	_debugLayer->drawLine(points[0], points[1], color);

	// bottom	
	points[2] = Vec2(r.getMaxX(), r.getMidY());
	_debugLayer->drawLine(points[1], points[2], color);
		
	// right
	points[3] = Vec2(r.getMidX(), r.getMaxY());	
	_debugLayer->drawLine(points[2], points[3], color);

	// top
	points[4] = Vec2(r.getMinX(), r.getMidY());
	_debugLayer->drawLine(points[3], points[4], color);

	_debugLayer->drawSolidPoly(points, 5, color);

	_debugLayer->drawPoint(Vec2(r.getMidX(), r.getMidY()), 2.0f, color);
};

Rect ExtendedTMXTiledMap::getTileRectFrom(Vec2 coord)
{
	return this->getLayer("ground")->getTileAt(coord)->getBoundingBox();
};

TileData ExtendedTMXTiledMap::getTileDataFromLayerAt(TMXLayer& layer, Vec2 tileCoordinates)
{
	int tileGID = 0;
	TileData tileData;

	// Make sure the coordinates are valid
	if (isTileCoordValid(tileCoordinates))
		tileGID = layer.getTileGIDAt(tileCoordinates);

	// create tiledata object
	if (tileGID)
	{
		tileData.GID = tileGID;
		tileData.tileRect = getTileRectFrom(tileCoordinates);
		tileData.tileCoordinates = tileCoordinates;
	}

	return tileData;
}

TileDataArray ExtendedTMXTiledMap::getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2 coords)
{
	// local variables
	int count = 0;
	TileDataArray tileDataArray;
	
	// 3x3 grid
	for (int i = 0; i < 9; i++)
	{
		// get column and row
		int col = i % 3;
		int row = static_cast<int>(i / 3);

		// disregard col & row (1,1)
		if (col == 1 && row == 1) continue;

		//
		Vec2 gridCoordinates = Vec2(coords.x + (col - 1), coords.y + (row - 1));

		//
		tileDataArray[count++] = this->getTileDataFromLayerAt(layer, gridCoordinates);
	}

	// sort tileDataArray for faster checking
	std::swap(tileDataArray[0], tileDataArray[6]); // bottom	now in position 0 //	OLD | NEW
	std::swap(tileDataArray[2], tileDataArray[3]); // left		now in position 2 //	--- + ---
	std::swap(tileDataArray[3], tileDataArray[4]); // right		now in position 3 //	012 | 415
	std::swap(tileDataArray[4], tileDataArray[6]); // top left	now in position 4 //	3 4 | 2 3 
	std::swap(tileDataArray[5], tileDataArray[6]); // top right now in position 5 //	567 | 607

	return tileDataArray;
}
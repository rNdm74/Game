#include "ParallaxTileMap.h"
#include "ParallaxBackground.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Utils.h"
#include "PathFinder.h"
#include "Path.h"

/// <summary>
/// Summary for create(std::string mapName)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
ParallaxTileMap* ParallaxTileMap::create(std::string mapName)
{
	// Create an instance of InfiniteParallaxNode
	ParallaxTileMap* node = new ParallaxTileMap(mapName);

	if (node) 
	{
		// Add it to autorelease pool
		node->autorelease();
		node->addObjects();
	}
	else 
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}


/// <summary>
/// Summary for ParallaxTileMap(std::string mapName)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
ParallaxTileMap::ParallaxTileMap(std::string mapName)
{
	// create tilemap
	_tileMap = TMXTiledMap::create(mapName);
	_tileMap->retain();

	//
	_mapSize = _tileMap->getMapSize();
	_tileSize = _tileMap->getTileSize();
		
	// get background layer
	_backgroundLayer = _tileMap->getLayer("background");
	_backgroundLayer->retain();
	_backgroundLayer->removeFromParentAndCleanup(false);
	_backgroundLayer->getTexture()->setAntiAliasTexParameters();
	_backgroundLayer->release();

	// get collision layer
	_collisionLayer = _tileMap->getLayer("collision");
	_collisionLayer->retain();
	_collisionLayer->removeFromParentAndCleanup(false);
	_collisionLayer->getTexture()->setAntiAliasTexParameters();
	_collisionLayer->release();

	// get ladder layer
	_ladderLayer = _tileMap->getLayer("ladder");
	_ladderLayer->retain();
	_ladderLayer->removeFromParentAndCleanup(false);
	_ladderLayer->getTexture()->setAntiAliasTexParameters();
	_ladderLayer->release();

	// get foreground layer
	_foregroundLayer = _tileMap->getLayer("foreground");
	_foregroundLayer->retain();
	_foregroundLayer->removeFromParentAndCleanup(false);
	_foregroundLayer->getTexture()->setAntiAliasTexParameters();
	_foregroundLayer->release();

	// create shadow layer
	_shadowLayer = Node::create();

	// create object layer
	_objectLayer = Node::create();

	// create debug layer
	_debugLayer = DrawNode::create();
    
	this->setAnchorPoint(Vec2::ZERO);
	this->setContentSize(_tileMap->getContentSize());
	
	_parallaxBackgroundLayer = ParallaxBackground::create(_mapSize.width * _tileSize.width);
	
	this->addChild(_parallaxBackgroundLayer, - 3, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_backgroundLayer,	-2, Vec2(0.7f, 0.7f), Vec2::ZERO);
	this->addChild(_shadowLayer,		-1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_collisionLayer,		 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_ladderLayer,		 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_objectLayer,		 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
    this->addChild(_debugLayer,			 2, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_foregroundLayer,	 3, Vec2(1.0f, 1.0f), Vec2::ZERO);

    //
	_pathFinder = new AStarPathFinder(this, 500, false);


}


/// <summary>
/// Summary for ~ParallaxTileMap()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
ParallaxTileMap::~ParallaxTileMap()
{
	_tileMap->release();
}


/// <summary>
/// Summary for setAliasTexParameters(TMXLayer& layer)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxTileMap::setAliasTexParameters(TMXLayer& layer)
{
	layer.getTexture()->setAntiAliasTexParameters();
}


/// <summary>
/// Summary for clearDebugDraw()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxTileMap::clearDebugDraw()
{
	_debugLayer->clear();
}


/// <summary>
/// Summary for drawDebugRect(Rect rect, Color4F color)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxTileMap::drawDebugRect(Rect rect, Color4F color)
{
	_debugLayer->drawSolidRect( rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()), color );
}


/// <summary>
/// Summary for drawDebugRectAt(Vec2 position, Color4F color)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxTileMap::drawDebugRectAt(Vec2 position, Color4F color)
{
    Vec2 coordinates = this->getTileCoordinatesFor(position);
    Rect rect = this->getTileRectFrom(coordinates);
    this->drawDebugRect(rect, color);
}

void ParallaxTileMap::drawDebugRectAtTile(Vec2 coordinates, Color4F color)
{
	Rect rect = this->getTileRectFrom(coordinates);
	this->drawDebugRect(rect, color);
}


/// <summary>
/// Summary for addObjects()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxTileMap::addObjects()
{
	// loop over the object groups in this tmx file
	for (auto& objectGroup : _tileMap->getObjectGroups())
	{
		for (auto& object : objectGroup->getObjects())
		{
			auto properties = object.asValueMap();
			auto type = properties.at("type");

			if (type.isNull() == false)
			{
				if(addObject(type.asString(), properties))
                {
                    _objectCount++;
                }
			}
		}
	}
}


/// <summary>
/// Summary for addObject(std::string className, ValueMap& properties)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
bool ParallaxTileMap::addObject(std::string className, ValueMap& properties)
{
	// create the object
	GameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{
		o->setName(className);
		_objectLayer->addChild(o);
        
		return true;
	}

	return false;
}


/// <summary>
/// Summary for update(float delta)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
void ParallaxTileMap::update(float& delta)
{
	// debug
	this->clearDebugDraw();

	_parallaxBackgroundLayer->update(delta);

    // Tile map is responsible for updating its children
	for (auto& child : _objectLayer->getChildren())
    {
        GameObject* gameObject = static_cast<GameObject*>(child);
        gameObject->update(this);

		this->drawDebugRect(gameObject->getBoundingBox(), Color4F(1.0f, 1.0f, 1.0f, 0.5f));
    }
}


/// <summary>
/// Summary for getShadowForNode(Node* node)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
Node* ParallaxTileMap::getShadowForNode(Node* node)
{	
	auto object = static_cast<Sprite*>(node);

	auto shadow = Sprite::create();
	shadow->setSpriteFrame(object->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.05f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);

	return shadow;
}


/// <summary>
/// Summary for getPlayer()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
Player* ParallaxTileMap::getPlayer()
{
	return static_cast<Player*>(_objectLayer->getChildByName("Player"));
}


/// <summary>
/// Summary for getCollisionLayer()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
TMXLayer* ParallaxTileMap::getCollisionLayer()
{
	return _collisionLayer;
}


/// <summary>
/// Summary for getLadderLayer()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
TMXLayer* ParallaxTileMap::getLadderLayer()
{
	return _ladderLayer;
}


/// <summary>
/// Summary for getObjectLayer()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
Node* ParallaxTileMap::getObjectLayer()
{
	return _objectLayer;
}


/// <summary>
/// Summary for getMapSize()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
Size ParallaxTileMap::getMapSize()
{
	return _tileMap->getMapSize();
}


/// <summary>
/// Summary for getTileSize()
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
Size ParallaxTileMap::getTileSize()
{
	return _tileMap->getTileSize();
}


/**
* Notification that the path finder visited a given tile. This is
* used for debugging new heuristics.
*
* @param x The x coordinate of the tile that was visited
* @param y The y coordinate of the tile that was visited
*/
void ParallaxTileMap::pathFinderVisited(Vec2 coordinate)
{
}


/**
* Check if the given location is blocked, i.e. blocks movement of
* the supplied mover.
*
* @param mover The mover that is potentially moving through the specified
* tile.
* @param x The x coordinate of the tile to check
* @param y The y coordinate of the tile to check
* @return True if the location is blocked
*/
bool ParallaxTileMap::blocked(Vec2 coordinate)
{
	TileData tileData = getTileDataFromLayerAt(*_collisionLayer, coordinate);

	return (tileData.GID != 0);		
}


/**
* Get the cost of moving through the given tile. This can be used to
* make certain areas more desirable. A simple and valid implementation
* of this method would be to return 1 in all cases.
*
* @param mover The mover that is trying to move across the tile
* @param sx The x coordinate of the tile we're moving from
* @param sy The y coordinate of the tile we're moving from
* @param tx The x coordinate of the tile we're moving to
* @param ty The y coordinate of the tile we're moving to
* @return The relative cost of moving across the given tile
*/
float ParallaxTileMap::getCost(Vec2 startLocation, Vec2 targetLocation)
{
	return 1;
}


/// <summary>
/// Summary for getPropertiesForGID(int GID)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
Value ParallaxTileMap::getPropertiesForGID(int GID)
{
	return _tileMap->getPropertiesForGID(GID);
}


/// <summary>
/// Summary for getTileDataArrayFromCollisionLayerAt(Vec2 position)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
TileDataArray ParallaxTileMap::getTileDataArrayFromCollisionLayerAt(Vec2 position)
{
	return this->getTileDataArrayFromLayerAt(*_collisionLayer, position);
}


/// <summary>
/// Summary for getTileDataArrayFromLadderLayerAt(Vec2 position)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
TileDataArray ParallaxTileMap::getTileDataArrayFromLadderLayerAt(Vec2 position)
{
	// local variables
	TileDataArray tileDataArray;

	// get gameObjects tileCoordinates from position
	Vec2 tileCoordinates = getTileCoordinatesFor(position);
	
	tileDataArray[ETileGrid::BOTTOM] = getTileDataFromLayerAt(*_ladderLayer, Vec2(tileCoordinates.x, tileCoordinates.y + 1));
	tileDataArray[ETileGrid::CENTER] = getTileDataFromLayerAt(*_ladderLayer, tileCoordinates);
	tileDataArray[ETileGrid::LEFT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x - 1, tileCoordinates.y));
	tileDataArray[ETileGrid::RIGHT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x + 1, tileCoordinates.y));
	tileDataArray[ETileGrid::BOTTOM_LEFT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x - 1, tileCoordinates.y + 1));
	tileDataArray[ETileGrid::BOTTOM_RIGHT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x + 1, tileCoordinates.y + 1));
				
	return tileDataArray;
}


/// <summary>
/// Summary for getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
TileDataArray ParallaxTileMap::getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position)
{
	// local variables
	int count = 0;
	TileDataArray tileDataArray;
		
	// get gameObjects tileCoordinates from position
	Vec2 tileCoordinates = this->getTileCoordinatesFor(position);

	// 3x3 grid
	for (int i = 0; i < 9; i++)
	{
		// get column and row
		int col = i % 3;
		int row = static_cast<int>(i / 3);

		// disregard col & row (1,1)
		if (col == 1 && row == 1) continue;

		//
		Vec2 gridCoordinates = Vec2(tileCoordinates.x + (col - 1), tileCoordinates.y + (row - 1));
		
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


/// <summary>
/// Summary for getTileDataFromLayerAt(TMXLayer layer, Vec2 tileCoordinates)
///	
/// PRE-CONDITION:	
///
/// POST-CONDITION: 
/// </summary>
TileData ParallaxTileMap::getTileDataFromLayerAt(TMXLayer& layer, Vec2 tileCoordinates)
{
	int tileGID = 0;
	TileData tileData;
		
	// Make sure the coordinates are valid
	if ( isValid(tileCoordinates) )
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


/// <summary>
/// Summary for isValidTileCoordinates
///	
/// PRE-CONDITION:	Vec of tilemap tile coordinates
///					Function accesses local mapSize variable
///
/// POST-CONDITION: Returns true if tile coordinates are within tilemap coordinate bounds.
/// </summary>
bool ParallaxTileMap::isValid(Vec2 tileCoordinates)
{	
	return ( tileCoordinates.y < _mapSize.height && tileCoordinates.y >= 0 &&
             tileCoordinates.x < _mapSize.width && tileCoordinates.x >= 0 );
}


/// <summary>
/// Summary for isTileLadder
///	
/// PRE-CONDITION:	Argument gameobject position in pixels 
///
/// POST-CONDITION: Returns true if position is on a ladder
/// </summary>
bool ParallaxTileMap::isTileLadder(Vec2 position)
{			
	//
	return true;
}


/// <summary>
/// Summary for getTileCoordinatesForPosition
///	
/// PRE-CONDITION:	A position vec of a gameobject in pixels must be passed in as an argument.
///					Functions accesses the tilemap mapSize & tileSize local variables to return coordinates.
///
/// POST-CONDITION: A tile coordinate vec of the gameobject is returned.
/// </summary>
Vec2 ParallaxTileMap::getTileCoordinatesFor(Vec2 position)
{
	float x = floor(position.x / _tileSize.width);

	float levelHeightInPixels = _mapSize.height * _tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / _tileSize.height);

	return Vec2(x, y);
}


/// <summary>
/// Summary for getTileRectFrom(tileCoordinates)
///	
/// PRE-CONDITION:	A vec of TMX tilemap coordinates must be passed in as an argument.
///				-_	Functions accesses the tilemap mapSize & tileSize local variables to return coordinates.
///
/// POST-CONDITION: A rect is returned from the passed in tile coordinates.
/// </summary>
Rect ParallaxTileMap::getTileRectFrom(Vec2 tileCoords)
{
	float levelHeightInPixels = _mapSize.height * _tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * _tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * _tileSize.height));

	return Rect(origin.x, origin.y, _tileSize.width, _tileSize.height);
}

Path* ParallaxTileMap::getPath(Vec2 startLocation, Vec2 targetLocation)
{
	return _pathFinder->findPath(startLocation, targetLocation);
}

Vec2 ParallaxTileMap::getMapTransition(Vec2 direction)
{
    Vec2 transition = Vec2::ZERO;
    
    if(direction.y < 0)
        transition = _objectLayer->getChildByName("Exit")->getPosition();
    else if(direction.y > 0)
        transition = _objectLayer->getChildByName("Enter")->getPosition();
    
    return transition;
    
}
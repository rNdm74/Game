#include "ParallaxTileMap.h"
#include "ParallaxBackground.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Utils.h"
#include "PathFinder.h"
#include "Path.h"


/**
* Constructor a new ParallaxTileMap,
* This will create and retain a new TMXTiledMap passing in the filename of the tmx map file,
* Define the Size in tile count of the tilemap and the tile size
*
* @param mapName The std::string that contains the tmx filename
* @return The ParallaxTileMap pointer that has been created with objects
*/
ParallaxTileMap::ParallaxTileMap(std::string mapName)
{
	// create tilemap
	_tileMap = TMXTiledMap::create(mapName);
	_tileMap->retain();

	//
	_mapSize = _tileMap->getMapSize();
	_tileSize = _tileMap->getTileSize();
		
	// get background layer
	auto _backgroundLayer = _tileMap->getLayer("background");	
	_backgroundLayer->retain();
	_backgroundLayer->setTag(kTagBackgroundLayer);
	_backgroundLayer->removeFromParentAndCleanup(false);
	_backgroundLayer->release();

	// get collision layer
	auto _collisionLayer = _tileMap->getLayer("collision");
	_collisionLayer->retain();
	_collisionLayer->setTag(kTagCollisionLayer);
	_collisionLayer->removeFromParentAndCleanup(false);
	_collisionLayer->release();

	// get ladder layer
	auto _ladderLayer = _tileMap->getLayer("ladder");
	_ladderLayer->retain();
	_ladderLayer->setTag(kTagLadderLayer);
	_ladderLayer->removeFromParentAndCleanup(false);
	_ladderLayer->release();

	// get foreground layer
	auto _foregroundLayer = _tileMap->getLayer("foreground");
	_foregroundLayer->retain();
	_foregroundLayer->setTag(kTagForegroundLayer);
	_foregroundLayer->removeFromParentAndCleanup(false);
	_foregroundLayer->release();

	// create shadow layer
	auto _shadowLayer = Node::create();
	_shadowLayer->setTag(kTagShadowLayer);

	// create object layer
	auto _objectLayer = Node::create();
	_objectLayer->setTag(kTagObjectLayer);

	// create debug layer
	_debugLayer = DrawNode::create();
	_debugLayer->setTag(kTagDebugLayer);
    
	this->setAnchorPoint(Vec2::ZERO);
	this->setContentSize(_tileMap->getContentSize());
			
	/** **/	
	this->addChild(_backgroundLayer,			-2, Vec2(0.9f, 1.0f), Vec2::ZERO);
	this->addChild(_shadowLayer,				-1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_collisionLayer,				 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_ladderLayer,				 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_objectLayer,				 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
    this->addChild(_debugLayer,					 2, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_foregroundLayer,			 3, Vec2(1.0f, 1.0f), Vec2::ZERO);

    /** **/
	this->addShadows(static_cast<TMXLayer&>(*_collisionLayer));
	this->addShadows(static_cast<TMXLayer&>(*_ladderLayer));
	this->addShadows(static_cast<TMXLayer&>(*_foregroundLayer));

	/** **/
	this->addObjects();
}

/**
* Destructor
*
* This will release the retained _tileMap instance
*/
ParallaxTileMap::~ParallaxTileMap()
{
	_tileMap->release();
}

/**
* Update all the game objects in the parallax tile map.
*
* @param delta The float fixed time step
*/
void ParallaxTileMap::update(float& delta)
{

#if DEBUG_ENABLE

	this->clearDebugDraw();

#endif // DEBUG_ENABLE

	// Tile map is responsible for updating its children
	for (auto& child : this->getChildByTag(kTagObjectLayer)->getChildren())
	{
		IGameObject* gameObject = static_cast<IGameObject*>(child);
		gameObject->update(this);

#if DEBUG_ENABLE

		this->drawDebugRect(gameObject->getBoundingBox(), Color4F(1.0f, 1.0f, 1.0f, 0.5f));

#endif // DEBUG_ENABLE

	}
}

#pragma region Pathfinding

/**
* Check if the given location is blocked, i.e. blocks movement of
* the supplied mover.
*
* @param mover The mover that is potentially moving through the specified
* tile.
* @param coordinate The Vec2
* @return True if the location is blocked
*/
bool ParallaxTileMap::isBlocked(Vec2 coordinate)
{
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagCollisionLayer));

	TileData tileData = getTileDataFromLayerAt(layer, coordinate);

	return (tileData.GID != 0);
}

/**
* Get the cost of moving through the given tile. This can be used to
* make certain areas more desirable. A simple and valid implementation
* of this method would be to return 1 in all cases.
*
* @param startLocation The mover that is trying to move across the tile
* @param targetLocation The x coordinate of the tile we're moving from
* @return The relative cost of moving across the given tile
*/
float ParallaxTileMap::getCost(Vec2 startLocation, Vec2 targetLocation)
{
	return 1;
}

#pragma endregion Pathfinding

#pragma region Getters

/**
* Gets the map size in tiles
*
* @return mapsize
*/
Size ParallaxTileMap::getMapSize()
{
	return _tileMap->getMapSize();
}

/**
* Gets the tile size
*
* @return the tilesize
*/
Size ParallaxTileMap::getTileSize()
{
	return _tileMap->getTileSize();
}

/**
* Check if the given location is blocked, i.e. blocks movement of
* the supplied mover.
*
* @return True if the location is blocked
*/
IGameObject* ParallaxTileMap::getPlayer()
{
	Node* player = this->getChildByTag(kTagObjectLayer)->getChildByTag(kTagPlayer);
	return static_cast<IGameObject*>(player);
}

/**
* Returns the collision TileDataArray surrounding the game objects position
*
* @param position The Vec2 of the game objects position
* @return The TileDataArray surrounding the game objects position
*/
TileDataArray ParallaxTileMap::getCollisionDataAt(Vec2 position)
{
	return this->getTileDataArrayFromCollisionLayerAt(position);
};

/**
* Returns the ladder TileDataArray surrounding the game objects position
*
* @param position The Vec2 of the game objects position
* @return The TileDataArray surrounding the game objects position
*/
TileDataArray ParallaxTileMap::getLadderDataAt(Vec2 position)
{
	return this->getTileDataArrayFromLadderLayerAt(position);
};

#pragma endregion Getters

#pragma region Player

/** **/
void ParallaxTileMap::addPlayer(IGameObject* player)
{
	/**  **/
	this->getChildByTag(kTagObjectLayer)->addChild(player);
};

/** **/
IGameObject* ParallaxTileMap::removePlayer()
{
	Node* player = this->getChildByTag(kTagObjectLayer)->getChildByTag(kTagPlayer);
	player->removeFromParentAndCleanup(true);

	return static_cast<IGameObject*>(player);
};

/** **/
void ParallaxTileMap::setPositionOnPlayer()
{
	Vec2 _position = this->getPlayer()->getCenterPosition();
	Size _winSize = Director::getInstance()->getWinSize() / this->getParent()->getScale();
	
	float x = MAX(_position.x, _winSize.width / 2);
	float y = MAX(_position.y, _winSize.height / 2);
	x = MIN(x, (_mapSize.width * _tileSize.width) - _winSize.width / 2);
	y = MIN(y, (_mapSize.height * _tileSize.height) - _winSize.height / 2);

	Vec2 _actualPosition = Vec2(x, y);
	Vec2 _centerOfView = Vec2(_winSize.width / 2, _winSize.height / 2);
	Vec2 _viewPoint = _centerOfView - _actualPosition;

	this->setPosition(_viewPoint);
};

/** **/
ValueMap ParallaxTileMap::getInitialProperties()
{
	Node* node = this->getChildByTag(kTagObjectLayer)->getChildByName("Left");

	return static_cast<IGameObject*>(node)->getProperties();
};

#pragma endregion Player

#pragma region Flags

/**
* Returns if the tilecoordinates are within the map bounds
*
* @param tileCoordinates The Vec2 that is the tile coordinates being queried
* @return true if tilecoordinates are within the map bounds
*/
bool ParallaxTileMap::isValid(Vec2 tileCoordinates)
{
	return (tileCoordinates.y < _mapSize.height && tileCoordinates.y >= 0 &&
		tileCoordinates.x < _mapSize.width && tileCoordinates.x >= 0);
}


/**
* Returns if the tilecoordinates are within the map bounds
*
* @param tileCoordinates The Vec2 that is the tile coordinates being queried
* @return true if tilecoordinates have found a ladder
*/
bool ParallaxTileMap::isTileLadder(Vec2 tileCoordinates)
{
	/** **/
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagLadderLayer));
	/** **/
	TileData tileData = this->getTileDataFromLayerAt(layer, tileCoordinates);
	/** **/
	return (tileData.GID != 0);
}

#pragma endregion Flags

#if DEBUG_ENABLE
/**
* Reset the _debug layer
*
* Clear of the debug rectangles that have been added to the layer
*/
void ParallaxTileMap::clearDebugDraw()
{
	_debugLayer->clear();
}


/**
* Adds a filled rectangle to the debug layer with a set color
*
* @param rect The Rect wanting to be drawn
* @param color The Color4F of the rectangle to be drawn
*/
void ParallaxTileMap::drawDebugRect(Rect rect, Color4F color)
{
	_debugLayer->drawSolidRect(rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()), color);
}


/**
* Adds a filled rectangle to the debug layer at from a given position with a set color
*
* @param position The Vec2 position where the rect is to be drawn
* @param color The Color4F of the rectangle to be drawn
*/
void ParallaxTileMap::drawDebugRectAt(Vec2 position, Color4F color)
{
	Vec2 coordinates = this->getTileCoordinatesFor(position);
	Rect rect = this->getTileRectFrom(coordinates);
	this->drawDebugRect(rect, color);
}


/**
* Adds a filled rectangle to the debug layer at from a given coordinate with a set color
*
* @param position The Vec2 tile coordinates where the rect is to be drawn
* @param color The Color4F of the rectangle to be drawn
*/
void ParallaxTileMap::drawDebugRectAtTile(Vec2 coordinates, Color4F color)
{
	Rect rect = this->getTileRectFrom(coordinates);
	this->drawDebugRect(rect, color);
}


#endif // DEBUG_ENABLE

#pragma region Protected Functions

/**
* Adds all objects from the object layer in the tmx file
*/
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
				if (addObject(type.asString(), properties))
				{
					_objectCount++;
				}
			}
		}
	}
}

/**
* Adds a new object from the tmx file using the GameObjectFactory to the object layer of the ParallaxTileMap
*
* @param className The std::string that tells us the name of the object being added
* @param properties The ValueMap of the object being added
*/
bool ParallaxTileMap::addObject(std::string className, ValueMap& properties)
{
	// create the object
	IGameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{
		o->setName(className);
		this->getChildByTag(kTagObjectLayer)->addChild(o);

		return true;
	}

	return false;
}

/**
* Check if the given location is blocked, i.e. blocks movement of
* the supplied mover.
*
* @param delta The float fixed time step
* @return True if the location is blocked
*/
void ParallaxTileMap::addShadows(TMXLayer& layer)
{

	Size s = layer.getLayerSize();

	for (int col = 0; col < s.width; ++col)
	{
		for (int row = 0; row < s.height; ++row)
		{
			int gid = layer.getTileGIDAt(Vec2(col, row));

			if (gid)
			{
				Sprite& tile = *layer.getTileAt(Vec2(col, row));

				Sprite* shadow = getShadowForNode(tile);
				shadow->setPosition(tile.getPosition());

				this->getChildByTag(kTagShadowLayer)->addChild(shadow);
			}
		}
	}
};

/**
* Get the cost of moving through the given tile. This can be used to
*
* @param GID The int that is trying to move across the tile
* @return The relative cost of moving across the given tile
*/
Value ParallaxTileMap::getPropertiesForGID(int GID)
{
	return _tileMap->getPropertiesForGID(GID);
}

/**
* Returns a Rect from a given tileCoordinates
*
* @param tileCoordinates The Vec2 that is the tile coordinates being queried
* @return Rect from the given tileCoordinates
*/
Vec2 ParallaxTileMap::getMapTransition(Vec2 direction)
{
	//
	Vec2 transition = Vec2::ZERO;
	//
	if (direction.y < 0)
	{
		//Rect r = _objectLayer->getChildByName("Exit")->getBoundingBox();
		//transition = Vec2(r.getMidX(), r.getMinY());
	}
	else if (direction.y > 0)
	{
		//Rect r = _objectLayer->getChildByName("Enter")->getBoundingBox();
		//transition = Vec2(r.getMidX(), r.getMaxY());
	}
	//
	return transition;
}

/**
* Returns if the tilecoordinates from a opengl position
*
* @param tileCoordinates The Vec2 that is the tile coordinates being queried
* @return true if tilecoordinates have found a ladder
*/
Vec2 ParallaxTileMap::getTileCoordinatesFor(Vec2 position)
{
	float x = floor(position.x / _tileSize.width);

	float levelHeightInPixels = _mapSize.height * _tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / _tileSize.height);

	return Vec2(x, y);
}

/**
* Returns a Rect from a given tileCoordinates
*
* @param tileCoordinates The Vec2 that is the tile coordinates being queried
* @return Rect from the given tileCoordinates
*/
Rect ParallaxTileMap::getTileRectFrom(Vec2 tileCoords)
{
	float levelHeightInPixels = _mapSize.height * _tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * _tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * _tileSize.height));

	return Rect(origin.x, origin.y, _tileSize.width, _tileSize.height);
}

/**
* Returns the TileData from the TMXLayer at the given tilecoordinates
*
* @param layer The TMXLayer that is being queried
* @param tileCoordinates The Vec2 that is the tile coordinates
* @return The TileData from the TMXLayer at the given tilecoordinates
*/
TileData ParallaxTileMap::getTileDataFromLayerAt(TMXLayer& layer, Vec2 tileCoordinates)
{
	int tileGID = 0;
	TileData tileData;

	// Make sure the coordinates are valid
	if (isValid(tileCoordinates))
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

/**
* Returns the ladder TileDataArray surrounding the game objects position
*
* @param position The Vec2 of the game objects position
* @return The TileDataArray surrounding the game objects position
*/
TileDataArray ParallaxTileMap::getTileDataArrayFromLadderLayerAt(Vec2 position)
{
	TileDataArray tileDataArray;

	/** get gameObjects tileCoordinates from position **/
	Vec2 tileCoordinates = getTileCoordinatesFor(position);

	/**  **/
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagLadderLayer));

	tileDataArray[ETileGrid::BOTTOM] = getTileDataFromLayerAt(layer, Vec2(tileCoordinates.x, tileCoordinates.y + 1));
	tileDataArray[ETileGrid::CENTER] = getTileDataFromLayerAt(layer, tileCoordinates);
	//tileDataArray[ETileGrid::LEFT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x - 1, tileCoordinates.y));
	//tileDataArray[ETileGrid::RIGHT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x + 1, tileCoordinates.y));
	//tileDataArray[ETileGrid::BOTTOM_LEFT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x - 1, tileCoordinates.y + 1));
	//tileDataArray[ETileGrid::BOTTOM_RIGHT] = getTileDataFromLayerAt(*_collisionLayer, Vec2(tileCoordinates.x + 1, tileCoordinates.y + 1));

	return tileDataArray;
}

/**
* Returns the collision TileDataArray surrounding the game objects position
*
* @param position The Vec2 of the game objects position
* @return The TileDataArray surrounding the game objects position
*/
TileDataArray ParallaxTileMap::getTileDataArrayFromCollisionLayerAt(Vec2 position)
{
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagCollisionLayer));

	return this->getTileDataArrayFromLayerAt(layer, position);
}

/**
* Returns the ladder TileDataArray surrounding the game objects position
*
* @param position The Vec2 of the game objects position
* @return The TileDataArray surrounding the game objects position
*/
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

/**
* Creates a shadow for a given sprite
*
* @param object The Sprite to create the shadow
* @return a new sprite shadow
*/
Sprite* ParallaxTileMap::getShadowForNode(Sprite& sprite)
{
	Sprite* shadow = Sprite::create();
	shadow->setSpriteFrame(sprite.getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.05f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);

	return shadow;
}

/**
* Turn on anitaliasling on a layer of the TileMap
*
* @param layer The TMXLayer reference of the layer your want to apply antialiasing to
*/
void ParallaxTileMap::setAliasTexParameters(TMXLayer& layer)
{
	layer.getTexture()->setAntiAliasTexParameters();
}

void ParallaxTileMap::enableForegroundOpacity(int opacity)
{
    TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagForegroundLayer));
    
    Size s = layer.getLayerSize();
    
    for (int col = 0; col < s.width; ++col)
    {
        for (int row = 0; row < s.height; ++row)
        {
            int gid = layer.getTileGIDAt(Vec2(col, row));
            
            if (gid)
            {
                Sprite* tile = layer.getTileAt(Vec2(col, row));
                
                int currentOpacity = tile->getOpacity();
                
                if(currentOpacity > opacity)
                {
                    currentOpacity -= 5;
                }
                else
                {
                    currentOpacity += 5;
                }
                
                if(currentOpacity < 0)
                    currentOpacity = 0;
                if(currentOpacity > 255)
                    currentOpacity = 255;
                
                tile->setOpacity(currentOpacity); // 0 - opaque,  255 - solid
            }
        }
    }
};

#pragma endregion Protected Functions

#pragma region Cave

/**
* Create a new ParallaxTileMap, This will create and addObjects from the tmx file
*
* @param mapName The std::string that contains the tmx filename
* @return The ParallaxTileMap pointer that has been created with objects
*/
IParallaxTileMap* Cave::create(std::string mapName)
{
	// Create an instance of InfiniteParallaxNode
	IParallaxTileMap* node = new Cave(mapName);

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

Cave::Cave(std::string mapName) : super(mapName)
{

};

#pragma endregion Cave

#pragma region Planet

/**
* Create a new ParallaxTileMap, This will create and addObjects from the tmx file
*
* @param mapName The std::string that contains the tmx filename
* @return The ParallaxTileMap pointer that has been created with objects
*/
IParallaxTileMap* Planet::create(std::string mapName)
{
	// Create an instance of InfiniteParallaxNode
	IParallaxTileMap* node = new Planet(mapName);

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

Planet::Planet(std::string mapName) : super(mapName)
{
	_parallaxBackgroundLayer = ParallaxBackground::create(_mapSize.width * _tileSize.width);

	this->addChild(_parallaxBackgroundLayer, -3, Vec2(1.0f, 1.0f), Vec2::ZERO);
};

#pragma endregion Planet
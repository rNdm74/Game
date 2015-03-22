#include "ParallaxTileMap.h"
#include "ParallaxTileMapFsm.h"
#include "ParallaxForeground.h"
#include "ParallaxBackground.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Utils.h"
#include "PathFinder.h"
#include "Path.h"
#include "AStarHeuristic.h"


ParallaxTileMap::ParallaxTileMap(std::string type)
{
	
}

ParallaxTileMap::~ParallaxTileMap()
{
	_tileMap->release();
	delete _pathFinder;
}

void ParallaxTileMap::init(TMXTiledMap* tileMap, Texture2D* texture)
{
	// create tilemap
	_tileMap = tileMap;
	_tileMap->retain();

	_pathFinder = new AStarPathFinder(this, 100, false, new ClosestHeuristic());

	//
	_mapSize = _tileMap->getMapSize();
	_tileSize = _tileMap->getTileSize();

	// get background layer
	auto _backgroundLayer = _tileMap->getLayer("background");
	_backgroundLayer->retain();
	_backgroundLayer->setTexture(texture);
	_backgroundLayer->setTag(kTagBackgroundLayer);
	_backgroundLayer->removeFromParentAndCleanup(false);
	_backgroundLayer->release();

    // get default layer
    auto _innerForegroundLayer = _tileMap->getLayer("innerForeground");
    _innerForegroundLayer->retain();
    _innerForegroundLayer->setTexture(texture);
    _innerForegroundLayer->setTag(kTagInnerForeground);
    _innerForegroundLayer->removeFromParentAndCleanup(false);
    _innerForegroundLayer->release();
    
    // get default layer
    auto _innerBackgroundLayer = _tileMap->getLayer("innerBackground");
    _innerBackgroundLayer->retain();
    _innerBackgroundLayer->setTexture(texture);
    _innerBackgroundLayer->setTag(kTagInnerBackground);
    _innerBackgroundLayer->removeFromParentAndCleanup(false);
    _innerBackgroundLayer->release();
    
	// get collision layer
	auto _collisionLayer = _tileMap->getLayer("collision");
	_collisionLayer->retain();
	_collisionLayer->setTexture(texture);
	_collisionLayer->getTexture()->setAntiAliasTexParameters();
	_collisionLayer->setTag(kTagCollisionLayer);
	_collisionLayer->removeFromParentAndCleanup(false);
	_collisionLayer->release();

	// get ladder layer
	auto _ladderLayer = _tileMap->getLayer("ladder");
	_ladderLayer->retain();
	_ladderLayer->setTexture(texture);
	_ladderLayer->setTag(kTagLadderLayer);
	_ladderLayer->removeFromParentAndCleanup(false);
	_ladderLayer->release();

	// get foreground layer
	auto _foregroundLayer = _tileMap->getLayer("foreground");
	_foregroundLayer->retain();
	_foregroundLayer->setTexture(texture);
	_foregroundLayer->setTag(kTagForegroundLayer);
	_foregroundLayer->removeFromParentAndCleanup(false);
	_foregroundLayer->release();

    
    /*for(int col = 0; col < _mapSize.width; col++)
    {
        for(int row = 0; row < _mapSize.height; row++)
        {
            Vec2 coord = Vec2(col, row);
            
			int foreGID = _innerForegroundLayer->getTileGIDAt(coord);
			int backGID = _innerBackgroundLayer->getTileGIDAt(coord);

            if(foreGID)
            {
				Sprite* tile = _innerForegroundLayer->getTileAt(Vec2(coord));
                
				float newScale = 1.0f;

				if (foreGID == 4)
				{
					newScale = random(0.5f, 2.0f);
				}
				else
				{
					newScale = random(0.25f, 1.0f);
				}

                tile->setScale(newScale);
                tile->getTexture()->setAntiAliasTexParameters();				
            }
            
            if(backGID)
            {
                Sprite* tile = _innerBackgroundLayer->getTileAt(Vec2(coord));
                
				float newScale = 1.0f;

				if (backGID == 4)
				{
					newScale = random(0.5f, 2.0f);
				}
				else
				{
					newScale = random(0.25f, 1.0f);
				}

                tile->setScale(newScale);
                tile->getTexture()->setAntiAliasTexParameters();
            }
        }
    }*/

    
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
	this->addChild(_backgroundLayer, -2, Vec2(0.9f, 1.0f), Vec2::ZERO);
	this->addChild(_shadowLayer, -1, Vec2(1.0f, 1.0f), Vec2::ZERO);
    
	this->addChild(_collisionLayer, 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_ladderLayer, 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
    
    this->addChild(_innerBackgroundLayer, 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_objectLayer, 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_innerForegroundLayer, 2, Vec2(1.0f, 1.0f), Vec2::ZERO);
    
    this->addChild(_debugLayer, 2, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(_foregroundLayer, 3, Vec2(1.0f, 1.0f), Vec2::ZERO);

	/** **/
    //this->addShadows(static_cast<TMXLayer&>(*_innerBackgroundLayer));
    //this->addShadows(static_cast<TMXLayer&>(*_innerForegroundLayer));

	this->addShadows(static_cast<TMXLayer&>(*_collisionLayer));
	this->addShadows(static_cast<TMXLayer&>(*_ladderLayer));
	this->addShadows(static_cast<TMXLayer&>(*_foregroundLayer));

	/** **/
	this->addObjects();
};

void ParallaxTileMap::update(float& delta)
{
#if DEBUG_ENABLE
	this->clearDebugDraw();
#endif // DEBUG_ENABLE MUST BE RUN FIRST

	auto children = this->getChildByTag(kTagObjectLayer)->getChildren();
	// If there is children
	if (children.size() > 0)
		// Tile map is responsible for updating its children
		for (auto& child : children) 
			// Update the children
			static_cast<IGameObject*>(child)->update(this);
}

bool ParallaxTileMap::isBlocked(Vec2 coordinate)
{
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagCollisionLayer));
	
	TileData tileData = getTileDataFromLayerAt(layer, coordinate);

	return (tileData.GID != 0);
}

float ParallaxTileMap::getCost(Vec2 startLocation, Vec2 targetLocation)
{
	return 1;
}

Size ParallaxTileMap::getMapSize()
{
	return _tileMap->getMapSize();
}

Size ParallaxTileMap::getTileSize()
{
	return _tileMap->getTileSize();
}

IGameObject* ParallaxTileMap::getPlayer()
{
	Node* player = this->getChildByTag(kTagObjectLayer)->getChildByTag(kTagPlayer);
	return static_cast<IGameObject*>(player);
}

TileDataArray ParallaxTileMap::getCollisionDataAt(Vec2 position)
{
	return this->getTileDataArrayFromCollisionLayerAt(position);
};

TileDataArray ParallaxTileMap::getLadderDataAt(Vec2 position)
{
	return this->getTileDataArrayFromLadderLayerAt(position);
};

float ParallaxTileMap::getWidth()
{
    return _tileSize.width * _mapSize.width;
};

cocos2d::Vector<cocos2d::Node*> ParallaxTileMap::getObjects()
{
	return this->getChildByTag(kTagObjectLayer)->getChildren();
};

IGameObject* ParallaxTileMap::getSelectedGameObject(Vec2 target)
{
	Vec2 nodeTargetLocation = convertToNodeSpaceAR(target);
			
	auto children  = this->getChildByTag(kTagObjectLayer)->getChildren();

	for (auto child : children)
	{
		Rect r = child->getBoundingBox();
		
		if (r.containsPoint(nodeTargetLocation))
		{
			return static_cast<IGameObject*>(child);
		}
	}			
	
	return nullptr;
};

IPath* ParallaxTileMap::findPath(Vec2 origin, Vec2 target)
{
	return _pathFinder->findPath(origin, target);
};


void ParallaxTileMap::addPlayer(IGameObject* player)
{
	/**  **/
	this->getChildByTag(kTagObjectLayer)->addChild(player);
};

void ParallaxTileMap::removePlayer()
{
	this->getChildByTag(kTagObjectLayer)->getChildByTag(kTagPlayer)->removeFromParentAndCleanup(true);
};

void ParallaxTileMap::setPositionOnPlayer()
{
	Vec2 v = this->getPlayer()->getVelocity();
	Rect r = this->getPlayer()->getCollisionBox();
	/** Bottom middle of sprite **/
	Vec2 p = Vec2(r.origin.x + (r.size.width / 2), r.origin.y);
	//p.x += v.x * kUpdateInterval;
			
	float s = this->getParent()->getScale();
		
	Size m = _mapSize;
	Size t = _tileSize;
	Size w = Director::getInstance()->getWinSize() / s;

	float x = MAX(p.x, w.width / 2);
	float y = MAX(p.y, w.height / 2);
	x = MIN(x, (m.width * t.width) - w.width / 2);
	y = MIN(y, (m.height * t.height) - w.height / 2);

	Vec2 ap = Vec2(x, y);
	Vec2 cov = Vec2(w.width / 2, w.height / 2);
	Vec2 vp = cov - ap;

	this->setPosition(vp);
};

Rect ParallaxTileMap::getViewportBoundingBox()
{
	
	Vec2 p = this->getPosition();
	float s = this->getParent()->getScale();
	Size w = Director::getInstance()->getWinSize() / s;

	return Rect(abs(p.x), abs(p.y), w.width, w.height);
};

ValueMap ParallaxTileMap::getInitialProperties()
{
	Node* node = this->getChildByTag(kTagObjectLayer)->getChildByName("LandingSite");

	return static_cast<IGameObject*>(node)->getProperties();
};

bool ParallaxTileMap::isValid(Vec2 tileCoordinates)
{
	return (tileCoordinates.y < _mapSize.height && tileCoordinates.y >= 0 &&
		tileCoordinates.x < _mapSize.width && tileCoordinates.x >= 0);
}

bool ParallaxTileMap::isTileLadder(Vec2 tileCoordinates)
{
	/** **/
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagLadderLayer));
	/** **/
	TileData tileData = this->getTileDataFromLayerAt(layer, tileCoordinates);
	/** **/
	return (tileData.GID != 0);
}

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
				if (type.asString() == "Npc")
				{
					for (int i = 0; i < 5; i++)
					{
						if (addObject(type.asString(), properties))
						{
							_objectCount++;
						}
					}
				}
				else
				{
					if (addObject(type.asString(), properties))
					{
						_objectCount++;
					}
				}				
			}
		}
	}
}

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
                shadow->setScale(tile.getScale());
				shadow->setPosition(tile.getPosition());

				this->getChildByTag(kTagShadowLayer)->addChild(shadow, tile.getZOrder());
			}
		}
	}
};

Value ParallaxTileMap::getPropertiesForGID(int GID)
{
	return _tileMap->getPropertiesForGID(GID);
}

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

Vec2 ParallaxTileMap::getTileCoordinatesFor(Vec2 position)
{
	float x = floor(position.x / _tileSize.width);

	float levelHeightInPixels = _mapSize.height * _tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / _tileSize.height);

	return Vec2(x, y);
}

Rect ParallaxTileMap::getTileRectFrom(Vec2 tileCoords)
{
	float levelHeightInPixels = _mapSize.height * _tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * _tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * _tileSize.height));

	return Rect(origin.x, origin.y, _tileSize.width, _tileSize.height);
}

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

TileDataArray ParallaxTileMap::getTileDataArrayFromCollisionLayerAt(Vec2 position)
{
	TMXLayer& layer = static_cast<TMXLayer&>(*this->getChildByTag(kTagCollisionLayer));

	return this->getTileDataArrayFromLayerAt(layer, position);
}

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

Sprite* ParallaxTileMap::getShadowForNode(Sprite& sprite)
{
	Sprite* shadow = Sprite::create();
	shadow->setSpriteFrame(sprite.getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.05f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);

	return shadow;
}

void ParallaxTileMap::setAliasTexParameters(TMXLayer& layer)
{
	layer.getTexture()->setAntiAliasTexParameters();
}

void ParallaxTileMap::enableForegroundOpacity(int fade)
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
                
				currentOpacity += kOpacityFadeFactor * fade;
                
				if (currentOpacity < 0)
					currentOpacity = 0;
				if (currentOpacity > kOpacityMax)
					currentOpacity = kOpacityMax;
                
                tile->setOpacity(currentOpacity); // 0 - opaque,  255 - solid
            }
        }
    }
};

void ParallaxTileMap::enableParallaxForegroundOpacity(int fade)
{
	Node& node = *this->getChildByTag(kTagPForegroundLayer);
		
	for (auto child : node.getChildren())
	{
		if (child->getTag() == kTagHill)
		{
			Sprite* sprite = static_cast<Sprite*>(child);

			int currentOpacity = sprite->getOpacity();

			currentOpacity += kOpacityFadeFactor * fade;

			if (currentOpacity < 0)
				currentOpacity = 0;
			if (currentOpacity > kOpacityMax)
				currentOpacity = kOpacityMax;

			sprite->setOpacity(currentOpacity); // 0 - opaque,  255 - solid
		}			
	}
};


PlanetSurface* PlanetSurface::create(std::string type)
{
	// Create an instance of InfiniteParallaxNode
	PlanetSurface* node = new PlanetSurface(type);

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

PlanetSurface::PlanetSurface(std::string type) : super(type)
{
	this->setTag(kTagPlanetSurface);
	this->init(TMXTiledMap::create(kPlanetSurfaceTmx), Sprite::create(type)->getTexture());

	float width = _mapSize.width * _tileSize.width;
	Vec2 parallaxRatio = Vec2(1.0f, 1.0f);
	Vec2 offset = Vec2::ZERO;
	int bZindex = -3;
	int fZindex = 6;

	if (type == GRASS_PNG)
	{		
		this->addChild(GrassBackground::create(width), bZindex, parallaxRatio, offset);
		this->addChild(GrassForeground::create(width), fZindex, parallaxRatio, offset);
	}
	else if (type == SNOW_PNG)
	{		
		this->addChild(SnowBackground::create(width), bZindex, parallaxRatio, offset);
		this->addChild(SnowForeground::create(width), fZindex, parallaxRatio, offset);
	}
	else if (type == SAND_PNG)
	{
		this->addChild(SandBackground::create(width), bZindex, parallaxRatio, offset);
		this->addChild(SandForeground::create(width), fZindex, parallaxRatio, offset);
	}
	else if (type == DIRT_PNG)
	{
		this->addChild(DirtBackground::create(width), bZindex, parallaxRatio, offset);
		this->addChild(DirtForeground::create(width), fZindex, parallaxRatio, offset);
	}	
};

ValueMap PlanetSurface::getToCaveProperties()
{
	return static_cast<ToCave*>(getChildByTag(kTagObjectLayer)->getChildByTag(kTagToCave))->getProperties();
};

Cave* Cave::create(std::string type)
{
	// Create an instance of InfiniteParallaxNode
	Cave* node = new Cave(type);

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

Cave::Cave(std::string type) : super(type)
{
	this->setTag(kTagCave);

	this->init(TMXTiledMap::create("cave1.tmx"), Sprite::create(type)->getTexture());

	//float width = _mapSize.width * _tileSize.width;
	//float height = _mapSize.height * _tileSize.height;

	//Vec2 parallaxRatio = Vec2(1.0f, 1.0f);
	//Vec2 offset = Vec2::ZERO;
	//int bZindex = -3;
	//int fZindex = 6;

	//if (type == GRASS_PNG)
	//{		
	//	auto grassForeground = GrassForeground::create(width);
	//	grassForeground->setScaleY(-2.0f);
	//	
	//	//grassForeground->setRotation(180);
	//	this->addChild(grassForeground, fZindex, parallaxRatio, Vec2(0.0f, height));
	//	this->addChild(GrassForeground::create(width), fZindex, parallaxRatio, offset);
	//}
	//else if (type == SNOW_PNG)
	//{		
	//	this->addChild(SnowForeground::create(width), fZindex, parallaxRatio, offset);
	//}
	//else if (type == SAND_PNG)
	//{		
	//	this->addChild(SandForeground::create(width), fZindex, parallaxRatio, offset);
	//}
	//else if (type == DIRT_PNG)
	//{		
	//	this->addChild(DirtForeground::create(width), fZindex, parallaxRatio, offset);
	//}
};

ValueMap Cave::getToSurfaceProperties()
{
	return static_cast<ToSurface*>(getChildByTag(kTagObjectLayer)->getChildByTag(kTagToSurface))->getProperties();
};

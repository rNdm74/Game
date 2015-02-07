#include "ParallaxTileMap.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Utils.h"

ParallaxTileMap* ParallaxTileMap::create(std::string mapName)
{
	// Create an instance of InfiniteParallaxNode
	ParallaxTileMap* node = new ParallaxTileMap(mapName);

	if (node) {
		// Add it to autorelease pool
		node->autorelease();
		node->addObjects();
	}
	else {
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}

ParallaxTileMap::ParallaxTileMap(std::string mapName)
{
	// create tilemap
	tileMap = TMXTiledMap::create(mapName);
	tileMap->retain();

	mapSize = tileMap->getMapSize();
	tileSize = tileMap->getTileSize();

	debugDraw = DrawNode::create();

	// get background layer
	backgroundLayer = tileMap->getLayer("background");
	backgroundLayer->retain();
	backgroundLayer->removeFromParentAndCleanup(false);
	backgroundLayer->getTexture()->setAntiAliasTexParameters();
	backgroundLayer->release();

	// get foreground layer
	collisionLayer = tileMap->getLayer("collision");
	collisionLayer->retain();
	collisionLayer->removeFromParentAndCleanup(false);
	collisionLayer->getTexture()->setAntiAliasTexParameters();
	collisionLayer->release();

	// get foreground layer
	ladderLayer = tileMap->getLayer("ladders");
	ladderLayer->retain();
	ladderLayer->removeFromParentAndCleanup(false);
	ladderLayer->getTexture()->setAntiAliasTexParameters();
	ladderLayer->release();

	// create shadow layer
	shadowLayer = Node::create();

	// create object layer
	objectLayer = Node::create();
	objectLayer->addChild(debugDraw);

	this->setAnchorPoint(Vec2::ZERO);
	this->setContentSize(tileMap->getContentSize());

	float parallaxRatioX = backgroundLayer->getProperty("parallaxRatioX").asFloat();
	float parallaxRatioY = backgroundLayer->getProperty("parallaxRatioY").asFloat();

	this->addChild(backgroundLayer,	-1, Vec2(0.7f, 0.7f), Vec2::ZERO);
	this->addChild(shadowLayer,		 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(collisionLayer,	 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(ladderLayer,      1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(objectLayer,		 2, Vec2(1.0f, 1.0f), Vec2::ZERO);	
}

ParallaxTileMap::~ParallaxTileMap()
{
	tileMap->release();
}

void ParallaxTileMap::setAliasTexParameters(TMXLayer& layer)
{
	layer.getTexture()->setAntiAliasTexParameters();
}

void ParallaxTileMap::clearDebugDraw()
{
	debugDraw->clear();
}

void ParallaxTileMap::drawDebugRect(Rect rect, Color4F color)
{
	debugDraw->drawSolidRect( rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()), color );
}

void ParallaxTileMap::drawDebugRectAt(Vec2 position, Color4F color)
{
    Vec2 coordinates = this->getTileCoordinatesForPosition(position);
    Rect rect = this->getTileRectFrom(coordinates);
    this->drawDebugRect(rect, color);
}

void ParallaxTileMap::addObjects()
{
	// loop over the object groups in this tmx file
	auto objectGroups = tileMap->getObjectGroups();

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

bool ParallaxTileMap::addObject(std::string className, ValueMap& properties)
{
	// create the object
	GameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{
		o->setName(className);
		objectLayer->addChild(o);
		return true;
	}

	return false;
}

void ParallaxTileMap::update(float delta)
{

}

Node* ParallaxTileMap::getShadowForNode(Node* node)
{
	auto shadow = Sprite::create();

	auto object = static_cast<Sprite*>(node);

	shadow->setSpriteFrame(object->getSpriteFrame());
	shadow->setAnchorPoint(Vec2(-0.05f, 0.02f));
	shadow->setColor(Color3B(0, 0, 0));
	shadow->setOpacity(50);

	return shadow;
}

Player* ParallaxTileMap::getPlayer()
{
	return static_cast<Player*>(objectLayer->getChildByName("Player"));
}

TMXLayer* ParallaxTileMap::getCollisionLayer()
{
	return collisionLayer;
}

TMXLayer* ParallaxTileMap::getLadderLayer()
{
	return ladderLayer;
}

Node* ParallaxTileMap::getObjectLayer()
{
	return objectLayer;
}

Size ParallaxTileMap::getMapSize()
{
	return tileMap->getMapSize();
}

Size ParallaxTileMap::getTileSize()
{
	return tileMap->getTileSize();
}

Value ParallaxTileMap::getPropertiesForGID(int GID)
{
	return tileMap->getPropertiesForGID(GID);
}

TileDataArray ParallaxTileMap::getTileDataArrayFromCollisionLayerAt(Vec2 position)
{
	return this->getTileDataArrayFromLayerAt(*collisionLayer, position);
}

TileDataArray ParallaxTileMap::getTileDataArrayFromLadderLayerAt(Vec2 position)
{
	// local variables
	TileDataArray tileDataArray;

	// get gameObjects tileCoordinates from position
	Vec2 tileCoordinates = getTileCoordinatesForPosition(position);

	tileDataArray[ETileGrid::BOTTOM] = getTileDataFromLayerAt(*ladderLayer, Vec2(tileCoordinates.x, tileCoordinates.y + 1));
	tileDataArray[ETileGrid::CENTER] = getTileDataFromLayerAt(*ladderLayer, tileCoordinates);
				
	return tileDataArray;
}

/// <summary>
/// Summary for getTileDataArrayFromLayerAt(TMXLayer layer, Vec2 position)
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
	Vec2 gameObjectTileCoordinates = getTileCoordinatesForPosition(position);

	// 3x3 grid
	for (int i = 0; i < 9; i++)
	{
		// get column and row
		int column = i % 3;
		int row = static_cast<int>(i / 3);

		// disregard col & row (1,1)
		if (column == 1 && row == 1) continue;

		//
		Vec2 tileCoordinates = Vec2( gameObjectTileCoordinates.x + (column - 1), gameObjectTileCoordinates.y + (row - 1) );
		
		//
		tileDataArray[count++] = getTileDataFromLayerAt(layer, tileCoordinates);
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
	if (isValidTileCoordinates(tileCoordinates))
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
bool ParallaxTileMap::isValidTileCoordinates(Vec2 v)
{	
	return ( v.y < mapSize.height && v.y >= 0 && v.x < mapSize.width && v.x >= 0 );
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
Vec2 ParallaxTileMap::getTileCoordinatesForPosition(Vec2 position)
{
	float x = floor(position.x / tileSize.width);

	float levelHeightInPixels = mapSize.height * tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / tileSize.height);

	return Vec2(x, y);
}

/// <summary>
/// Summary for getTileRectFrom(tileCoordinates)
///	
/// PRE-CONDITION:	A vec of TMX tilemap coordinates must be passed in as an argument.
///					Functions accesses the tilemap mapSize & tileSize local variables to return coordinates.
///
/// POST-CONDITION: A rect is returned from the passed in tile coordinates.
/// </summary>
Rect ParallaxTileMap::getTileRectFrom(Vec2 tileCoords)
{
	float levelHeightInPixels = mapSize.height * tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * tileSize.height));

	return Rect(origin.x, origin.y, tileSize.width, tileSize.height);
}
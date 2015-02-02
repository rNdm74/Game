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
		//node->addObjects();
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

	debugDraw = DrawNode::create();

	// get background layer
	backgroundLayer = tileMap->getLayer("background");
	backgroundLayer->retain();
	backgroundLayer->removeFromParentAndCleanup(false);
	backgroundLayer->getTexture()->setAntiAliasTexParameters();
	backgroundLayer->release();

	// get foreground layer
	foregroundLayer = tileMap->getLayer("foreground");
	foregroundLayer->retain();
	foregroundLayer->removeFromParentAndCleanup(false);
	foregroundLayer->getTexture()->setAntiAliasTexParameters();
	foregroundLayer->release();

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

	this->addChild(backgroundLayer,	-1, Vec2(0.4f, 0.4f), Vec2::ZERO);
	this->addChild(shadowLayer,		 0, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(foregroundLayer,	 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
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

GameObject* ParallaxTileMap::addObject(std::string className, ValueMap& properties)
{
	// create the object
	GameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{
		o->setName(className);
		objectLayer->addChild(o);
	}

	//
	return o;
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

TMXLayer* ParallaxTileMap::getForegroundLayer()
{
	return foregroundLayer;
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
	return this->getTileDataArrayFromLayerAt(*foregroundLayer, position);
}

TileDataArray ParallaxTileMap::getTileDataArrayFromLadderLayerAt(Vec2 position)
{
	return this->getTileDataArrayFromLayerAt(*ladderLayer, position);
}

TileDataArray ParallaxTileMap::getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position)
{
	// local variables
	int count = 0;
	int tileGid = 0;
	TileDataArray tileDataArray;

	Size mapSize = this->getMapSize();
	Size tileSize = this->getTileSize();

	Vec2 gameObjectTileCoordinates = Utils::getTileCoordForPosition(position, mapSize, tileSize);

	// 3x3 grid
	for (int i = 0; i < 9; i++)
	{
		int column = i % 3;
		int row = static_cast<int>(i / 3);

		// 0,0 | 0,1 | 0,2
		// 1,0 | 1,1 | 1,2
		// 2,0 | 2,1 | 2,2
		if (column == 1 && row == 1)
			continue;

		Vec2 tileCoordinates = Vec2( gameObjectTileCoordinates.x + (column - 1), gameObjectTileCoordinates.y + (row - 1) );

		// if its a valid tilepos for layer
		if (isValidTileCoordinates(tileCoordinates))
			tileGid = layer.getTileGIDAt(tileCoordinates);

		// create tiledata object
		if (tileGid)
		{
			tileDataArray[count].GID = tileGid;
			tileDataArray[count].tileRect = Utils::getTileRectFromTileCoords(tileCoordinates, mapSize, tileSize);
			tileDataArray[count].tileCoordinates = tileCoordinates;
		}

		count++;
	}

	// sort tileDataArray for faster checking
	// top left and bottom
	std::swap(tileDataArray[0], tileDataArray[6]); // bottom now in position 0 
	std::swap(tileDataArray[2], tileDataArray[3]); // left now in position 2
	std::swap(tileDataArray[3], tileDataArray[4]); // right now in position 3

	std::swap(tileDataArray[4], tileDataArray[6]); // top left now in position 4
	std::swap(tileDataArray[5], tileDataArray[6]); // top right now in position 5

	/*
	* OLD | NEW
	* --- + ---
	* 012 | 415
	* 3 4 | 2 3
	* 567 | 607
	*/

	return tileDataArray;
}

TileData ParallaxTileMap::getTileDataFromLadderLayerAtPosition(Vec2 position)
{
	int tileGID = 0;
	TileData tileData;

	Size mapSize = this->getMapSize();
	Size tileSize = this->getTileSize();

	Vec2 tileCoordinates = Utils::getTileCoordForPosition(position, mapSize, tileSize);

	// Make sure the coordinates are valid
	if ( isValidTileCoordinates(tileCoordinates) )
		tileGID = ladderLayer->getTileGIDAt(tileCoordinates);

	// create tiledata object
	if (tileGID)
	{
		tileData.GID = tileGID;
		tileData.tileRect = Utils::getTileRectFromTileCoords(tileCoordinates, mapSize, tileSize);
		tileData.tileCoordinates = tileCoordinates;
	}

	return tileData;
}

bool ParallaxTileMap::isValidTileCoordinates(Vec2 v)
{
	Size mapSize = this->getMapSize();	
	return ( v.y <= mapSize.height && v.y >= 0 && v.x <= mapSize.width && v.x >= 0 );
}

bool ParallaxTileMap::isTileLadder(Vec2 position)
{			
	return this->getTileDataFromLadderLayerAtPosition(position).GID;
}
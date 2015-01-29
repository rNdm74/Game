#include "ParallaxTileMap.h"
#include "GameObject.h"
#include "GameObjectFactory.h"

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

void ParallaxTileMap::setAliasTexParameters(TMXLayer* layer)
{
	layer->getTexture()->setAntiAliasTexParameters();

	/*Size layerSize = layer->getLayerSize();

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
	}*/
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
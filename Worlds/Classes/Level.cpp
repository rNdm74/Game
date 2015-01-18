#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Level.h"

void Level::loadMap(std::string mapname)
{
	this->createPhysicsWorld();

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	mapOrigin = Vec2(origin.x + visibleSize.width / 2, (origin.y + visibleSize.height / 2));

	map = TMXTiledMap::create(mapname);
	map->retain();
	
	parallaxNode = ParallaxNode::create();
	map->addChild(parallaxNode);

	TMXLayer* backgroundLayer = map->layerNamed("background");
	backgroundLayer->retain();
	backgroundLayer->removeFromParentAndCleanup(false);
	parallaxNode->addChild(backgroundLayer, -1, Vec2(0.2f, 0.2f), Vec2::ZERO);
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

		parallaxNode->addChild
		(
			rockGrassDown, 
			// Set z-index of rock
			3, 
			// Set ration (rocks don't move)
			Vec2(0.0f, 0.4f), 
			// Set position with random component
			Vec2(((visibleSize.width/2) / rocksQuantity) * (i) + RAND(0, s.width/2), top)
		);

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
				
		parallaxNode->addChild
		(
			rockGrassDown,
			// Set z-index of rock
			3,
			// Set ration (rocks don't move)
			Vec2(0.0f, 0.4f),
			// Set position with random component
			Vec2
			(
				(visibleSize.width / 1.6) + ((visibleSize.width / 2) / rocksQuantity) * (i) + RAND(0, s.width / 2), 
				top
			)
		);

		rockGrassDown->getTexture()->setAntiAliasTexParameters();
	}
	
	Sprite* groundGrass = Sprite::createWithSpriteFrameName("groundDirt.png");
	groundGrass->setAnchorPoint(Vec2(0, 0));
	auto shadow = getShadowForNode(groundGrass);
	shadow->setAnchorPoint(Vec2(-0.005f, 0.01f));
	shadow->setPosition(groundGrass->getPosition());
	groundGrass->addChild(shadow, -1);
	parallaxNode->addChild(groundGrass, 3, Vec2(0.0f, 0.4f), Vec2(0, 0));
	groundGrass->getTexture()->setAntiAliasTexParameters();
	groundGrass = Sprite::createWithSpriteFrameName("groundDirt.png");
	groundGrass->setAnchorPoint(Vec2(0, 0));	
	parallaxNode->addChild(groundGrass, 3, Vec2(0.0f, 0.4f), Vec2(groundGrass->getContentSize().width, 0));

	TMXLayer* foregroundLayer = map->layerNamed("foreground");
	foregroundLayer->retain();
	foregroundLayer->removeFromParentAndCleanup(false);
	parallaxNode->addChild(foregroundLayer, 1, Vec2(0.4f, 0.4f), Vec2::ZERO);
	foregroundLayer->release();
		
	Node* shadowLayer = Node::create();
	parallaxNode->addChild(shadowLayer, 0, Vec2(0.4f, 0.4f), Vec2::ZERO);

	// create all the rectangular fixtures for each tile
	Size layerSize = foregroundLayer->getLayerSize();

	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			// create a fixture if this tile has a sprite
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
	parallaxNode->addChild(collisionLayer, 2, Vec2(0.4f, 0.4f), Vec2::ZERO);
}

void Level::setAliasTexParameters(TMXLayer* layer)
{
	Size layerSize = layer->getLayerSize();

	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			// create a fixture if this tile has a sprite
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

void Level::createPhysicsWorld()
{
	// initialize variables, load the tmx, create the objects, etc...
	world = new b2World(b2Vec2(0, kGravity));

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
	world->SetContactListener(new ContactListener());
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
		o->addBodyToWorld(*world);

		collisionLayer->addChild(o);
	}
		
	//
	return o;
}

void Level::update(float& delta)
{	
	// 
	GameObject* player = static_cast<GameObject*>(collisionLayer->getChildByName("Player"));	
	b2Vec2 linearVelocity = player->getBody()->GetLinearVelocity();
	Vec2 mapPosition = map->getPosition();
	Vec2 playerPosition = player->getPosition();

	Rect mapBoundingBox = map->getBoundingBox();

	// call update functions of entities that uses cocos2d-x action methods, so the physics of this entities syncs with its sprites
	if (playerPosition.x < mapOrigin.x || playerPosition.x > mapOrigin.x)
		mapBoundingBox.origin.x -= linearVelocity.x;
	if (playerPosition.y < mapOrigin.y || playerPosition.y > mapOrigin.y)
		mapPosition.y -= linearVelocity.y;

	// only update if within bounds
	bool top = mapPosition.y < kMapBoundsY;
	bool left = mapBoundingBox.getMinX() <= origin.x;
	bool right = mapBoundingBox.getMaxX() > (origin.x + visibleSize.width);
	bool bottom = mapPosition.y > -kMapBoundsY;

	//if (left && right)
		map->setPosition(mapBoundingBox.origin);
		

	// call world step
	world->Step(delta, 1, 1);

	// manage the contacts registered by the contacts listenner that saves the contacts into a vector when they happens inside BeginContact

	// update entities that syncs its sprites with body positions
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
		static_cast<GameObject*>(body->GetUserData())->update(collisionLayer);


	
	// debug
	log("x: %f, y: %f", mapBoundingBox.getMaxX(), origin.x + visibleSize.width);
}
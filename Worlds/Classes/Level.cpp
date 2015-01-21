#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Level.h"

void Level::loadMap(std::string mapname)
{
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	
	size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + size.width / 2, (origin.y + size.height / 2));

	//get screen resolution 
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	
	//get design resolution
	auto winSize = Director::getInstance()->getWinSize();
	
	//get design resolution’s visable area size
	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//get origin of the visable area of design resolution
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
		
	map = TMXTiledMap::create(mapname);
	map->retain();

	mapInfo = TMXMapInfo::formatWithTMXFile(mapname.c_str());
	tileProperties = mapInfo->getTileProperties();

	this->createPhysicsWorld();
	this->load();
}

void Level::load()
{
	parallaxNode = ParallaxNode::create();
	parallaxNode->setContentSize(map->getContentSize());
	parallaxNode->setAnchorPoint(Vec2::ZERO);

	this->addChild(parallaxNode);

	backgroundLayer = map->getLayer("background");
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
			Vec2(((size.width / 2) / rocksQuantity) * (i)+RAND(0, s.width / 2), top)
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
			(size.width / 1.6) + ((size.width / 2) / rocksQuantity) * (i)+RAND(0, s.width / 2),
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

	foregroundLayer = map->getLayer("foreground");
	foregroundLayer->retain();
	foregroundLayer->removeFromParentAndCleanup(false);
	parallaxNode->addChild(foregroundLayer, 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	foregroundLayer->release();

	Node* shadowLayer = Node::create();
	parallaxNode->addChild(shadowLayer, 0, Vec2(1.0f, 1.0f), Vec2::ZERO);

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
	parallaxNode->addChild(collisionLayer, 2, Vec2(1.0f, 1.0f), Vec2::ZERO);
}

void Level::followPlayer()
{	
	//
	player = static_cast<Player*>(collisionLayer->getChildByName("Player"));

	/*player->retain();
	player->removeFromParentAndCleanup(false);
	this->addChild(player);
	player->release();*/
	
	player->setPosition(center);
	player->getBody()->SetGravityScale(0);
    
    this->setViewPointCenter(player->getPosition());

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

void Level::setViewPointCenter(Vec2 position)
{
    Size mapSize = map->getMapSize();
    Size tileSize = map->getTileSize();
    Size winSize = Director::getInstance()->getWinSize();
    
    float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (mapSize.width * tileSize.width) - winSize.width / 2);
    y = MIN(y, (mapSize.height * tileSize.height) - winSize.height / 2);
    
    Vec2 actualPosition = Vec2(x, y);
    
    Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
    Vec2 viewPoint = centerOfView - actualPosition;
    
    parallaxNode->setPosition(viewPoint);
}

void Level::update(float& delta)
{	
	Size mapSize = map->getMapSize();
	Size tileSize = map->getTileSize();
    Size playerSize = player->getSize();
    
    float mapWidthPixels = mapSize.width * tileSize.width;
    float mapHeightPixels = mapSize.height * tileSize.height;

	// call update functions of entities that uses cocos2d-x action methods, so the physics of this entities syncs with its sprites
	Vec2 newPlayerPos = player->getPosition();
	Vec2 velocity = Vec2(350, 350);
	Vec2 desiredVel = Vec2::ZERO;
	Vec2 direction = Vec2::ZERO;
	
	AppGlobal* global = AppGlobal::getInstance();

	if (global->states.LEFT)	
		direction.x = -1;
	if (global->states.RIGHT)	
		direction.x = 1;
	if (global->states.DOWN)	
		direction.y = -1;
	if (global->states.UP)		
		direction.y = 1;
	if (global->states.STOP)
		direction = Vec2::ZERO;
	
	newPlayerPos.x += velocity.x * delta * direction.x;
	newPlayerPos.y += velocity.y * delta * direction.y;

    bool xBounds = newPlayerPos.x <= mapWidthPixels && newPlayerPos.x >= 0;
    bool yBounds = newPlayerPos.y <= mapHeightPixels && newPlayerPos.y >= 0;
    
	// safety check on the bounds of the map
	if (xBounds && yBounds)
	{
        // player origin vector converted to tile co-ordinates
		int x = newPlayerPos.x / tileSize.width;
		int y = (mapHeightPixels - newPlayerPos.y) / tileSize.height;

        // calculate left, right, top, bottom tile co-ordinates of player
		int left = (newPlayerPos.x - playerSize.width / 2) / tileSize.width;
		int right = (newPlayerPos.x + playerSize.width / 2) / tileSize.width;
		int top = (mapHeightPixels - (newPlayerPos.y + playerSize.height / 2)) / tileSize.height;
		int bottom = (mapHeightPixels - (newPlayerPos.y - playerSize.height / 2)) / tileSize.height;

        // depending on direction player moving get gid
        Vec2 tileOrdinate = Vec2(x,y);
        
        if(direction.x == 1) tileOrdinate = Vec2(right, y);
        if(direction.x == -1) tileOrdinate = Vec2(left, y);
        if(direction.y == 1) tileOrdinate = Vec2(x, top);
        if(direction.y == -1) tileOrdinate = Vec2(x, bottom);
        
		int gid = foregroundLayer->getTileGIDAt(tileOrdinate);
		
		log("left: %i, right: %i, top: %i, bottom: %i", left, right, top, bottom);

		bool collided = false;

        if (gid)
        {
            auto valuemap = tileProperties[gid].asValueMap();
            collided = valuemap["Collidable"].asBool();
        }
		
		if (collided == false)
		{
			player->setPosition(newPlayerPos);
		}
	}
	
	this->setViewPointCenter(player->getPosition());

	// update world step
	world->Step(1.0f / 60, 8, 1);

	// manage the contacts registered by the contacts listener that saves the contacts into a vector when they happens inside BeginContact
	
	// update entities that syncs its sprites with body position
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
		static_cast<GameObject*>(body->GetUserData())->update(collisionLayer);

	// debug
	//log(": %f, : %f", parallaxNode->getContentSize().width, parallaxNode->getContentSize().height);
}
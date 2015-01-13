#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "Level.h"
#include "Box2dFactory.h"

void Level::loadMap(std::string mapname){

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 mapOrigin = Vec2(origin.x + visibleSize.width / 2, (origin.y + visibleSize.height / 2));

	map = TMXTiledMap::create(mapname);
	map->retain();
		
	auto background = map->getChildren();

	ParallaxNode* parallaxNode = ParallaxNode::create();
	parallaxNode->setAnchorPoint(Vec2(0.0f,0.0f));
	parallaxNode->setPosition(Vec2(0.0f, 0.0f));

	TMXLayer* foregroundLayer = map->layerNamed("foreground");
	foregroundLayer->retain();
	foregroundLayer->removeFromParentAndCleanup(false);	
	foregroundLayer->getTexture()->setAntiAliasTexParameters();
	foregroundLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	parallaxNode->addChild(foregroundLayer, 1, Vec2(0.5f, 0.0f), mapOrigin);//some points
	foregroundLayer->release();
	
	TMXLayer* backgroundLayer = map->layerNamed("background");
	backgroundLayer->retain();
	backgroundLayer->removeFromParentAndCleanup(false);
	backgroundLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	parallaxNode->addChild(backgroundLayer, -1, Vec2(0.4f, 0.5f), mapOrigin);//some points
	backgroundLayer->release();

	map->addChild(parallaxNode);
		
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
				parallaxNode->addChild(shadow, 0, Vec2(0.5f, 0.0f), Point(x * 70, origin.y + visibleSize.height - (y * 70)));//some points			
			}
		}
	}
}

Node* Level::getShadowForNode(Node* node)
{
	// Step 1
	auto shadow = Sprite::create();

	auto object = (Sprite*)node;
	shadow->setSpriteFrame(object->getSpriteFrame());

	// Step 2
	shadow->setAnchorPoint(Point(0.6f, 0.6f)); // position it to the center of the target node
	//shadow->setPosition(Point(0, 0));

	// Step 6
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
	physicsWorld = new b2World(b2Vec2(0, kGravity));

	physicsWorld->SetAllowSleeping(true);
	physicsWorld->SetContinuousPhysics(true);
	physicsWorld->SetContactListener(new ContactListener());

	factory = new Box2dFactory(map, physicsWorld);
}
void Level::prepareLayers()
{
	for (auto& object : this->map->getChildren())
	{
		// is this map child a tile layer?
		auto layer = static_cast<TMXLayer*>(object);

		if (layer != nullptr)
		{
			this->createFixtures(*layer);
		}			
	}
}
void Level::createFixtures(TMXLayer& layer)
{
	// create all the rectangular fixtures for each tile
	Size layerSize = layer.getLayerSize();

	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			// create a fixture if this tile has a sprite
			auto tileSprite = layer.getTileAt(Point(x, y));

			if (tileSprite)
			{
				this->createRectangularFixture(layer, x, y, 1.0f, 1.0f);
			}				
		}
	}
}
void Level::createRectangularFixture(TMXLayer& layer, int x, int y, int width, int height)
{
	// get position & size
	auto p = layer.convertToWorldSpace(layer.getPositionAt(Point(x, y)));
	auto tileSize = this->map->getTileSize();
		
	// create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set
	(
		(p.x + (tileSize.width / 2.0f)) / kPixelsPerMeter,
		(p.y + (tileSize.height / 2.0f)) / kPixelsPerMeter
	);
	
	b2Body* body = physicsWorld->CreateBody(&bodyDef);

	// define the shape
	b2PolygonShape shape;
	shape.SetAsBox
	(
		(tileSize.width / kPixelsPerMeter) * 0.5f * width,
		(tileSize.width / kPixelsPerMeter) * 0.5f * height
	);

	// create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 2.0f;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.2f;
	fixtureDef.filter.categoryBits = kFilterCategoryLevel;
	fixtureDef.filter.maskBits = 0xffff;
	body->CreateFixture(&fixtureDef);
}

void Level::createRectangularFixture(float x, float y, float width, float height, bool isSensor, uint16 categoryBits, uint16 maskBits)
{
	auto worldPos = map->convertToWorldSpace(Vec2(x, y));

	// create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set
	(
		(worldPos.x + width / 2.0f) / kPixelsPerMeter,
		(worldPos.y + height / 2.0f) / kPixelsPerMeter
	);

	b2Body* body = physicsWorld->CreateBody(&bodyDef);

	// define the shape
	b2PolygonShape shape;
	shape.SetAsBox
	(
		(width / kPixelsPerMeter) * 0.5f,
		(height / kPixelsPerMeter) * 0.5f 
	);

	// create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.02f;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isSensor;
	body->CreateFixture(&fixtureDef);
}

void Level::createGhostFixture(float x, float y, float width, float height, bool isSensor, uint16 categoryBits, uint16 maskBits)
{
	auto worldPos = map->convertToWorldSpace(Vec2(x, y));

	// create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(0, 0);
	
	b2Body* body = physicsWorld->CreateBody(&bodyDef);
	
	b2EdgeShape edgeShape;

	b2Vec2 v1 = b2Vec2
	(
		worldPos.x / kPixelsPerMeter,
		(worldPos.y + height) / kPixelsPerMeter
	);
	
	b2Vec2 v2 = b2Vec2
	(
		(worldPos.x + width) / kPixelsPerMeter,
		(worldPos.y + height) / kPixelsPerMeter
	);

	// create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &edgeShape;
	edgeShape.Set(v1, v2);
	edgeShape.m_vertex0.Set(v1.x - 1, v1.y);
	edgeShape.m_vertex3.Set(v2.x + 1, v2.y);
	edgeShape.m_hasVertex0 = true;
	edgeShape.m_hasVertex3 = true;

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.02f;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isSensor;
	body->CreateFixture(&fixtureDef);
}

void Level::createPolyLineFixture(std::vector<b2Vec2> vectors, int32 count, bool isSensor, uint16 categoryBits, uint16 maskBits)
{
	//auto worldPos = map->convertToWorldSpace(Vec2(x, y));

	// create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(0, 0);

	b2Body* body = physicsWorld->CreateBody(&bodyDef);

	b2PolygonShape polyShape;
	
	
	// create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polyShape;
	polyShape.m_count = count;

	for (int i = 0; i < count; i++)
	{
		polyShape.m_vertices[i].Set(vectors.at(i).x, vectors.at(i).y);
	}

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.02f;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isSensor;
	body->CreateFixture(&fixtureDef);
}

void Level::addObjects()
{
 //   auto group = map->getObjectGroup("ground");
	//auto slope = group->getObject("Slope");// ["polylinePoints"].asValueVector();
 //   float x1 = slope["x"].asFloat();
 //   float y1 = slope["y"].asFloat();
 //   auto points = slope["points"].asValueVector();//["polylinePoints"].asValueVector();
    //points[]
    //createPolyline(map->getObjectGroup("ground")->getObject("Slope"));
    
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
	auto x = properties["x"].asFloat();
	auto y = properties["y"].asFloat();
	auto width = properties["width"].asFloat();
	auto height = properties["height"].asFloat();

	if (className == "Ghost")
		this->createGhostFixture(x, y, width, height, false, kFilterCatagory::BOUNDARY, kFilterCatagory::PLAYER | kFilterCatagory::ENEMY);
	else if (className == "Slope")
		this->createPolyline(properties);
	else if (className == "Ladder")
		factory->createBody(kFilterCatagory::LADDER, Rect(x, y, width, height));
	else if (className == "Bounds")	
		factory->createBody(kFilterCatagory::BOUNDARY, Rect(x, y, width, height));
	else if (className == "Sensor")
		this->createRectangularFixture(x, y, width, height, true, kFilterCatagory::SENSOR, kFilterCatagory::PLAYER | kFilterCatagory::ENEMY);
	else if (className == "Start")
        AppGlobal::getInstance()->StartPosition = Vec2(x, y);

	// create the object
	GameObject* o = nullptr;

	//if (className == "GamePlayer")
	//	o = GamePlayer::createWithFrameName("alienBeige_stand.png");

	///*else if (className == "Monster")
	//	o = new Monster;
	//else if (className == "MagicChest")
	//	o = new MagicChest;*/

	//// process the new object
	//if (o != nullptr)
	//{
	//	o->setProperties(properties);
	//	o->addBodyToWorld(*physicsWorld);
	//	o->addFixturesToBody();
	//	this->addChild(o);
	//}

	return o;
}


void Level::createPolyline(ValueMap object)
{
    ValueVector pointsVector = object["points"].asValueVector();
    auto position = Point(object["x"].asFloat() / kPixelsPerMeter, object["y"].asFloat() / kPixelsPerMeter);
    
    //CCLOG("Size of pointsVector: %f", pointsVector.size());
    
    b2ChainShape polylineshape;
    float verticesSize = pointsVector.size()+ 1;
    b2Vec2 vertices[30];
    int vindex = 0;
    
    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0, 0);
    
    b2Body* body = physicsWorld->CreateBody(&bodyDef);
    
    b2FixtureDef fixtureDef;;
    
    for(Value point : pointsVector) {
        //CCLOG("Initializing vector at index: %d", vindex);
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / kPixelsPerMeter + position.x);
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / kPixelsPerMeter + position.y);
        vindex++;
    }
    
    polylineshape.CreateChain(vertices, vindex);
    
    fixtureDef.shape = &polylineshape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0;
    fixtureDef.filter.categoryBits = kFilterCatagory::BOUNDARY;
    fixtureDef.filter.maskBits = kFilterCatagory::PLAYER | kFilterCatagory::ENEMY;
    fixtureDef.isSensor = false;
    body->CreateFixture(&fixtureDef);
    
}

void Level::update(float& delta)
{
	physicsWorld->Step(delta, 1, 1);

	

	switch (AppGlobal::getInstance()->state)
	{
	case STATE_LEFT:
	{
		float currentX = map->getPositionX();
		currentX += 50 * delta * 1;
		map->setPositionX(currentX);
	}
	break;

	case STATE_STOP:

		break;

	case STATE_RIGHT:
	{
		float currentX = map->getPositionX();
		currentX += 50 * delta * -1;
		map->setPositionX(currentX);
	}
	break;

	case STATE_UP:

		break;

	case STATE_DOWN:

		break;
	}
}
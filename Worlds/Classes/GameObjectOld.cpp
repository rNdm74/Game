#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "PhysicsComponent.h"

#pragma region Menu
bool GameObject::isMenuActive() 
{ 
	return _menu->isActive(); 
}

void GameObject::addMenu()
{
	_menu->addMenu(*this);
}

void GameObject::showMenu()
{
	_menu->showMenu(*this);
}

void GameObject::hideMenu()
{
	_menu->hideMenu(*this);
}
#pragma endregion Menu

#pragma region Box2D

void GameObject::addBodyToWorld(b2World& world)
{
	// add a dynamic body to world
	// (subclasses can use other body types by overriding
	// this method and calling body->SetType())
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set
	(
		this->getPositionX() / kPixelsPerMeter,
		this->getPositionY() / kPixelsPerMeter
	);
	
	bodyDef.userData = this;
	bodyDef.fixedRotation = true;
	this->_body = world.CreateBody(&bodyDef);
	//this->_body->SetGravityScale(0);
	
}

void GameObject::addCircularHeadFixtureToBody(float radius, b2Vec2 offset)
{
	b2CircleShape shape;
	shape.m_radius = (radius * this->getScale()) / kPixelsPerMeter;
	shape.m_p = offset;
	this->createFixture(&shape, false, kFilterCatagory::PLAYER, kFilterCatagory::ENEMY);
}

void GameObject::addCircularBodyFixtureToBody(float radius, b2Vec2 offset)
{
	b2CircleShape shape;
	shape.m_radius = (radius * this->getScale()) / kPixelsPerMeter;
	shape.m_p = offset;
	this->createFixture(&shape, false, kFilterCatagory::PLAYER, kFilterCatagory::BOUNDARY | kFilterCatagory::LADDER);
}

void GameObject::addPolygonShapeToBody()
{
	//setup platform shape for reuse
	b2PolygonShape polygonShape;
	b2Vec2 verts[8];

	verts[0].Set(0.25,	0);		// center point
	verts[1].Set(1.075,	0.5);	// right bottom
	verts[2].Set(1.075,	1);		// right bottom
	verts[3].Set(0.25,	2.15);	// right top
	verts[4].Set(-0.25, 2.15);	// left top
	verts[5].Set(-1.075,	1);		// right bottom
	verts[6].Set(-1.075,	0.5);	// left bottom
	verts[7].Set(-0.25, 0);		// center point

	polygonShape.Set(verts, 8);

	this->createFixture(&polygonShape, false, kFilterCatagory::PLAYER, kFilterCatagory::BOUNDARY | kFilterCatagory::LADDER | kFilterCatagory::SENSOR);
}

void GameObject::addRectangularFixtureToBody(float width, float height)
{	
	b2PolygonShape shape;
	shape.SetAsBox
	(
		(width / kPixelsPerMeter) * 0.5f,
		(height / kPixelsPerMeter) * 0.5f,
		b2Vec2(0, 1.1),
		0.0f
	);
	
	this->createFixture(&shape, false, kFilterCatagory::PLAYER, kFilterCatagory::BOUNDARY | kFilterCatagory::ENEMY | kFilterCatagory::LADDER | kFilterCatagory::SENSOR);
}

void GameObject::addSensorRectangleToBody(float offset)
{
	b2PolygonShape shape;
	shape.SetAsBox
	(
		(40 / kPixelsPerMeter) * 0.5f,
		(1 / kPixelsPerMeter) * 0.5f,
		b2Vec2(0, offset),
		0.0f
	);

	this->createFixture(&shape, true, kFilterCatagory::PLAYER, kFilterCatagory::LADDER);
}

void GameObject::createFixture(b2Shape* shape, bool isSensor, uint16 categoryBits, uint16 maskBits)
{
	// note that friction, etc. can be modified later by looping
	// over the body's fixtures and calling fixture->SetFriction()
	b2FixtureDef fixtureDef;

	fixtureDef.shape = shape;
	fixtureDef.density = kGameObjectFixtureDensity;
	fixtureDef.friction = kGameObjectFixtureFriction;
	//fixtureDef.restitution = kGameObjectFixtureRestitution;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isSensor;

	this->_body->CreateFixture(&fixtureDef);
}

void GameObject::setProperties(ValueMap& properties)
{
	this->setPosition
	(
		Point
		(
			properties["x"].asFloat(),
			properties["y"].asFloat()
		)
	);
}

#pragma endregion Box2D

void GameObject::initListeners()
{
	auto listener = EventListenerTouchOneByOne::create();
	
	listener->onTouchBegan = [=](cocos2d::Touch* touch, cocos2d::Event* event) -> bool {

		auto touchEvent = static_cast<EventTouch*>(event);

		auto node = touchEvent->getCurrentTarget();
		
		if (node->getBoundingBox().containsPoint(touch->getLocation()))
		{
			log("Bingo");
			
			auto player = static_cast<GamePlayer*>(node);

			auto scaleUpAction = ScaleTo::create(0.1, 1.1);
			auto scaleDownAction = ScaleTo::create(0.1, 1.0);

			// GameObject selected effect
			node->runAction(Sequence::createWithTwoActions(scaleUpAction, scaleDownAction));

			if (player->isMenuActive())
			{
				player->hideMenu();
			}
			else
			{
				player->showMenu();
			}			
		}

		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

GamePlayer* GamePlayer::createWithFrameName(const std::string& arg)
{
	auto sprite = new GamePlayer
	(
		new PlayerMenuComponent(),
		new PlayerInputComponent(),
		new PlayerPhysicsComponent(),
		new PlayerGraphicsComponent()
	);
    
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(arg);
    
    if(sprite && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        sprite->setAnchorPoint(Vec2(0.5, 0));
		sprite->setName("Player");
		sprite->initListeners();
		sprite->addMenu();
		sprite->hideMenu();
		                
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}

void GamePlayer::addFixturesToBody()
{
	auto size = this->getContentSize();
	//this->addRectangularFixtureToBody(40,70);
	this->addCircularHeadFixtureToBody(33, b2Vec2(0, 1.8));
	//this->addCircularBodyFixtureToBody(20, b2Vec2(0, 0.6));
	//this->addSensorRectangleToBody(0);
	this->addPolygonShapeToBody();
}

void GamePlayer::update(float& delta, b2World& physics)
{
	_physics->update(*this);
	_input->update(*this, delta);    
	_graphics->update(*this);
}


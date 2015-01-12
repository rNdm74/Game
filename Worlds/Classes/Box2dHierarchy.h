#ifndef __com_dotdat_World__BOX2D_HIERARCHY_H__
#define __com_dotdat_World__BOX2D_HIERARCHY_H__

#include "cocos2d.h"
#include "Constants.h"
#include "Box2D.h"

using namespace cocos2d;

class Helper
{
public:
	Helper(TMXTiledMap* map, b2World* physicsWorld)
	{
		_map = map;
		_physicsWorld = physicsWorld;
	};

	~Helper(){};

	b2BodyDef createBodyDef(b2BodyType bodyType, Rect rect);
	b2Body* createBody(b2BodyDef& bodyDef);
	b2PolygonShape createShape(float width, float height);
	b2FixtureDef createFixture
	(
		b2PolygonShape& shape,
		float density,
		float friction,
		float restitution,
		uint16 categoryBits,
		uint16 maskBits,
		bool isSensor
	);

private:
	TMXTiledMap* _map;
	b2World* _physicsWorld;
};

// Base class
class Body
{
public:
	Body(Rect rect, TMXTiledMap* map, b2World* physicsWorld)
	{
		helper = new Helper(map, physicsWorld);
		bodyDef = helper->createBodyDef(b2_staticBody, rect);
		body = helper->createBody(bodyDef);
		shape = helper->createShape(rect.size.width, rect.size.height);
	};

	virtual ~Body(){};

protected:
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape shape;
	b2FixtureDef fixture;
	Helper* helper;
};

// Children classes

class MapLevel : public Body
{
public:
	MapLevel(Rect rect, TMXTiledMap* map, b2World* physicsWorld) : Body(rect, map, physicsWorld)
	{
		fixture = helper->createFixture
		(
			shape,
			2.0f,
			0.2f,
			0.2f,
			kFilterCatagory::SENSOR,
			kFilterCatagory::PLAYER | kFilterCatagory::ENEMY,
			true
		);

		body->CreateFixture(&fixture);
	};

	~MapLevel(){};
};

class Ladder : public Body
{
public:
	Ladder(Rect rect, TMXTiledMap* map, b2World* physicsWorld) : Body(rect, map, physicsWorld)
	{
		fixture = helper->createFixture
		(
			shape,
			2.0f,
			0.2f,
			0.2f,
			kFilterCatagory::LADDER,
			kFilterCatagory::PLAYER | kFilterCatagory::ENEMY,
			true
		);

		body->CreateFixture(&fixture);
	}

	~Ladder(){};
};

class Boundary : public Body
{
public:
	Boundary(Rect rect, TMXTiledMap* map, b2World* physicsWorld) : Body(rect, map, physicsWorld)
	{
		fixture = helper->createFixture
		(
			shape,
			2.0f,
			0.2f,
			0.0f,
			kFilterCatagory::BOUNDARY,
			kFilterCatagory::PLAYER | kFilterCatagory::ENEMY,
			false
		);

		body->CreateFixture(&fixture);
	}

	~Boundary(){};
};

class StartPosition : public Body
{

};

#endif /* defined(__com_dotdat_World__BOX2D_HIERARCHY_H__) */

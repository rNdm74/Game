#include "Box2dHelper.h"
#include "Box2D.h"
#include "GameObject.h"

// Helper functions

Box2dHelper::Box2dHelper()
{

};

b2BodyDef Box2dHelper::createBodyDef(b2BodyType bodyType, float x_, float y_, GameObject* gameObject)
{
	b2BodyDef bodyDef;

	bodyDef.type = bodyType;
	bodyDef.userData = gameObject;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(x_, y_);

	return bodyDef;
};

b2Body* Box2dHelper::createBody(b2World& world, b2BodyDef& bodyDef, b2FixtureDef& fixtureDef)
{
	b2Body* body = world.CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	return body;
};

b2PolygonShape Box2dHelper::createBoxShape(float width, float height)
{
	// define the shape
	b2PolygonShape shape;

	shape.SetAsBox
	(
		(width / kPixelsPerMeter) * 0.5f,
		(height / kPixelsPerMeter) * 0.5f
	);

	return shape;
};

b2ChainShape Box2dHelper::createPolyineShape(ValueVector pointsVector, float x, float y)
{
	// define the shape
	b2ChainShape shape;

	float verticesSize = pointsVector.size() + 1;
	b2Vec2 vertices[30];
	int vindex = 0;

	Vec2 position = Vec2(x / kPixelsPerMeter, y / kPixelsPerMeter);

	for (Value point : pointsVector)
	{
		vertices[vindex].x = (point.asValueMap()["x"].asFloat() / kPixelsPerMeter + position.x);
		vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / kPixelsPerMeter + position.y);
		vindex++;
	}

	shape.CreateChain(vertices, vindex);

	return shape;
};

b2FixtureDef Box2dHelper::createFixtureDef
(
	b2Shape& shape,
	float density,
	float friction,
	float restitution,
	uint16 categoryBits,
	uint16 maskBits,
	bool isSensor
)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isSensor;

	return fixtureDef;
};

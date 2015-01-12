#include "Box2dHierarchy.h"


// Helper functions
b2BodyDef Helper::createBodyDef(b2BodyType bodyType, Rect rect)
{
	auto worldPos = _map->convertToWorldSpace(rect.origin);

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	bodyDef.position.Set
	(
		(worldPos.x + rect.size.width / 2.0f) / kPixelsPerMeter,
		(worldPos.y + rect.size.height / 2.0f) / kPixelsPerMeter
	);

	return bodyDef;
};

b2Body* Helper::createBody(b2BodyDef& bodyDef)
{
	return _physicsWorld->CreateBody(&bodyDef);
};

b2PolygonShape Helper::createShape(float width, float height)
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

b2FixtureDef Helper::createFixture
(
	b2PolygonShape& shape,
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
#ifndef __com_dotdat_World__BOX2D_HELPER_H__
#define __com_dotdat_World__BOX2D_HELPER_H__

#include "cocos2d.h"
#include "Constants.h"
#include "Box2D.h"

using namespace cocos2d;

class GameObject;

class IBox2dHelper
{
	static b2BodyDef createBodyDef(b2BodyType bodyType, float x_, float y_, GameObject* gameObject);
	static b2Body* createBody(b2World& world, b2BodyDef& bodyDef, b2FixtureDef& fixture);
	static b2PolygonShape createBoxShape(float width, float height);
	static b2ChainShape createPolyineShape(ValueVector pointsVector, float x, float y);
	static b2FixtureDef createFixtureDef
	(
		b2Shape& shape,
		float density,
		float friction,
		float restitution,
		uint16 categoryBits,
		uint16 maskBits,
		bool isSensor
	);
};

class Box2dHelper : public IBox2dHelper
{
public:
	Box2dHelper();
	virtual ~Box2dHelper(){};

	static b2BodyDef createBodyDef(b2BodyType bodyType, float x_, float y_, GameObject* gameObject);
	static b2Body* createBody(b2World& world, b2BodyDef& bodyDef, b2FixtureDef& fixture);
	static b2PolygonShape createBoxShape(float width, float height);
	static b2ChainShape createPolyineShape(ValueVector pointsVector, float x, float y);
	static b2FixtureDef createFixtureDef
	(
		b2Shape& shape,
		float density,
		float friction,
		float restitution,
		uint16 categoryBits,
		uint16 maskBits,
		bool isSensor
	);
};

#endif /* defined(__com_dotdat_World__BOX2D_HELPER_H__) */

#include "Box2dFactory.h"

Body* Box2dFactory::createBody(kFilterCatagory catagory, Rect rect)
{
	Body* body = nullptr;

	switch (catagory)
	{
	case kFilterCatagory::BOUNDARY:
		body = new Boundary(rect, _map, _physicsWorld);
		break;

	case kFilterCatagory::LADDER:
		body = new Ladder(rect, _map, _physicsWorld);
		break;

	case kFilterCatagory::SENSOR:
		body = new MapLevel(rect, _map, _physicsWorld);
		break;
	}

	return body;
}
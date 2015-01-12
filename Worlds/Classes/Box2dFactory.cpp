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
		body = new Boundary(rect, _map, _physicsWorld);
		break;

	case kFilterCatagory::SENSOR:
		body = new Boundary(rect, _map, _physicsWorld);
		break;
	}

	return body;
}
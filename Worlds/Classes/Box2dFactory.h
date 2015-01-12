#ifndef __com_dotdat_World__BOX2D_FACTORY_H__
#define __com_dotdat_World__BOX2D_FACTORY_H__

#include "cocos2d.h"
#include "Box2D.h"
#include "Box2dHierarchy.h"

using namespace cocos2d;

class IBox2dFactory
{
public:
	virtual Body* createBody(kFilterCatagory catagory, Rect rect) = 0;
};

class Box2dFactory : public IBox2dFactory
{
public:
	Box2dFactory(TMXTiledMap* map, b2World* physicsWorld)
	{
		_map = map;
		_physicsWorld = physicsWorld;
	};

	~Box2dFactory(){};
	
	virtual Body* createBody(kFilterCatagory catagory, Rect rect);

private:
	TMXTiledMap* _map;
	b2World* _physicsWorld;
};

#endif /* defined(__com_dotdat_World__BOX2D_FACTORY_H__) */

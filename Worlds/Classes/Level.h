#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"
#include "Box2D.h"

class GameObject;
class Box2dFactory;

using namespace cocos2d;

class Level : public Node, public b2ContactListener
{
public:

	void loadMap(std::string mapname);
	
	Level();
	virtual ~Level(void);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	void createPhysicsWorld();
	void prepareLayers();
	void createFixtures(TMXLayer& layer);
	void createRectangularFixture(TMXLayer& layer, int x, int y, int width, int height);
	void createRectangularFixture(float x, float y, float width, float height, bool isSensor, uint16 categoryBits, uint16 maskBits);
	void createGhostFixture(float x, float y, float width, float height, bool isSensor, uint16 categoryBits, uint16 maskBits);
	void createPolyLineFixture(std::vector<b2Vec2> vectors, int32 count, bool isSensor, uint16 categoryBits, uint16 maskBits);
	void addObjects();

	GameObject* addObject(std::string className, ValueMap& properties);

	void update(float& delta);

	TMXTiledMap* getMap() { return map; }
	b2World* getPhysicsWorld() { return physicsWorld; }

private:
	TMXTiledMap* map;
	b2World* physicsWorld;

	Box2dFactory* factory;

	int objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */
#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"
#include "Box2D.h"

class GameObject;
class GameObjectFactory;

using namespace cocos2d;

class Level : public Node
{
public:

	void loadMap(std::string mapname);
	
	Node* getShadowForNode(Node* node);

	Level();
	virtual ~Level(void);
		
	void createPhysicsWorld();
	
	void addObjects();

	GameObject* addObject(std::string className, ValueMap& properties);

	void update(float& delta);

	void setAliasTexParameters(TMXLayer* layer);

	TMXTiledMap* getMap() { return map; }
	b2World* getWorld() { return world; }
	GameObject* getPlayer() { return player; }

private:
	GameObject* player;

	TMXTiledMap* map;
	b2World* world;

	//GameObjectFactory* factory;

	ParallaxNode* parallaxNode;
	Node* collisionLayer;

	Vec2 origin;
	Vec2 mapOrigin;
	Size visibleSize;

	int objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

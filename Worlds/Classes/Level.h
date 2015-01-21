#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"
#include "Box2D.h"

class Player;
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
		
	void load();

	void createPhysicsWorld();
	
	void addObjects();
	void followPlayer();

	GameObject* addObject(std::string className, ValueMap& properties);

	void update(float& delta);

	void setAliasTexParameters(TMXLayer* layer);
    
    void setViewPointCenter(Vec2 position);

	TMXTiledMap* getMap() { return map; }
	b2World* getWorld() { return world; }
	Player* getPlayer() { return player; }

private:
	Player* player;

	TMXTiledMap* map;
	TMXMapInfo* mapInfo;
	
	TMXLayer* backgroundLayer;
	TMXLayer* foregroundLayer;

	ValueMapIntKey tileProperties;

	b2World* world;

	//GameObjectFactory* factory;

	ParallaxNode* parallaxNode;
	Node* collisionLayer;

	Vec2 origin;
	Vec2 center;
	Size size;

	int objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

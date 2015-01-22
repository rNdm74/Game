#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"

class AppGlobal;
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
	void addObjects();
	void followPlayer();

	GameObject* addObject(std::string className, ValueMap& properties);

	void update(float& delta);

	void setAliasTexParameters(TMXLayer* layer);
    
    void setViewPointCenter(Vec2 position);

	TMXTiledMap* getMap() { return map; }
	
	Player* getPlayer() { return player; }

private:
	AppGlobal* global;

	Player* player;

	TMXTiledMap* map;
	TMXMapInfo* mapInfo;
	
	TMXLayer* backgroundLayer;
	TMXLayer* foregroundLayer;

	ValueMapIntKey tileProperties;
	
	ParallaxNode* parallaxNode;
	Node* collisionLayer;

	Vec2 origin;
	Vec2 center;
	Size size;

	int objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

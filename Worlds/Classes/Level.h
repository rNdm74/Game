#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"
#include <array>

class AppGlobal;
class Player;
class GameObject;
class GameObjectFactory;
class ParallaxTileMap;

using namespace cocos2d;

typedef std::array<TileData, 8> TileDataArray;

class Level : public Node
{	
public:
	Level();
	virtual ~Level(void);

	void loadMap(std::string mapname);
		
	void followPlayer();
	
	void update(float& delta);
		    
    void setViewPointCenter(Vec2 position);
		
	void checkForAndResolveCollisions(GameObject* gameObject);

private:
	AppGlobal* global;

	Player* player;

	ValueMapIntKey tileProperties;
	
	ParallaxTileMap* parallaxTileMap;

	Vec2 origin;
	Vec2 center;
	Size size;

	int objectCount;

	std::stack<ParallaxTileMap*> activeMap;
	std::vector<GameObject*> gameObjectList;

	DrawNode* drawNode;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

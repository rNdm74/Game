#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"
#include <array>

class AppGlobal;
class Player;
class GameObject;
class ParallaxTileMap;

using namespace cocos2d;

class Level : public Node
{	
public:
	// "creator" methods first	
	static Level* create(std::string mapName);

	Level();
	virtual ~Level();

	void loadMap(std::string mapname);		
	void loadPlayer();
	
	void update(float& delta);
		    
    void setViewPointCenter(Vec2 position);
		
	void checkNextMap(GameObject* gameObject);

	void renderViewPort();

private:
	ParallaxTileMap* parallaxTileMap;
	AppGlobal* global;
	Player* player;
		
	Vec2 origin;
	Vec2 center;

	Size visibleSize;
	Size winSize;
	Size mapSize;
	Size tileSize;

	int objectCount;

	std::stack<ParallaxTileMap*> activeMap;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

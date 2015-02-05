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
	// "creator" methods first	
	static Level* create(std::string mapName);

	Level();
	virtual ~Level();

	void loadMap(std::string mapname);		
	void loadPlayer();
	
	void update(float& delta);
		    
    void setViewPointCenter(Vec2 position);
	void scrollLevelMap(Vec2 position);
		
	void checkNextMap(GameObject* gameObject);

private:
	ParallaxTileMap* parallaxTileMap;
	AppGlobal* global;
	Player* player;

	bool scrollMap;
	
	Vec2 origin;
	Vec2 center;
	Size size;

	int objectCount;

	std::stack<ParallaxTileMap*> activeMap;
	std::vector<GameObject*> gameObjectList;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

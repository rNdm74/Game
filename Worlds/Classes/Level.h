#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"

class AppGlobal;
class IGameObject;
class ParallaxTileMap;
class IPathFinder;
class IPath;

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

	void updateState();
		    
    void setViewPointCenter();
		
	void checkNextMap();	

private:
	ParallaxTileMap* _parallaxTileMap;
	IPathFinder* _pathFinder;

	

	Vec2 _origin;
	Vec2 _center;

	Size _visibleSize;
	Size _mapSize;
	Size _tileSize;

	int _objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

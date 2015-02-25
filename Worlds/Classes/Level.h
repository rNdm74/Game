#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"

using namespace cocos2d;

typedef std::stack<class IParallaxTileMap*> ActiveTileMap;

class Level : public Node
{	
public:
	// "creator" methods first	
	static Level* create();

	Level();
	virtual ~Level(){};

	void loadMap();		
	void loadPlayer();
	
	void update(float& delta);
	
    void setViewPointCenter();		
	void checkNextMap();	

private:
	ActiveTileMap activeMap;

	/** **/
	Size _visibleSize, _mapSize, _tileSize;
	/** **/
	Vec2 _origin, _center;
	/** **/
	int _objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

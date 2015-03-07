#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "Constants.h"

class Level : public cocos2d::Node
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
	cocos2d::Size _visibleSize, _mapSize, _tileSize;
	/** **/
	cocos2d::Vec2 _origin, _center;
	/** **/
	int _objectCount;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

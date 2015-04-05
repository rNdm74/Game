#ifndef __PoliticallyIncorrect__Components_H__
#define __PoliticallyIncorrect__Components_H__

#include "cocos2d.h"
#include "Constants.h"

class IGameObject;

class ExtendedTMXTiledMap : public cocos2d::TMXTiledMap
{
public:
	cocos2d::Vec2 velocity;

public:
	static ExtendedTMXTiledMap* create(const std::string& tmxFile);

	ExtendedTMXTiledMap();
	virtual ~ExtendedTMXTiledMap();

	virtual void update(float delta);

	virtual void setPositionOnPlayer(cocos2d::Rect collisionBox);	

	virtual void initGameObjects();
	
	virtual cocos2d::Vec2 getTileCoordFromTouch(cocos2d::Vec2 position);
	virtual void selectTile(cocos2d::Vec2 coord);
	virtual void deselectTile(cocos2d::Vec2 coord);

private:
	virtual bool initGameObject(std::string className, cocos2d::ValueMap& properties);

	virtual cocos2d::Vec2 getTileCoordFrom(IGameObject* gameObject);
	
	virtual void setChildZOrder(IGameObject* gameObject, cocos2d::Vec2 tileCoord);

	int _objectCount;
		
	cocos2d::DrawNode* _debugLayer;
	cocos2d::TMXLayer* floorLayer;
};

#endif // __PoliticallyIncorrect__Components_H__

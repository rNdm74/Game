#ifndef __PoliticallyIncorrect__Components_H__
#define __PoliticallyIncorrect__Components_H__

#include "cocos2d.h"
#include "Constants.h"

class IPath;
class AStarPathFinder;
class IGameObject;

class ExtendedTMXTiledMap : public cocos2d::TMXTiledMap
{
public:
    
    cocos2d::Vec2 source;
    cocos2d::Vec2 destination;

public:
	static ExtendedTMXTiledMap* create(const std::string& tmxFile);

	ExtendedTMXTiledMap();
	virtual ~ExtendedTMXTiledMap();

	virtual void update(float delta);

	virtual void setPositionOnPlayer(cocos2d::Rect collisionBox);	

	virtual void initGameObjects();
	
	virtual cocos2d::Vec2 getTileCoordFrom(cocos2d::Vec2 position);
	
    /**  **/
    virtual void selectTile(cocos2d::Vec2 coord);
	virtual void deselectTile(cocos2d::Vec2 coord);

    /** Pathfinding **/
    virtual bool isBlocked(cocos2d::Vec2 coordinate);
    virtual float getCost(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation);
    virtual IPath* getPath(cocos2d::Vec2 startLocation, cocos2d::Vec2 targetLocation);
    
private:
	virtual bool initGameObject(std::string className, cocos2d::ValueMap& properties);

	virtual cocos2d::Vec2 getTileCoordFrom(IGameObject* gameObject);
	
	virtual void setChildZOrder(IGameObject* gameObject, cocos2d::Vec2 tileCoord);

	int _objectCount;
		
	cocos2d::DrawNode* _debugLayer;
	cocos2d::TMXLayer* floorLayer;
    
    AStarPathFinder* pathfinder;
};

#endif // __PoliticallyIncorrect__Components_H__

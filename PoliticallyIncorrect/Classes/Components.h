#ifndef __PoliticallyIncorrect__Components_H__
#define __PoliticallyIncorrect__Components_H__

#include "cocos2d.h"
#include "Constants.h"

class IPath;
class AStarPathFinder;
class Player;
class IGameObject;

class ExtendedTMXTiledMap : public cocos2d::TMXTiledMap
{
public:
	IPath* playerPath;

public:
	static ExtendedTMXTiledMap* create(const std::string& tmxFile);

	ExtendedTMXTiledMap();
	virtual ~ExtendedTMXTiledMap();

	virtual void update(float delta);
		
	virtual void initGameObjects();
	
	virtual bool isTileCoordValid(cocos2d::Vec2 coord);
	virtual cocos2d::Vec2 getTileCoordFrom(cocos2d::Vec2 position);
	virtual cocos2d::Vec2 getTileCoordFrom(IGameObject* gameObject);
	virtual cocos2d::Rect getTileRectFrom(cocos2d::Vec2 coord);
	
    /**  Tiles **/
    virtual void selectTile(cocos2d::Vec2 coord);
	virtual void deselectTile(cocos2d::Vec2 coord);
	virtual TileData getTileDataFromLayerAt(cocos2d::TMXLayer& layer, cocos2d::Vec2 tileCoordinates);
	virtual TileDataArray getTileDataArrayFromLayerAt(cocos2d::TMXLayer& layer, cocos2d::Vec2 coords);


    /** Pathfinding **/
    virtual bool isBlocked(cocos2d::Vec2 coordinate);
	virtual float getCost(cocos2d::Vec2 startLocation, cocos2d::Vec2 neighbourCoordinate);
	virtual int calculateCostFactor(TileDataArray tileDataArray);
	virtual IPath* findPath(cocos2d::Vec2 sourceCoordinate, cocos2d::Vec2 targetCoordinate);

	/** Debug **/
	virtual void drawRect(cocos2d::Rect rect, cocos2d::Color4F color);

	/** Player methods **/
	virtual void setPositionOnPlayer();
	virtual cocos2d::Vec2 getPlayerPosition();
	virtual void selectPlayer();
	virtual void deselectPlayer();
	virtual bool playerIsSelected();
	virtual void playerSetPath();
	virtual void playerUnSetPath();
	virtual bool playerHasActivePath();
	virtual void movePlayerAlongPath(IPath& path);

	
    
private:
	virtual bool initGameObject(std::string className, cocos2d::ValueMap& properties);
		
	virtual void setChildZOrder(IGameObject* gameObject, cocos2d::Vec2 tileCoord);

	int _objectCount;
		
	cocos2d::DrawNode* _debugLayer;
	cocos2d::TMXLayer* groundLayer;
    
    AStarPathFinder* pathfinder;

	Player* playerInstance;
};

#endif // __PoliticallyIncorrect__Components_H__

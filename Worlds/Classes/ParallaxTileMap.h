#ifndef __com_dotdat_World__PARALLAX_TILEMAP_H__
#define __com_dotdat_World__PARALLAX_TILEMAP_H__

#include "cocos2d.h"
#include "Constants.h"

class IPath;
class IGameObject;

using namespace cocos2d;

class IParallaxTileMap : public ParallaxNode
{
public:	
	IParallaxTileMap(){};
	virtual ~IParallaxTileMap(){};

	virtual void update(float& delta) = 0;

	/** Pathfinding **/
	virtual bool isBlocked(Vec2 coordinate) = 0;
	virtual float getCost(Vec2 startLocation, Vec2 targetLocation) = 0;

	/** Getters **/
	virtual Rect getViewportBoundingBox() = 0;
	virtual Size getMapSize() = 0;
	virtual Size getTileSize() = 0;
	virtual IGameObject* getPlayer() = 0;
	virtual TileDataArray getCollisionDataAt(Vec2 position) = 0;
	virtual TileDataArray getLadderDataAt(Vec2 position) = 0;
    virtual float getWidth() = 0;
	virtual Vec2 getTileCoordinatesFor(Vec2 position) = 0;
	virtual Rect getTileRectFrom(Vec2 tileCoordinates) = 0;
	virtual cocos2d::Vector<cocos2d::Node*> getObjects() = 0;
	virtual IGameObject* getSelectedGameObject(Vec2 target) = 0;
    
	/** Setters **/
	virtual void enableForegroundOpacity(int fade) = 0;
	virtual void enableParallaxForegroundOpacity(int fade) = 0;

	/** Pathfinding **/
	virtual IPath* findPath(Vec2 origin, Vec2 target) = 0;

	/** Player **/
	virtual void addPlayer(IGameObject* player) = 0;
	virtual void removePlayer() = 0;
	virtual void setPositionOnPlayer() = 0;
	virtual ValueMap getInitialProperties() = 0;
		
#if DEBUG_ENABLE
	
	virtual void clearDebugDraw() = 0;
	virtual void drawDebugRect(Rect rect, Color4F color) = 0;
	virtual void drawDebugRectAt(Vec2 position, Color4F color) = 0;
	virtual void drawDebugRectAtTile(Vec2 coordinates, Color4F color) = 0;

#endif // DEBUG_ENABLE

};

class ParallaxTileMap : public IParallaxTileMap
{
public:
	/** Constructors and the Destructor **/
	ParallaxTileMap(std::string type);
	virtual ~ParallaxTileMap();

	void init(TMXTiledMap* tileMap, Texture2D* texture);

	/** Overrides**/
	void update(float& delta) override;	
	
	/** Pathfinding **/
	bool isBlocked(Vec2 coordinate) override;
	float getCost(Vec2 startLocation, Vec2 targetLocation) override;

	/** Getters **/
	virtual Rect getViewportBoundingBox() override;
	virtual Size getMapSize() override;
	virtual Size getTileSize() override;
	virtual IGameObject* getPlayer() override;
	virtual TileDataArray getCollisionDataAt(Vec2 position) override;
	virtual TileDataArray getLadderDataAt(Vec2 position) override;
    virtual float getWidth() override;
	virtual Vec2 getTileCoordinatesFor(Vec2 position) override;
	virtual Rect getTileRectFrom(Vec2 tileCoordinates) override;
	virtual cocos2d::Vector<cocos2d::Node*> getObjects() override;
	virtual IGameObject* getSelectedGameObject(Vec2 target) override;

    /** Setters **/
	virtual void enableForegroundOpacity(int fade) override;
	virtual void enableParallaxForegroundOpacity(int fade) override;
    
	/** Pathfinding **/
	virtual IPath* findPath(Vec2 origin, Vec2 target) override;

	/** Player **/
	virtual void addPlayer(IGameObject* player);
	virtual void removePlayer();
	virtual void setPositionOnPlayer();
	virtual ValueMap getInitialProperties();

	/** Flags **/
	bool isValid(Vec2 tileCoordinates);
	bool isTileLadder(Vec2 position);
								
#if DEBUG_ENABLE

	virtual void clearDebugDraw();
	virtual void drawDebugRect(Rect rect, Color4F color);
	virtual void drawDebugRectAt(Vec2 position, Color4F color);
	virtual void drawDebugRectAtTile(Vec2 coordinates, Color4F color);

#endif // DEBUG_ENABLE
    	    
protected: /** Protected Functions **/
    
	void addObjects();
	bool addObject(std::string className, ValueMap& properties);
	void addShadows(TMXLayer& layer);
	/**  **/
	Value getPropertiesForGID(int GID);
	Vec2 getMapTransition(Vec2 direction);	
	
	/**  **/
	TileData getTileDataFromLayerAt(TMXLayer& layer, Vec2 tileCoordinates);	
	/**  **/
	TileDataArray getTileDataArrayFromLadderLayerAt(Vec2 position);
	TileDataArray getTileDataArrayFromCollisionLayerAt(Vec2 position);	
	TileDataArray getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position);
	/**  **/
	Sprite* getShadowForNode(Sprite& sprite);
	void setAliasTexParameters(TMXLayer& layer);
    
    
protected: /** Variables **/
	TMXTiledMap* _tileMap;	
	DrawNode* _debugLayer;		
	int _objectCount;
	Size _tileSize;
	Size _mapSize;	
	
	class IPathFinder* _pathFinder;
	class ParallaxTileMapFsm* _fsm;
};


class Cave : public ParallaxTileMap
{
	typedef ParallaxTileMap super;
	typedef Cave self;

public:
	/** Node create **/
	static self* create(std::string type);

	/** Constructor **/
	Cave(std::string mapName);
	virtual ~Cave(){};

	ValueMap getToSurfaceProperties();
};


class PlanetSurface : public ParallaxTileMap
{
	typedef ParallaxTileMap super;
	typedef PlanetSurface self;

public:
	/** Node create **/
	static self* create(std::string type);

	/** Constructor**/
	PlanetSurface(std::string mapName);
	virtual ~PlanetSurface(){};

	ValueMap getToCaveProperties();

private:
	class ParallaxBackground* _parallaxBackgroundLayer;
	class ParallaxForeground* _parallaxForegroundLayer;

	

};


#endif /* defined(__com_dotdat_World__PARALLAX_TILEMAP_H__) */

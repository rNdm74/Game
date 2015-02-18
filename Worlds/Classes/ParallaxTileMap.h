#ifndef __com_dotdat_World__PARALLAX_TILEMAP_H__
#define __com_dotdat_World__PARALLAX_TILEMAP_H__

#include "cocos2d.h"
#include <array>
#include "Constants.h"

class GameObject;
class Player;
class ParallaxBackground;
class AStarPathFinder;
class Path;

using namespace cocos2d;

typedef std::array<TileData, 8> TileDataArray;

class ParallaxTileMap : public ParallaxNode
{
public:
	// "creator" methods first	
	static ParallaxTileMap* create(std::string mapName);

	// If applicable, then Constructors and the Destructor
	ParallaxTileMap(std::string mapName);
	virtual ~ParallaxTileMap();

	void update(float& delta);

	void setAliasTexParameters(TMXLayer& layer);

	void addObjects();
	bool addObject(std::string className, ValueMap& properties);

	void addShadows(TMXLayer* layer);
	Node* getShadowForNode(Node* node);

	Player* getPlayer();

	TMXLayer* getCollisionLayer();
	TMXLayer* getLadderLayer();
	Node* getObjectLayer();

	Size getMapSize();
	Size getTileSize();

	void pathFinderVisited(Vec2 coordinate);
	bool blocked(Vec2 coordinate);
	float getCost(Vec2 startLocation, Vec2 targetLocation);

	Value getPropertiesForGID(int GID);

	TileDataArray getTileDataArrayFromCollisionLayerAt(Vec2 position);
	TileDataArray getTileDataArrayFromLadderLayerAt(Vec2 position);
		
	bool isValid(Vec2 tileCoordinates);
	bool isTileLadder(Vec2 position);

	Vec2 getTileCoordinatesFor(Vec2 position);
	Rect getTileRectFrom(Vec2 tileCoordinates);

	Path* getPath(Vec2 startLocation, Vec2 targetLocation);
    
    Vec2 getMapTransition(Vec2 direction);

	
#if DEBUG_ENABLE
	void clearDebugDraw();
	void drawDebugRect(Rect rect, Color4F color);
    void drawDebugRectAt(Vec2 position, Color4F color);
	void drawDebugRectAtTile(Vec2 coordinates, Color4F color);
#endif // DEBUG_ENABLE
    
	bool isPlayerLoaded;
    
private:
	TileDataArray getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position);
	TileData getTileDataFromLayerAt(TMXLayer& layer, Vec2 tileCoordinates);
	
	AStarPathFinder* _pathFinder;
	Path* _path;

	TMXTiledMap* _tileMap;
	ParallaxBackground* _parallaxBackgroundLayer;
	TMXLayer* _backgroundLayer;
	TMXLayer* _collisionLayer;
	TMXLayer* _ladderLayer;
	TMXLayer* _foregroundLayer;
	Node* _shadowLayer;
	Node* _objectLayer;
	DrawNode* _debugLayer;

	
	int _objectCount;

	Size _tileSize;
	Size _mapSize;

	
};

#endif /* defined(__com_dotdat_World__PARALLAX_TILEMAP_H__) */

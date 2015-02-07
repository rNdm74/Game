#ifndef __com_dotdat_World__PARALLAX_TILEMAP_H__
#define __com_dotdat_World__PARALLAX_TILEMAP_H__

#include "cocos2d.h"
#include <array>
#include "Constants.h"

class Player;

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

	void update(float delta);

	void setAliasTexParameters(TMXLayer& layer);

	void addObjects();
	bool addObject(std::string className, ValueMap& properties);

	Node* getShadowForNode(Node* node);

	Player* getPlayer();

	TMXLayer* getCollisionLayer();
	TMXLayer* getLadderLayer();
	Node* getObjectLayer();

	Size getMapSize();
	Size getTileSize();

	Value getPropertiesForGID(int GID);

	TileDataArray getTileDataArrayFromCollisionLayerAt(Vec2 position);
	TileDataArray getTileDataArrayFromLadderLayerAt(Vec2 position);
		
	bool isValidTileCoordinates(Vec2 tileCoordinates);
	bool isTileLadder(Vec2 position);

	// Debug
	void clearDebugDraw();
	void drawDebugRect(Rect rect, Color4F color);
    void drawDebugRectAt(Vec2 position, Color4F color);
private:
	TileDataArray getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position);
	TileData getTileDataFromLayerAt(TMXLayer& layer, Vec2 tileCoordinates);
	Vec2 getTileCoordinatesForPosition(Vec2 position);
	Rect getTileRectFrom(Vec2 tileCoordinates);

	TMXTiledMap* tileMap;
	TMXLayer* backgroundLayer;
	TMXLayer* collisionLayer;
	TMXLayer* ladderLayer;
	Node* shadowLayer;
	Node* objectLayer;

	DrawNode* debugDraw;
	
	int objectCount;

	Size tileSize;
	Size mapSize;
};

#endif /* defined(__com_dotdat_World__PARALLAX_TILEMAP_H__) */

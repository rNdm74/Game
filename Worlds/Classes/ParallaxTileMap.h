#ifndef __com_dotdat_World__PARALLAX_TILEMAP_H__
#define __com_dotdat_World__PARALLAX_TILEMAP_H__


#include "cocos2d.h"
#include <array>
#include "Constants.h"

class AppGlobal;
class Player;
class GameObject;
class GameObjectFactory;

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
	GameObject* addObject(std::string className, ValueMap& properties);

	Node* getShadowForNode(Node* node);

	Player* getPlayer();

	TMXLayer* getForegroundLayer();
	TMXLayer* getLadderLayer();
	Node* getObjectLayer();

	Size getMapSize();
	Size getTileSize();

	Value getPropertiesForGID(int GID);

	TileDataArray getTileDataArrayFromCollisionLayerAt(Vec2 position);
	TileDataArray getTileDataArrayFromLadderLayerAt(Vec2 position);

	TileData getTileDataFromLadderLayerAtPosition(Vec2 position);

	bool isValidTileCoordinates(Vec2 tileCoordinates);
	bool isTileLadder(Vec2 position);
	//bool isTile

	void clearDebugDraw();
	void drawDebugRect(Rect rect, Color4F color);
private:
	TileDataArray getTileDataArrayFromLayerAt(TMXLayer& layer, Vec2& position);

	TMXTiledMap* tileMap;
	TMXLayer* backgroundLayer;
	TMXLayer* foregroundLayer;
	TMXLayer* ladderLayer;
	Node* shadowLayer;
	Node* objectLayer;

	DrawNode* debugDraw;
	
	int objectCount;
};

#endif /* defined(__com_dotdat_World__PARALLAX_TILEMAP_H__) */

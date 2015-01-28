#ifndef __com_dotdat_World__PARALLAX_TILEMAP_H__
#define __com_dotdat_World__PARALLAX_TILEMAP_H__

#include "cocos2d.h"

class AppGlobal;
class Player;
class GameObject;
class GameObjectFactory;

using namespace cocos2d;

class ParallaxTileMap : public ParallaxNode
{
public:
	// "creator" methods first	
	static ParallaxTileMap* create(std::string mapName);

	// If applicable, then Constructors and the Destructor
	ParallaxTileMap(std::string mapName);
	virtual ~ParallaxTileMap();

	void update(float delta);

	void setAliasTexParameters(TMXLayer* layer);

	void addObjects();
	GameObject* addObject(std::string className, ValueMap& properties);

	Node* getShadowForNode(Node* node);

	Player* getPlayer();

	TMXLayer* getForegroundLayer();
	Node* getObjectLayer();

	Size getMapSize();
	Size getTileSize();

private:
	TMXTiledMap* tileMap;
	TMXLayer* backgroundLayer;
	TMXLayer* foregroundLayer;
	Node* shadowLayer;
	Node* objectLayer;
	
	int objectCount;
};

#endif /* defined(__com_dotdat_World__PARALLAX_TILEMAP_H__) */

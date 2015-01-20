#ifndef __FranticAlien__LEVEL_H__
#define __FranticAlien__LEVEL_H__

#include "cocos2d.h"
#include "Box2D.h"

class GameObject;
class GameObjectFactory;

using namespace cocos2d;

class Level : public Node
{
public:

	void loadMap(std::string mapname);
	
	Node* getShadowForNode(Node* node);

	Level();
	virtual ~Level(void);
		
	void load();

	void createPhysicsWorld();
	
	void addObjects();
	void followPlayer();

	GameObject* addObject(std::string className, ValueMap& properties);

	void update(float& delta);

	void setAliasTexParameters(TMXLayer* layer);
    
    void setViewPointCenter(Vec2 position);

	TMXTiledMap* getMap() { return map; }
	b2World* getWorld() { return world; }
	GameObject* getPlayer() { return player; }

private:
	GameObject* player;

	TMXTiledMap* map;
	b2World* world;

	//GameObjectFactory* factory;

	ParallaxNode* parallaxNode;
	Node* collisionLayer;

	Vec2 origin;
	Vec2 center;
	Size size;

	int objectCount;

	//
	// Base values.
	Size _worldSizeMeters;
	Size _screenSizePixels;
	// How much of the world (meters) is projected
	// onto the screen.  At 1.0, the scale is 1:1.
	float _vScale;
	// The location of the center of the viewport
	// in terms of the physics world (meters).
	Vec2 _vCenterMeters;
	Vec2 _vBottomLeftMeters;
	Vec2 _vTopRightMeters;
	Size _vSizeMeters;

	// Min/Max Scale Limits
	float _vScaleMin;
	float _vScaleMax;

	// Values for converting meters to pixels.
	Point _vScalePixelToMeter;
	Point _vOffsetPixels;

	// Aspect ratio of the screen
	float _aspectRatio;

	// Pixels to meters ratio of the screen.
	// This is based on the viewport pixels to meters.
	float _ptmRatio;
};

#endif /* defined(__FranticAlien__LEVEL_H__) */

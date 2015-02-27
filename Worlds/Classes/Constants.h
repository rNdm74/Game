#ifndef __FranticAlien__CONSTANTS_H__
#define __FranticAlien__CONSTANTS_H__

#include <array>
#include <string>
#include "cocos2d.h"

struct TileData
{
	cocos2d::Rect tileRect;
	cocos2d::Vec2 tileCoordinates;
	int GID = 0;
};

typedef std::array<TileData, 8> TileDataArray;
typedef std::array<cocos2d::Vec2, 7> States;
typedef std::array<bool, 7> KeyMatrix;
typedef std::array<bool, 4> IsMoving;
typedef std::array<bool, 4> CanMove;

#define DEBUG_ENABLE 1

#define CLOUDS_VARIETY			3
#define CLOUDS_SPACE_BETWEEN	10
#define CLOUDS_STARTING_TAG		100
#define CLOUDS_SPEED			150
/**
* PNG assets
*/
#define BACKGROUND_PNG		"background.png"
#define CHARACTERS_PNG		"characters.png"
#define CONTROLS_PNG		"controls.png"
#define HUD_PNG				"hud.png"
#define ITEMS_PNG			"items.png"
#define PARTICLES_PNG		"particles.png"
#define TILES_PNG			"tiles.png"
/**
* PLIST assets
*/ 
#define BACKGROUND_PLIST	"background.plist"
#define CHARACTERS_PLIST	"characters.plist"
#define CONTROLS_PLIST		"controls.plist"
#define HUD_PLIST			"hud.plist"
#define ITEMS_PLIST			"items.plist"
#define PARTICLES_PLIST		"particles.plist"
#define TILES_PLIST			"tiles.plist"
/****/
#define _PREFIX				"alienBeige"
#define _SUFFIX				".png"
#define _WALK				"_walk"
#define _DUCK				"_duck"
#define _STAND				"_stand"
#define _JUMP				"_jump"
#define _CLIMB				"_climb"
#define _HURT				"_hurt"
/****/
#define kPlanetTMX			"grassPlanet.tmx"
#define kCaveTMX			"planet1.tmx"
#define kPlayerFileName		"alienBeige.png"
#define kFilePrefix			"alien"
#define kFileSuffix			"_badge2.png"
/****/
#define kTagClimbUp			0
#define kTagClimbDown		1
#define kTagWalkLeft		2
#define kTagWalkRight		3
#define kTagStop			4
#define kTagHurt			5
/****/
#define kTagBackgroundLayer	0
#define kTagCollisionLayer	1
#define kTagLadderLayer		2
#define kTagForegroundLayer	3
#define kTagShadowLayer		4
#define kTagObjectLayer		5
#define kTagDebugLayer		6
/****/
#define kTagPlanet			0
#define kTagCave			1
#define kTagPlayer			2 
/****/
#define kTagCursor			0
#define KTagSceneLayer		1
/****/
#define PI					3.14159265
/****/
#define kFrameRate			12.0f
#define kFrameRateFactor	100.0f
#define kZoomMin			1.2f
#define kZoomMax			1.5f
#define kZoomInFactor		50000.0f
#define kZoomOutFactor		100000.0f
/****/
const float kUpdateInterval = 1.0f / 60.0f;
/**  **/
const States GameObjectStates =
{
	cocos2d::Vec2( 0.0f,  1.0f ),	/** CheckCanClimbUp **/
	cocos2d::Vec2( 0.0f, -1.0f ),	/** CheckCanClimbDown **/
	cocos2d::Vec2(-1.0f,  0.0f ),	/** CheckCanWalkLeft **/
	cocos2d::Vec2( 1.0f,  0.0f ),	/** CheckCanWalkRight **/
	cocos2d::Vec2::ZERO,			/** Stop **/
	cocos2d::Vec2::ZERO,			/** LoadNextMap **/
	cocos2d::Vec2::ZERO				/** LoadPreviousMap **/
};

enum EGameObjectState
{
	CheckCanClimbUp,
	CheckCanClimbDown,
	CheckCanWalkLeft,
	CheckCanWalkRight,
	Stop,
	LoadNextMap,
	LoadPreviousMap
};

enum ETileGrid
{
	BOTTOM,
	TOP,
	LEFT,
	RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER = 1
};

#endif /* defined(__CONSTANTS__) */

#ifndef __FranticAlien__CONSTANTS_H__
#define __FranticAlien__CONSTANTS_H__

#include <array>
#include <string>

#include "cocos2d.h"

enum EGameObjectEvent
{
    Up,
    Down,
    Left,
    Right,
    Stop,
    Jump,
};

enum EParallaxTileMapState
{
    LoadCave,
    LoadPlanet
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

enum EAnimationStates
{
    CLIMBING,
    WALKING,
    IDLE,
    HURT,
    CROUCH,
    JUMP
};

struct TileData
{
	cocos2d::Rect tileRect;
	cocos2d::Vec2 tileCoordinates;
	int GID = 0;
};

typedef std::stack<EGameObjectEvent> Events;
typedef std::stack<class IParallaxTileMap*> ActiveTileMap;
typedef std::array<TileData, 8> TileDataArray;
typedef std::array<cocos2d::Vec2, 7> States;
typedef std::array<bool, 7> KeyMatrix;
typedef std::array<bool, 4> IsMoving;
typedef std::array<bool, 4> CanMove;
typedef std::string AnimationFrames[6][2];
typedef std::string Type;

#define DEBUG_ENABLE 1

#define CLOUDS_VARIETY			3
#define CLOUDS_SPACE_BETWEEN	10
#define CLOUDS_STARTING_TAG		100
#define CLOUDS_SPEED			150
/**
* PNG assets
*/
#define BACKGROUND_PNG			"background.png"
#define CHARACTERS_PNG			"characters.png"
#define CONTROLS_PNG			"controls.png"
#define HUD_PNG					"hud.png"
#define ITEMS_PNG				"items.png"
#define PARTICLES_PNG			"particles.png"
#define TILES_PNG				"tiles.png"
/**
* PLIST assets
*/ 
#define BACKGROUND_PLIST		"background.plist"
#define CHARACTERS_PLIST		"characters.plist"
#define CONTROLS_PLIST			"controls.plist"
#define HUD_PLIST				"hud.plist"
#define ITEMS_PLIST				"items.plist"
#define PARTICLES_PLIST			"particles.plist"
#define TILES_PLIST				"tiles.plist"
/****/
#define _PREFIX					"alienBeige"
#define _SUFFIX					".png"
#define _WALK					"_walk"
#define _DUCK					"_duck"
#define _STAND					"_stand"
#define _JUMP					"_jump"
#define _CLIMB					"_climb"
#define _HURT					"_hurt"
/****/
#define kPlanetTMX				"grassPlanet.tmx"
#define kCaveTMX				"planet1.tmx"
#define kPlayerFileName			"alienBeige.png"
#define kFilePrefix				"alien"
#define kFileSuffix				"_badge2.png"
/****/
#define kTagClimbUp				0
#define kTagClimbDown			1
#define kTagWalkLeft			2
#define kTagWalkRight			3
#define kTagStop				4
#define kTagHurt				5
/****/
#define kTagBackgroundLayer		0
#define kTagCollisionLayer		1
#define kTagLadderLayer			2
#define kTagForegroundLayer		3
#define kTagShadowLayer			4
#define kTagObjectLayer			5
#define kTagDebugLayer			6
#define kTagPBackgroundLayer	7
#define kTagPForegroundLayer	8
/****/
#define kTagMountain			0
#define kTagHill				1
#define kTagCloud				2
#define kTagBackground			3
/****/
#define kTagPlanet				0
#define kTagCave				1
#define kTagPlayer				2 
/****/
#define kTagCursor				0
#define KTagSceneLayer			1
/****/
#define PI						3.14159265
/****/
#define kFadeIn					1
#define kFadeOut			   -1
#define kOpacityMin				100
#define kOpacityMax				255
#define kOpacityFadeFactor		10
#define kFrameDelay				300.0f /** milliseconds **/
#define kFrameTimeFactor		20.0f
#define kZoomMin				1.2f
#define kZoomMax				1.5f
#define kZoomInFactor			50000.0f
#define kZoomOutFactor			100000.0f
/****/
const float kUpdateInterval =	1.0f / 60.0f;
/**  **/
const States GameObjectStates =
{
	cocos2d::Vec2( 0.0f,  1.0f ),	/** Up **/
	cocos2d::Vec2( 0.0f, -1.0f ),	/** Down **/
	cocos2d::Vec2(-1.0f,  0.0f ),	/** Left **/
	cocos2d::Vec2( 1.0f,  0.0f ),	/** Right **/
	cocos2d::Vec2::ZERO,			/** Stop **/
	cocos2d::Vec2( 0.0f,  1.0f )	/** Jump **/
};

#endif /* defined(__CONSTANTS__) */

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

enum ENpcEvent
{
	Wander,
	Scared,
	Curious,
	DropGem,
	Decision,
	Interact,
	Resting,
};

enum EAnimationStates
{
	UpAnimation,
	DownAnimation,
	LeftAnimation,
	RightAnimation,
	StopAnimation,
	JumpAnimation,
	IdleAnimation
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

struct TileData
{
	cocos2d::Rect tileRect;
	cocos2d::Vec2 tileCoordinates;
	int GID = 0;
};

typedef std::stack<EGameObjectEvent> Events;
typedef std::stack<ENpcEvent> NpcEvents;

typedef std::stack<class IParallaxTileMap*> ActiveTileMap;
typedef std::array<TileData, 8> TileDataArray;
typedef std::array<cocos2d::Vec2, 7> States;
typedef std::array<bool, 7> KeyMatrix;
typedef std::array<bool, 4> IsMoving;
typedef std::array<bool, 4> CanMove;
typedef std::string AnimationFrames[7][2];
typedef std::string Type;

typedef std::vector<std::vector<std::string>> Frames;

#define DEBUG_ENABLE 0

#define MAX_FRAME_STATES 7


typedef std::array<std::string, 5> AlienTypes;

static const AlienTypes alienTypes = { "Beige", "Blue", "Green", "Pink", "Yellow" };
//std::array<std::string, 5> alienTypes = { "Beige", "Blue", "Green", "Pink", "Yellow" };

#define ANIMATION_FRAMES(__TYPE__) \
{ \
    { "alien"__TYPE__"_climb1.png", "alien"__TYPE__"_climb2.png" }, \
    { "alien"__TYPE__"_climb1.png", "alien"__TYPE__"_climb2.png" }, \
	{ "alien"__TYPE__"_walk1.png", "alien"__TYPE__"_walk2.png" }, \
    { "alien"__TYPE__"_walk1.png", "alien"__TYPE__"_walk2.png" }, \
    { "alien"__TYPE__"_stand.png" }, \
	{ "alien"__TYPE__"_jump.png" },	\
	{ "alien"__TYPE__".png" } \
}\

#define CLOUDS_VARIETY			3
#define CLOUDS_SPACE_BETWEEN	10
#define CLOUDS_STARTING_TAG		100
#define CLOUDS_SPEED			150
/****/
#define mapGrass				"Grass"
#define mapSnow					"Snow"
/**
* PNG assets
*/
#define BACKGROUND_PNG			"background.png"
#define CHARACTERS_PNG			"characters.png"
#define CONTROLS_PNG			"controls.png"
#define HUD_PNG					"hud.png"
#define ITEMS_PNG				"items.png"
#define PARTICLES_PNG			"particles.png"
#define GRASS_PNG				"grass.png"
#define SNOW_PNG				"snow.png"
#define SAND_PNG				"sand.png"
#define DIRT_PNG				"dirt.png"
/**
* PLIST assets
*/ 
#define BACKGROUND_PLIST		"background.plist"
#define CHARACTERS_PLIST		"characters.plist"
#define CONTROLS_PLIST			"controls.plist"
#define HUD_PLIST				"hud.plist"
#define ITEMS_PLIST				"items.plist"
#define PARTICLES_PLIST			"particles.plist"
#define GRASS_PLIST				"grass.plist"
#define SNOW_PLIST				"snow.plist"
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
#define kPlanetSurfaceTmx		"planet_surface.tmx"
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
#define kTagInnerForeground     9
#define kTagInnerBackground     10
/****/
#define kTagMountain			0
#define kTagHill				1
#define kTagCloud				2
#define kTagBackground			3
/****/
#define kTagPlanetSurface		0
#define kTagCave				1
#define kTagPlayer				2 
#define kTagToSurface			3
#define kTagToCave				4
#define kTagNpc					5
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
#define kFrameDelay				150.0f /** milliseconds **/
#define kFrameTimeFactor		20.0f
#define kZoomMin				1.2f
#define kZoomMax				1.5f
#define kZoomInFactor			50000.0f
#define kZoomOutFactor			100000.0f
#define kBornAge				0.01f
#define kAdultAge				1.0f
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

#define ForegroundLayerHillsParallaxRatio			( Vec2(1.1f, 1.0f) )
#define ForeGroundLayerHillsShadowParallaxRatio		( Vec2(1.1025f, 1.0f) )

#define BackgroundLayerBackgroundParallaxRatio		( Vec2(0.6f, 0.3f) )

#define BackgroundLayerMountainsParallaxRatio		( Vec2(0.7f, 0.5f) )
#define BackgroundLayerMountainsShadowParallaxRatio	( Vec2(0.703f, 0.51f) )

#define BackgroundLayerHillsParallaxRatio			( Vec2(0.8f, 0.9f) )
#define BackgroundLayerHillsShadowParallaxRatio		( Vec2(0.803f, 0.91f) )

#define BackgroundLayerRocksParallaxRatio			( Vec2(1.1f, 1.0f) )
#define BackgroundLayerRocksShadowParallaxRatio		( Vec2(1.1f, 1.0f) )

/****/
#define GetSize(name) ( SpriteFrameCache::getInstance()->getSpriteFrameByName( (name) )->getOriginalSize() )

#endif /* defined(__CONSTANTS__) */

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
/**
*
*/
#define kTagBackgroundLayer	0
#define kTagCollisionLayer	1
#define kTagLadderLayer		2
#define kTagForegroundLayer	3
#define kTagShadowLayer		4
#define kTagObjectLayer		5
#define kTagDebugLayer		6

#define kTagPlanet 0
#define kTagCave 1
#define kTagPlayer 2 

#define kTagCursor 999
#define KTagSceneLayer 1

#define kTagScrollingBackground 0
#define kTagScrollingCloud 1
#define kTagScrollingBush 2

#define kBackgroundLayer 1
#define kMidgroundLayer 2
#define kForegroundLayer 3

#define FILE_PREFIX "alien"
#define FILE_SUFFIX "_badge2.png"

#define MENU 1

#define PI 3.14159265

//#define kGameObjectVelocity	( Vec2( kPixelsPerMeter * kMaxVelocityX, kPixelsPerMeter * kMaxVelocityX ) )

const float kZoomFactor = 0.025f;
const float kUpdateInterval = 1.0f / 60.0f;
const double kSecondsPerUpdate = 0.1;

const std::string kPlanetTMX					= "grassPlanet.tmx";
const std::string kCaveTMX						= "planet1.tmx";
const std::string kPlayerFileName				= "alienBeige.png";

/**  **/
const States GameObjectStates =
{
	cocos2d::Vec2( 0.0f,  1.0f),	/** CheckCanClimbUp **/
	cocos2d::Vec2( 0.0f, -1.0f),	/** CheckCanClimbDown **/
	cocos2d::Vec2(-1.0f,  0.0f),	/** CheckCanWalkLeft **/
	cocos2d::Vec2( 1.0f,  0.0f),	/** CheckCanWalkRight **/
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

//enumeration of possible input states
enum EState 
{		
	STATE_UP,
	STATE_DOWN,
	STATE_LEFT,
	STATE_RIGHT,
	STATE_STOP
};

enum EBearing
{
	NORTH,
	SOUTH,
	WEST,
	EAST,	
	STOP
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

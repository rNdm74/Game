#ifndef __FranticAlien__CONSTANTS_H__
#define __FranticAlien__CONSTANTS_H__

#include <array>
#include <string>
#include "cocos2d.h"

#define DEBUG_ENABLE 1

#define CLOUDS_VARIETY			3
#define CLOUDS_SPACE_BETWEEN	10
#define CLOUDS_STARTING_TAG		100
#define CLOUDS_SPEED			150

// PNG assets
#define BACKGROUND_PNG		"background.png"
#define CHARACTERS_PNG		"characters.png"
#define CONTROLS_PNG		"controls.png"
#define HUD_PNG				"hud.png"
#define ITEMS_PNG			"items.png"
#define PARTICLES_PNG		"particles.png"
#define TILES_PNG			"tiles.png"

// PLIST assets
#define BACKGROUND_PLIST	"background.plist"
#define CHARACTERS_PLIST	"characters.plist"
#define CONTROLS_PLIST		"controls.plist"
#define HUD_PLIST			"hud.plist"
#define ITEMS_PLIST			"items.plist"
#define PARTICLES_PLIST		"particles.plist"
#define TILES_PLIST			"tiles.plist"

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


const float kZoomFactor = 0.025f;
const float kPixelsPerMeter = 32.0f;
const float kGravity = -100.0f; // adjust this to taste
const float kUpdateInterval = 1.0f / 60.0f;
const double kSecondsPerUpdate = 0.1;

const float kMapBoundsX = 0;
const float kMapBoundsY = 1.0f;

const float kMinVelocityX						= 1.0f;
const float kMaxVelocityX						= 15.0f;
const float kStopVelocity						= 0.70f; // 98%

const float kGameObjectFixtureDensity			= 1.0f;
const float kGameObjectFixtureFriction			= 0.1f;
const float kGameObjectFixtureRestitution		= 0.0f;
const int	kGameObjectFixtureFilterMaskBits	= 0xffff;

#define kGameObjectVelocity	( Vec2( kPixelsPerMeter * kMaxVelocityX, kPixelsPerMeter * kMaxVelocityX ) )

const std::string kPlanetTMX					= "grassPlanet.tmx";
const std::string kCaveTMX						= "planet1.tmx";
const std::string kPlayerFileName				= "alienBeige.png";

//enumeration of possible input states
enum EState 
{		
	STATE_UP,
	STATE_DOWN,
	STATE_LEFT,
	STATE_RIGHT,
	STATE_JUMP,
	STATE_ENTER,
	STATE_ESCAPE,
	STATE_HUD,
	STATE_SPRINT,
	STATE_STOP,
};

enum kFilterCatagory 
{
	SOLID_PLATFORM	= 0x0001,
	SOLID_SLOPE		= 0x0002,
	PLAYER			= 0x0004,
	ENEMY			= 0x0008,
	SENSOR			= 0x0010,
	//FRIENDLY_TOWER = 0x0020,
	//RADAR_SENSOR = 0x0040,
};

enum EBearing
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
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

struct TileData
{
	cocos2d::Rect tileRect;
    cocos2d::Vec2 tileCoordinates;
	int GID = 0;
};

typedef std::array<TileData, 8> TileDataArray;

typedef std::array<bool, 4> CanMove;

typedef std::array<bool, 4> IsMoving;

typedef std::array<bool, 7> KeyMatrix;

#endif /* defined(__CONSTANTS__) */

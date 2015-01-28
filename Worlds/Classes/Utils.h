#ifndef __FranticAlien__UTILS_H__
#define __FranticAlien__UTILS_H__

#include "cocos2d.h"
#include <array>
#include "Constants.h"

using namespace cocos2d;

typedef std::array<TileData, 8> TileDataArray;

class IUtils
{
	static Vec2 tileCoordForPosition(Vec2 position, Size mapSize, Size tileSize);
	static Rect tileRectFromTileCoords(Vec2 tileCoords, Size mapSize, Size tileSize);
	static Rect RectIntersection(Rect r1, Rect r2);
	static bool RectIntersectsRect(Rect r1, Rect r2);
	static TileDataArray getSurroundingTilesAtPosition(Vec2 position, TMXLayer* layer, Size mapSize, Size tileSize);
};

class Utils : public IUtils
{
public:
	virtual ~Utils(){};

	static Vec2 tileCoordForPosition(Vec2 position, Size mapSize, Size tileSize);
	static Rect tileRectFromTileCoords(Vec2 tileCoords, Size mapSize, Size tileSize);
	static Rect RectIntersection(Rect r1, Rect r2);
	static bool RectIntersectsRect(Rect r1, Rect r2);
	static TileDataArray getSurroundingTilesAtPosition(Vec2 position, TMXLayer& layer, Size mapSize, Size tileSize);
};

#endif /* defined(__FranticAlien__UTILS_H__) */

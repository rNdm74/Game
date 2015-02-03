#ifndef __FranticAlien__UTILS_H__
#define __FranticAlien__UTILS_H__

#include "cocos2d.h"

#include "Constants.h"

using namespace cocos2d;

class IUtils
{	
	static Rect getRectIntersection(Rect r1, Rect r2);
	static bool isRectIntersectsRect(Rect r1, Rect r2);
	static bool isRectContainsRect(Rect r1, Rect r2);	
	static int getTilesetMaxGID(TMXLayer& layer);
	static bool isPixelCollision(Sprite* s1, Sprite* s2, Rect intersection, bool pixelCollision);
};

class Utils : public IUtils
{
public:
	virtual ~Utils(){};
	
	static Rect getRectIntersection(Rect r1, Rect r2);
	static bool isRectIntersectsRect(Rect r1, Rect r2);
	static bool isRectContainsRect(Rect r1, Rect r2);	
	static int getTilesetMaxGID(TMXLayer& layer);
	static bool isPixelCollision(Sprite* s1, Sprite* s2, Rect intersection, bool pixelCollision);
};

#endif /* defined(__FranticAlien__UTILS_H__) */

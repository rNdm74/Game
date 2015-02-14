#ifndef __com_dotdat_World__PATH_H__
#define __com_dotdat_World__PATH_H__

#include "cocos2d.h"

using namespace cocos2d;

/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class Path
{
public:
	/**
	* Create an empty path
	*/
	Path();
	virtual ~Path(){}

	int getLength();

	Vec2 getStep(int index);

	float getX(int index);
	float getY(int index);

	void appendStep(Vec2 step);
	void prependStep(Vec2 step);

	bool contains(Vec2 step);

	std::vector<Vec2> steps;

private:
	/** The list of steps building up this path */
	//std::vector<Vec2> steps;
};

#endif /* defined(__com_dotdat_World__PATH_H__) */

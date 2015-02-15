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
class Path : public Ref
{
public:
	static Path* create();
	/**
	* Create an empty path
	*/
	Path();
	virtual ~Path(){}

	int getLength();

	Vec2 getStep(unsigned int index);
	float getX(unsigned int index);
	float getY(unsigned int index);
		
	bool contains(Vec2 step);

	Vec2 peek_front();
	Vec2 peek_back();
	Vec2 pop_front();
	Vec2 pop_back();
	void push_back(Vec2 step);
	void push_front(Vec2 step);

	void addPath(Path* path);

private:
	/** The list of steps building up this path */
	std::list<Vec2> steps;
};

#endif /* defined(__com_dotdat_World__PATH_H__) */

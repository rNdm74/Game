#ifndef __com_dotdat_World__PATH_H__
#define __com_dotdat_World__PATH_H__

#include "cocos2d.h"

using namespace cocos2d;

class Path;

/**
* A path determined by some path finding algorithm. A series of steps from
* the starting location to the target location. This includes a step for the
* initial location.
*
* @author Adam Charlton
*/
class IPath
{
public:
	IPath(){};
	virtual ~IPath(){};
	virtual int getLength() = 0;
	virtual Vec2 getStep(unsigned int index) = 0;
	virtual float getX(unsigned int index) = 0;
	virtual float getY(unsigned int index) = 0;
	virtual bool contains(Vec2 step) = 0;
	virtual Vec2 peek_front() = 0;
	virtual Vec2 peek_back() = 0;
	virtual Vec2 pop_front() = 0;
	virtual Vec2 pop_back() = 0;
	virtual void push_back(Vec2 step) = 0;
	virtual void push_front(Vec2 step) = 0;
	virtual void add(IPath* path) = 0;
};

class Path : public IPath
{
public:
    
	/**
	* Create an empty path
	*/
	Path();
	virtual ~Path(){}

	virtual int getLength();

	virtual Vec2 getStep(unsigned int index);
	virtual float getX(unsigned int index);
	virtual float getY(unsigned int index);
		
	virtual bool contains(Vec2 step);

	virtual Vec2 peek_front();
	virtual Vec2 peek_back();
	virtual Vec2 pop_front();
	virtual Vec2 pop_back();
	virtual void push_back(Vec2 step);
	virtual void push_front(Vec2 step);
	virtual void add(IPath* path) override;

private:
	/** The list of steps building up this path */
	std::list<Vec2> steps;
};

#endif /* defined(__com_dotdat_World__PATH_H__) */

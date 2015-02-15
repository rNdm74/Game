#include "Path.h"


/**
* Create a path and it is added to the autorelease pool
*
* @return created Path
*/
Path* Path::create()
{
	// Create an instance of Level
	Path* node = new Path();

	if (node)
	{
		// Add it to autorelease pool
		node->autorelease();
	}
	else
	{
		// Otherwise delete
		delete node;
		node = 0;
	}

	return node;
}

Path::Path(){}

/**
* Get the length of the path, i.e. the number of steps
*
* @return The number of steps in this path
*/
int Path::getLength()
{
	return steps.size();
}

/**
* Get the step at a given index in the path
*
* @param index The index of the step to retrieve. Note this should
* be >= 0 and < getLength();
* @return The step information, the position on the map.
*/
Vec2 Path::getStep(unsigned int index)
{
	Vec2 step = Vec2::ZERO;

	if (steps.size() > index)
	{
		std::list<Vec2>::iterator it = std::next(steps.begin(), index);		
		step = *it;
	}

	return step;
}

/**
* Get the x coordinate for the step at the given index
*
* @param index The index of the step whose x coordinate should be retrieved
* @return The x coordinate at the step
*/
float Path::getX(unsigned int index)
{
	Vec2 step = Vec2::ZERO;

	if (steps.size() > index)
	{
		std::list<Vec2>::iterator it = std::next(steps.begin(), index);
		step = *it;
	}

	return step.x;
}

/**
* Get the y coordinate for the step at the given index
*
* @param index The index of the step whose y coordinate should be retrieved
* @return The y coordinate at the step
*/
float Path::getY(unsigned int index)
{
	Vec2 step = Vec2::ZERO;

	if (steps.size() > index)
	{
		std::list<Vec2>::iterator it = std::next(steps.begin(), index);
		step = *it;
	}

	return step.y;
}

/**
* Append a step to the path.
*
* @param x The x coordinate of the new step
* @param y The y coordinate of the new step
*/
void Path::push_back(Vec2 step)
{
	steps.push_back(step);
}

/**
* Prepend a step to the path.
*
* @param x The x coordinate of the new step
* @param y The y coordinate of the new step
*/
void Path::push_front(Vec2 step)
{
	steps.push_front(step);
}

/**
* Peek a step from front.
*
* @return the given step at the front of the list
*/
Vec2 Path::peek_front()
{
	return steps.front();
}


/**
* Peek a step from front.
*
* @return the given step at the front of the list
*/
Vec2 Path::peek_back()
{
	return steps.back();
}


/**
* Pop a step from front.
*
* @return the given step at the front of the list then removes from list
*/
Vec2 Path::pop_front()
{
	Vec2 front = steps.front();
	steps.pop_front();
	return front;
}

/**
* Pop a step from front.
*
* @return the given step at the front of the list then removes from list
*/
Vec2 Path::pop_back()
{
	Vec2 back = steps.back();
	steps.pop_back();
	return back;
}


/**
* Check if this path contains the given step
*
* @param x The x coordinate of the step to check for
* @param y The y coordinate of the step to check for
* @return True if the path contains the given step
*/
bool Path::contains(Vec2 step)
{
	return (std::find(steps.begin(), steps.end(), step) != steps.end());
}


/**
* Check if this path contains the given step
*
* @param x The x coordinate of the step to check for
* @param y The y coordinate of the step to check for
* @return True if the path contains the given step
*/
void Path::addPath(Path* path)
{
	if (path != nullptr)
	{
		for (int i = 0; i < path->getLength(); ++i)
		{
			this->steps.push_back(path->getStep(i));
		}
	}	
}
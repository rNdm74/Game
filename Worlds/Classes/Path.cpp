#include "Path.h"

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
Vec2 Path::getStep(int index)
{
	return steps[index];
}

/**
* Get the x coordinate for the step at the given index
*
* @param index The index of the step whose x coordinate should be retrieved
* @return The x coordinate at the step
*/
float Path::getX(int index)
{
	return steps[index].x;
}

/**
* Get the y coordinate for the step at the given index
*
* @param index The index of the step whose y coordinate should be retrieved
* @return The y coordinate at the step
*/
float Path::getY(int index)
{
	return steps[index].y;
}

/**
* Append a step to the path.
*
* @param x The x coordinate of the new step
* @param y The y coordinate of the new step
*/
void Path::appendStep(Vec2 step)
{
	steps.push_back(step);
}

/**
* Prepend a step to the path.
*
* @param x The x coordinate of the new step
* @param y The y coordinate of the new step
*/
void Path::prependStep(Vec2 step)
{
	std::vector<Vec2> temp = steps;
	steps.clear();
	steps.push_back(step);
	steps.insert(steps.begin() + 1, temp.begin(), temp.end());
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
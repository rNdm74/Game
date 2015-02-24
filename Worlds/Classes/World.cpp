#include "Constants.h"
#include "Level.h"
#include "World.h"

#pragma region World

/**
*
*/
World* World::create()
{
	// Create an instance of InfiniteParallaxNode
	World* node = new World();

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

/**
*
*/
World::World()
{
	level = Level::create(kLevelTMX);
	this->addChild(level);
}

/**
*
*/
void World::update(float& delta)
{
	level->update(delta);
}

#pragma endregion World
#include "GameObjectFactory.h"
#include "GameObject.h"

// Objects that are interactable
#include "Player.h"
#include "Wall.h"

USING_NS_CC;

IGameObject* GameObjectFactory::create(std::string className, ValueMap& properties)
{
	IGameObject* gameObject = nullptr;

	if (className == "Player")
	{
		gameObject = Player::create(properties);
	}
	else if (className == "Wall")
	{
		gameObject = Wall::create(properties);
	}
	
	return gameObject;
}
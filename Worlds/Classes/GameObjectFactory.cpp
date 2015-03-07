#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"
#include "FsmComponent.h"

IGameObject* GameObjectFactory::create(std::string className, ValueMap& properties)
{
	IGameObject* gameObject = nullptr;

	if (className == "Player")
	{
		gameObject = Player::create(properties);
	}
    else if(className == "ShowCave")
    {
        gameObject = ShowCave::create(properties);
    }
	else if (className == "Left")
	{
		gameObject = Left::create(properties);
	}
	else if (className == "Right")
	{
		gameObject = Right::create(properties);
	}
	else if (className == "Enter")
	{
		gameObject = Enter::create(properties);
	}
	else if (className == "Exit")
	{
		gameObject = Exit::create(properties);
	}

	return gameObject;
}
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
		gameObject = Player::create
		(
			properties,
			new PlayerCollisionComponent(),
			new PlayerGraphicsComponent(),
			new PlayerMenuComponent(),
			new PlayerInputComponent(),
			new PlayerFsmComponent()
		);
	}
    else if(className == "ShowCave")
    {
        gameObject = ShowCave::create(properties, new ShowCaveCollisionComponent(), new GraphicsComponent());
    }
	else if (className == "Left")
	{
		gameObject = Left::create(properties, new CollisionComponent(), new GraphicsComponent());
	}
	else if (className == "Right")
	{
		gameObject = Right::create(properties, new CollisionComponent(), new GraphicsComponent());
	}
	else if (className == "Enter")
	{
		gameObject = Enter::create(properties, new CollisionComponent(), new GraphicsComponent());
	}
	else if (className == "Exit")
	{
		gameObject = Exit::create(properties, new CollisionComponent(), new GraphicsComponent());
	}

	return gameObject;
}
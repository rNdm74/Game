#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"

GameObject* GameObjectFactory::create(std::string className, ValueMap& properties)
{
	GameObject* gameObject = nullptr;

	if (className == "Player")
	{
		gameObject = new Player
		(
			properties,
			new PlayerMenuComponent(),
			new PlayerInputComponent(),
			new PlayerCollisionComponent(),
			new PlayerGraphicsComponent()
		);
	}
	else if (className == "Left")
	{
		gameObject = new Left(properties, new CollisionComponent());
	}
	else if (className == "Right")
	{
		gameObject = new Right(properties, new CollisionComponent());
	}
	else if (className == "Enter")
	{
		gameObject = new Enter(properties, new CollisionComponent());
	}
	else if (className == "Exit")
	{
		gameObject = new Exit(properties, new CollisionComponent());
	}

	return gameObject;
}
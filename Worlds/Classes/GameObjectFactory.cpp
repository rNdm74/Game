#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "CollisionComponent.h"

GameObjectNode* GameObjectFactory::create(std::string className, ValueMap& properties)
{
	GameObjectNode* gameObject = nullptr;

	if (className == "Player")
	{
		gameObject = new Player
		(
			properties,
			new PlayerCollisionComponent(),
			new PlayerGraphicsComponent(),
			new PlayerMenuComponent(),
			new PlayerInputComponent()
			
		);
	}
	else if (className == "Left")
	{
		gameObject = new Left(properties, new CollisionComponent(), new GraphicsComponent());
	}
	else if (className == "Right")
	{
		gameObject = new Right(properties, new CollisionComponent(), new GraphicsComponent());
	}
	else if (className == "Enter")
	{
		gameObject = new Enter(properties, new CollisionComponent(), new GraphicsComponent());
	}
	else if (className == "Exit")
	{
		gameObject = new Exit(properties, new CollisionComponent(), new GraphicsComponent());
	}

	return gameObject;
}
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "MenuComponent.h"
#include "PhysicsComponent.h"

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
			new PlayerPhysicsComponent(),
			new PlayerGraphicsComponent()
		);
	}		

	return gameObject;
}
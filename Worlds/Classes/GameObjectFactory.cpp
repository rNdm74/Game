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
	else if (className == "Npc")
	{
		gameObject = Npc::create(properties);
	}
    else if(className == "ShowCave")
    {
        gameObject = ShowCave::create(properties);
    }
	else if (className == "ToSurface")
	{
		gameObject = ToSurface::create(properties);
	}
	else if (className == "ToCave")
	{
		gameObject = ToCave::create(properties);
	}
	else if (className == "LandingSite")
	{
		gameObject = LandingSite::create(properties);
	}
	else if (className == "Food")
	{
		gameObject = Food::create(properties);
	}
	
	return gameObject;
}
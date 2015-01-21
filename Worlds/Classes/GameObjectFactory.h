#ifndef __com_dotdat_World__GAMEOBJECT_FACTORY_H__
#define __com_dotdat_World__GAMEOBJECT_FACTORY_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameObject;

class IGameObjectFactory
{
public:
	static GameObject* create(std::string className, ValueMap& properties);
};

class GameObjectFactory : public IGameObjectFactory
{
public:	
	static GameObject* create(std::string className, ValueMap& properties);
};

#endif /* defined(__com_dotdat_World__GAMEOBJECT_FACTORY_H__) */
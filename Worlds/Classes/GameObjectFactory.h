#ifndef __com_dotdat_World__GAMEOBJECT_FACTORY_H__
#define __com_dotdat_World__GAMEOBJECT_FACTORY_H__

#include "cocos2d.h"

using namespace cocos2d;

class IGameObject;

class IGameObjectFactory
{
public:
	IGameObjectFactory(){};
	virtual ~IGameObjectFactory(){};
	static IGameObject* create(std::string className, ValueMap& properties);
};

class GameObjectFactory : public IGameObjectFactory
{
public:	
	static IGameObject* create(std::string className, ValueMap& properties);
};

#endif /* defined(__com_dotdat_World__GAMEOBJECT_FACTORY_H__) */

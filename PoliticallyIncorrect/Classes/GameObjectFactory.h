#ifndef __PoliticallyIncorrect__GameObjectFactory_H__
#define __PoliticallyIncorrect__GameObjectFactory_H__

#include "cocos2d.h"

class IGameObject;

class GameObjectFactory
{
public:
	static IGameObject* create(std::string className, cocos2d::ValueMap& properties);
};

#endif // __PoliticallyIncorrect__GameObjectFactory_H__

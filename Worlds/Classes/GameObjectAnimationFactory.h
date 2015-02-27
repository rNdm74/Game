#ifndef __com_dotdat_World__GameObjectAnimationFactory_H__
#define __com_dotdat_World__GameObjectAnimationFactory_H__

#include "cocos2d.h"

using namespace cocos2d;

class IGameObject;

class IGameObjectAnimationFactory
{
public:
	IGameObjectAnimationFactory(){};
	virtual ~IGameObjectAnimationFactory(){};

	virtual void Climb(IGameObject& gameObject) = 0;
	virtual void Walk(IGameObject& gameObject) = 0;
	virtual void Stand(IGameObject& gameObject) = 0;
	virtual void Hurt(IGameObject& gameObject) = 0;
};

class GameObjectAnimationFactory : public IGameObjectAnimationFactory
{
public:
	GameObjectAnimationFactory();
	virtual ~GameObjectAnimationFactory(){};

	virtual void Climb(IGameObject& gameObject);
	virtual void Walk(IGameObject& gameObject);
	virtual void Stand(IGameObject& gameObject);
	virtual void Hurt(IGameObject& gameObject);
};

#endif /* defined(__com_dotdat_World__GameObjectAnimationFactory_H__) */

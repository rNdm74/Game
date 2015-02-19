#ifndef __FranticAlien__GRAPHICS_COMPONENT_H__
#define __FranticAlien__GRAPHICS_COMPONENT_H__

#include "cocos2d.h"

class GameObject;

using namespace cocos2d;

class IGraphicsComponent
{
public:  
	//
	IGraphicsComponent(){};
	virtual ~IGraphicsComponent() {};
	virtual void update(IGameObject& gameObject) = 0;
};

class GraphicsComponent : public IGraphicsComponent
{
public:
	virtual void update(IGameObject& gameObject){};
};

class PlayerGraphicsComponent : public IGraphicsComponent
{
public:
	void update(IGameObject& gameObject) override;
};

#endif /* defined(__FranticAlien__GraphicsComponent__) */

#ifndef __FranticAlien__GRAPHICS_COMPONENT_H__
#define __FranticAlien__GRAPHICS_COMPONENT_H__

#include "cocos2d.h"

class GameObject;

using namespace cocos2d;

class IGraphicsComponent
{
public:    
	virtual void update(GameObjectNode& gameObjectNode) = 0;
};

class GraphicsComponent : IGraphicsComponent
{
public:
	virtual void update(GameObjectNode& gameObjectNode){};
};

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
	void update(GameObjectNode& gameObjectNode) override;
};

#endif /* defined(__FranticAlien__GraphicsComponent__) */

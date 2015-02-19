#ifndef __FranticAlien__INPUT_COMPONENT_H__
#define __FranticAlien__INPUT_COMPONENT_H__

#include "cocos2d.h"

class IGameObject;

using namespace cocos2d;

class IInputComponent
{
public:
	IInputComponent(){};
	virtual ~IInputComponent() {};
	virtual void update(Node& node, IGameObject& gameObject) = 0;
	virtual void findpath(Node& node, IGameObject& gameObject) = 0;
	virtual void desiredPosition(IGameObject& gameObject) = 0;
};

class InputComponent : public IInputComponent
{
public:
	virtual void update(Node& node, IGameObject& gameObject){};
	void findpath(Node& node, IGameObject& gameObject);
	void desiredPosition(IGameObject& gameObject);
};

class PlayerInputComponent : public InputComponent
{
public:
	void update(Node& node, IGameObject& gameObject) override;
};

#endif /* defined(__FranticAlien__InputComponent__) */

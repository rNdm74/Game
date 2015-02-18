#ifndef __FranticAlien__INPUT_COMPONENT_H__
#define __FranticAlien__INPUT_COMPONENT_H__

#include "cocos2d.h"

//class Moveable;
class GameObject;
class ParallaxTileMap;

using namespace cocos2d;

class IInputComponent
{
public:
	virtual void update(Node& node, GameObject& gameObject) = 0;
};

class InputComponent : public IInputComponent
{
public:
	virtual void update(Node& node, GameObject& gameObject){};

protected:
	void findpath(ParallaxTileMap& parallaxTileMap, GameObject& gameObject);
	void desiredPosition(GameObject& gameObject);
};

class PlayerInputComponent : public InputComponent
{
public:
	void update(Node& node, GameObject& gameObject) override;
};

#endif /* defined(__FranticAlien__InputComponent__) */

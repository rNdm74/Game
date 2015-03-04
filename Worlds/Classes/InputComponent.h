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

	/** Actions **/	
	virtual void ClimbUp(IGameObject& gameObject) = 0;
	virtual void ClimbDown(IGameObject& gameObject) = 0;
	virtual void WalkLeft(IGameObject& gameObject) = 0;
	virtual void WalkRight(IGameObject& gameObject) = 0;
	virtual void Stop(IGameObject& gameObject) = 0;

	/** **/
	virtual void update(Node& node, IGameObject& gameObject) = 0;
	//virtual void findpath(Node& node, IGameObject& gameObject) = 0;
	virtual void desiredPosition(IGameObject& gameObject) = 0;
};

class InputComponent : public IInputComponent
{
public:
	/** Actions **/	
	virtual void ClimbUp(IGameObject& gameObject);
	virtual void ClimbDown(IGameObject& gameObject);
	virtual void WalkLeft(IGameObject& gameObject);
	virtual void WalkRight(IGameObject& gameObject);
	virtual void Stop(IGameObject& gameObject);

	virtual void update(Node& node, IGameObject& gameObject){};
	//void findpath(Node& node, IGameObject& gameObject);
	void desiredPosition(IGameObject& gameObject);

protected:
	void updateDesiredPosition(IGameObject& gameObject, Vec2 direction);

	const Vec2 _gravity = Vec2(0.0, -1000.0);
	const Vec2 _movement = Vec2(840.0, 840.0);
	const Vec2 _minMovement = Vec2(-340.0, -340.0);
	const Vec2 _maxMovement = Vec2(340.0, 340.0);
};

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent();
	virtual ~PlayerInputComponent(){};
	void update(Node& node, IGameObject& gameObject) override;		

};

#endif /* defined(__FranticAlien__InputComponent__) */

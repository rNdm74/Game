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
	virtual void ExecuteAction(IGameObject& gameObject) = 0;
	/*virtual void ClimbUp(IGameObject& gameObject) = 0;
	virtual void ClimbDown(IGameObject& gameObject) = 0;
	virtual void WalkLeft(IGameObject& gameObject) = 0;
	virtual void WalkRight(IGameObject& gameObject) = 0;
	virtual void Stop(IGameObject& gameObject) = 0;*/

	/** **/
	virtual void update(Node& node, IGameObject& gameObject) = 0;
	//virtual void findpath(Node& node, IGameObject& gameObject) = 0;
	virtual void desiredPosition(IGameObject& gameObject) = 0;
};

class InputComponent : public IInputComponent
{
public:
	/** Actions **/
	virtual void ExecuteAction(IGameObject& gameObject);
	/*virtual void ClimbUp(IGameObject& gameObject);
	virtual void ClimbDown(IGameObject& gameObject);
	virtual void WalkLeft(IGameObject& gameObject);
	virtual void WalkRight(IGameObject& gameObject);
	virtual void Stop(IGameObject& gameObject);*/

	virtual void update(Node& node, IGameObject& gameObject){};
	//void findpath(Node& node, IGameObject& gameObject);
	void desiredPosition(IGameObject& gameObject);

private:
	void updateDesiredPosition(IGameObject& gameObject, Vec2 direction);

	const Vec2 _movement = Vec2(1600.0, 1600.0);
	const Vec2 _minMovement = Vec2(-340.0, -340.0);
	const Vec2 _maxMovement = Vec2(340.0, 340.0);
};

class PlayerInputComponent : public InputComponent
{
public:
	void update(Node& node, IGameObject& gameObject) override;
};

#endif /* defined(__FranticAlien__InputComponent__) */

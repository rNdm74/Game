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
	virtual void Up(IGameObject& gameObject) = 0;
	virtual void Down(IGameObject& gameObject) = 0;
	virtual void Left(IGameObject& gameObject) = 0;
	virtual void Right(IGameObject& gameObject) = 0;
	virtual void Stop(IGameObject& gameObject) = 0;
	virtual void Jump(IGameObject& gameObject) = 0;
	virtual void Gravity(IGameObject& gameObject) = 0;
	virtual void HitWall(IGameObject& gameObject) = 0;
	/** **/
	virtual void update(Node& node, IGameObject& gameObject) = 0;
};

class InputComponent : public IInputComponent
{
public:
	/** Actions **/	
	virtual void Up(IGameObject& gameObject);
	virtual void Down(IGameObject& gameObject);
	virtual void Left(IGameObject& gameObject);
	virtual void Right(IGameObject& gameObject);
	virtual void Stop(IGameObject& gameObject);
	virtual void Jump(IGameObject& gameObject);
	virtual void Gravity(IGameObject& gameObject);
	virtual void HitWall(IGameObject& gameObject);

	virtual void update(Node& node, IGameObject& gameObject){};
	
protected:
	const float _jumpLimit = 650.0f;
	const Vec2 _jumpForce = Vec2(0.0f, 2800.0f);
	const Vec2 _gravity = Vec2(0.0, -1850.0f);
	const Vec2 _movement = Vec2(1800.0f, 0.0f);
	const Vec2 _minMovement = Vec2(-320.0f, -850.0f);
	const Vec2 _maxMovement = Vec2(320.0f, 520.0f);
};

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(){};
	virtual ~PlayerInputComponent(){};
	void update(Node& node, IGameObject& gameObject) override;		

};

#endif /* defined(__FranticAlien__InputComponent__) */

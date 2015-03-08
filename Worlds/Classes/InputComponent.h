#ifndef __FranticAlien__INPUT_COMPONENT_H__
#define __FranticAlien__INPUT_COMPONENT_H__

class IGameObject;

class IInputComponent
{
public:
	IInputComponent(){};
	virtual ~IInputComponent() {};

	/** Actions **/	
	virtual void Up() = 0;
	virtual void Down() = 0;
	virtual void Left() = 0;
	virtual void Right() = 0;
	virtual void Stop() = 0;
	virtual void Jump() = 0;
	virtual void ApplyGravity() = 0;
	virtual void HitWall() = 0;
	/** **/
	virtual void update() = 0;
};

class InputComponent : public IInputComponent
{
public:
	InputComponent(){};
	virtual ~InputComponent(){};

	/** Actions **/	
	virtual void Up();
	virtual void Down();
	virtual void Left();
	virtual void Right();
	virtual void Stop();
	virtual void Jump();
	virtual void ApplyGravity();
	virtual void HitWall();

	virtual void update(){};
	
protected:
	IGameObject* _gameObject;

	const float _jumpLimit = 850.0f;
	const Vec2 _jumpForce = Vec2(0.0f, 2800.0f);
	const Vec2 _gravity = Vec2(0.0, -1850.0f);
	const Vec2 _movement = Vec2(2800.0f, 2800.0f);
	const Vec2 _minMovement = Vec2(-520.0f, -850.0f);
	const Vec2 _maxMovement = Vec2(520.0f, 520.0f);
};

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(IGameObject& gameObject){ _gameObject = &gameObject; };
	void update() override;		

};

#endif /* defined(__FranticAlien__InputComponent__) */

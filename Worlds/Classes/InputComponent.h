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
	typedef IInputComponent super;
	typedef InputComponent self;

public:
	InputComponent(IGameObject& gameObject){ _gameObject = &gameObject; };
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

	float _jumpLimit;
	Vec2 _jumpForce;
	Vec2 _gravity;
	Vec2 _movement;
	Vec2 _minMovement;
	Vec2 _maxMovement;
};

class PlayerInputComponent : public InputComponent
{
	typedef InputComponent super;
	typedef PlayerInputComponent self;

public:
	PlayerInputComponent(IGameObject& gameObject);
	void update() override;		

};

class NpcInputComponent : public InputComponent
{
	typedef InputComponent super;
	typedef NpcInputComponent self;

public:
	NpcInputComponent(IGameObject& gameObject);
	void update() override;

};

#endif /* defined(__FranticAlien__InputComponent__) */

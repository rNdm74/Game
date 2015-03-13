#ifndef __FranticAlien__FSM_COMPONENT_H__
#define __FranticAlien__FSM_COMPONENT_H__

#include "AppGlobal.h"

class IGameObject;
class IFsmComponent;

struct IFsmState
{
	virtual void ActionUp(IFsmComponent& fsm) = 0;
	virtual void ActionDown(IFsmComponent& fsm) = 0;
	virtual void ActionLeft(IFsmComponent& fsm) = 0;
	virtual void ActionRight(IFsmComponent& fsm) = 0;
	virtual void ActionStop(IFsmComponent& fsm) = 0;
	virtual void ActionJump(IFsmComponent& fsm) = 0;
};

struct FsmState : public IFsmState
{
	/** Actions **/
	virtual void ActionUp(IFsmComponent& fsm);
	virtual void ActionDown(IFsmComponent& fsm);
	virtual void ActionLeft(IFsmComponent& fsm);
	virtual void ActionRight(IFsmComponent& fsm);
	virtual void ActionStop(IFsmComponent& fsm);
	virtual void ActionJump(IFsmComponent& fsm);
};

struct UpState : public FsmState
{	
    void ActionUp(IFsmComponent& fsm) override;
	void ActionStop(IFsmComponent& fsm) override;
};

struct DownState : public FsmState
{	
	void ActionDown(IFsmComponent& fsm) override;
	void ActionStop(IFsmComponent& fsm) override;
};

struct LeftState : public FsmState
{	
	void ActionLeft(IFsmComponent& fsm) override;
};

struct RightState : public FsmState
{
	void ActionRight(IFsmComponent& fsm) override;
};

struct StopState : public FsmState
{	
	void ActionLeft(IFsmComponent& fsm) override;
	void ActionRight(IFsmComponent& fsm) override;
    void ActionStop(IFsmComponent& fsm) override;
    
    long timeout = 0l;
};

struct IdleState : public FsmState
{	
    void ActionStop(IFsmComponent& fsm) override;
};

struct JumpState : public FsmState
{
	void ActionJump(IFsmComponent& fsm) override;	
};


class IFsmComponent
{
public:
	UpState* StateUp;
	DownState* StateDown;
	LeftState* StateLeft;
	RightState* StateRight;
	StopState* StateStop;
    IdleState* StateIdle;
	JumpState* StateJump;

	IFsmComponent()
    {
        StateUp = new UpState();
        StateDown = new DownState();
        StateLeft = new LeftState();
        StateRight = new RightState();
        StateStop = new StopState();
        StateIdle = new IdleState();
		StateJump = new JumpState();
    };

	virtual ~IFsmComponent()
    {
        delete StateUp;
        delete StateDown;
        delete StateLeft;
        delete StateRight;
        delete StateStop;
        delete StateIdle;
		delete StateJump;
    };

	virtual void update() = 0;

	/** Events**/
	virtual void EventUp() = 0;
	virtual void EventDown() = 0;
	virtual void EventLeft() = 0;
	virtual void EventRight() = 0;
	virtual void EventStop() = 0;
	virtual void EventJump() = 0;

	/** Events end **/

	virtual void setCurrentState(IFsmState* currentState) = 0;

public: /** Variables **/
    IGameObject* gameObject;
};

class FsmComponent : public IFsmComponent
{
	typedef IFsmComponent super;
	typedef FsmComponent self;

public:
	FsmComponent(IGameObject& gameObject);
	virtual ~FsmComponent()
	{
		delete gameObject;
		delete currentState;
	};

	virtual void update() override;

	virtual void EventUp(){ currentState->ActionUp(*this); };
	virtual void EventDown(){ currentState->ActionDown(*this); };
	virtual void EventLeft(){ currentState->ActionLeft(*this); };
	virtual void EventRight(){ currentState->ActionRight(*this); };
	virtual void EventStop(){ currentState->ActionStop(*this); };
	virtual void EventJump(){ currentState->ActionJump(*this); };
	
	virtual void setCurrentState(IFsmState* newState){ currentState = newState; };

protected:
	IFsmState* currentState;
};

class PlayerFsmComponent : public FsmComponent
{
	typedef FsmComponent super;
	typedef PlayerFsmComponent self;

public:
	PlayerFsmComponent(IGameObject& gameObject);
	virtual ~PlayerFsmComponent(){};
};

class NpcFsmComponent : public FsmComponent
{
	typedef FsmComponent super;
	typedef NpcFsmComponent self;

public:
	NpcFsmComponent(IGameObject& gameObject);
	virtual ~NpcFsmComponent(){};
};




#endif /* defined(__FranticAlien__FSM_COMPONENT_H__) */
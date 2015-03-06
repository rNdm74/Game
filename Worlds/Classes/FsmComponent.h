#ifndef __FranticAlien__FSM_COMPONENT_H__
#define __FranticAlien__FSM_COMPONENT_H__

#include <typeinfo>
#include "cocos2d.h"
#include "AppGlobal.h"


class IGameObject;
class IFsmComponent;

using namespace cocos2d;

struct IFsmState
{
	virtual void ActionUp(IFsmComponent& fsm) = 0;
	virtual void ActionDown(IFsmComponent& fsm) = 0;
	virtual void ActionLeft(IFsmComponent& fsm) = 0;
	virtual void ActionRight(IFsmComponent& fsm) = 0;
	virtual void ActionStop(IFsmComponent& fsm) = 0;
	virtual void ActionJump(IFsmComponent& fsm) = 0;
	
	virtual Type GetState() = 0;
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
	
	virtual Type GetState(){ return "FsmState"; };
};

struct UpState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };

    void ActionUp(IFsmComponent& fsm) override;
};

struct DownState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };

	void ActionDown(IFsmComponent& fsm) override;
};

struct LeftState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };

	void ActionLeft(IFsmComponent& fsm) override;
};

struct RightState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };

	void ActionRight(IFsmComponent& fsm) override;
};

struct StopState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };
	
	void ActionLeft(IFsmComponent& fsm) override;
	void ActionRight(IFsmComponent& fsm) override;
    void ActionStop(IFsmComponent& fsm) override;
    
    long timeout = 0l;
};

struct IdleState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };

    void ActionStop(IFsmComponent& fsm) override;
};

struct JumpState : public FsmState
{
	Type GetState() override { return typeid(this).name(); };

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

	virtual void update(Node& node, IGameObject& gameObject) = 0;

	/** Events**/
	virtual void EventUp() = 0;
	virtual void EventDown() = 0;
	virtual void EventLeft() = 0;
	virtual void EventRight() = 0;
	virtual void EventStop() = 0;
	virtual void EventJump() = 0;

	virtual Type getCurrentState() = 0;
	virtual void setCurrentState(FsmState* currentState) = 0;
    
    IGameObject* gameObject;
};

class FsmComponent : public IFsmComponent
{
public:
	FsmComponent(){};
	virtual ~FsmComponent(){};
	virtual void update(Node& node, IGameObject& gameObject){};

	virtual void EventUp(){ currentState->ActionUp(*this); };
	virtual void EventDown(){ currentState->ActionDown(*this); };
	virtual void EventLeft(){ currentState->ActionLeft(*this); };
	virtual void EventRight(){ currentState->ActionRight(*this); };
	virtual void EventStop(){ currentState->ActionStop(*this); };
	virtual void EventJump(){ currentState->ActionJump(*this); };
	
	virtual Type getCurrentState(){ return currentState->GetState(); };
	virtual void setCurrentState(FsmState* newState){ currentState = newState; };

protected:
	FsmState* currentState;
};

class PlayerFsmComponent : public FsmComponent
{
public:
    PlayerFsmComponent()
    {
        this->currentState = StateStop;
    };
	virtual ~PlayerFsmComponent(){};
    
	virtual void update(Node& node, IGameObject& gameObject) override;
};



#endif /* defined(__FranticAlien__FSM_COMPONENT_H__) */
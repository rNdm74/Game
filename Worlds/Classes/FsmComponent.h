#ifndef __FranticAlien__FSM_COMPONENT_H__
#define __FranticAlien__FSM_COMPONENT_H__

#include "cocos2d.h"
#include "AppGlobal.h"

class IGameObject;
class IFsmComponent;

using namespace cocos2d;

struct IFsmStates
{
	virtual void ActionUp(IFsmComponent& fsm) = 0;
	virtual void ActionDown(IFsmComponent& fsm) = 0;
	virtual void ActionLeft(IFsmComponent& fsm) = 0;
	virtual void ActionRight(IFsmComponent& fsm) = 0;
	virtual void ActionStop(IFsmComponent& fsm) = 0;
};

struct FsmStates : public IFsmStates
{
	/** Actions **/
	virtual void ActionUp(IFsmComponent& fsm);
	virtual void ActionDown(IFsmComponent& fsm);
	virtual void ActionLeft(IFsmComponent& fsm);
	virtual void ActionRight(IFsmComponent& fsm);
	virtual void ActionStop(IFsmComponent& fsm);
};

struct UpState : public FsmStates
{
	void ActionUp(IFsmComponent& fsm) override
	{
		AppGlobal::getInstance()->zoomOut();
		log("Up");
	};
};

struct DownState : public FsmStates
{
	virtual void ActionDown(IFsmComponent& fsm) override
	{
		AppGlobal::getInstance()->zoomOut();
		log("Down");
	};
};

struct LeftState : public FsmStates
{
	virtual void ActionLeft(IFsmComponent& fsm) override
	{
		AppGlobal::getInstance()->zoomOut();
		log("Left");
	};
};

struct RightState : public FsmStates
{
	virtual void ActionRight(IFsmComponent& fsm) override
	{
		AppGlobal::getInstance()->zoomOut();
		log("Right");
	};
};

struct StopState : public FsmStates
{
	virtual void ActionStop(IFsmComponent& fsm) override
	{
		AppGlobal::getInstance()->zoomIn();
		log("Stop");
	};
};

class IFsmComponent
{
public:
	UpState StateUp;
	DownState StateDown;
	LeftState StateLeft;
	RightState StateRight;
	StopState StateStop;

	IFsmComponent(){};
	virtual ~IFsmComponent(){};

	virtual void update(Node& node, IGameObject& gameObject) = 0;

	/** Events**/
	virtual void EventUp() = 0;
	virtual void EventDown() = 0;
	virtual void EventLeft() = 0;
	virtual void EventRight() = 0;
	virtual void EventStop() = 0;

	virtual void setCurrentState(FsmStates& currentState) = 0;
};

class FsmComponent : public IFsmComponent
{
public:
	FsmComponent(){};
	virtual ~FsmComponent(){};
	virtual void update(Node& node, IGameObject& gameObject){};

	virtual void EventUp(){ currentState.ActionUp(*this); };
	virtual void EventDown(){ currentState.ActionDown(*this); };
	virtual void EventLeft(){ currentState.ActionLeft(*this); };
	virtual void EventRight(){ currentState.ActionRight(*this); };
	virtual void EventStop(){ currentState.ActionStop(*this); };

	virtual void setCurrentState(FsmStates& newState){ currentState = newState; log("%i", static_cast<int>(currentState)); };

protected:
	FsmStates currentState;
};

class PlayerFsmComponent : public FsmComponent
{
public:
	PlayerFsmComponent(){};
	virtual ~PlayerFsmComponent(){};		
	virtual void update(Node& node, IGameObject& gameObject) override;
};



#endif /* defined(__FranticAlien__FSM_COMPONENT_H__) */
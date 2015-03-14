#ifndef __FranticAlien__AI_COMPONENT_H__
#define __FranticAlien__AI_COMPONENT_H__

#include "AppGlobal.h"

class IGameObject;
class IAiComponent;

struct IAiState
{
    virtual ~IAiState(){};
    
    /** Declare the actions **/
	virtual void ActionWander(IAiComponent& fsm) = 0;
    virtual void ActionScared(IAiComponent& fsm) = 0;
	virtual void ActionCurious(IAiComponent& fsm) = 0;
	virtual void ActionDropGem(IAiComponent& fsm) = 0;
	virtual void ActionDecision(IAiComponent& fsm) = 0;
	virtual void ActionInteract(IAiComponent& fsm) = 0;
	virtual void ActionResting(IAiComponent& fsm) = 0;
	virtual void ActionCaptured(IAiComponent& fsm) = 0;
};

struct AiState : public IAiState
{
	/** Actions **/
	virtual void ActionWander(IAiComponent& fsm){};
	virtual void ActionScared(IAiComponent& fsm){};
	virtual void ActionCurious(IAiComponent& fsm){};
	virtual void ActionDropGem(IAiComponent& fsm){};
	virtual void ActionDecision(IAiComponent& fsm){};
	virtual void ActionInteract(IAiComponent& fsm){};
	virtual void ActionResting(IAiComponent& fsm){};
	virtual void ActionCaptured(IAiComponent& fsm);
};

struct WanderState : public AiState
{
    /** Actions **/
	virtual void ActionWander(IAiComponent& fsm) override;
};

struct DecisionState : public AiState
{
    /** Actions **/
	virtual void ActionWander(IAiComponent& fsm) override;
	virtual void ActionScared(IAiComponent& fsm) override {};
	virtual void ActionCurious(IAiComponent& fsm) override {};
	virtual void ActionDropGem(IAiComponent& fsm) override {};
	virtual void ActionDecision(IAiComponent& fsm) override;
	virtual void ActionInteract(IAiComponent& fsm) override {};
	virtual void ActionResting(IAiComponent& fsm) override {};
};

struct ScaredState : public AiState
{
	/** Actions **/
};

struct CuriousState : public AiState
{
	/** Actions **/
};

struct DropGemState : public AiState
{
	/** Actions **/
};

struct InteractState : public AiState
{
	/** Actions **/
};

struct RestingState : public AiState
{
	/** Actions **/
	virtual void ActionDecision(IAiComponent& fsm) override;
	virtual void ActionResting(IAiComponent& fsm) override;
};

struct CapturedState : public AiState
{
	/** Actions **/
	virtual void ActionCaptured(IAiComponent& fsm) override;
};


class IAiComponent
{
public:
	WanderState* StateWander;    
    ScaredState* StateScared;
    CuriousState* StateCurious;
	DropGemState* StateDropGem;
	DecisionState* StateDecision;
    InteractState* StateInteract;
	RestingState* StateResting;
	CapturedState* StateCaptured;
    
	IAiComponent()
    {		
        StateWander = new WanderState();
		StateScared = new ScaredState();
		StateCurious = new CuriousState();
		StateDropGem = new DropGemState();
		StateDecision = new DecisionState();
		StateInteract = new InteractState();
		StateResting = new RestingState();
		StateCaptured = new CapturedState();
    };

	virtual ~IAiComponent()
    {
        delete StateWander;        
        delete StateScared;
        delete StateCurious;
		delete StateDropGem;
		delete StateDecision;
        delete StateInteract;
		delete StateResting;
		delete StateCaptured;
        
        delete gameObject;
	};

	virtual void update() = 0;

	/** Events**/
	virtual void EventWander() = 0;
	virtual void EventScared() = 0;
	virtual void EventCurious() = 0;
	virtual void EventDropGem() = 0;
	virtual void EventDecision() = 0;
	virtual void EventInteract() = 0;
	virtual void EventResting() = 0;
	virtual void EventCaptured() = 0;
	/** Events end **/

	virtual void setCurrentState(IAiState* currentState) = 0;

public: /** Variables **/
    IGameObject* gameObject;
    
    long timeout;
    
    long decisionTime;
	long maxStamina;
};

class AiComponent : public IAiComponent
{
    typedef IAiComponent super;
    typedef AiComponent self;
    
public:
	AiComponent(IGameObject& gameObject);
	virtual ~AiComponent()
    {
        delete currentState;
    };

	virtual void update();

    /** Events **/
	virtual void EventWander(){ currentState->ActionWander(*this); };
	virtual void EventScared(){ currentState->ActionScared(*this); };
	virtual void EventCurious(){ currentState->ActionCurious(*this); };
	virtual void EventDropGem(){ currentState->ActionDropGem(*this); };
	virtual void EventDecision(){ currentState->ActionDecision(*this); };
	virtual void EventInteract(){ currentState->ActionInteract(*this); };
	virtual void EventResting(){ currentState->ActionResting(*this); };
	virtual void EventCaptured(){ currentState->ActionCaptured(*this); };
    
	virtual void setCurrentState(IAiState* newState){ currentState = newState; };

protected:
	IAiState* currentState;
};

class NpcAiComponent : public AiComponent
{
    typedef AiComponent super;
    typedef NpcAiComponent self;
    
public:
	NpcAiComponent(IGameObject& gameObject);
	virtual ~NpcAiComponent(){};
};




#endif /* defined(__FranticAlien__AI_COMPONENT_H__) */
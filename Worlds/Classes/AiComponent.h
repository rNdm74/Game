#ifndef __FranticAlien__AI_COMPONENT_H__
#define __FranticAlien__AI_COMPONENT_H__

#include "AppGlobal.h"

class IGameObject;
class IAiComponent;

struct IAiState
{
    /** Declare the actions **/
	virtual void ActionWander(IAiComponent& fsm) = 0;
    virtual void ActionDecision(IAiComponent& fsm) = 0;
};

struct AiState : public IAiState
{
	/** Actions **/
	virtual void ActionWander(IAiComponent& fsm);
    virtual void ActionDecision(IAiComponent& fsm);
};

struct WanderState : public AiState
{
    /** Actions **/
	virtual void ActionWander(IAiComponent& fsm) override;
    virtual void ActionDecision(IAiComponent& fsm) override;
};

struct DecisionState : public AiState
{
    /** Actions **/
    virtual void ActionWander(IAiComponent& fsm) override;
    virtual void ActionDecision(IAiComponent& fsm) override;
};

struct ScaredState : public AiState{};
struct CuriousState : public AiState{};
struct PoopState : public AiState{};
struct InteractState : public AiState{};

class IAiComponent
{
public:
	WanderState* StateWander;
    DecisionState* StateDecision;
    ScaredState* StateScared;
    CuriousState* StateCurious;
    PoopState* StatePoop;
    InteractState* StateInteract;
    
	IAiComponent()
    {
        StateWander = new WanderState();
        StateDecision = new DecisionState();
        StateScared = new ScaredState();
        StateCurious = new CuriousState();
        StatePoop = new PoopState();
        StateInteract = new InteractState();
    };

	virtual ~IAiComponent()
    {
        delete StateWander;
        delete StateDecision;
        delete StateScared;
        delete StateCurious;
        delete StatePoop;
        delete StateInteract;
	};

	virtual void update() = 0;

	/** Events**/
	virtual void EventWander() = 0;

	/** Events end **/

	virtual void setCurrentState(IAiState* currentState) = 0;

public: /** Variables **/
    IGameObject* gameObject;
    
    long timeout;
    
    long decisionTime;
};

class AiComponent : public IAiComponent
{
    typedef IAiComponent super;
    typedef AiComponent self;
    
public:
	AiComponent(IGameObject& gameObject);
	virtual ~AiComponent(){};

	virtual void update();

    /** Events **/
    virtual void EventWander();
    
    virtual void setCurrentState(IAiState* newState);

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
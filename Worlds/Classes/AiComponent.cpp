#include "AiComponent.h"
#include "GameObject.h"

AiComponent::AiComponent(IGameObject& gameObject)
{
    this->gameObject = &gameObject;
    this->currentState = StateDecision;
};

NpcAiComponent::NpcAiComponent(IGameObject& gameObject) : super(gameObject)
{
    timeout = 0l;
    decisionTime  = 0l;
};

void AiState::ActionWander(IAiComponent& fsm)
{ 		
	fsm.setCurrentState( fsm.StateWander );
};

void DecisionState::ActionDecision(IAiComponent& fsm)
{
    AiState newState;
    
    switch (random(0, 6))
    {
        case ENpcStates::DecisionState:
            
            break;
            
        case ENpcStates::CuriousState:
            
            break;
        case ENpcStates::InteractState:
            
            break;
        case ENpcStates::DecisionState:
            
            break;
        case ENpcStates::DecisionState:
            
            break;
        case ENpcStates::DecisionState:
            
            break;
        default:
            break;
    }
};

void WanderState::ActionWander(IAiComponent& fsm) /** Override **/
{
	//log("Example action overriden");
    Events& events = fsm.gameObject->events;
    
    
    int choice = random(0, 5);
    
    EGameObjectEvent gameObjectEvent = EGameObjectEvent::Stop;
    
    if(choice == 0) /** 10% probability **/
    {
        gameObjectEvent = EGameObjectEvent::Left;
    }
    else if(choice == 5)
    {
        gameObjectEvent = EGameObjectEvent::Right;
    }
    
    
    if(fsm.timeout > 30l)
    {
        fsm.timeout = 0l;
        
        if(events.top() != EGameObjectEvent::Stop)
        {
            while(!events.empty()) events.pop();
        }
            
        events.push(gameObjectEvent);
    }
    
    fsm.timeout++;
};

void AiComponent::update()
{
	void(IAiComponent:: *ptrs[])() =
	{
		&IAiComponent::EventWander,
	};
	
	int runningEvent = 0;

	(this->*ptrs[runningEvent])();
}

void AiComponent::EventWander()
{
    currentState->ActionWander(*this);
};

void AiComponent::setCurrentState(IAiState *newState)
{
    currentState = newState;
};




#include "AiComponent.h"
#include "GameObject.h"


void AiState::ActionCaptured(IAiComponent& fsm){ fsm.setCurrentState(fsm.StateCaptured); };

void DecisionState::ActionDecision(IAiComponent& fsm)
{
	// Lets make a decision on what we want todo
	// Hmmm, how about we go for a wander
	fsm.gameObject->addAiEvent(EAiEvent::Wander);
};
void DecisionState::ActionWander(IAiComponent& fsm)
{
	// Choose an event and then change our state to wandering
	// In this example, we are going to start walking left
	// We request a single left event
	int min = EMovementEvent::Left;
	int max = EMovementEvent::Right;

	EMovementEvent movementEvent = static_cast<EMovementEvent>(random(min, max));
	
	/** Well I will walk left for a little bit **/
	fsm.gameObject->addMovementEvent(movementEvent);


	// Change state
	fsm.setCurrentState(fsm.StateWander); 
};


void WanderState::ActionWander(IAiComponent& fsm)
{
	long staminaRate = fsm.gameObject->getVelocity().x * kUpdateInterval;
	//log("%i", staminaRate);

	fsm.gameObject->Stamina -= std::abs(staminaRate);
		
	// Know I need todo some wandering until i get tired
	if (fsm.gameObject->Stamina < 0l)
	{
		// We get rid of the walking left 
		// Clear the movement events		
		// Should have only the stop event in stack
				
		// Tell gameObject to rest
		fsm.gameObject->addAiEvent(EAiEvent::Resting);
		
		// Change the resting state
		fsm.setCurrentState(fsm.StateResting);
	}	
};

void RestingState::ActionResting(IAiComponent& fsm)
{
	// Well I will walk left for a little bit, then stop and rest, I get tired quickly

	// Let the npc regain its stamina
	fsm.gameObject->Stamina += 15l;

	// Ahhh I am all rested now lets continue walking 
	if (fsm.gameObject->Stamina > fsm.maxStamina)
	{
		// Max stamina set
		fsm.gameObject->Stamina = fsm.maxStamina;
		
		// Push the decision event request
		fsm.gameObject->addAiEvent(EAiEvent::Decision);

		// Change the state to decision
		fsm.setCurrentState(fsm.StateDecision);
	}
};
void RestingState::ActionDecision(IAiComponent& fsm)
{
	
};

void CapturedState::ActionCaptured(IAiComponent& fsm)
{
	fsm.gameObject->Captured();
};

AiComponent::AiComponent(IGameObject& gameObject)
{
	this->gameObject = &gameObject;
	this->currentState = StateDecision;
};

NpcAiComponent::NpcAiComponent(IGameObject& gameObject) : super(gameObject)
{
	timeout = 0l;
	decisionTime = 0l;
	maxStamina = random(500l, 1000l);
};

void AiComponent::update()
{
	void(IAiComponent:: *ptrs[])() =
	{
		&IAiComponent::EventWander,
		&IAiComponent::EventScared,
		&IAiComponent::EventCurious,
		&IAiComponent::EventDropGem,
		&IAiComponent::EventDecision,
		&IAiComponent::EventInteract,
		&IAiComponent::EventResting,
		&IAiComponent::EventCaptured
	};
	
	int runningEvent = gameObject->getCurrentAiEvent();

	(this->*ptrs[runningEvent])();
}




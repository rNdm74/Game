#include "AiComponent.h"
#include "GameObject.h"


void DecisionState::ActionDecision(IAiComponent& fsm)
{
	// Lets make a decision on what we want todo
	// Hmmm, how about we go for a wander
	NpcEvents& npcEvents = static_cast<Npc*>(fsm.gameObject)->npcEvents;

	npcEvents.push(ENpcEvent::Wander);
};
void DecisionState::ActionWander(IAiComponent& fsm)
{
	// Choose an event and then change our state to wandering
	// In this example, we are going to start walking left
	// We request a single left event
	int min = EGameObjectEvent::Left;
	int max = EGameObjectEvent::Right;

	EGameObjectEvent pickedEvent = static_cast<EGameObjectEvent>(random(min, max));

	fsm.gameObject->events.push(pickedEvent); /** Well I will walk left for a little bit **/


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
		fsm.gameObject->events.pop();
		// Should have only the stop event in stack

		NpcEvents& npcEvents = static_cast<Npc*>(fsm.gameObject)->npcEvents;

		// Pop the wandering event
		npcEvents.pop();
		// Push a resting event
		npcEvents.push(ENpcEvent::Resting);
		
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

		NpcEvents& npcEvents = static_cast<Npc*>(fsm.gameObject)->npcEvents;
		// Pop the resting event request
		npcEvents.pop();

		// Push the decision event request
		//npcEvents.push(ENpcEvent::Decision);

		// Change the state to decision
		fsm.setCurrentState(fsm.StateDecision);
	}
};

void RestingState::ActionDecision(IAiComponent& fsm)
{
	
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
	float age = gameObject->getSprite().getScale();
	age += gameObject->GrowFactor;
	if (age > kAdultAge)
		age = kAdultAge;

	gameObject->getSprite().setScale(age);

	void(IAiComponent:: *ptrs[])() =
	{
		&IAiComponent::EventWander,
		&IAiComponent::EventScared,
		&IAiComponent::EventCurious,
		&IAiComponent::EventDropGem,
		&IAiComponent::EventDecision,
		&IAiComponent::EventInteract,
		&IAiComponent::EventResting,
	};
	
	int runningEvent = static_cast<Npc*>(gameObject)->npcEvents.top();

	(this->*ptrs[runningEvent])();
}




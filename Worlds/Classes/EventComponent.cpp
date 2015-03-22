#include "EventComponent.h"
#include "GameObject.h"


EventComponent::EventComponent(IGameObject& gameObject)
{
	_gameObject = &gameObject;
};


void EventComponent::update()
{
	//printMovementEvents();
	//printAiEvents();
};

/** MOVEMENT EVENTS **/
void EventComponent::addMovementEvent(Vec2 direction)
{
	EMovementEvent movementEvent = EMovementEvent::Stop;

	if (direction.x > 0) // Right
		movementEvent = EMovementEvent::Right;
	else if (direction.x < 0) // Left
		movementEvent = EMovementEvent::Left;
	else if (direction.y > 0) // Jump
		movementEvent = EMovementEvent::Jump;
	else if (direction.y < 0) // do nothing
		movementEvent = EMovementEvent::Down;

	if (movementEvent == EMovementEvent::Jump)
		_movementEvents.push(EMovementEvent::Stop);

	_movementEvents.push(movementEvent);

#if DEBUG_ENABLE
	log((getMovementEventName(movementEvent) + " movement event is being pushed to the stack").c_str());
#endif // DEBUG_ENABLE
};

void EventComponent::removeMovementEvent()
{
	// Pop the stack
	EMovementEvent movementEvent = _movementEvents.top();
	_movementEvents.pop();

#if DEBUG_ENABLE
	log((getMovementEventName(movementEvent) + " is being removed from the stack").c_str());
#endif // DEBUG_ENABLE
};

EMovementEvent EventComponent::runningMovementEvent()
{
	// Peek stack
	return _movementEvents.top();
#if DEBUG_ENABLE
	//log((getMovementEventName(_movementEvents.top()) + " currently running movement event").c_str());
#endif // DEBUG_ENABLE
};

bool EventComponent::clearMovementEvents()
{
	for (_movementEvents; !_movementEvents.empty(); _movementEvents.pop());

#if DEBUG_ENABLE
	log("movement events cleared !");
#endif // DEBUG_ENABLE

	return true;
};

#if DEBUG_ENABLE
void EventComponent::printMovementEvents()
{
	for (MovementEvents dump = _movementEvents; !dump.empty(); dump.pop())
		log(("Movement Event: " + getMovementEventName(dump.top())).c_str());

	log(("(" + std::to_string(_movementEvents.size()) + " movement elements)").c_str());
};
#endif // DEBUG_ENABLE
/** MOVEMENT EVENTS END **/

/** AI EVENTS **/
void EventComponent::addAiEvent(EAiEvent newAiEvent)
{
	// Check event is not added
	for (AiEvents dump = _aiEvents; !dump.empty(); dump.pop())
		if (newAiEvent != dump.top())
			return;

	// if event is not added :: push to stack
	_aiEvents.push(newAiEvent);

#if DEBUG_ENABLE
	log((getAiEventName(newAiEvent) + " movement event is being pushed to the stack").c_str());
#endif // DEBUG_ENABLE
};

void EventComponent::removeAiEvent(EAiEvent aiEvent)
{
	// Pop the stack
	_aiEvents.pop();

#if DEBUG_ENABLE
	log((getAiEventName(aiEvent) + " is being removed from the stack").c_str());
#endif // DEBUG_ENABLE
};

EAiEvent EventComponent::runningAiEvent()
{
	// Peek stack
	return _aiEvents.top();

#if DEBUG_ENABLE
	log((getAiEventName(_aiEvents.top()) + " currently running movement event").c_str());
#endif // DEBUG_ENABLE
};

bool EventComponent::clearAiEvents()
{
	for (_aiEvents; !_aiEvents.empty(); _aiEvents.pop());
	return true;
};

#if DEBUG_ENABLE
void EventComponent::printAiEvents()
{
	for (AiEvents dump = _aiEvents; !dump.empty(); dump.pop())
		log(("Ai Event: " + getAiEventName(dump.top())).c_str());

	log(("(" + std::to_string(_aiEvents.size()) + " ai elements)").c_str());
};
#endif // DEBUG_ENABLE
/** AI EVENTS END**/


/** PRIVATE FUNCTIONS **/
/**
* Switch statements that return strings of the events
*/
std::string EventComponent::getMovementEventName(EMovementEvent movementEvent)
{
	switch (movementEvent)
	{
	case EMovementEvent::Up:
		return "Up";
	case EMovementEvent::Down:
		return "Down";
	case EMovementEvent::Left:
		return "Left";
	case EMovementEvent::Right:
		return "Right";
	case EMovementEvent::Stop:
		return "Stop";
	case EMovementEvent::Jump:
		return "Jump";
	default:
		return "Event does not exist";
	};
};
/**
* Switch statements that return strings of the events
*/
std::string EventComponent::getAiEventName(EAiEvent aiEvent)
{
	switch (aiEvent)
	{
	case EAiEvent::Captured:
		return "Captured";
	case EAiEvent::Curious:
		return "Curious";
	case EAiEvent::Decision:
		return "Decision";
	case EAiEvent::DropGem:
		return "DropGem";
	case EAiEvent::Interact:
		return "Interact";
	case EAiEvent::Resting:
		return "Resting";
	case EAiEvent::Scared:
		return "Scared";
	case EAiEvent::Wander:
		return "Wander";
	default:
		return "Event does not exist";
	};
};
/** PRIVATE FUNCTIONS END **/


PlayerEventComponent::PlayerEventComponent(IGameObject& gameObject) : super(gameObject)
{
	// Player is in the stop state and it is pushed onto the event stack
	_movementEvents.push(EMovementEvent::Stop);
};


NpcEventComponent::NpcEventComponent(IGameObject& gameObject) : super(gameObject)
{
	/**  **/
	_movementEvents.push(EMovementEvent::Stop);
	_aiEvents.push(EAiEvent::Decision);
};
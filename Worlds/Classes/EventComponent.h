#ifndef __FranticAlien__EVENT_COMPONENT_H__
#define __FranticAlien__EVENT_COMPONENT_H__

#include "AppGlobal.h"

class IGameObject;

class IEventComponent
{
public:	
	virtual void update() = 0;
	virtual void addMovementEvent(Vec2 direction) = 0;
	virtual void removeMovementEvent() = 0;
	virtual EMovementEvent runningMovementEvent() = 0;
	virtual bool clearMovementEvents() = 0;
#if DEBUG_ENABLE
	virtual void printMovementEvents() = 0;
#endif // DEBUG_ENABLE

	virtual void addAiEvent(EAiEvent newEvent) = 0;
	virtual void removeAiEvent(EAiEvent aiEvent) = 0;
	virtual EAiEvent runningAiEvent() = 0;	
	virtual bool clearAiEvents() = 0;	
#if DEBUG_ENABLE
	virtual void printAiEvents() = 0;
#endif // DEBUG_ENABLE
};

class EventComponent : public IEventComponent
{
    typedef IEventComponent super;
    typedef EventComponent self;
    
public:
	EventComponent(IGameObject& gameObject);
	virtual ~EventComponent(){};	

	virtual void update() override;

	virtual void addMovementEvent(Vec2 direction);
	virtual void removeMovementEvent();
	virtual EMovementEvent runningMovementEvent();
	virtual bool clearMovementEvents();
#if DEBUG_ENABLE
	virtual void printMovementEvents();
#endif // DEBUG_ENABLE

	virtual void addAiEvent(EAiEvent newEvent);
	virtual void removeAiEvent(EAiEvent aiEvent);
	virtual EAiEvent runningAiEvent();
	virtual bool clearAiEvents();
#if DEBUG_ENABLE
	virtual void printAiEvents();
#endif // DEBUG_ENABLE

protected:
	std::string getMovementEventName(EMovementEvent gameObjectEvent);
	std::string getAiEventName(EAiEvent aiEvent);

	IGameObject* _gameObject;

	MovementEvents _movementEvents;
	AiEvents _aiEvents;	
};


class PlayerEventComponent : public EventComponent
{
	typedef EventComponent super;
	typedef PlayerEventComponent self;

public:
	PlayerEventComponent(IGameObject& gameObject);
	virtual ~PlayerEventComponent(){};

private:	
};


class NpcEventComponent : public EventComponent
{
    typedef EventComponent super;
    typedef NpcEventComponent self;
    
public:
	NpcEventComponent(IGameObject& gameObject);
	virtual ~NpcEventComponent(){};

private:	
};




#endif /* defined(__FranticAlien__EVENT_COMPONENT_H__) */
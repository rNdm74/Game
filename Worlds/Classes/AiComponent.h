#ifndef __FranticAlien__AI_COMPONENT_H__
#define __FranticAlien__AI_COMPONENT_H__

#include "AppGlobal.h"

class IGameObject;
class IAiComponent;

struct IAiState
{
    /** Declare the actions **/
	virtual void ActionExample(IAiComponent& fsm) = 0;
};

struct AiState : public IAiState
{
	/** Actions **/
	virtual void ActionExample(IAiComponent& fsm);
};

struct ExampleState : public AiState
{	
	void ActionExample(IAiComponent& fsm) override;
};

class IAiComponent
{
public:
	ExampleState* StateExample;
    
	IAiComponent()
    {
        StateExample = new ExampleState();
    };

	virtual ~IAiComponent()
    {
        delete StateExample;    
	};

	virtual void update() = 0;

	/** Events**/
	virtual void EventExample() = 0;

	/** Events end **/

	virtual void setCurrentState(IAiState* currentState) = 0;

public: /** Variables **/
    IGameObject* gameObject;
};

class AiComponent : public IAiComponent
{
public:
	AiComponent(){};
	virtual ~AiComponent(){};

	virtual void update();

	virtual void EventExample(){ currentState->ActionExample(*this); };
	
	virtual void setCurrentState(IAiState* newState){ currentState = newState; };

protected:
	IAiState* currentState;
};

class NpcAiComponent : public AiComponent
{
public:
	NpcAiComponent(IGameObject& gameObject){};
	virtual ~NpcAiComponent(){};
};




#endif /* defined(__FranticAlien__AI_COMPONENT_H__) */
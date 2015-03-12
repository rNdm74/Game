#include "AiComponent.h"
#include "GameObject.h"

void AiState::ActionExample(IAiComponent& fsm)
{ 		
	fsm.setCurrentState( fsm.StateExample ); 
};


void ExampleState::ActionExample(IAiComponent& fsm) /** Override **/
{
	log("Example action overriden");
};

void AiComponent::update()
{
	void(IAiComponent:: *ptrs[])() =
	{
		&IAiComponent::EventExample,
	};
	
	int runningEvent = 0;// gameObject->events.top();

	(this->*ptrs[runningEvent])();
}


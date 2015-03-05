#include "FsmComponent.h"
#include "GameObject.h"

void FsmStates::ActionUp(IFsmComponent& fsm){ 
	fsm.setCurrentState( fsm.StateUp ); 
};
void FsmStates::ActionDown(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateDown); };
void FsmStates::ActionLeft(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateLeft); };
void FsmStates::ActionRight(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateRight); };
void FsmStates::ActionStop(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateStop); };



void PlayerFsmComponent::update(Node& node, IGameObject& gameObject)
{
	void(IFsmComponent:: *ptrs[])() =
	{
		&IFsmComponent::EventUp,
		&IFsmComponent::EventDown,
		&IFsmComponent::EventLeft,
		&IFsmComponent::EventRight,
		&IFsmComponent::EventStop
	};
	
	int index = gameObject.getCurrentState();

	(this->*ptrs[index])();	
}


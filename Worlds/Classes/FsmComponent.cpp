#include "FsmComponent.h"
#include "GameObject.h"

void FsmState::ActionUp(IFsmComponent& fsm){ fsm.setCurrentState( fsm.StateUp ); };
void FsmState::ActionDown(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateDown); };
void FsmState::ActionLeft(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateLeft); };
void FsmState::ActionRight(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateRight); };
void FsmState::ActionStop(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateStop); };

void UpState::ActionUp(IFsmComponent& fsm)
{
    AppGlobal::getInstance()->zoomOut();
    
    fsm.gameObject->setOnGround(false);
    fsm.gameObject->ClimbUp();
};

void DownState::ActionDown(IFsmComponent& fsm)
{
    AppGlobal::getInstance()->zoomOut();
    
    if(fsm.gameObject->getOnGround())
    {
        fsm.gameObject->Crouch();
    }
    else
    {
        fsm.gameObject->ClimbDown();
    }
};

void LeftState::ActionLeft(IFsmComponent& fsm)
{
    AppGlobal::getInstance()->zoomOut();
    fsm.gameObject->WalkLeft();
};

void RightState::ActionRight(IFsmComponent& fsm)
{
    AppGlobal::getInstance()->zoomOut();
    fsm.gameObject->WalkRight();
    fsm.gameObject->setIsMoving(true);
};

void StopState::ActionStop(IFsmComponent& fsm)
{
    AppGlobal::getInstance()->zoomIn();
    fsm.gameObject->Stop();
    
    /**  Wait so many seconds then change state to idle **/
    if (timeout > 100l /** Reached timeout period  **/)
    {
        /** Reset timeout period **/
        timeout = 0l;
        /** Change to idle state **/
        fsm.setCurrentState(fsm.StateIdle);
    }
    
    /** Increment the timeout period  **/
    timeout += 1l;
};

void IdleState::ActionStop(IFsmComponent& fsm)
{
    fsm.gameObject->Hurt();
};

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


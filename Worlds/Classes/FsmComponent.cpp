#include "FsmComponent.h"
#include "GameObject.h"

void FsmState::ActionUp(IFsmComponent& fsm)
{ 
	//
	fsm.gameObject->OnGround = false;		
	fsm.setCurrentState( fsm.StateUp ); 
};
void FsmState::ActionDown(IFsmComponent& fsm)
{
	fsm.gameObject->OnGround = false;
	fsm.setCurrentState(fsm.StateDown); 
};
void FsmState::ActionLeft(IFsmComponent& fsm)
{ 
	fsm.gameObject->HitWall();
	fsm.setCurrentState(fsm.StateLeft); 
};
void FsmState::ActionRight(IFsmComponent& fsm)
{ 
	fsm.gameObject->HitWall();
	fsm.setCurrentState(fsm.StateRight); 
};
void FsmState::ActionStop(IFsmComponent& fsm){ fsm.setCurrentState(fsm.StateStop); };
void FsmState::ActionJump(IFsmComponent& fsm)
{
	//
	fsm.gameObject->removeMovementEvent(EMovementEvent::Jump);
	// Tell the gameObject to jump
	fsm.gameObject->Jump();
	// Then change to the jump state
	fsm.setCurrentState(fsm.StateJump); 
};

void UpState::ActionUp(IFsmComponent& fsm) /** Override **/
{   
	Vec2 ladderOrigin = fsm.gameObject->LadderOrigin;
	fsm.gameObject->setPositionX(ladderOrigin.x);
    
	fsm.gameObject->Up();
};

void UpState::ActionStop(IFsmComponent& fsm) /** Override **/
{	
	fsm.gameObject->HitWall();
};

void DownState::ActionDown(IFsmComponent& fsm) /** Override **/
{    
	Vec2 ladderOrigin = fsm.gameObject->LadderOrigin;
	fsm.gameObject->setPositionX(ladderOrigin.x);

	fsm.gameObject->Down(); 	       
};

void DownState::ActionStop(IFsmComponent& fsm) /** Override **/
{
	fsm.gameObject->HitWall();
};

void LeftState::ActionLeft(IFsmComponent& fsm) /** Override **/
{
	fsm.gameObject->Left();
};

void RightState::ActionRight(IFsmComponent& fsm) /** Override **/
{
	fsm.gameObject->Right();
};

void StopState::ActionLeft(IFsmComponent& fsm) /** Override **/
{	
	fsm.setCurrentState(fsm.StateLeft);
};

void StopState::ActionRight(IFsmComponent& fsm) /** Override **/
{
	fsm.setCurrentState(fsm.StateRight);		
};

void StopState::ActionStop(IFsmComponent& fsm) /** Override **/
{	
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

void IdleState::ActionStop(IFsmComponent& fsm) /** Override **/
{
    fsm.gameObject->Idle();
};

void JumpState::ActionJump(IFsmComponent& fsm) /** Override **/
{
	log("Lets jump again!!");
};


FsmComponent::FsmComponent(IGameObject& gameObject)
{
	this->gameObject = &gameObject;
	this->currentState = StateStop;
};

void FsmComponent::update()
{
	void(IFsmComponent:: *ptrs[])() =
	{
		&IFsmComponent::EventUp,
		&IFsmComponent::EventDown,
		&IFsmComponent::EventLeft,
		&IFsmComponent::EventRight,
		&IFsmComponent::EventStop,
		&IFsmComponent::EventJump
	};

	int runningEvent = gameObject->getCurrentMovementEvent();

	(this->*ptrs[runningEvent])();
};

PlayerFsmComponent::PlayerFsmComponent(IGameObject& gameObject) : super(gameObject){};

NpcFsmComponent::NpcFsmComponent(IGameObject& gameObject) : super(gameObject){};
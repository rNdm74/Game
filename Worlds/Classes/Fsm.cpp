#include "Fsm.h"
#include <typeinfo>

#pragma region CheckCanClimb

void CheckCanClimb::CanClimbUp(IGameObjectFsm& fsm)
{	
	//log("Setting currentstate to the ClimbingUp state");
	fsm.setCurrentState(new ClimbingUp());	
	delete this;
};

void CheckCanClimb::CanClimbDown(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the ClimbingDown state");
	fsm.setCurrentState(new ClimbingDown());
	delete this;
};

void CheckCanClimb::CheckFailed(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the IsIdle state");
	fsm.setCurrentState(new IsIdle());
	delete this;
};

#pragma endregion CheckCanClimb

#pragma region ClimbingUp

void ClimbingUp::Up(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void ClimbingUp::Down(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void ClimbingUp::Left(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void ClimbingUp::Right(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void ClimbingUp::StopClimbing(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the Stopped state");
	fsm.setCurrentState(new Stopped());
	delete this;
};

void ClimbingUp::PreviousMap(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the LoadingPreviousMap state");
	fsm.setCurrentState(new LoadingPreviousMap());
	delete this;
};

#pragma endregion ClimbingUp

#pragma region ClimbingDown

void ClimbingDown::Up(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void ClimbingDown::Down(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void ClimbingDown::Left(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void ClimbingDown::Right(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void ClimbingDown::StopClimbing(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the Stopped state");
	fsm.setCurrentState(new Stopped());
	delete this;
};

void ClimbingDown::NextMap(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the LoadingPreviousMap state");
	fsm.setCurrentState(new LoadingNextMap());
	delete this;
};

#pragma endregion ClimbingDown

#pragma region CheckCanWalk

void CheckCanWalk::CanWalkLeft(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the WalkingLeft state");
	fsm.setCurrentState(new WalkingLeft());
	delete this;
};

void CheckCanWalk::CanWalkRight(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the WalkingRight state");
	fsm.setCurrentState(new WalkingRight());
	delete this;
};

void CheckCanWalk::CheckFailed(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the IsIdle state");
	fsm.setCurrentState(new IsIdle());
	delete this;
};

#pragma endregion CheckCanWalk

#pragma region WalkingLeft

void WalkingLeft::Up(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void WalkingLeft::Down(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void WalkingLeft::Left(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void WalkingLeft::Right(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void WalkingLeft::StopWalking(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the Stopped state");
	fsm.setCurrentState(new Stopped());
	delete this;
};

#pragma endregion WalkingLeft

#pragma region WalkingRight

void WalkingRight::Up(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void WalkingRight::Down(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void WalkingRight::Left(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void WalkingRight::Right(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void WalkingRight::StopWalking(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the Stopped state");
	fsm.setCurrentState(new Stopped());
	delete this;
};

#pragma endregion WalkingRight

#pragma region IsIdle

void IsIdle::Up(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void IsIdle::Down(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void IsIdle::Left(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void IsIdle::Right(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

#pragma endregion IsIdle

#pragma region Stopped

void Stopped::Up(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void Stopped::Down(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanClimb state");
	fsm.setCurrentState(new CheckCanClimb());
	delete this;
};

void Stopped::Left(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void Stopped::Right(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the CheckCanWalk state");
	fsm.setCurrentState(new CheckCanWalk());
	delete this;
};

void Stopped::StoppedTimeout(IGameObjectFsm& fsm)
{
	//log("Setting currentstate to the Stopped state");
	fsm.setCurrentState(new IsIdle());
	delete this;
};

#pragma endregion Stopped

#pragma region LoadingPreviousMap

void LoadingPreviousMap::Loaded(IGameObjectFsm& fsm)
{
	//log("Finished loading new map");	
	delete this;
};

#pragma endregion LoadingPreviousMap

#pragma region LoadingNextMap

void LoadingNextMap::Loaded(IGameObjectFsm& fsm)
{
	//log("Finished loading new map");
	delete this;
};

#pragma endregion LoadingNextMap

/**
* Ctor
* The initial state is set to IsIdle
*/
GameObjectFsm::GameObjectFsm()
{
	currentState = new IsIdle();
	log("Current state == IsIdle");
};

void GameObjectFsm::CheckCanWalkLeft()
{	
	currentState->Left(*this);
};

void GameObjectFsm::CheckCanWalkRight()
{
	currentState->Right(*this);
};

void GameObjectFsm::WalkLeft()
{
	static_cast<CheckCanWalk*>(currentState)->CanWalkLeft(*this);
};

void GameObjectFsm::WalkRight()
{
	static_cast<CheckCanWalk*>(currentState)->CanWalkRight(*this);
};

void GameObjectFsm::CheckCanClimbUp()
{
	static_cast<CheckCanClimb*>(currentState)->CanClimbUp(*this);
};

void GameObjectFsm::CheckCanClimbDown()
{
	static_cast<CheckCanClimb*>(currentState)->CanClimbDown(*this);
};

void GameObjectFsm::ClimbUp()
{
	currentState->Up(*this);
};

void GameObjectFsm::ClimbDown()
{
	currentState->Down(*this);
};

void GameObjectFsm::Stop()
{
	if (instanceof<WalkingLeft>(currentState))
	{
		static_cast<WalkingLeft*>(currentState)->StopWalking(*this);
	}
	else if (instanceof<WalkingRight>(currentState))
	{
		static_cast<WalkingRight*>(currentState)->StopWalking(*this);
	}
	else if (instanceof<ClimbingUp>(currentState))
	{
		static_cast<ClimbingUp*>(currentState)->StopClimbing(*this);
	}
	else if (instanceof<ClimbingDown>(currentState))
	{
		static_cast<ClimbingDown*>(currentState)->StopClimbing(*this);
	}
};

void GameObjectFsm::BecomeIdle()
{
	if (instanceof<CheckCanWalk>(currentState))
	{
		static_cast<CheckCanWalk*>(currentState)->CheckFailed(*this);
	}
	else if (instanceof<CheckCanClimb>(currentState))
	{
		static_cast<CheckCanClimb*>(currentState)->CheckFailed(*this);
	}
	else if (instanceof<Stopped>(currentState))
	{
		static_cast<Stopped*>(currentState)->StoppedTimeout(*this);
	}	
};

void GameObjectFsm::LoadNextMap()
{
};

void GameObjectFsm::LoadPreviousMap()
{
};

void GameObjectFsm::update()
{
	//currentState->update();
};

/**
* SetCurrentState(IGameObjectState* currentState)
* Sets the currentState to a new GameObjectState
*/
void GameObjectFsm::setCurrentState(IGameObjectState* currentState)
{
	this->currentState = currentState;
	
	log(currentState->type.c_str());
};
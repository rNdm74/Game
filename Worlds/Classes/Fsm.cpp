#include "Fsm.h"

/** Event Overrides**/
#pragma region Event Overrides

/**
*
*/
#pragma region GameObjectState

void GameObjectState::Up(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanClimb::getInstance());
};

void GameObjectState::Down(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanClimb::getInstance());
};

void GameObjectState::Left(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanWalk::getInstance());
};

void GameObjectState::Right(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanWalk::getInstance());
};

void GameObjectState::NextMap(IGameObjectFsm& fsm)
{
	log("LoadingNextMap State");
	fsm.setCurrentState(LoadingNextMap::getInstance());
};

void GameObjectState::PreviousMap(IGameObjectFsm& fsm)
{
	log("LoadingPreviousMap State");
	fsm.setCurrentState(LoadingPreviousMap::getInstance());
};

#pragma endregion GameObjectState

/**
*
*/
#pragma region CheckCanClimb

CheckCanClimb* CheckCanClimb::m_pInstance = NULL;

CheckCanClimb* CheckCanClimb::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new CheckCanClimb();
}

void CheckCanClimb::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void CheckCanClimb::Up(IGameObjectFsm& fsm)
{
	log("We are checking if we can climb up");

	if (false /** if check returns true state is now climbing up **/)
	{
		/** we can climb up **/
		this->canClimbUp(fsm);
	}
};

void CheckCanClimb::Down(IGameObjectFsm& fsm)
{
	log("We are checking if we can climb down");

	if (false /** if check returns true state is now climbing down **/)
	{
		/** we can climb down **/
		this->canClimbDown(fsm);
	}
};

void CheckCanClimb::Left(IGameObjectFsm& fsm)
{	
	fsm.setCurrentState(CheckCanWalk::getInstance());
};

void CheckCanClimb::Right(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanWalk::getInstance());
};

void CheckCanClimb::Stop(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(Stopped::getInstance());
};

/**
* Private functions
*/
void CheckCanClimb::canClimbUp(IGameObjectFsm& fsm)
{	
	fsm.setCurrentState(ClimbingUp::getInstance());
};

void CheckCanClimb::canClimbDown(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(ClimbingDown::getInstance());
};

#pragma endregion CheckCanClimb

/**
*
*/
#pragma region CheckCanWalk

CheckCanWalk* CheckCanWalk::m_pInstance = NULL;

CheckCanWalk* CheckCanWalk::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new CheckCanWalk();
}

void CheckCanWalk::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void CheckCanWalk::Up(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanClimb::getInstance());
};

void CheckCanWalk::Down(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(CheckCanClimb::getInstance());
};

void CheckCanWalk::Left(IGameObjectFsm& fsm)
{
	log("We are checking if we can walk left");

	if (false /** if check returns true state can walk left **/)
	{
		/** we can walk left **/
		this->canWalkLeft(fsm);
	}
};

void CheckCanWalk::Right(IGameObjectFsm& fsm)
{
	log("We are checking if we can walk right");

	if (false /** if check returns true state can walk right **/)
	{
		/** we can walk right **/
		this->canWalkRight(fsm);
	}
};

void CheckCanWalk::Stop(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(Stopped::getInstance());
};

/**
* Private functions
*/
void CheckCanWalk::canWalkLeft(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(WalkingLeft::getInstance());
};

void CheckCanWalk::canWalkRight(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(WalkingRight::getInstance());
};

#pragma endregion CheckCanWalk

/**
*
*/
#pragma region ClimbingUp

ClimbingUp* ClimbingUp::m_pInstance = NULL;

ClimbingUp* ClimbingUp::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new ClimbingUp();
}

void ClimbingUp::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void ClimbingUp::StopClimbing(IGameObjectFsm& fsm)
{	
	fsm.setCurrentState(Stopped::getInstance());
};

#pragma endregion ClimbingUp

/**
*
*/
#pragma region ClimbingDown

ClimbingDown* ClimbingDown::m_pInstance = NULL;

ClimbingDown* ClimbingDown::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new ClimbingDown();
}

void ClimbingDown::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void ClimbingDown::StopClimbing(IGameObjectFsm& fsm)
{	
	fsm.setCurrentState(Stopped::getInstance());
};

#pragma endregion ClimbingDown

/**
*
*/
#pragma region WalkingLeft

WalkingLeft* WalkingLeft::m_pInstance = NULL;

WalkingLeft* WalkingLeft::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new WalkingLeft();
}

void WalkingLeft::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void WalkingLeft::StopWalking(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(Stopped::getInstance());
};

#pragma endregion WalkingLeft

/**
*
*/
#pragma region WalkingRight

WalkingRight* WalkingRight::m_pInstance = NULL;

WalkingRight* WalkingRight::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new WalkingRight();
}

void WalkingRight::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void WalkingRight::StopWalking(IGameObjectFsm& fsm)
{	
	fsm.setCurrentState(Stopped::getInstance());
};

#pragma endregion WalkingRight

/**
*
*/
#pragma region IsIdle

IsIdle* IsIdle::m_pInstance = NULL;

IsIdle* IsIdle::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new IsIdle();
}

void IsIdle::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void IsIdle::Stop(IGameObjectFsm& fsm)
{
	/**   **/
};

#pragma endregion IsIdle

/**
*
*/
#pragma region Stopped

Stopped* Stopped::m_pInstance = NULL;

Stopped* Stopped::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new Stopped();
}

void Stopped::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void Stopped::Stop(IGameObjectFsm& fsm)
{	
	// Wait so many seconds then change state to idle
	if (true /** Reached timeout period  **/)
	{
		this->becomeIdle(fsm);
	}
};

/** Private functions **/
void Stopped::becomeIdle(IGameObjectFsm& fsm)
{
	log("Moving to Idle state");
	fsm.setCurrentState(IsIdle::getInstance());
};

#pragma endregion Stopped

/**
*
*/
#pragma region LoadingNextMap

LoadingNextMap* LoadingNextMap::m_pInstance = NULL;

LoadingNextMap* LoadingNextMap::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new LoadingNextMap();
}

void LoadingNextMap::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void LoadingNextMap::Up(IGameObjectFsm& fsm)
{
	log("Cannot move when loading next map");
};

void LoadingNextMap::Down(IGameObjectFsm& fsm)
{
	log("Cannot move when loading next map");
};

void LoadingNextMap::Left(IGameObjectFsm& fsm)
{
	log("Cannot move when loading next map");
};

void LoadingNextMap::Right(IGameObjectFsm& fsm)
{
	log("Cannot move when loading next map");
};

void LoadingNextMap::Loaded(IGameObjectFsm& fsm)
{
	log("Finished loading new map");
};

#pragma endregion LoadingNextMap

/**
*
*/
#pragma region LoadingPreviousMap

LoadingPreviousMap* LoadingPreviousMap::m_pInstance = NULL;

LoadingPreviousMap* LoadingPreviousMap::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new LoadingPreviousMap();
}

void LoadingPreviousMap::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void LoadingPreviousMap::Up(IGameObjectFsm& fsm)
{
	log("Cannot move when loading previous map");
};

void LoadingPreviousMap::Down(IGameObjectFsm& fsm)
{
	log("Cannot move when loading previous map");
};

void LoadingPreviousMap::Left(IGameObjectFsm& fsm)
{
	log("Cannot move when loading previous map");
};

void LoadingPreviousMap::Right(IGameObjectFsm& fsm)
{
	log("Cannot move when loading previous map");
};

void LoadingPreviousMap::Loaded(IGameObjectFsm& fsm)
{
	log("Finished loading new map");
};

#pragma endregion LoadingPreviousMap

#pragma endregion Event Overrides


/** GameObjectFSM Actions **/
#pragma region GameObjectFSM Actions

/**
*
*/
void GameObjectFsm::CheckCanClimbUp()
{
	currentState->Up(*this);
};

/**
*
*/
void GameObjectFsm::CheckCanClimbDown()
{
	currentState->Down(*this);
};

/**
* 
*/
void GameObjectFsm::CheckCanWalkLeft()
{	
	currentState->Left(*this);
};

/**
*
*/
void GameObjectFsm::CheckCanWalkRight()
{
	currentState->Right(*this);
};

/**
*
*/
void GameObjectFsm::Stop()
{
	currentState->Stop(*this);
};

/**
*
*/
void GameObjectFsm::LoadNextMap()
{
	currentState->NextMap(*this);
};

/**
*
*/
void GameObjectFsm::LoadPreviousMap()
{
	currentState->PreviousMap(*this);
};

#pragma endregion GameObjectFSM Actions


/**
* Ctor
* The initial state is set to IsIdle
*/
GameObjectFsm::GameObjectFsm()
{	
	log("Current state == IsIdle");
	currentState = IsIdle::getInstance();
};

/**
*
*/
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
};
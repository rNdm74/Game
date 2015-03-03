#include "AppGlobal.h"
#include "GameObject.h"
#include "GameObjectFsm.h"

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

void GameObjectState::Stop(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(Stopped::getInstance());
};

void GameObjectState::NextMap(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(LoadingNextMap::getInstance());
};

void GameObjectState::PreviousMap(IGameObjectFsm& fsm)
{
	fsm.setCurrentState(LoadingPreviousMap::getInstance());
};

#pragma endregion GameObjectState


#pragma region OnGround

OnGround* OnGround::m_pInstance = NULL;

OnGround* OnGround::getInstance()
{
	log("Check can climb state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new OnGround();
}

void OnGround::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void OnGround::Up(IGameObjectFsm& fsm)
{
	log("You can only jump when on the ground");
};

void OnGround::Down(IGameObjectFsm& fsm)
{
	log("Lets crouch");
};

void OnGround::Left(IGameObjectFsm& fsm)
{
	log("Check walk left");
};

void OnGround::Right(IGameObjectFsm& fsm)
{
	log("Check walk right");
};


#pragma endregion OnGround


/**
*
*/
#pragma region CheckCanClimb

CheckCanClimb* CheckCanClimb::m_pInstance = NULL;

CheckCanClimb* CheckCanClimb::getInstance()
{
	log("Check can climb state is being accessed");
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
	if (true /** if check returns true state is now climbing up **/)
	{
		/** we can climb up **/
		this->canClimbUp(fsm);
	}
};

void CheckCanClimb::Down(IGameObjectFsm& fsm)
{
	log("We are checking if we can climb down");
	if (true /** if check returns true state is now climbing down **/)
	{
		/** we can climb down **/
		this->canClimbDown(fsm);
	}
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
	log("Check can walk state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new CheckCanWalk();
}

void CheckCanWalk::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void CheckCanWalk::Left(IGameObjectFsm& fsm)
{
	log("We are checking if we can walk left");
	if (true /** if check returns true state can walk left **/)
	{
		fsm.gameObject->setVelocity(Vec2::ZERO);
		/** we can walk left **/
		this->canWalkLeft(fsm);
	}
};

void CheckCanWalk::Right(IGameObjectFsm& fsm)
{
	log("We are checking if we can walk right");
	if (true /** if check returns true state can walk right **/)
	{
		fsm.gameObject->setVelocity(Vec2::ZERO);
		/** we can walk right **/
		this->canWalkRight(fsm);
	}
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
	log("Climbing up state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new ClimbingUp();
}

void ClimbingUp::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void ClimbingUp::Up(IGameObjectFsm& fsm)
{
    AppGlobal::getInstance()->zoomOut();
    
	/** Tell the game object to climb up **/
	fsm.gameObject->ClimbUp();
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
	log("Climbing down state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new ClimbingDown();
}

void ClimbingDown::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void ClimbingDown::Down(IGameObjectFsm& fsm)
{
    AppGlobal::getInstance()->zoomOut();
    
    if(fsm.gameObject->isOnGround())
    {
        //fsm.gameObject->Crouch();
    }
    else
    {
        /** Tell the game object to climb down **/
        //fsm.gameObject->ClimbDown();
    }
    
    fsm.gameObject->ClimbDown();
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
	log("Walking left state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new WalkingLeft();
}

void WalkingLeft::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void WalkingLeft::Left(IGameObjectFsm& fsm)
{	
	AppGlobal::getInstance()->zoomOut();

	/** Tell the game object to walk left **/
	fsm.gameObject->WalkLeft();
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
	log("Walking right state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new WalkingRight();
}

void WalkingRight::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void WalkingRight::Right(IGameObjectFsm& fsm)
{	
	AppGlobal::getInstance()->zoomOut();

	/** Tell the game object to walk right **/
	fsm.gameObject->WalkRight();
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
	log("Is idle state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new IsIdle();
}

void IsIdle::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void IsIdle::Stop(IGameObjectFsm& fsm)
{
	/** Run idle animation  **/

	/** Zoom in effect **/
	AppGlobal::getInstance()->zoomIn();

	/** Tell game object to continue stopping **/
	fsm.gameObject->Idle();
};

#pragma endregion IsIdle

/**
*
*/
#pragma region Stopped

Stopped* Stopped::m_pInstance = NULL;

Stopped* Stopped::getInstance()
{
	log("Stopped state instance is accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new Stopped();
}

void Stopped::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void Stopped::Stop(IGameObjectFsm& fsm)
{
	/** Zoom in effect **/
	AppGlobal::getInstance()->zoomIn();
		
	/** Tell the game object to stop **/	
	fsm.gameObject->Stop();
			
	/**  Wait so many seconds then change state to idle **/
	if (fsm.timeout > 100l /** Reached timeout period  **/)
	{
		/** Reset timeout period **/
		fsm.timeout = 0l;
		/** Change to idle state **/
		this->becomeIdle(fsm);
	}

	/** Increment the timeout period  **/
	fsm.timeout += 1l;
	//log("%f", fsm.timeout);
};

void Stopped::update(IGameObjectFsm& fsm)
{	
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
	log("Loading next map state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new LoadingNextMap();
}

void LoadingNextMap::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

/**  **/
void LoadingNextMap::Loaded(IGameObjectFsm& fsm)
{
	log("Finished loading new map");
};

/** Overrides **/
void LoadingNextMap::Up(IGameObjectFsm& fsm)
{
	log("Up requested, loading previous map in progress");
};

void LoadingNextMap::Down(IGameObjectFsm& fsm)
{
	log("Down requested, loading previous map in progress");
};

void LoadingNextMap::Left(IGameObjectFsm& fsm)
{
	log("Left requested, loading previous map in progress");
};

void LoadingNextMap::Right(IGameObjectFsm& fsm)
{
	log("Right requested, loading previous map in progress");
};

void LoadingNextMap::Stop(IGameObjectFsm& fsm)
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
	log("Loading previous map state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new LoadingPreviousMap();
}

void LoadingPreviousMap::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

/**  **/
void LoadingPreviousMap::Loaded(IGameObjectFsm& fsm)
{
	log("Finished loading new map");
};

/** Overrides **/
void LoadingPreviousMap::Up(IGameObjectFsm& fsm)
{
	log("Up requested, loading previous map in progress");
};

void LoadingPreviousMap::Down(IGameObjectFsm& fsm)
{
	log("Down requested, loading previous map in progress");
};

void LoadingPreviousMap::Left(IGameObjectFsm& fsm)
{
	log("Left requested, loading previous map in progress");
};

void LoadingPreviousMap::Right(IGameObjectFsm& fsm)
{
	log("Right requested, loading previous map in progress");
};

void LoadingPreviousMap::Stop(IGameObjectFsm& fsm)
{
	log("Stop requested, loading previous map in progress");
};

#pragma endregion LoadingPreviousMap

#pragma endregion Event Overrides


/** GameObjectFSM Actions **/
#pragma region GameObjectFSM Actions

/**
* Public access function to the finite state machine to check if the game object can climb up
*/
void GameObjectFsm::CheckCanClimbUp()
{
	currentState->Up(*this);
};

/**
* Public access function to the finite state machine to check if the game object can climb down
*/
void GameObjectFsm::CheckCanClimbDown()
{
	currentState->Down(*this);
};

/**
* Public access function to the finite state machine to check if the game object can walk left
*/
void GameObjectFsm::CheckCanWalkLeft()
{	
	currentState->Left(*this);
};

/**
* Public access function to the finite state machine to check if the game object can walk right
*/
void GameObjectFsm::CheckCanWalkRight()
{
	currentState->Right(*this);
};

/**
* Public access function to the finite state machine to stop the game object
*/
void GameObjectFsm::Stop()
{
	currentState->Stop(*this);
};

/**
* Public access function to the finite state machine to load next map
*/
void GameObjectFsm::LoadNextMap()
{
	currentState->NextMap(*this);
};

/**
* Public access function to the finite state machine to load previous map
*/
void GameObjectFsm::LoadPreviousMap()
{
	currentState->PreviousMap(*this);
};

void GameObjectFsm::OnGround()
{
	currentState->OnGr
};

#pragma endregion GameObjectFSM Actions


/**
* Ctor
* The initial state is set to IsIdle
*/
GameObjectFsm::GameObjectFsm(IGameObject* gameObject)
{	
	this->gameObject = gameObject;

	log("Current state == IsIdle");
	
	timeout = 0.0f;
	timeoutBegin = false;

	currentState = IsIdle::getInstance();
};

/**
*
*/
void GameObjectFsm::update()
{
	currentState->update(*this);
};

/**
* SetCurrentState(IGameObjectState* currentState)
* Sets the currentState to a new GameObjectState
*/
void GameObjectFsm::setCurrentState(IGameObjectState* currentState)
{
	this->currentState = currentState;
};
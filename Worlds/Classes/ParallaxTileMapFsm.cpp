#include "AppGlobal.h"
#include "ParallaxTileMap.h"
#include "ParallaxTileMapFsm.h"

/** Event Overrides**/
#pragma region Event Overrides

/**
*
*/
#pragma region ParallaxTileMapState

void ParallaxTileMapState::ToCave(IParallaxTileMapFsm& fsm)
{
	//fsm.setCurrentState(Cave::getInstance());
};

void ParallaxTileMapState::ToPlanet(IParallaxTileMapFsm& fsm)
{
	//fsm.setCurrentState(Planet::getInstance());
};

#pragma endregion ParallaxTileMapState

/**
*
*/
#pragma region Cave

Cave* Cave::m_pInstance = NULL;

Cave* Cave::getInstance()
{
	log("Check can climb state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new Cave();
}

void Cave::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void Cave::ToPlanet(IParallaxTileMapFsm& fsm)
{
	log("Moving to planet");
};

#pragma endregion Cave

/**
*
*/
#pragma region Planet

Planet* Planet::m_pInstance = NULL;

Planet* Planet::getInstance()
{
	log("Check can walk state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new Planet();
}

void Planet::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void Planet::ToCave(IParallaxTileMapFsm& fsm)
{
	log("We moving into a cave");
};

#pragma endregion CheckCanWalk


#pragma endregion Event Overrides


/** ParallaxTileMapFsm Actions **/
#pragma region ParallaxTileMapFsm Actions

/**
* Public access function to the finite state machine to load the cave state
*/
void ParallaxTileMapFsm::LoadCave()
{
	currentState->ToCave(*this);
};

/**
* Public access function to the finite state machine to load the planet state
*/
void ParallaxTileMapFsm::LoadPlanet()
{
	currentState->ToPlanet(*this);
};

#pragma endregion ParallaxTileMapFsm Actions


/**
* Ctor
* The initial state is set to Planet
*/
ParallaxTileMapFsm::ParallaxTileMapFsm(IParallaxTileMap* parallaxTileMap)
{	
	this->parallaxTileMap = parallaxTileMap;

	currentState = Planet::getInstance();

	log("Current state == Planet");
};

/**
* SetCurrentState(IGameObjectState* currentState)
* Sets the currentState to a new GameObjectState
*/
void ParallaxTileMapFsm::setCurrentState(IParallaxTileMapState* currentState)
{
	this->currentState = currentState;
};
#include "AppGlobal.h"
#include "ParallaxTileMap.h"
#include "ParallaxTileMapFsm.h"

/** Event Overrides**/
#pragma region Event Overrides


/**
*
*/
#pragma region CaveActive

CaveActive* CaveActive::m_pInstance = NULL;

CaveActive* CaveActive::getInstance()
{
	log("Check can climb state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new CaveActive();
}

void CaveActive::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void CaveActive::ToPlanet(IParallaxTileMapFsm& fsm)
{
	log("Moving to planet");
	fsm.setCurrentState(PlanetActive::getInstance());
};

#pragma endregion CaveActive

/**
*
*/
#pragma region PlanetActive

PlanetActive* PlanetActive::m_pInstance = NULL;

PlanetActive* PlanetActive::getInstance()
{
	log("Planet active state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new PlanetActive();
}

void PlanetActive::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void PlanetActive::ToCave(IParallaxTileMapFsm& fsm)
{
	log("We moving into a cave");
	fsm.setCurrentState(CaveActive::getInstance());
};

#pragma endregion PlanetActive

/**
*
*/
#pragma region StationActiveState

StationActive* StationActive::m_pInstance = NULL;

StationActive* StationActive::getInstance()
{
	log("Space station active state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new StationActive();
}

void StationActive::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void StationActive::ToPlanet(IParallaxTileMapFsm& fsm)
{
	log("We moving to a planet");
	fsm.setCurrentState(PrisonActive::getInstance());
};

void StationActive::ToPrison(IParallaxTileMapFsm& fsm)
{
	log("We moving to the prison");
	fsm.setCurrentState(PrisonActive::getInstance());
};

void StationActive::ToBossBattle(IParallaxTileMapFsm& fsm)
{
	log("We moving to the boss battle");
	fsm.setCurrentState(BossBattleActive::getInstance());
};

#pragma endregion StationActiveState

/**
*
*/
#pragma region PrisonActiveState

PrisonActive* PrisonActive::m_pInstance = NULL;

PrisonActive* PrisonActive::getInstance()
{
	log("Space station active state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new PrisonActive();
}

void PrisonActive::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void PrisonActive::ToStation(IParallaxTileMapFsm& fsm)
{
	log("We moving into the station");
	fsm.setCurrentState(StationActive::getInstance());
};

#pragma endregion PrisonActiveState

/**
*
*/
#pragma region BossBattleActiveState

BossBattleActive* BossBattleActive::m_pInstance = NULL;

BossBattleActive* BossBattleActive::getInstance()
{
	log("Space station active state is being accessed");
	return m_pInstance ? m_pInstance : m_pInstance = new BossBattleActive();
}

void BossBattleActive::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
};

void BossBattleActive::ToStation(IParallaxTileMapFsm& fsm)
{
	log("We moving into the station");
	fsm.setCurrentState(StationActive::getInstance());
};

#pragma endregion BossBattleActiveState

#pragma endregion Event Overrides


/** PlanetFsm Events **/
#pragma region PlanetFsm Events

/**
* Public access function to the finite state machine to load the cave state
*/
void PlanetFsm::LoadCave()
{
	currentState->ToCave(*this);
};

/**
* Public access function to the finite state machine to load the planet state
*/
void PlanetFsm::LoadPlanet()
{
	currentState->ToPlanet(*this);
};

#pragma endregion PlanetFsm Events

/** StationFsm Events **/
#pragma region StationFsm Events

/**
* Public access function to the finite state machine to load the cave state
*/
void StationFsm::LoadPrison()
{
	currentState->ToPrison(*this);
};

/**
* Public access function to the finite state machine to load the planet state
*/
void StationFsm::LoadStation()
{
	currentState->ToStation(*this);
};

/**
* Public access function to the finite state machine to load the planet state
*/
void StationFsm::LoadBossBattle()
{
	currentState->ToBossBattle(*this);
};

#pragma endregion StationFsm Events

/**
* Ctor
* The initial state is set to Planet
*/
ParallaxTileMapFsm::ParallaxTileMapFsm(IParallaxTileMap* parallaxTileMap)
{	
	this->parallaxTileMap = parallaxTileMap;

	currentState = PlanetActive::getInstance();

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
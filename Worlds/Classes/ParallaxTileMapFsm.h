#ifndef __com_dotdat_World__ParallaxTileMapFsm_H__
#define __com_dotdat_World__ParallaxTileMapFsm_H__

#include "cocos2d.h"

using namespace cocos2d;

class IParallaxTileMapFsm;

/** IParallaxTileMapState Interface **/
class IParallaxTileMapState
{
public:
	virtual void ToCave(IParallaxTileMapFsm& fsm) = 0;
	virtual void ToPlanet(IParallaxTileMapFsm& fsm) = 0;
	virtual void ToPrison(IParallaxTileMapFsm& fsm) = 0;
	virtual void ToStation(IParallaxTileMapFsm& fsm) = 0;
	virtual void ToBossBattle(IParallaxTileMapFsm& fsm) = 0;

	virtual void destroyInstance() = 0;

protected:
	IParallaxTileMapState(){};
	virtual ~IParallaxTileMapState(){};
};

/** ParallaxTileMapState Base Class **/
class ParallaxTileMapState : public IParallaxTileMapState
{
public:	
	/** Events **/
	virtual void ToCave(IParallaxTileMapFsm& fsm){};
	virtual void ToPlanet(IParallaxTileMapFsm& fsm){};
	virtual void ToPrison(IParallaxTileMapFsm& fsm){};
	virtual void ToStation(IParallaxTileMapFsm& fsm){};
	virtual void ToBossBattle(IParallaxTileMapFsm& fsm){};

	virtual void destroyInstance(){};

protected:
	ParallaxTileMapState(){};
	ParallaxTileMapState(const ParallaxTileMapState&); // Prevent construction by copying
	ParallaxTileMapState& operator=(const ParallaxTileMapState&); // Prevent assignment
	virtual ~ParallaxTileMapState(){};
};

/** GameObjectState Inherited Classes **/

class CaveActive : public ParallaxTileMapState
{
public:
	static CaveActive* getInstance();

	/** Overrides **/
	virtual void ToPlanet(IParallaxTileMapFsm& fsm) override;

	virtual void destroyInstance();

private:
	CaveActive(){ log("CaveActive State"); };
	virtual ~CaveActive(){};

	static CaveActive* m_pInstance;
};

class PlanetActive : public ParallaxTileMapState
{
public:
	static PlanetActive* getInstance();

	/** Overrides **/
	virtual void ToCave(IParallaxTileMapFsm& fsm) override;

	virtual void destroyInstance();

private:
	PlanetActive(){ log("PlanetActive State"); };
	virtual ~PlanetActive(){};

	static PlanetActive* m_pInstance;
};

class StationActive : public ParallaxTileMapState
{
public:
	static StationActive* getInstance();
	virtual void destroyInstance();

	/** Overrides **/
	virtual void ToPlanet(IParallaxTileMapFsm& fsm) override;
	virtual void ToPrison(IParallaxTileMapFsm& fsm) override;
	virtual void ToBossBattle(IParallaxTileMapFsm& fsm) override;
		
private:
	StationActive(){ log("SpaceStationActive State"); };
	virtual ~StationActive(){};

	static StationActive* m_pInstance;
};

class PrisonActive : public ParallaxTileMapState
{
public:
	static PrisonActive* getInstance();
	virtual void destroyInstance();

	/** Overrides **/
	virtual void ToStation(IParallaxTileMapFsm& fsm) override;	

private:
	PrisonActive(){ log("PrisonActive State"); };
	virtual ~PrisonActive(){};

	static PrisonActive* m_pInstance;
};

class BossBattleActive : public ParallaxTileMapState
{
public:
	static BossBattleActive* getInstance();
	virtual void destroyInstance();

	/** Overrides **/
	virtual void ToStation(IParallaxTileMapFsm& fsm) override;

private:
	BossBattleActive(){ log("BossBattleActive State"); };
	virtual ~BossBattleActive(){};

	static BossBattleActive* m_pInstance;
};


#pragma endregion Inherited Classes

/** GameObjectState Interface **/
class IParallaxTileMapFsm
{
public:
	IParallaxTileMapFsm(){};
	virtual ~IParallaxTileMapFsm(){};

public:
	/** Actions - Public outside the machine **/
	virtual void LoadCave() = 0;
	virtual void LoadPlanet() = 0;
	virtual void LoadSpaceStation() = 0;
	virtual void LoadPrison() = 0;
	virtual void LoadBossBattle() = 0;

	/**  **/
	virtual void setCurrentState(IParallaxTileMapState* state) = 0;

	class IParallaxTileMap* parallaxTileMap;
};

class ParallaxTileMapFsm : public IParallaxTileMapFsm
{
public:
	ParallaxTileMapFsm(IParallaxTileMap* parallaxTileMap);
	virtual ~ParallaxTileMapFsm()
	{	
		CaveActive::getInstance()->destroyInstance();
		PlanetActive::getInstance()->destroyInstance();
		StationActive::getInstance()->destroyInstance();
		PrisonActive::getInstance()->destroyInstance();
		BossBattleActive::getInstance()->destroyInstance();
	};
		
	/** Setters **/
	virtual void setCurrentState(IParallaxTileMapState* currentState);

protected:
	class IParallaxTileMapState* currentState;
};

class PlanetFsm : public ParallaxTileMapFsm
{
public:
	/** Actions - Public outside the machine **/
	virtual void LoadCave();
	virtual void LoadPlanet();
};

class StationFsm : public ParallaxTileMapFsm
{
public:
	/** Actions - Public outside the machine **/
	virtual void LoadPrison();
	virtual void LoadStation();
	virtual void LoadBossBattle();
};

#endif /* defined(__com_dotdat_World__ParallaxTileMapFsm_H__) */

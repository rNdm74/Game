#ifndef __com_dotdat_World__FSM_H__
#define __com_dotdat_World__FSM_H__

#include "cocos2d.h"

using namespace cocos2d;

class IGameObjectFsm;

/** GameObjectState Interface **/
class IGameObjectState
{
public:
	virtual void Up(IGameObjectFsm& fsm) = 0;
	virtual void Down(IGameObjectFsm& fsm) = 0;
	virtual void Left(IGameObjectFsm& fsm) = 0;
	virtual void Right(IGameObjectFsm& fsm) = 0;
	virtual void Stop(IGameObjectFsm& fsm) = 0;
	virtual void StopWalking(IGameObjectFsm& fsm) = 0;
	virtual void StopClimbing(IGameObjectFsm& fsm) = 0;	
	virtual void PreviousMap(IGameObjectFsm& fsm) = 0;
	virtual void NextMap(IGameObjectFsm& fsm) = 0;
	virtual void Loaded(IGameObjectFsm& fsm) = 0;

	virtual void update(IGameObjectFsm& fsm) = 0;

	virtual void destroyInstance() = 0;

protected:
	IGameObjectState(){};
	virtual ~IGameObjectState(){};
};

/** GameObjectState Base Class **/
class GameObjectState : public IGameObjectState
{
public:	
	/** Events **/
	virtual void Up(IGameObjectFsm& fsm);
	virtual void Down(IGameObjectFsm& fsm);
	virtual void Left(IGameObjectFsm& fsm);
	virtual void Right(IGameObjectFsm& fsm);
	virtual void Stop(IGameObjectFsm& fsm);	
	virtual void StopWalking(IGameObjectFsm& fsm){ log("You requested the StopWalking event, not available from your current state!"); };
	virtual void StopClimbing(IGameObjectFsm& fsm){ log("You requested the StopClimbing event, not available from your current state!"); };		
	virtual void NextMap(IGameObjectFsm& fsm);
	virtual void PreviousMap(IGameObjectFsm& fsm);
	virtual void Loaded(IGameObjectFsm& fsm){ log("You requested the Loaded event, not available from your current state!"); };

	virtual void update(IGameObjectFsm& fsm){ /** log("Default update state"); **/ };

	virtual void destroyInstance(){};

protected:
	GameObjectState(){};
	GameObjectState(const GameObjectState&); // Prevent construction by copying
	GameObjectState& operator=(const GameObjectState&); // Prevent assignment
	virtual ~GameObjectState(){};
};

/** GameObjectState Inherited Classes **/
#pragma region GameObjectState 

class CheckCanClimb : public GameObjectState
{
public:
	static CheckCanClimb* getInstance();

	/** Overrides **/
	virtual void Up(IGameObjectFsm& fsm) override;
	virtual void Down(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	CheckCanClimb(){ log("CheckCanClimb State"); };
	virtual ~CheckCanClimb(){};

	virtual void canClimbUp(IGameObjectFsm& fsm);
	virtual void canClimbDown(IGameObjectFsm& fsm);	

	static CheckCanClimb* m_pInstance;
};

class CheckCanWalk : public GameObjectState
{
public:
	static CheckCanWalk* getInstance();

	/** Overrides **/
	virtual void Left(IGameObjectFsm& fsm) override;
	virtual void Right(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	CheckCanWalk(){ log("CheckCanWalk State"); };
	virtual ~CheckCanWalk(){};

	virtual void canWalkLeft(IGameObjectFsm& fsm);
	virtual void canWalkRight(IGameObjectFsm& fsm);

	static CheckCanWalk* m_pInstance;
};

class ClimbingUp : public GameObjectState
{
public:
	static ClimbingUp* getInstance();

	/** Overrides **/
	virtual void Up(IGameObjectFsm& fsm) override;
	virtual void StopClimbing(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	ClimbingUp(){ log("ClimbingUp State"); };
	virtual ~ClimbingUp(){};

	static ClimbingUp* m_pInstance;
};

class ClimbingDown : public GameObjectState
{
public:
	static ClimbingDown* getInstance();

	/** Overrides **/
	virtual void Down(IGameObjectFsm& fsm) override;
	virtual void StopClimbing(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	ClimbingDown(){ log("ClimbingDown State"); };
	virtual ~ClimbingDown(){};

	static ClimbingDown* m_pInstance;
};

class WalkingLeft : public GameObjectState
{
public:
	static WalkingLeft* getInstance();

	/** Overrides **/
	virtual void Left(IGameObjectFsm& fsm) override;
	
	virtual void StopWalking(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	WalkingLeft(){ log("WalkingLeft State"); };
	virtual ~WalkingLeft(){};

	static WalkingLeft* m_pInstance;
};

class WalkingRight : public GameObjectState
{
public:
	static WalkingRight* getInstance();

	/** Overrides **/
	virtual void Right(IGameObjectFsm& fsm) override;
	virtual void StopWalking(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	WalkingRight(){ log("WalkingRight State"); };
	virtual ~WalkingRight(){};

	static WalkingRight* m_pInstance;
};

class IsIdle : public GameObjectState
{	
public:
	static IsIdle* getInstance();

	/** Overrides **/
	virtual void Stop(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	IsIdle(){ log("IsIdle State"); };
	virtual ~IsIdle(){};

	static IsIdle* m_pInstance;
};

class Stopped : public GameObjectState
{
public:
	static Stopped* getInstance();

	/** Overrides **/
	virtual void Stop(IGameObjectFsm& fsm) override;
	virtual void update(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	Stopped(){ log("Stopped State"); };
	virtual ~Stopped(){};

	void becomeIdle(IGameObjectFsm& fsm);

	static Stopped* m_pInstance;
};

class LoadingNextMap : public GameObjectState
{
public:
	static LoadingNextMap* getInstance();

	/** Overrides **/
	virtual void Up(IGameObjectFsm& fsm) override;
	virtual void Down(IGameObjectFsm& fsm) override;
	virtual void Left(IGameObjectFsm& fsm) override;
	virtual void Right(IGameObjectFsm& fsm) override;
	virtual void Stop(IGameObjectFsm& fsm) override;
	virtual void Loaded(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	LoadingNextMap(){ log("LoadingNextMap State"); };
	virtual ~LoadingNextMap(){};
		
	static LoadingNextMap* m_pInstance;
};

class LoadingPreviousMap : public GameObjectState
{
public:
	static LoadingPreviousMap* getInstance();

	/** Overrides **/
	virtual void Up(IGameObjectFsm& fsm) override;
	virtual void Down(IGameObjectFsm& fsm) override;
	virtual void Left(IGameObjectFsm& fsm) override;
	virtual void Right(IGameObjectFsm& fsm) override;
	virtual void Stop(IGameObjectFsm& fsm) override;
	virtual void Loaded(IGameObjectFsm& fsm) override;
	virtual void destroyInstance();

private:
	LoadingPreviousMap(){ log("LoadingPreviousMap State"); };
	virtual ~LoadingPreviousMap(){};

	static LoadingPreviousMap* m_pInstance;
};

#pragma endregion Inherited Classes

/** GameObjectState Interface **/
class IGameObjectFsm
{
public:
	class IGameObject* gameObject;

	IGameObjectFsm(){};
	virtual ~IGameObjectFsm(){};

	/** Actions **/	
	virtual void CheckCanClimbUp() = 0;
	virtual void CheckCanClimbDown() = 0;
	virtual void CheckCanWalkLeft() = 0;
	virtual void CheckCanWalkRight() = 0;	
	virtual void Stop() = 0;
	virtual void LoadNextMap() = 0;
	virtual void LoadPreviousMap() = 0;

	/**  **/
	virtual void setCurrentState(IGameObjectState* state) = 0;
		
	bool timeoutBegin;
	float timeout;
};

class GameObjectFsm : public IGameObjectFsm
{
	class IGameObjectState* currentState;

public:
	GameObjectFsm(IGameObject* gameObject);
	virtual ~GameObjectFsm()
	{	
		CheckCanClimb::getInstance()->destroyInstance();
		CheckCanWalk::getInstance()->destroyInstance();
		ClimbingUp::getInstance()->destroyInstance();
		ClimbingDown::getInstance()->destroyInstance();
		WalkingLeft::getInstance()->destroyInstance();
		WalkingRight::getInstance()->destroyInstance();
		IsIdle::getInstance()->destroyInstance();
		Stopped::getInstance()->destroyInstance();
		LoadingNextMap::getInstance()->destroyInstance();
		LoadingPreviousMap::getInstance()->destroyInstance();
	};
		
	/** Actions **/	
	virtual void CheckCanClimbUp();
	virtual void CheckCanClimbDown();
	virtual void CheckCanWalkLeft();
	virtual void CheckCanWalkRight();		
	virtual void Stop();
	virtual void LoadNextMap();
	virtual void LoadPreviousMap();

	virtual void update();

	/** Setters **/
	virtual void setCurrentState(IGameObjectState* currentState);	
};

#endif /* defined(__com_dotdat_World__FSM_H__) */

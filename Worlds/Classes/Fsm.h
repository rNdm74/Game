#ifndef __com_dotdat_World__FSM_H__
#define __com_dotdat_World__FSM_H__

#include "cocos2d.h"

using namespace cocos2d;

class IGameObjectFsm;

class IGameObjectState
{
public:
	IGameObjectState(){};
	virtual ~IGameObjectState(){};

	virtual void Up(IGameObjectFsm& fsm) = 0;
	virtual void Down(IGameObjectFsm& fsm) = 0;
	virtual void Left(IGameObjectFsm& fsm) = 0;
	virtual void Right(IGameObjectFsm& fsm) = 0;

	std::string type;
};

class CheckCanClimb : public IGameObjectState
{
public:
	CheckCanClimb(){ type = "CheckCanClimb"; };
	virtual ~CheckCanClimb(){};
		
	/** Events **/
	virtual void Up(IGameObjectFsm& fsm){};
	virtual void Down(IGameObjectFsm& fsm){};
	virtual void Left(IGameObjectFsm& fsm){};
	virtual void Right(IGameObjectFsm& fsm){};

	void CanClimbUp(IGameObjectFsm& fsm);
	void CanClimbDown(IGameObjectFsm& fsm);
	void CheckFailed(IGameObjectFsm& fsm);	
};

class ClimbingUp : public IGameObjectState
{
public:
	ClimbingUp(){};
	virtual ~ClimbingUp(){};

	/** Events **/
	void Up(IGameObjectFsm& fsm) override;
	void Down(IGameObjectFsm& fsm) override;
	void Left(IGameObjectFsm& fsm) override;
	void Right(IGameObjectFsm& fsm) override;
	void StopClimbing(IGameObjectFsm& fsm);
	void PreviousMap(IGameObjectFsm& fsm);
};

class ClimbingDown : public IGameObjectState
{
public:
	ClimbingDown(){};
	virtual ~ClimbingDown(){};

	/** Events **/
	void Up(IGameObjectFsm& fsm) override;
	void Down(IGameObjectFsm& fsm) override;
	void Left(IGameObjectFsm& fsm) override;
	void Right(IGameObjectFsm& fsm) override;
	void StopClimbing(IGameObjectFsm& fsm);
	virtual void NextMap(IGameObjectFsm& fsm);
};

class CheckCanWalk : public IGameObjectState
{
public:
	CheckCanWalk(){ type = "CheckCanWalk"; };
	virtual ~CheckCanWalk(){};

	/** Events **/
	virtual void Up(IGameObjectFsm& fsm){};
	virtual void Down(IGameObjectFsm& fsm){};
	virtual void Left(IGameObjectFsm& fsm){};
	virtual void Right(IGameObjectFsm& fsm){};

	void CanWalkLeft(IGameObjectFsm& fsm);
	void CanWalkRight(IGameObjectFsm& fsm);
	void CheckFailed(IGameObjectFsm& fsm);
};

class WalkingLeft : public IGameObjectState
{
public:
	WalkingLeft(){};
	virtual ~WalkingLeft(){};

	/** Events **/
	void Up(IGameObjectFsm& fsm) override;
	void Down(IGameObjectFsm& fsm) override;
	void Left(IGameObjectFsm& fsm) override;
	void Right(IGameObjectFsm& fsm) override;
	void StopWalking(IGameObjectFsm& fsm);
};

class WalkingRight : public IGameObjectState
{
public:
	WalkingRight(){};
	virtual ~WalkingRight(){};

	/** Events **/
	void Up(IGameObjectFsm& fsm) override;
	void Down(IGameObjectFsm& fsm) override;
	void Left(IGameObjectFsm& fsm) override;
	void Right(IGameObjectFsm& fsm) override;
	void StopWalking(IGameObjectFsm& fsm);
};

class IsIdle : public IGameObjectState
{	
public:
	IsIdle(){};
	virtual ~IsIdle(){};

	/** Events **/
	void Up(IGameObjectFsm& fsm) override;
	void Down(IGameObjectFsm& fsm) override;
	void Left(IGameObjectFsm& fsm) override;
	void Right(IGameObjectFsm& fsm) override;
};

class Stopped : public IGameObjectState
{
public:
	Stopped(){};
	virtual ~Stopped(){};

	/** Events **/
	void Up(IGameObjectFsm& fsm) override;
	void Down(IGameObjectFsm& fsm) override;
	void Left(IGameObjectFsm& fsm) override;
	void Right(IGameObjectFsm& fsm) override;
	void StoppedTimeout(IGameObjectFsm& fsm);
};

class LoadingNextMap : public IGameObjectState
{
public:
	LoadingNextMap(){};
	virtual ~LoadingNextMap(){};

	/** Events **/
	virtual void Up(IGameObjectFsm& fsm){};
	virtual void Down(IGameObjectFsm& fsm){};
	virtual void Left(IGameObjectFsm& fsm){};
	virtual void Right(IGameObjectFsm& fsm){};
	void Loaded(IGameObjectFsm& fsm);
};

class LoadingPreviousMap : public IGameObjectState
{
public:
	LoadingPreviousMap(){};
	virtual ~LoadingPreviousMap(){};

	/** Events **/
	virtual void Up(IGameObjectFsm& fsm){};
	virtual void Down(IGameObjectFsm& fsm){};
	virtual void Left(IGameObjectFsm& fsm){};
	virtual void Right(IGameObjectFsm& fsm){};
	void Loaded(IGameObjectFsm& fsm);
};


class IGameObjectFsm
{
public:
	IGameObjectFsm(){};
	virtual ~IGameObjectFsm(){};

	/** Actions **/	
	virtual void CheckCanWalkLeft() = 0;
	virtual void CheckCanWalkRight() = 0;
	virtual void WalkLeft() = 0;
	virtual void WalkRight() = 0;
	virtual void CheckCanClimbUp() = 0;
	virtual void CheckCanClimbDown() = 0;
	virtual void ClimbUp() = 0;
	virtual void ClimbDown() = 0;
	virtual void Stop() = 0;
	virtual void BecomeIdle() = 0;
	virtual void LoadNextMap() = 0;
	virtual void LoadPreviousMap() = 0;

	virtual void setCurrentState(IGameObjectState* state) = 0;
};

class GameObjectFsm : public IGameObjectFsm
{
	class IGameObjectState* currentState;

public:
	GameObjectFsm();
	virtual ~GameObjectFsm(){};
		
	/** Actions **/	
	virtual void CheckCanWalkLeft();
	virtual void CheckCanWalkRight();
	virtual void WalkLeft();
	virtual void WalkRight();			
	virtual void CheckCanClimbUp();
	virtual void CheckCanClimbDown();
	virtual void ClimbUp();
	virtual void ClimbDown();
	virtual void Stop();
	virtual void BecomeIdle();
	virtual void LoadNextMap();
	virtual void LoadPreviousMap();

	virtual void update();

	/** Setters **/
	virtual void setCurrentState(IGameObjectState* currentState);
};

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return std::is_base_of<Base, T>::value;
};

#endif /* defined(__com_dotdat_World__FSM_H__) */

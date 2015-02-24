#ifndef  __FranticAlien__APP_GLOBAL_H_
#define  __FranticAlien__APP_GLOBAL_H_

#include "cocos2d.h"
#include "Constants.h"
#include "Fsm.h"

using namespace cocos2d;

struct GameStates {
	bool SPRINT;
	bool JUMP;
	bool ESCAPE;
	bool HUD;
	bool ENTER;
	bool UP;
	bool DOWN;
	bool LEFT;
	bool RIGHT;
	bool STOP;
};

enum EGameObjectState
{
	CheckCanClimbUp,
	CheckCanClimbDown,
	CheckCanWalkLeft,
	CheckCanWalkRight,	
	Stop,
	LoadNextMap,
	LoadPreviousMap
};

class  AppGlobal
{
public:
    bool IsGameSceneRunning;
    
	float scale;
	
    bool touchEvent;
    
	GameStates states;
		
	EGameObjectState gameObjectState;

	class ParallaxTileMap* activeMap;

	class IGameObject* player;
    
public:
    static AppGlobal* getInstance();

	void initMouseListener();
	void initKeyboardListener();
	void initTouchListener();
	void initPathFinderListener();
			    
	

	void addCursor(Layer& layer);

	void zoomIn();
	void zoomOut();	
		    
private:
	
    void setGameObjectState(Vec2 direction);
    
	KeyMatrix keyMatrix;

    AppGlobal();
	AppGlobal(const AppGlobal&); // Prevent construction by copying
	AppGlobal& operator=(const AppGlobal&); // Prevent assignment
	virtual ~AppGlobal(){};
	
    static AppGlobal* m_pInstance;    
};

//#define GETSCALEX ( AppGlobal::getInstance()->GetScaleX() )
//#define GETSCALEY ( AppGlobal::getInstance()->GetScaleY() )
//#define GETSCALEFACTOR ( AppGlobal::getInstance()->GetScaleFactor() )
//
//#define SCALEX(p) ( (p) * GETSCALEX )
//#define SCALEY(p) ( (p) * GETSCALEY )
//#define SCALEPOSITION(x,y) ( Vec2( GETSCALEX * (x) * GETSCALEFACTOR, GETSCALEY * (y) * GETSCALEFACTOR ) )
//#define SCALEFONT(p) ( (p) * SCALEY * GETSCALEFACTOR )
//
//#define SCALENODE_XY(n) n->setScaleX(GETSCALEX); n->setScaleY(GETSCALEY)
//
//#define SCALENODE_Y(n) n->setScale(GETSCALEY)


#endif // _APP_GLOBAL_H_


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
    int ActiveLevel;
    Point StartPosition;

	Vec2 cursorDownLocation;
	Vec2 cursorLocation;
    Vec2 cursorDelta;
	Vec2 cursorMove;
	bool mouseUp;
	bool mouseDown;
	float scale;	

	bool mouseToggle;

	bool leftMouseButton;
	bool rightMouseButton;
	
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
			    
	float getRandom(float begin, float end);
	void addCursor(Layer& layer);

    std::string GetActiveLevel() { return std::to_string(ActiveLevel); };
		    
private:
	
    void setGameObjectState(Vec2 direction);
    
	KeyMatrix keyMatrix;

    AppGlobal();
    virtual ~AppGlobal();
	
    static AppGlobal* m_pInstance;    
};

#define RAND(begin, end)  ( AppGlobal::getInstance()->getRandom( (begin), (end) ) )

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


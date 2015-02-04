#ifndef  __FranticAlien__APP_GLOBAL_H_
#define  __FranticAlien__APP_GLOBAL_H_

#include "cocos2d.h"
#include "Constants.h"

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

class  AppGlobal
{
public:
    bool IsGameSceneRunning;
    int ActiveLevel;
    Point StartPosition;

	Vec2 cursorMove;
	bool mouseUp;
	bool mouseDown;
	float scale;	

	Vec2 startLocation;
	Vec2 endLocation;

	//EState state;
	//EState previousState;

	GameStates states;
    
public:
    static AppGlobal* getInstance();

	void initMouseListener();
	void initKeyboardListener();
	void initTouchListener();
			    
	float getRandom(float begin, float end);
	void addCursor(Layer& layer);

    std::string GetActiveLevel() { return std::to_string(ActiveLevel); };
    
private:
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


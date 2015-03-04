#ifndef  __FranticAlien__APP_GLOBAL_H_
#define  __FranticAlien__APP_GLOBAL_H_

#include "cocos2d.h"
#include "Constants.h"

using namespace cocos2d;

class AppGlobal
{
public:
	EGameObjectState gameObjectState;

	ActiveTileMap activeMap;
	
	class IGameObject* player;
    
public:
    static AppGlobal* getInstance();

	/** Init functions **/
	void initMouseListener();
	void initKeyboardListener();
	void initTouchListener();
	void initPathFinderListener();
		        
	void addCursor(Layer& layer);

	void zoomIn();
	void zoomOut();	

	float getScale();

	bool getIsGameSceneRunning() { return _isGameSceneRunning; };
	void setIsGameSceneRunning(bool isRunning) { _isGameSceneRunning = isRunning; };
		    
private:
    AppGlobal();
	AppGlobal(const AppGlobal&); // Prevent construction by copying
	AppGlobal& operator=(const AppGlobal&); // Prevent assignment
	virtual ~AppGlobal(){};
	
	/** **/
	void setGameObjectState(Vec2 direction);

    static AppGlobal* m_pInstance;  

	bool _isGameSceneRunning;
	bool _touchEvent;

	float _scaleFactor;
	float _zoomFactor;
	
	KeyMatrix _keyMatrix;
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


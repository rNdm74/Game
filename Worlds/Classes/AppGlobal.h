#ifndef  __FranticAlien__APP_GLOBAL_H_
#define  __FranticAlien__APP_GLOBAL_H_

#include "cocos2d.h"
#include "Constants.h"
#include "Planet.h"

using namespace cocos2d;

class IGameObject;

class AppGlobal
{ 
public:
    static AppGlobal* getInstance();

	/** Init functions **/
	void initMouseListener();
	void initKeyboardListener();
	void initTouchListener();
	void initPathFinderListener();
	void initControllerListener();
	
//	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void addCursor(Layer& layer);

	void zoomIn();
	void zoomOut();	 

	float getScale();

	bool getIsGameSceneRunning() { return _isGameSceneRunning; };
	void setIsGameSceneRunning(bool isRunning) { _isGameSceneRunning = isRunning; };
	
public: /** Variables **/		
	IGameObject* PlayerInstance;	
	IGameObject* SelectedNpc;

	IPlanet* PlanetInstance;
    Vec2 MovePoint;
    
private:
    AppGlobal();
	AppGlobal(const AppGlobal&); // Prevent construction by copying
	AppGlobal& operator=(const AppGlobal&); // Prevent assignment
	virtual ~AppGlobal(){};	
    static AppGlobal* m_pInstance;  

private: /** Variables **/
	bool _isGameSceneRunning;
	bool _touchEvent;

	float _scaleFactor;
	float _zoomFactor;	
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


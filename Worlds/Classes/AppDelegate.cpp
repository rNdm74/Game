#include "AppDelegate.h"
#include "AppGlobal.h"
#include "AppResources.h"
#include "SplashScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
	
	auto glview = director->getOpenGLView();
    
	if(!glview) {
        glview = GLViewImpl::create("Worlds");
		//glview = GLViewImpl::createWithRect("Worlds", Rect(0,0, 480,320), 1.0f);
		//glview = GLViewImpl::createWithFullScreen("Worlds");
   		//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // Only disable cursor on a desktop computer
        static_cast<GLViewImpl*>(glview)->setCursorVisible(false);
#endif
		director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	// set directors projection
	director->setProjection(kCCDirectorProjection2D);

	//
	glview->setDesignResolutionSize(920, 640, ResolutionPolicy::NO_BORDER);

	// create global resources manager
	auto appResources = AppResources::getInstance();
	appResources->initLoad();

	// create global area for game
	auto appGlobal = AppGlobal::getInstance();
	appGlobal->setIsGameSceneRunning(false);
	appGlobal->initMouseListener();
	appGlobal->initKeyboardListener();
	appGlobal->initTouchListener();

    // create a scene. it's an autorelease object
    auto scene = SplashScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

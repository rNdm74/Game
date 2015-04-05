#include "AppDelegate.h"
#include "Constants.h"
#include "Scenes.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
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
    
	if(!glview)
	{
		glview = GLViewImpl::create(APP_NAME);
		//glview = GLViewImpl::createWithRect(APP_NAME, Rect(0, 0, 2560, 1440), 1.0f);
		//glview = GLViewImpl::createWithRect(APP_NAME, Rect(0, 0, 1920, 1080), 1.0f);
		//glview = GLViewImpl::createWithRect(APP_NAME, Rect(0, 0, 480, 320), 1.0f);
        
		director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


	auto screenSize = glview->getFrameSize();
	auto designSize = cocos2d::Size(920, 640);
	
	std::vector<std::string> searchPaths;

	if (screenSize.height <= SCREEN_HEIGHT_SD) 
	{
		searchPaths.push_back("SD");
		director->setContentScaleFactor(SCREEN_HEIGHT_SD / designSize.height);
		cocos2d::log("Set SD Design Res");
	}
	else if (screenSize.height <= SCREEN_HEIGHT_HD) 
	{
		searchPaths.push_back("HD");
		director->setContentScaleFactor(SCREEN_HEIGHT_HD / designSize.height);
		cocos2d::log("Set HD Design Res");
	}
	else 
	{
		searchPaths.push_back("RETINA");
		director->setContentScaleFactor(SCREEN_HEIGHT_RETINA / designSize.height);
		cocos2d::log("Set RETINA Design Res");
	}
	
	FileUtils::getInstance()->setSearchPaths(searchPaths);

	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);
	
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

#include "AppDelegate.h"
#include "Constants.h"
#include "Scenes.h"

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
    
	if(!glview)
	{
		glview = GLViewImpl::create(APP_NAME);
		//glview = GLViewImpl::createWithRect(APP_NAME, Rect(0, 0, 1920, 1080), 1.0f);
		
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    //
	//glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);


	auto fileUtils = FileUtils::getInstance();
	auto screenSize = glview->getFrameSize();
	std::vector<std::string> resDirOrders;
	Vec2 designResolution = Vec2::ZERO;
	
	// check which assets the devices requires
	if (2048 == screenSize.width || 2048 == screenSize.height || screenSize.width >= 1920 || screenSize.height >= 1920)
	{
		resDirOrders.push_back("ipadhd");
		resDirOrders.push_back("ipad");
		resDirOrders.push_back("iphonehd5");
		resDirOrders.push_back("iphonehd");
		resDirOrders.push_back("iphone");

		designResolution = Vec2(2048, 1536);
	}
	else if (1024 == screenSize.width || 1024 == screenSize.height)
	{
		resDirOrders.push_back("ipad");
		resDirOrders.push_back("iphonehd5");
		resDirOrders.push_back("iphonehd");
		resDirOrders.push_back("iphone");

		designResolution = Vec2(1024, 768);
	}
	else if (1136 == screenSize.width || 1136 == screenSize.height)
	{
		resDirOrders.push_back("iphonehd5");
		resDirOrders.push_back("iphonehd");
		resDirOrders.push_back("iphone");

		designResolution = Vec2(1136, 640);
	}
	else if (960 == screenSize.width || 960 == screenSize.height)
	{
		//resDirOrders.push_back("iphonehd");
		resDirOrders.push_back("iphone");

		designResolution = Vec2(960, 640);
	}
	else
	{
		resDirOrders.push_back("iphone");

		designResolution = Vec2(480, 320);
	}

	fileUtils->setSearchPaths(resDirOrders);

	// Configure design resolution for game
	glview->setDesignResolutionSize(designResolution.x, designResolution.y, ResolutionPolicy::NO_BORDER);

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

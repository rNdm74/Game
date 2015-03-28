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
		//glview = GLViewImpl::createWithRect(APP_NAME, Rect(0, 0, 1920, 1080), 1.0f);
		
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	auto fileUtils = FileUtils::getInstance();
	auto screenSize = glview->getFrameSize();
	std::vector<std::string> resDirOrders;
    Size designSize = Size(2048, 1536);
	
	// check which assets the devices requires
//	if (2048 == screenSize.width ||
//        1920 == screenSize.width ||
//        2048 == screenSize.height ||
//        1920 == screenSize.height)
//	{
//		resDirOrders.push_back("ipadhd");
//		resDirOrders.push_back("ipad");
//		resDirOrders.push_back("iphonehd5");
//		resDirOrders.push_back("iphonehd");
//		resDirOrders.push_back("iphone");
//
//		designSize = Size(2048, 1536);
//	}
//	else if (1024 == screenSize.width || 1024 == screenSize.height)
//	{
//		resDirOrders.push_back("ipad");
//		resDirOrders.push_back("iphonehd5");
//		resDirOrders.push_back("iphonehd");
//		resDirOrders.push_back("iphone");
//
//		designSize = Size(1024, 768);
//	}
//	else if (1136 == screenSize.width || 1136 == screenSize.height)
//	{
//		resDirOrders.push_back("iphonehd5");
//		resDirOrders.push_back("iphonehd");
//		resDirOrders.push_back("iphone");
//
//		designSize = Size(1136, 640);
//	}
//	else if (960 == screenSize.width || 960 == screenSize.height)
//	{
//		//resDirOrders.push_back("iphonehd");
//		resDirOrders.push_back("iphone");
//
//		designSize = Size(960, 640);
//	}
//	else
//	{
//		resDirOrders.push_back("iphone");
//
//		designSize = Size(480, 320);
//	}

    
    // Configure design resolution for game
    glview->setDesignResolutionSize
    (
     designSize.width,
     designSize.height,
     ResolutionPolicy::NO_BORDER
     );
    
    if (screenSize.height > 768)
    {
        resDirOrders.push_back("ipadhd");
    }
    else if (screenSize.height < 768 && screenSize.height > 320)
    {
        resDirOrders.push_back("iphone");
    }
    else
    {
        resDirOrders.push_back("iphone");
    }
    
	fileUtils->setSearchPaths(resDirOrders);

    for(auto i : resDirOrders)
    {
        log(i.c_str());
    }
    
    float contentScaleFactor = screenSize.height / designSize.height;
    log("scalefactor: %f", contentScaleFactor);
    director->setContentScaleFactor(contentScaleFactor);
    
    
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

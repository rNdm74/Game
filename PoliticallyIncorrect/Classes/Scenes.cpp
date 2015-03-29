#include "Scenes.h"
#include "Components.h"

USING_NS_CC;

/**
* IScene
*/

void IScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
};

/**
* IScene END
*/

/**
* SplashScene
*/
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // create and initialize a label
    
    auto label = LabelTTF::create(APP_NAME, "Marker Felt", 50);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, (origin.y + visibleSize.height / 2) + (label->getContentSize().height) / 2));
    
    label->setOpacity(0);

    // add the label as a child to this layer
    this->addChild(label, 1);
    	
    // fade in / out action   
    Vector<cocos2d::FiniteTimeAction*> fadeActions(3);
	
    fadeActions.insert(0, CallFunc::create(CC_CALLBACK_0(SplashScene::loadMenuScene, this)));
    fadeActions.insert(0, FadeOut::create(0.5));
    fadeActions.insert(0, FadeIn::create(0.5));
        
    label->runAction(Sequence::create(fadeActions));
		    
    return true;
}

void SplashScene::loadMenuScene()
{        
    //auto menuScene = MenuScene::createScene();
    //Director::getInstance()->replaceScene(menuScene);

	auto gameplayScene = GameplayScene::createScene();
	Director::getInstance()->replaceScene(gameplayScene);
}

/**
* SplashScene END
*/

/**
* MenuScene
*/

bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// create and initialize a menu items
	auto menu_item_1 = MenuItemFont::create("Continue", CC_CALLBACK_1(MenuScene::Continue, this));
	auto menu_item_2 = MenuItemFont::create("Play", CC_CALLBACK_1(MenuScene::Play, this));
	auto menu_item_3 = MenuItemFont::create("Settings", CC_CALLBACK_1(MenuScene::Settings, this));
	auto menu_item_4 = MenuItemFont::create("Quit", CC_CALLBACK_1(MenuScene::Quit, this));

	
	menu = Menu::create(menu_item_1, menu_item_3, menu_item_4, NULL);
	menu->alignItemsVerticallyWithPadding(25.0f);
	this->addChild(menu);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(MenuScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
		
	return true;
}

void MenuScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
	log("Key with keycode %d released", keyCode);
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d released", keyCode);
}

void MenuScene::Continue(Ref* sender)
{
	log("Continue");
}

void MenuScene::Play(Ref* sender)
{
	log("Play");
}

void MenuScene::Settings(Ref* sender)
{
	log("Settings");
}

void MenuScene::Quit(Ref* sender)
{
	exit(0);
}

/**
* MenuScene END
*/


/**
* GameplayScene
*/

// on "init" you need to initialize your instance
bool GameplayScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
		return false;

	// Create the tilemap
	ExtendedTMXTiledMap* bedroom = ExtendedTMXTiledMap::create("bedroom.tmx");
	//TMXTiledMap* bedroom = TMXTiledMap::create("bedroom.tmx");
	this->addChild(bedroom);
			
	// Scale must be set on all objects added to the game
	bedroom->setScale(Director::getInstance()->getContentScaleFactor());

	for (auto child : bedroom->getChildren())
	{
		//static_cast<Sprite*>(child)->getTexture()->setAntiAliasTexParameters();
	}

	/** **/
	this->scheduleUpdateWithPriority(42);

	return true;
}

void GameplayScene::Pause(Ref* sender)
{
	Scene* menuScene = MenuScene::createScene();
	Director::getInstance()->pushScene(menuScene);
}

void GameplayScene::GameplaySceneFinished(Ref* sender)
{
	//auto gameplayFinishedScene = GameplayFinishedScene::createScene();
	//Director::getInstance()->replaceScene(gameplayFinishedScene);
}

void GameplayScene::update(float delta)
{
	
}

void GameplayScene::actionFinished()
{
}


/**
* GameplayScene END
*/
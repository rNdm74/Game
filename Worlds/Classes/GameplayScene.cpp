#include "AppGlobal.h"
#include "Constants.h"
#include "GameplayScene.h"
#include "GameplayFinishedScene.h"
#include "MenuScene.h"
#include "Planet.h"
#include "PlanetFactory.h"
#include "GameObject.h"
#include "GameObjectFactory.h"

Scene* GameplayScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
	
    // 'layer' is an autorelease object
    auto layer = GameplayScene::create();
	layer->setTag(KTagSceneLayer);

	// add cursor
	AppGlobal::getInstance()->addCursor(*layer);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameplayScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
        return false;

    planet = PlanetFactory::create("Snow");
	this->addChild(planet);
	
    ValueMap landingSite = planet->getLandingSite();
    
	player = GameObjectFactory::create("Player", landingSite);
	planet->addPlayer(player);

	AppGlobal::getInstance()->PlayerInstance = player;
	AppGlobal::getInstance()->PlanetInstance = planet;
	
	//
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
	auto gameplayFinishedScene = GameplayFinishedScene::createScene();
	Director::getInstance()->replaceScene(gameplayFinishedScene);
}

void GameplayScene::update(float delta)
{
	planet->update(delta);
}

void GameplayScene::actionFinished()
{	
}

void GameplayScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

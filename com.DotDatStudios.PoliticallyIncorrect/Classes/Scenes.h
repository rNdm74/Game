#ifndef __PoliticallyIncorrect__Scenes_H__
#define __PoliticallyIncorrect__Scenes_H__

#include "cocos2d.h"
#include "Constants.h"


class IScene : public cocos2d::Layer
{
public:
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
};


class SplashScene : public IScene
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(SplashScene);

	INIT_FUNC(SplashScene);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(); 
    
private:
    void loadMenuScene();
};


class MenuScene : public IScene
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

	INIT_FUNC(MenuScene);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
		
private:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	void Continue(cocos2d::Ref* sender);
	void Play(cocos2d::Ref* sender);
	void Settings(cocos2d::Ref* sender);
	void Quit(cocos2d::Ref* sender);

private:
	cocos2d::Menu* menu;
};


class GameplayScene : public IScene
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(GameplayScene);

	INIT_FUNC(GameplayScene);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void update(float) override;

private:
	void actionFinished();

	void Pause(Ref* sender);
	void GameplaySceneFinished(Ref* sender);
};

#endif // __PoliticallyIncorrect__Scenes_H__

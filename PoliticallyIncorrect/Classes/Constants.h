#ifndef __PoliticallyIncorrect__Constants_H__
#define __PoliticallyIncorrect__Constants_H__

#include <array>
#include <string>

#define SCREEN_WIDTH_SD     627
#define SCREEN_HEIGHT_SD    396
#define SCREEN_WIDTH_HD     1254
#define SCREEN_HEIGHT_HD    792
#define SCREEN_WIDTH_RETINA 2508
#define SCREEN_HEIGHT_RETINA 1584

#define GAME_WIDTH_SD       528
#define GAME_HEIGHT_SD      360
#define GAME_WIDTH_HD       1056
#define GAME_HEIGHT_HD      704
#define GAME_WIDTH_RETINA   2112
#define GAME_HEIGHT_RETINA  1408

#define TAG_GAMEOBJECT_DOOR	 0
#define TAG_GAMEOBJECT_LIGHT 1

static const std::string APP_NAME = "POLITICALLY INCORRECT";

/**
* define a create function for a specific type, such as Layer
* @param \__TYPE__  class type to add create(), such as Layer
*/
#define INIT_FUNC(__TYPE__) \
static cocos2d::Scene* createScene() \
{ \
	auto scene = cocos2d::Scene::create(); \
	auto layer = __TYPE__::create(); \
	scene->addChild(layer); \
	return scene; \
}

#endif //__PoliticallyIncorrect__Constants_H__

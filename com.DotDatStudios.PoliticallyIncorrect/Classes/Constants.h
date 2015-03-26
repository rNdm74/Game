#ifndef __PoliticallyIncorrect__Constants_H__
#define __PoliticallyIncorrect__Constants_H__

#include <array>
#include <string>

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

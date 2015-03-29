#ifndef __PoliticallyIncorrect__Components_H__
#define __PoliticallyIncorrect__Components_H__

#include "cocos2d.h"
#include "Constants.h"

class ExtendedTMXTiledMap : public cocos2d::TMXTiledMap
{
public:
	static ExtendedTMXTiledMap* create(const std::string& tmxFile);

	ExtendedTMXTiledMap();
	virtual ~ExtendedTMXTiledMap(){};

	virtual void update(float delta);

	virtual void setPositionOnPlayer(cocos2d::Rect collisionBox);
		
	virtual void initGameObjects();
	virtual bool initGameObject(std::string className, cocos2d::ValueMap& properties);

private:
	int _objectCount;

	cocos2d::DrawNode* _debugLayer;
};

#endif // __PoliticallyIncorrect__Components_H__

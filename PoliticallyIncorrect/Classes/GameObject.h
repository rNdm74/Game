#ifndef __PoliticallyIncorrect__GameObject_H__
#define __PoliticallyIncorrect__GameObject_H__

#include "cocos2d.h"
#include "Constants.h"

/*
* A gameobject in the game will be a door that the player will interact with
* The player character
* An NPC
* .... Others which still need to be finalized
*/

class IGameObject : public cocos2d::Sprite
{
public:
	IGameObject(){};
	virtual ~IGameObject(){};
	virtual void update(float delta) = 0;

	virtual cocos2d::Size getObjectSize() = 0;
};

class GameObject : public IGameObject
{
public:
	CREATE_FUNC_GAMEOBJECT(GameObject);

	GameObject(cocos2d::ValueMap& properties);
	virtual ~GameObject(){};
	virtual void update(float delta);

	virtual cocos2d::Size getObjectSize() { return _objectSize; };
	virtual void setObjectSize(cocos2d::Size objectSize) { _objectSize = objectSize; };

protected:
	cocos2d::Size _objectSize;
	cocos2d::ValueMap _properties;
};

#endif // __PoliticallyIncorrect__GameObject_H__



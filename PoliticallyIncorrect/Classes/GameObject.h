#ifndef __PoliticallyIncorrect__GameObject_H__
#define __PoliticallyIncorrect__GameObject_H__

#include "cocos2d.h"
#include "Constants.h"

class IGameObject : public cocos2d::Node
{
public:
	IGameObject(){};
	virtual ~IGameObject(){};
	virtual void update(float delta) = 0;

	//virtual cocos2d::Vec2 getPosition() = 0;
	//virtual void setPosition(cocos2d::Vec2 position) = 0;

};

class GameObjectFactory
{
public:
	static IGameObject* create(std::string className, cocos2d::ValueMap& properties);
};

class GameObject : public IGameObject
{
public:
	GameObject(){};
    virtual ~GameObject(){};
    virtual void update(float delta);

protected:
	cocos2d::ValueMap _properties;
	cocos2d::Sprite* _sprite;
};

/** The player character **/
class Player : public IGameObject
{};

/** All other characters in the game  **/
class Npc : public IGameObject
{};

class Pedestrian : public Npc
{};

class Light : public GameObject
{
public:
	static Light* create(cocos2d::ValueMap& properties);

	Light(cocos2d::ValueMap& properties);
	virtual ~Light(){};

	virtual void update(float delta){};
};

/****/
class Door : public GameObject
{
public:
	static Door* create(cocos2d::ValueMap& properties);

	Door(cocos2d::ValueMap& properties);
	virtual ~Door(){};

	virtual void update(float delta){};	
};

/*
 * A gameobject in the game will be a door that the player will interact with
 * The player character
 * An NPC
 * .... Others which still need to be finalized
 */

#endif // __PoliticallyIncorrect__GameObject_H__

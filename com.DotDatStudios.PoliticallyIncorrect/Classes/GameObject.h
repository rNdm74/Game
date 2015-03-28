#ifndef __PoliticallyIncorrect__GameObject_H__
#define __PoliticallyIncorrect__GameObject_H__

#include "cocos2d.h"
#include "Constants.h"


class IGameObject
{
public:
    IGameObject(){};
    virtual ~IGameObject(){};
    
    virtual void update(float delta) = 0;
};


class GameObject : public IGameObject
{
public:
    GameObject(){};
    virtual ~GameObject(){};
    virtual void update(float delta);
};

class Player : IGameObject{};
class Npc : IGameObject{};

class Door: IGameObject{};

/*
 * A gameobject in the game will be a door that the player will interact with
 * The player character
 * An NPC
 * .... Others which still need to be finalized
 */

#endif // __PoliticallyIncorrect__GameObject_H__

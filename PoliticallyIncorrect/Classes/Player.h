#ifndef __PoliticallyIncorrect__Player_H__
#define __PoliticallyIncorrect__Player_H__

#include "cocos2d.h"
#include "GameObject.h"

class IPath;

class Player : public GameObject
{
	typedef GameObject super;
	typedef Player self;

public:
	CREATE_FUNC_GAMEOBJECT(Player);

	Player(cocos2d::ValueMap& properties);
	~Player();

	virtual void update(float delta) override;

	virtual void moveTo(cocos2d::Vec2 coord);
};

#endif // __PoliticallyIncorrect__Player_H__


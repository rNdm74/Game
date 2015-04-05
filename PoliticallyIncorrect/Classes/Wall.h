#ifndef __PoliticallyIncorrect__Wall_H__
#define __PoliticallyIncorrect__Wall_H__

#include "cocos2d.h"
#include "GameObject.h"

class Wall : public GameObject
{
	typedef GameObject super;
	typedef Wall self;

public:
	CREATE_FUNC_GAMEOBJECT(Wall);

	Wall(cocos2d::ValueMap& properties);
	~Wall();
};

#endif // __PoliticallyIncorrect__Wall_H__


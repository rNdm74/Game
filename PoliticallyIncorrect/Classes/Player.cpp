#include "Player.h"
#include "Components.h"
#include "Pathfinding.h"

USING_NS_CC;

Player::Player(ValueMap& properties) : super(properties)
{
	this->setTag(TAG_PLAYER);
	
	ActivePath = nullptr;

	/** Flags **/
	Selected = false;	
}


Player::~Player()
{
}

void Player::moveTo(Vec2 coord)
{
	ExtendedTMXTiledMap* map = static_cast<ExtendedTMXTiledMap*>(this->getParent());

	Rect r = map->getTileRectFrom(coord);
	Vec2 rCenter = Vec2(r.getMidX(), r.getMidY());

	Action* action = MoveTo::create(0.0f, rCenter);
	this->runAction(action);
};

void Player::update(float delta)
{
	//log("The player is updating");

	/*Vec2 p = this->getPosition();

	p.x += 250 * delta;
	p.y += 250 * delta;

	this->setPosition(p);*/

	/*if (ActivePath)
	{
		log("I have a path");
		ExtendedTMXTiledMap* map = static_cast<ExtendedTMXTiledMap*>(this->getParent());
		Vec2 coord = ActivePath->pop_front();

		Rect r = map->getTileRectFrom(coord);
		
		Vec2 rCenter = Vec2(r.getMidX(), r.getMidY());

		Action* action = MoveTo::create(0.0f, rCenter);
		this->runAction(action); 
	}*/
}
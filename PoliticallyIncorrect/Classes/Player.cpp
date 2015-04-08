#include "Player.h"
#include "Components.h"
#include "Pathfinding.h"

USING_NS_CC;

Player::Player(ValueMap& properties) : super(properties)
{
	this->setTag(TAG_PLAYER);
	
	ActivePath = nullptr;

	/** Flags **/
	Moving = false;
	Selected = false;	
}


Player::~Player()
{
}

void Player::moveAlong(IPath& path)
{
	ActivePath = &path;

	if (ActivePath)
	{
		Moving = true;

		this->move();
	}	
};

void Player::move()
{
	this->stopAllActions();

	if (!ActivePath || ActivePath->getLength() <= 0)
	{
		ActivePath = nullptr;
		Moving = false;

		return;
	}
	
	//
	auto map = static_cast<ExtendedTMXTiledMap*>(this->getParent());
	//log("path length: %i", map->playerPath->getLength());

	Rect tile = map->getTileRectFrom(ActivePath->pop_front());
		
	auto moveAction = MoveTo::create(0.2f, tile.origin);
	auto moveFinished = CallFunc::create(CC_CALLBACK_0(Player::move, this));

	this->runAction(Sequence::createWithTwoActions(moveAction, moveFinished));
};

void Player::update(float delta)
{
	//log("The player is updating");
}
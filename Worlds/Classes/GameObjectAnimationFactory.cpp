#include "Constants.h"
#include "GameObject.h"
#include "GameObjectAnimationFactory.h"

GameObjectAnimationFactory::GameObjectAnimationFactory()
{

};

void GameObjectAnimationFactory::Climb(IGameObject& gameObject)
{
};

void GameObjectAnimationFactory::Walk(IGameObject& gameObject)
{
	Sprite* sprite = static_cast<Player&>(gameObject).getSprite();
	// Stop all running actions
	sprite->stopAllActions();

	//  create the Animation, and populate it with frames fetched from the SpriteFrameCache
	auto _anim = Animation::create();
	_anim->setDelayPerUnit(0.1f);

	for (int i = 1; i <= 2; i++)
	{
		auto fileName = "";//_PREFIX + _WALK + std::to_string(i) + _SUFFIX;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
		_anim->addSpriteFrame(frame);
	}

	sprite->runAction(RepeatForever::create(Animate::create(_anim)));

	//sprite->runAction(FlipX::create(_direction.x < 0));
};

void GameObjectAnimationFactory::Stand(IGameObject& gameObject)
{
};

void GameObjectAnimationFactory::Hurt(IGameObject& gameObject)
{
};
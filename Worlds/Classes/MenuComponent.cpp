#include "Constants.h"
#include "GameObject.h"
#include "MenuComponent.h"

MenuComponentItem* MenuComponentItem::createMenuWithFrameName(std::string frameName)
{
	auto sprite = new MenuComponentItem();

	if (sprite && sprite->initWithSpriteFrameName(frameName))
	{
		sprite->autorelease();
		sprite->setName(frameName);
		sprite->initListeners();

		return sprite;
	}

	CC_SAFE_DELETE(sprite);

	return NULL;
}

void MenuComponentItem::initListeners()
{
	//auto listener = EventListenerTouchOneByOne::create();

	//listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {

	//	auto touchEvent = static_cast<EventTouch*>(event);

	//	auto node = touchEvent->getCurrentTarget();
	//			
	//	auto tPos = node->getParent()->convertTouchToNodeSpace(touch);
	//	auto nBox = node->getBoundingBox();

	//	if (nBox.containsPoint(tPos))
	//	{
	//		log(node->getName().c_str());

	//		auto scaleUpAction = ScaleTo::create(0.1f, 1.0f);
	//		auto scaleDownAction = ScaleTo::create(0.1f, 0.8f);

	//		// Button effect
	//		node->runAction(Sequence::createWithTwoActions(scaleUpAction, scaleDownAction));
	//	}

	//	return true;
	//};


	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool PlayerMenuComponent::addMenu()
{
    int _ptr = 0;
    
	auto spriteColors = { "Beige", "Blue", "Green", "Pink", "Yellow" };

	for (auto color : spriteColors)
    {
		std::string name( color );

		auto menu_item = MenuComponentItem::createMenuWithFrameName(kFilePrefix + name + kFileSuffix);
		menu_item->setName(kFilePrefix + name + kFileSuffix);
        menu_item->setTag(_ptr++);
		menu_item->getTexture()->setAntiAliasTexParameters();

        _gameObject->addChild(menu_item, -10);
    }
    
	return true;
}

void PlayerMenuComponent::showMenu()
{
    // Show menu
	auto center = cocos2d::Vec2(_gameObject->getContentSize().width / 2, 20 + _gameObject->getContentSize().height / 2);
    
    // Get all children from gameObject
	auto children = _gameObject->getChildren();
    
    // Displays the radial menu around sprite
    for (int i = 0; i < children.size(); i++)
    {
		// Work out angle
        float radians = -0.2 + (i * 50) * (PI / 180);
        
		// x,y co-ordinate
        float x = center.x + cos(radians) * 80;
        float y = center.y + sin(radians) * 80;
        
		children.at(i)->runAction(cocos2d::MoveTo::create(0.1f, cocos2d::Vec2(x, y)));
		children.at(i)->runAction(cocos2d::ScaleTo::create(0.1f, 0.8f));
    }
    
	this->_isActive = true;
}

void PlayerMenuComponent::hideMenu()
{
    // Hides menu
	auto center = cocos2d::Vec2(_gameObject->getContentSize().width / 2, _gameObject->getContentSize().height / 2);
    
	for (auto child : _gameObject->getChildren())
    {
		child->runAction(cocos2d::MoveTo::create(0.1f, center));
		child->runAction(cocos2d::ScaleTo::create(0.1f, 0));
    }
    
	this->_isActive = false;
}
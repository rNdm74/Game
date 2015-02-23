#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "PathFinder.h"

AppGlobal* AppGlobal::m_pInstance = NULL;

AppGlobal* AppGlobal::getInstance()
{
    return m_pInstance ? m_pInstance : m_pInstance = new AppGlobal();
}

AppGlobal::AppGlobal() 
{
    ActiveLevel = 1;
	scale = 1.0f;

	mouseToggle = false;
	leftMouseButton = false;
	rightMouseButton = false;
	mouseDown = false;
    mouseUp = false;

	gameObjectStates = EGameObjectStates::Stop;

	for (bool& key : keyMatrix)
	{
		key = false;
	}
}

AppGlobal::~AppGlobal()
{
}

void AppGlobal::initMouseListener()
{
	auto listener = EventListenerMouse::create();

	listener->onMouseMove = [=](Event* event)
	{
		// Cast Event to EventMouse for position details like above
		auto eventMouse = static_cast<EventMouse*>(event);
		this->cursorDelta = eventMouse->getDelta();
        this->cursorLocation = eventMouse->getLocationInView();
		this->cursorMove = Vec2(eventMouse->getCursorX(), eventMouse->getCursorY());

		if (Director::getInstance()->getRunningScene()->isRunning())
		{
			auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

			auto cursor = layer->getChildByTag(kTagCursor);

			cursor->setPosition(cursorMove);
		}
	};

	listener->onMouseDown = [=](Event* event)
	{	
		auto eventMouse = static_cast<EventMouse*>(event);
		cursorDownLocation = eventMouse->getLocationInView();
		int mouseButton = eventMouse->getMouseButton();		
		if (mouseButton == 0) leftMouseButton = true;
		if (mouseButton == 1) rightMouseButton = true;

		auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

		auto cursor = static_cast<Sprite*>(layer->getChildByTag(kTagCursor));

		cursor->setSpriteFrame("tapTick.png");

		this->mouseUp = false;
		this->mouseDown = true;

		this->mouseToggle = !mouseToggle;
	};

	listener->onMouseUp = [=](Event* event)
	{
		auto eventMouse = static_cast<EventMouse*>(event);
		int mouseButton = eventMouse->getMouseButton();
		if (mouseButton == 0) leftMouseButton = false;
		if (mouseButton == 1) rightMouseButton = false;
		
		auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

		auto cursor = static_cast<Sprite*>(layer->getChildByTag(kTagCursor));

		cursor->setSpriteFrame("tap.png");

		this->mouseUp = true;
		this->mouseDown = false;
	};

	listener->onMouseScroll = [=](Event* event)
	{
		auto eventMouse = static_cast<EventMouse*>(event);
		
		int delta = eventMouse->getScrollY();

		if (delta > 0) scale += -0.1;
		else if (delta < 0) scale += 0.1;
		
		//if (scale < 0.81f) scale = 0.81f;
		if (scale > 2.0f) scale = 2.05f;
	};
	
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::initKeyboardListener()
{	
	// Keyboard input
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if ((keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanClimbUp] = true;
			gameObjectStates = EGameObjectStates::CheckCanClimbUp;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanClimbDown] = true;
			gameObjectStates = EGameObjectStates::CheckCanClimbDown;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanWalkLeft] = true;
			gameObjectStates = EGameObjectStates::CheckCanWalkLeft;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanWalkRight] = true;
			gameObjectStates = EGameObjectStates::CheckCanWalkRight;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_KP_PG_UP))
		{			
			keyMatrix[EGameObjectStates::LoadNextMap] = true;
			gameObjectStates = EGameObjectStates::LoadNextMap;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_KP_PG_DOWN))
		{
			keyMatrix[EGameObjectStates::LoadPreviousMap] = true;
			gameObjectStates = EGameObjectStates::LoadPreviousMap;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_SPACE))
		{
			
		}
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if ((keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanClimbUp] = false;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanClimbDown] = false;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanWalkLeft] = false;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
		{
			keyMatrix[EGameObjectStates::CheckCanWalkRight] = false;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_PG_UP))
		{
			keyMatrix[EGameObjectStates::LoadNextMap] = false;
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_KP_PG_DOWN))
		{
			keyMatrix[EGameObjectStates::LoadPreviousMap] = false;
		}
		
		bool isAllStatesFalse = false;

		int index = 0;

		while (index < keyMatrix.size() && isAllStatesFalse == false)
		{
			//log("index: %i", index);
			isAllStatesFalse = keyMatrix[index];
			index++;
		}
		
		if (isAllStatesFalse == false)
		{
			gameObjectStates = EGameObjectStates::Stop;
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::initPathFinderListener()
{
	if (activeMap /** When there is an instance of a ParallaxTileMap **/)
	{
		/** Create the pathfinder **/
		auto _pathFinder = new AStarPathFinder(activeMap, 500, false);
	}
};

void AppGlobal::initTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch* touch, Event* e) -> bool
	{		
		if (player)
		{
			Vec2 v1 = player->getCenterPosition();
			Vec2 v2 = player->getParent()->convertToNodeSpaceAR(touch->getLocation());

			Vec2 n = Vec2(v2 - v1).getNormalized();
			Vec2 direction = Vec2(std::round(n.x), std::round(n.y));

			if (direction.y > 0)
			{
				gameObjectStates = EGameObjectStates::CheckCanClimbUp;
			}
			else if (direction.y < 0)
			{
				gameObjectStates = EGameObjectStates::CheckCanClimbDown;
			}
			else if (direction.x < 0)
			{
				gameObjectStates = EGameObjectStates::CheckCanWalkLeft;
			}
			else if (direction.x > 0)
			{
				gameObjectStates = EGameObjectStates::CheckCanWalkRight;
			}
		}

		return true;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* e)
	{
		Vec2 location = touch->getLocationInView();
	};

	listener->onTouchEnded = [=](Touch* touch, Event* e)
	{		
		gameObjectStates = EGameObjectStates::Stop;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::addCursor(Layer& layer)
{
	
	auto _cursor = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tap.png"));//Sprite::createWithSpriteFrameName("tap.png");
	_cursor->setTag(kTagCursor);
	_cursor->setAnchorPoint(Vec2(0.5, 0.5));
	_cursor->setPosition(cursorMove);

	layer.addChild(_cursor, kTagCursor);
}

float AppGlobal::getRandom(float begin, float end)
{
    double value;
    
    value = static_cast<double>(rand()) / RAND_MAX;
    value = value * (end - begin) + begin;
    
    return value;
}
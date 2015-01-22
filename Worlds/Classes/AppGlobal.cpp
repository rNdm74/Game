#include "AppGlobal.h"
#include "Constants.h"

AppGlobal* AppGlobal::m_pInstance = NULL;

AppGlobal* AppGlobal::getInstance()
{
    return m_pInstance ? m_pInstance : m_pInstance = new AppGlobal();
}

AppGlobal::AppGlobal() 
{
    ActiveLevel = 1;
	scale = 1.0f;

	states.JUMP =
	states.LEFT =
	states.RIGHT =
	states.UP =
	states.DOWN =
	states.ESCAPE = false;
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
		auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

		auto cursor = static_cast<Sprite*>(layer->getChildByTag(kTagCursor));

		cursor->setSpriteFrame("tapTick.png");

		this->mouseUp = false;
		this->mouseDown = true;
	};

	listener->onMouseUp = [=](Event* event)
	{
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

		if (delta > 0)
		{
			scale += -0.1;
		}
		else if (delta < 0)
		{
			scale += 0.1;
		}

		if (scale < 0.81f)
			scale = 0.81f;
		if (scale > 2.0f)
			scale = 2.05f;

		log("%f", scale);

	};


	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::initKeyboardListener()
{
	// Keyboard input
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{		
		states.SPRINT	= (keyCode == EventKeyboard::KeyCode::KEY_SHIFT);
		states.JUMP		= (keyCode == EventKeyboard::KeyCode::KEY_SPACE);
		states.ESCAPE	= (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE);
		states.HUD		= (keyCode == EventKeyboard::KeyCode::KEY_TAB);
		states.ENTER	= (keyCode == EventKeyboard::KeyCode::KEY_ENTER);
		states.ENTER	= (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER);
		states.ENTER	= (keyCode == EventKeyboard::KeyCode::KEY_RETURN);
		states.UP		= (keyCode == EventKeyboard::KeyCode::KEY_W);
		states.DOWN		= (keyCode == EventKeyboard::KeyCode::KEY_S);
		states.LEFT		= (keyCode == EventKeyboard::KeyCode::KEY_A);
		states.RIGHT	= (keyCode == EventKeyboard::KeyCode::KEY_D);
		states.UP		= (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW);
		states.DOWN		= (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW);
		states.LEFT		= (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW);
		states.RIGHT	= (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		states.STOP		= false;
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{
		states.SPRINT	= (keyCode == EventKeyboard::KeyCode::KEY_SHIFT);
		states.JUMP		= (keyCode == EventKeyboard::KeyCode::KEY_SPACE);
		states.ESCAPE	= (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE);
		states.HUD		= (keyCode == EventKeyboard::KeyCode::KEY_TAB);
		states.ENTER	= (keyCode == EventKeyboard::KeyCode::KEY_ENTER);
		states.ENTER	= (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER);
		states.ENTER	= (keyCode == EventKeyboard::KeyCode::KEY_RETURN);
		states.UP		= (keyCode == EventKeyboard::KeyCode::KEY_W);
		states.DOWN		= (keyCode == EventKeyboard::KeyCode::KEY_S);
		states.LEFT		= (keyCode == EventKeyboard::KeyCode::KEY_A);
		states.RIGHT	= (keyCode == EventKeyboard::KeyCode::KEY_D);
		states.UP		= (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW);
		states.DOWN		= (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW);
		states.LEFT		= (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW);
		states.RIGHT	= (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		states.STOP		= true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::addCursor(Layer& layer)
{
	auto _cursor = Sprite::createWithSpriteFrameName("tap.png");
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
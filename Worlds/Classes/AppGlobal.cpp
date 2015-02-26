#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "PathFinder.h"

AppGlobal* AppGlobal::m_pInstance = NULL;

AppGlobal* AppGlobal::getInstance()
{
	//log("AppGlobal instance is being accessed");
    return m_pInstance ? m_pInstance : m_pInstance = new AppGlobal();
}

AppGlobal::AppGlobal() 
{
	gameObjectState = EGameObjectState::Stop;

	player = nullptr;

    _zoomFactor = 0.0f;

    _scaleFactor = 1.5f;
    	
	for (bool& key : _keyMatrix) key = false;
}

void AppGlobal::initMouseListener()
{
	auto listener = EventListenerMouse::create();

	listener->onMouseMove = [=](Event* event)
	{
		// Cast Event to EventMouse for position details like above
		auto eventMouse = static_cast<EventMouse*>(event);		

		auto cursorMove = Vec2(eventMouse->getCursorX(), eventMouse->getCursorY());

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
	};

	listener->onMouseUp = [=](Event* event)
	{		
		auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

		auto cursor = static_cast<Sprite*>(layer->getChildByTag(kTagCursor));

		cursor->setSpriteFrame("tap.png");
	};
	
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::initKeyboardListener()
{	
	// Keyboard input
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanClimbUp] = true;
			gameObjectState = EGameObjectState::CheckCanClimbUp;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanClimbDown] = true;
			gameObjectState = EGameObjectState::CheckCanClimbDown;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanWalkLeft] = true;
			gameObjectState = EGameObjectState::CheckCanWalkLeft;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanWalkRight] = true;
			gameObjectState = EGameObjectState::CheckCanWalkRight;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_KP_PG_UP)
		{			
			_keyMatrix[EGameObjectState::LoadNextMap] = true;
			gameObjectState = EGameObjectState::LoadNextMap;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_KP_PG_DOWN)
		{
			_keyMatrix[EGameObjectState::LoadPreviousMap] = true;
			gameObjectState = EGameObjectState::LoadPreviousMap;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			
		}
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanClimbUp] = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanClimbDown] = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanWalkLeft] = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			_keyMatrix[EGameObjectState::CheckCanWalkRight] = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_PG_UP)
		{
			_keyMatrix[EGameObjectState::LoadNextMap] = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_KP_PG_DOWN)
		{
			_keyMatrix[EGameObjectState::LoadPreviousMap] = false;
		}
		
		bool isAllStatesFalse = false;

		int index = 0;

		while (index < _keyMatrix.size() && isAllStatesFalse == false)
		{
			//log("index: %i", index);
			isAllStatesFalse = _keyMatrix[index];
			index++;
		}
		
		if (isAllStatesFalse == false)
		{
			gameObjectState = EGameObjectState::Stop;
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void AppGlobal::initPathFinderListener()
{
	//if (activeMap /** When there is an instance of a ParallaxTileMap **/)
	//{
	//	/** Create the pathfinder **/
	//	auto _pathFinder = new AStarPathFinder(activeMap, 500, false);
	//		_pathFinder->findPath(<#cocos2d::Vec2 startLocation#>, <#cocos2d::Vec2 targetLocation#>)
	//}
};

void AppGlobal::initTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch* touch, Event* e) -> bool
	{
        _touchEvent = true;
        
		if (player)
		{
            Vec2 v1 = player->getCenterPosition();
            Vec2 v2 = player->getParent()->convertToNodeSpaceAR(touch->getLocation());
            
            Vec2 n = Vec2(v2 - v1).getNormalized();
            Vec2 d = Vec2(std::round(n.x), std::round(n.y));
            
			if (player->containsPoint(v2))
			{
				gameObjectState = EGameObjectState::Stop;
			}
			else
			{
				this->setGameObjectState(d);
			}
        }

		return true;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* e)
	{
        if(_touchEvent /** When we have depressed the pad **/)
        {
            Vec2 v1 = player->getCenterPosition();
            Vec2 v2 = player->getParent()->convertToNodeSpaceAR(touch->getLocation());
            
            Vec2 n = Vec2(v2 - v1).getNormalized();
            Vec2 d = Vec2(std::round(n.x), std::round(n.y));
            
			if (player->containsPoint(v2))
			{
				gameObjectState = EGameObjectState::Stop;
			}
			else
			{
				this->setGameObjectState(d);
			}
        }
        
		Vec2 location = touch->getLocationInView();
	};

	listener->onTouchEnded = [=](Touch* touch, Event* e)
	{
        _touchEvent = false;
        
		gameObjectState = EGameObjectState::Stop;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

float AppGlobal::getScale()
{
    return _scaleFactor;
};

void AppGlobal::setGameObjectState(Vec2 direction)
{
    if (direction.y > 0)
    {
		gameObjectState = EGameObjectState::CheckCanClimbUp;
    }
    else if (direction.y < 0)
    {
		gameObjectState = EGameObjectState::CheckCanClimbDown;
    }
    else if (direction.x < 0)
    {
		gameObjectState = EGameObjectState::CheckCanWalkLeft;
    }
    else if (direction.x > 0)
    {
		gameObjectState = EGameObjectState::CheckCanWalkRight;
    }
	else
	{
		gameObjectState = EGameObjectState::Stop;
	}
};

void AppGlobal::addCursor(Layer& layer)
{	
	auto _cursor = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tap.png"));//Sprite::createWithSpriteFrameName("tap.png");
	_cursor->setTag(kTagCursor);
	_cursor->setAnchorPoint(Vec2(0.5, 0.5));
	//_cursor->setPosition(cursorMove);

	layer.addChild(_cursor, kTagCursor);
}

void AppGlobal::zoomIn()
{
    Vec2 v = player->getVelocity();
    float zoom = v.x + v.y;
	/** Increment to scale factor **/
	_scaleFactor += std::abs(zoom / kZoomInFactor);

	/** Clamp scale factor **/
	if (_scaleFactor > kZoomMax) 
		_scaleFactor = kZoomMax;
};

void AppGlobal::zoomOut()
{
    Vec2 v = player->getVelocity();
    float zoom = v.x + v.y;
	/** Increment to scale factor **/
	_scaleFactor -= std::abs(zoom / kZoomOutFactor);

	/** Clamp scale factor **/
	if (_scaleFactor < kZoomMin)
		_scaleFactor = kZoomMin;
};
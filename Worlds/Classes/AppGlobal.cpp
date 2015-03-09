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
	PlayerEvents.push(EGameObjectEvent::Stop);
	
	PlanetInstance = Planet::create();

	// Create the player instance however, properties are reqiured for the instantiation of the player instance
	// This means that the tile map instance is to be instantiated before the player instance so we can get the the starting 
	// properties of the player
	// The properties used to instantiate the player are:
	//
	// _desiredPosition	:: Position of the landing site
	// _name			:: The name of the object
	// _position		:: Equal to _desiredPosition
	// _width			:: 70px
	// _height			:: 70px
	// 
	// Note:: The object will always have a 70 x 70 dimension this is for collision detection purposes


	PlayerInstance = Player::create(PlanetInstance->getPlayerStartingProperties());
	
	_scaleFactor = kZoomMin;
}

void AppGlobal::initMouseListener()
{
	auto listener = EventListenerMouse::create();

	listener->onMouseMove = [=](Event* event)
	{
		// Cast Event to EventMouse for position details like above
		auto eventMouse = static_cast<EventMouse*>(event);		

		auto cursorMove = Vec2(eventMouse->getCursorX(), eventMouse->getCursorY());

		/*if (Director::getInstance()->getRunningScene()->isRunning())
		{
			auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

			auto cursor = layer->getChildByTag(kTagCursor);

			cursor->setPosition(cursorMove);
		}*/
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
		switch (keyCode)
		{
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			{
				if (PlayerInstance->OnLadder)
					PlayerEvents.push(EGameObjectEvent::Up);
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			{
				if (PlayerInstance->OnLadder)
					PlayerEvents.push(EGameObjectEvent::Down);
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{	
				PlayerEvents.push(EGameObjectEvent::Left);
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			{
				PlayerEvents.push(EGameObjectEvent::Right);
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_SPACE:
			{
				PlayerInstance->JumpRequest = true;

				if (PlayerInstance->OnGround)
					PlayerEvents.push(EGameObjectEvent::Jump);
			}
			break;
		};				
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event)
	{
		switch (keyCode)
		{
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			{
				if (PlayerInstance->OnLadder)
					PlayerEvents.pop();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			{
				if (PlayerInstance->OnLadder)
					PlayerEvents.pop();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{
				PlayerEvents.pop();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:	
			{
				PlayerEvents.pop();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_SPACE:	
			{			
				PlayerInstance->JumpRequest = false;
			}	
			break;
		};
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

//void AppGlobal::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
//{	
//	log("Http Test");
//
//	// The data will be placed in the buffer.
//	std::vector<char> * buffer = response->getResponseData();
//	char * concatenated = (char *)malloc(buffer->size() + 1);
//	std::string s2(buffer->begin(), buffer->end());
//	strcpy(concatenated, s2.c_str());
//
//	Json* json = Json_create(concatenated);
//	const char * test1 = Json_getString(json, "test1", "default");
//	const char * test2 = Json_getString(json, "test2", "default");
//
//	// View the console
//	log("HTTP Response : key 1 : %s", test1);
//	log("HTTP Response : key 2 : %s", test2);
//
//	// Delete the JSON object
//	Json_dispose(json);
//};

void AppGlobal::initControllerListener()
{
	/*cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl("http://www.httpbin.org/get");
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(AppGlobal::onHttpRequestCompleted, this));
	request->setTag("GET test1");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();*/
	
	//auto listener = EventListenerController::create();
	
	/*listener->onConnected = [=](Controller* controller, Event* event)
	{
		log("Controller connected");
	};

	listener->onDisconnected = [=](Controller* controller, Event* event)
	{
		log("Controller disconnected");
	};

	listener->onKeyDown = [=](Controller* controller, int keyCode, Event* event)
	{
		log("Controller keydown");
	};

	listener->onKeyUp = [=](Controller* controller, int keyCode, Event* event)
	{
		log("Controller keyup");
	};

	listener->onAxisEvent = [=](Controller* controller, int keyCode, Event* event)
	{
		log("Controller axis event");
	};*/

	//Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	//Controller::startDiscoveryController();
};

void AppGlobal::initTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch* touch, Event* e) -> bool
	{
        _touchEvent = true;
        
		if (PlayerInstance)
		{
			Vec2 v1 = PlayerInstance->getCenterPosition();
			Vec2 v2 = PlayerInstance->getParent()->convertToNodeSpaceAR(touch->getLocation());
            
            Vec2 n = Vec2(v2 - v1).getNormalized();
            Vec2 d = Vec2(std::round(n.x), std::round(n.y));
            
			if (PlayerInstance->getBoundingBox().containsPoint(v2))
			{
				//PlayerInstance->setRunningEvent(EGameObjectEvent::Stop);
			}
			else
			{		
				if (d.y > 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Up);
				}
				else if (d.y < 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Down);
				}
				else if (d.x < 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Left);
				}
				else if (d.x > 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Right);
				}
			}
        }

		return true;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* e)
	{
        if(_touchEvent /** When we have depressed the pad **/)
        {
			Vec2 v1 = PlayerInstance->getCenterPosition();
			Vec2 v2 = PlayerInstance->getParent()->convertToNodeSpaceAR(touch->getLocation());
            
            Vec2 n = Vec2(v2 - v1).getNormalized();
            Vec2 d = Vec2(std::round(n.x), std::round(n.y));
            
			if (PlayerInstance->getBoundingBox().containsPoint(v2))
			{
				//PlayerInstance->setRunningEvent(EGameObjectEvent::Stop);
			}
			else
			{
				if (d.y > 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Up);
				}
				else if (d.y < 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Down);
				}
				else if (d.x < 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Left);
				}
				else if (d.x > 0)
				{
					//PlayerInstance->setRunningEvent(EGameObjectEvent::Right);
				}
			}
        }
	};

	listener->onTouchEnded = [=](Touch* touch, Event* e)
	{
        _touchEvent = false;
        
		//PlayerInstance->setRunningEvent(EGameObjectEvent::Stop);
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

float AppGlobal::getScale()
{
    return _scaleFactor;
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
    Vec2 v = PlayerInstance->getVelocity();
    float zoom = v.x + v.y;
	/** Increment to scale factor **/
	_scaleFactor += std::abs(zoom / kZoomInFactor);

	/** Clamp scale factor **/
	if (_scaleFactor > kZoomMax) 
		_scaleFactor = kZoomMax;
};

void AppGlobal::zoomOut()
{
	Vec2 v = PlayerInstance->getVelocity();
    float zoom = v.x + v.y;
	/** Increment to scale factor **/
	_scaleFactor -= std::abs(zoom / kZoomOutFactor);

	/** Clamp scale factor **/
	if (_scaleFactor < kZoomMin)
		_scaleFactor = kZoomMin;
};
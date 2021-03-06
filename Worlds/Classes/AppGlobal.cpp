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
    //PlanetInstance = Planet::create("Grass");

    //ValueMap landingSite = PlanetInstance->getLandingSite();
    
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


	//PlayerInstance = Player::create(landingSite);
	
	SelectedNpc = nullptr;

	_scaleFactor = kZoomMin;
}

void AppGlobal::initMouseListener()
{
	auto listener = EventListenerMouse::create();

	listener->onMouseMove = [=](Event* event)
	{
		if (Director::getInstance()->getRunningScene() == nullptr) return;
			

		// Cast Event to EventMouse for position details like above
		auto eventMouse = static_cast<EventMouse*>(event);		

		auto cursorMove = Vec2(eventMouse->getCursorX(), eventMouse->getCursorY());
				
		auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

		auto cursor = layer->getChildByTag(kTagCursor);

		if (cursor != nullptr)
			cursor->setPosition(cursorMove);
	};

	listener->onMouseDown = [=](Event* event)
	{
		auto layer = Director::getInstance()->getRunningScene()->getChildByTag(KTagSceneLayer);

		auto cursor = static_cast<Sprite*>(layer->getChildByTag(kTagCursor));

		cursor->setSpriteFrame("tapTick.png");
        
        auto eventMouse = static_cast<EventMouse*>(event);
        this->MovePoint = Vec2(eventMouse->getCursorX(), eventMouse->getCursorY());

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
					PlayerInstance->addMovementEvent(Vec2(0.0f, 1.0f));
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			{
				if (PlayerInstance->OnLadder)
					PlayerInstance->addMovementEvent(Vec2(0.0f, -1.0f));
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{	
				PlayerInstance->addMovementEvent(Vec2(-1.0f, 0.0f));
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			{
				PlayerInstance->addMovementEvent(Vec2(1.0f, 0.0f));
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_SPACE:
			{
				PlayerInstance->JumpRequest = true;

				if (PlayerInstance->OnGround)
					PlayerInstance->addMovementEvent(Vec2(0.0f, 1.0f));
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
					PlayerInstance->removeMovementEvent();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			{
				if (PlayerInstance->OnLadder)
					PlayerInstance->removeMovementEvent();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{
				PlayerInstance->removeMovementEvent();
			}
			break;
			
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:	
			{
				PlayerInstance->removeMovementEvent();
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
        
		if (PlayerInstance == nullptr || PlanetInstance == nullptr)
			return true;

		SelectedNpc = PlanetInstance->getSelectedGameObject(touch->getLocation());
		
		Vec2 v1 = PlayerInstance->getCenterPosition();

		if (SelectedNpc == nullptr)
		{
			if (PlayerInstance->isPathActive())
			{
				PlayerInstance->setPath(nullptr);
				PlayerInstance->removeMovementEvent();
				PlayerInstance->addMovementEvent(Vec2::ZERO);
			}
			else
			{
				Vec2 v1 = PlayerInstance->getCenterPosition();
				Vec2 v2 = PlanetInstance->getPlanetSurface().convertToNodeSpaceAR(touch->getLocation());

				IPath* path = PlanetInstance->findPath(v1, v2);
				PlayerInstance->setPath(path);
			}			
		}
		else
		{
			// Create and set path to travel
			Vec2 v2 = SelectedNpc->getCenterPosition();
			IPath* path = PlanetInstance->findPath(v1, v2);
			PlayerInstance->setPath(path);
		}
		
		return true;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* e)
	{
		if (PlayerInstance == nullptr || PlanetInstance == nullptr)
			return;

		Vec2 v1 = PlayerInstance->getCenterPosition();
		Vec2 v2 = PlanetInstance->getPlanetSurface().convertToNodeSpaceAR(touch->getLocation());
		
		IPath* path = PlanetInstance->findPath(v1, v2);
		PlayerInstance->setPath(path);

   //     if(_touchEvent /** When we have depressed the pad **/)
   //     {
			//Vec2 v1 = PlayerInstance->getCenterPosition();
			//Vec2 v2 = PlayerInstance->getParent()->convertToNodeSpaceAR(touch->getLocation());
   //         
   //         Vec2 n = Vec2(v2 - v1).getNormalized();
   //         Vec2 d = Vec2(std::round(n.x), std::round(n.y));
   //         
			//if (PlayerInstance->getBoundingBox().containsPoint(v2))
			//{
			//	//PlayerInstance->setRunningEvent(EGameObjectEvent::Stop);
			//}
			//else
			//{
			//	if (d.y > 0)
			//	{
			//		//PlayerInstance->setRunningEvent(EGameObjectEvent::Up);
			//	}
			//	else if (d.y < 0)
			//	{
			//		//PlayerInstance->setRunningEvent(EGameObjectEvent::Down);
			//	}
			//	else if (d.x < 0)
			//	{
			//		//PlayerInstance->setRunningEvent(EGameObjectEvent::Left);
			//	}
			//	else if (d.x > 0)
			//	{
			//		//PlayerInstance->setRunningEvent(EGameObjectEvent::Right);
			//	}
			//}
   //     }
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
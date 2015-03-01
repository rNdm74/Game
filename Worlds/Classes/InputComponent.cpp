#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "ParallaxTileMap.h"
#include "Path.h"
#include "Utils.h"


void InputComponent::ClimbUp(IGameObject& gameObject)
{
	this->updateDesiredPosition(gameObject, GameObjectStates[AppGlobal::getInstance()->gameObjectState]);
};

void InputComponent::ClimbDown(IGameObject& gameObject)
{
	this->updateDesiredPosition(gameObject, GameObjectStates[AppGlobal::getInstance()->gameObjectState]);
};

void InputComponent::WalkLeft(IGameObject& gameObject)
{
	this->updateDesiredPosition(gameObject, GameObjectStates[AppGlobal::getInstance()->gameObjectState]);
};

void InputComponent::WalkRight(IGameObject& gameObject)
{
	this->updateDesiredPosition(gameObject, GameObjectStates[AppGlobal::getInstance()->gameObjectState]);
};

void InputComponent::Stop(IGameObject& gameObject)
{
	this->updateDesiredPosition(gameObject, GameObjectStates[AppGlobal::getInstance()->gameObjectState]);

	Vec2 velocity = gameObject.getVelocity();
	
	if (velocity.x > 1.0f)
	{
		velocity.x -= 10.0f;
	}
	else if (velocity.x < -1.0f)
	{
		velocity.x += 10.0f;
	}
	else
	{
		velocity.x = 0.0f;
	}
		
    if (velocity.y > 1.0f)
    {
        velocity.y -= 10.0f;
    }
    else if (velocity.y < -1.0f)
    {
        velocity.y += 10.0f;
    }
    else
    {
        velocity.y = 0.0f;
    }

	gameObject.setVelocity(velocity);
};

void InputComponent::updateDesiredPosition(IGameObject& gameObject, Vec2 direction)
{
	Vec2 velocity = gameObject.getVelocity();
	//
	Vec2 step = _movement * kUpdateInterval;
	velocity.x = velocity.x + step.x * direction.x;
	velocity.y = velocity.y + step.y * direction.y;
	//
	velocity.clamp(_minMovement, _maxMovement);
	Vec2 stepVelocity = velocity * kUpdateInterval;
	// 
	gameObject.setDesiredPosition(gameObject.getPosition() + stepVelocity);
	gameObject.setVelocity(velocity);
};


void PlayerInputComponent::update(Node& node, IGameObject& gameObject)
{	
	//this->findpath(node, gameObject);	
	//this->desiredPosition(gameObject);
}

//void InputComponent::findpath(Node& node, IGameObject& gameObject)
//{		
//	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
//
//	Vec2 cursorLocation = parallaxTileMap.convertToNodeSpaceAR(AppGlobal::getInstance()->cursorLocation);
//
//#if DEBUG_ENABLE
//	parallaxTileMap.drawDebugRectAt(cursorLocation, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
//#endif // DEBUG_ENABLE
//
//	gameObject.setDirection(Vec2::ZERO);
//
//	Vec2 direction = gameObject.getDirection();
//	Vec2 velocity = gameObject.getVelocity();
//
//	IPath* path = gameObject.getPath();
//
//	//
//	if (AppGlobal::getInstance()->leftMouseButton)
//	{
//		//
//		Vec2 v1 = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
//		Vec2 v2 = parallaxTileMap.getTileCoordinatesFor(cursorLocation);
//		//
//		if (path == nullptr)
//		{
//			path = parallaxTileMap.getPath(v1, v2);
//		}
//		else
//		{
//			IPath* newPath = parallaxTileMap.getPath(path->peek_back(), v2);
//			path->add(newPath);
//		}
//	}
//	else if (AppGlobal::getInstance()->mouseToggle)
//	{
//		//
//		Vec2 v1 = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
//		Vec2 v2 = parallaxTileMap.getTileCoordinatesFor(cursorLocation);
//		//
//		if (path == nullptr && AppGlobal::getInstance()->leftMouseButton)
//		{
//			path = parallaxTileMap.getPath(v1, v2);
//		}
//	}
//	else
//	{
//		path = nullptr;
//		velocity = Vec2::ZERO;
//		direction = Vec2::ZERO;
//	}
//
//	//
//	if (path != nullptr && path->getLength() > 0)
//	{
//
//#if DEBUG_ENABLE
//		for (int i = 0; i < path->getLength(); ++i)
//		{
//			parallaxTileMap.drawDebugRectAtTile(path->getStep(i), Color4F(0.3f, 1.0f, 0.3f, 0.5f));
//		}
//#endif // DEBUG_ENABLE
//
//		//
//		Rect r1 = gameObject.getCollisionBox();
//		Rect r2 = parallaxTileMap.getTileRectFrom(path->peek_front());
//		//
//		Vec2 v1 = Vec2(r1.getMidX(), r1.getMinY());
//		Vec2 v2 = Vec2(r2.getMidX(), r2.getMinY());
//		//		        
//		Vec2 n = Vec2(v2 - v1).getNormalized();
//		direction = Vec2(std::round(n.x), std::round(n.y));
//		//
//		float distance = v1.distance(v2);
//		//log("distance: %f", distance);
//		//
//		if (distance < 13.0f)
//		{
//			path->pop_front();
//
//			if (path->getLength() < 1)
//			{
//				AppGlobal::getInstance()->mouseToggle = false;
//
//				path = nullptr;
//				velocity = Vec2::ZERO;
//				direction = Vec2::ZERO;
//
//				gameObject.setPositionX(v2.x - gameObject.getContentSize().width / 2);
//				gameObject.setPositionY(v2.y);
//			}
//		}
//	}
//
//	gameObject.setDirection(direction);
//	gameObject.setVelocity(velocity);
//	gameObject.setPath(path);
//}

void InputComponent::desiredPosition(IGameObject& gameObject)
{
};

//void InputComponent::desiredPosition(IGameObject& gameObject)
//{	
//	CanMove canMove = gameObject.getCanMove();
//	Vec2 direction = gameObject.getDirection();
//	Vec2 velocity = gameObject.getVelocity();	
//	IsMoving isMoving;
//
//	for (bool& b : isMoving) b = false;
//
//	bool move = false;
//
//	if (canMove[STATE_UP] && direction.y > 0)
//	{
//		isMoving[STATE_UP] = true;
//		move = true;
//	}
//	else if (canMove[STATE_DOWN] && direction.y < 0)
//	{
//		isMoving[STATE_DOWN] = true;
//		move = true;
//	}
//	else if (canMove[STATE_LEFT] && direction.x < 0)
//	{
//		isMoving[STATE_LEFT] = true;
//		move = true;
//	}
//	else if (canMove[STATE_RIGHT] && direction.x > 0)
//	{
//		isMoving[STATE_RIGHT] = true;
//		move = true;
//	}
//	else if (direction == Vec2::ZERO)
//	{
//		for (bool& b : isMoving) b = false;
//
//		move = false;
//		velocity = Vec2::ZERO;
//	}
//	//
//	Vec2 movement = Vec2(1600.0, 1600.0);
//	Vec2 step = movement * kUpdateInterval;
//	//
//	if (move)
//	{
//		velocity.x = velocity.x + step.x * direction.x;
//		velocity.y = velocity.y + step.y * direction.y;
//	}
//	//
//	Vec2 minMovement = Vec2(-340.0, -340.0);
//	Vec2 maxMovement = Vec2(340.0, 340.0);
//	//
//	velocity.clamp(minMovement, maxMovement);
//	//
//	Vec2 stepVelocity = velocity * kUpdateInterval;
//	// 
//	gameObject.setVelocity(velocity);
//	gameObject.setIsMoving(isMoving);
//	//
//	gameObject.setDesiredPosition(gameObject.getPosition() + stepVelocity);
//
//	
//}
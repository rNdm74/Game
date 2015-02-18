#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "ParallaxTileMap.h"
#include "Path.h"


void PlayerInputComponent::update(Node& node, GameObject& gameObject)
{	
	ParallaxTileMap& parallaxTileMap = static_cast<ParallaxTileMap&>(node);
	
	this->findpath(parallaxTileMap, gameObject);
	
	this->desiredPosition(gameObject);
}


void InputComponent::findpath(ParallaxTileMap& parallaxTileMap, GameObject& gameObject)
{		
	Vec2 cursorLocation = parallaxTileMap.convertToNodeSpaceAR(AppGlobal::getInstance()->cursorLocation);

#if DEBUG_ENABLE
	parallaxTileMap.drawDebugRectAt(cursorLocation, Color4F(0.3f, 0.3f, 1.0f, 0.5f));
#endif // DEBUG_ENABLE

	gameObject.direction = Vec2::ZERO;
	//
	if (AppGlobal::getInstance()->leftMouseButton)
	{
		//
		Vec2 v1 = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
		Vec2 v2 = parallaxTileMap.getTileCoordinatesFor(cursorLocation);
		//
		if (gameObject.path == nullptr)
		{
			gameObject.path = parallaxTileMap.getPath(v1, v2);
		}
		else
		{
			Path* newPath = parallaxTileMap.getPath(gameObject.path->peek_back(), v2);
			gameObject.path->addPath(newPath);
		}
	}
	else if (AppGlobal::getInstance()->mouseToggle)
	{
		//
		Vec2 v1 = parallaxTileMap.getTileCoordinatesFor(gameObject.getCenterPosition());
		Vec2 v2 = parallaxTileMap.getTileCoordinatesFor(cursorLocation);
		//
		if (gameObject.path == nullptr && AppGlobal::getInstance()->leftMouseButton)
		{
			gameObject.path = parallaxTileMap.getPath(v1, v2);
		}
	}
	else
	{
		gameObject.path = nullptr;
		gameObject.velocity = Vec2::ZERO;
		gameObject.direction = Vec2::ZERO;
	}

	//
	if (gameObject.path != nullptr && gameObject.path->getLength() > 0)
	{

#if DEBUG_ENABLE
		for (int i = 0; i < gameObject.path->getLength(); ++i)
		{
			parallaxTileMap.drawDebugRectAtTile(gameObject.path->getStep(i), Color4F(0.3f, 1.0f, 0.3f, 0.5f));
		}
#endif // DEBUG_ENABLE

		//
		Rect r1 = gameObject.getCollisionBoundingBox();
		Rect r2 = parallaxTileMap.getTileRectFrom(gameObject.path->peek_front());
		//
		Vec2 v1 = Vec2(r1.getMidX(), r1.getMinY());
		Vec2 v2 = Vec2(r2.getMidX(), r2.getMinY());
		//		        
		Vec2 n = Vec2(v2 - v1).getNormalized();
		gameObject.direction = Vec2(std::round(n.x), std::round(n.y));
		//
		float distance = v1.distance(v2);
		//log("distance: %f", distance);
		//
		if (distance < 13.0f)
		{
			gameObject.path->pop_front();

			if (gameObject.path->getLength() < 1)
			{
				AppGlobal::getInstance()->mouseToggle = false;

				gameObject.path = nullptr;
				gameObject.velocity = Vec2::ZERO;
				gameObject.direction = Vec2::ZERO;

				gameObject.setPositionX(v2.x - gameObject.getContentSize().width / 2);
				gameObject.setPositionY(v2.y);
			}
		}
	}
}


void InputComponent::desiredPosition(GameObject& gameObject)
{
	gameObject.isMovingUp = false;
	gameObject.isMovingDown = false;
	gameObject.isMovingLeft = false;
	gameObject.isMovingRight = false;

	if (gameObject.canMoveUp && gameObject.direction.y > 0)
	{
		gameObject.isMovingUp = true;
		gameObject.canMove = true;
	}
	else if (gameObject.canMoveDown && gameObject.direction.y < 0)
	{
		gameObject.isMovingDown = true;
		gameObject.canMove = true;
	}
	else if (gameObject.canMoveLeft && gameObject.direction.x < 0)
	{
		gameObject.isMovingLeft = true;
		gameObject.canMove = true;
	}
	else if (gameObject.canMoveRight && gameObject.direction.x > 0)
	{
		gameObject.isMovingRight = true;
		gameObject.canMove = true;
	}
	else if (gameObject.direction == Vec2::ZERO)
	{
		gameObject.isMovingUp = false;
		gameObject.isMovingDown = false;
		gameObject.isMovingLeft = false;
		gameObject.isMovingRight = false;
		gameObject.canMove = false;
		gameObject.velocity = Vec2::ZERO;
	}
	//
	Vec2 move = Vec2(1600.0, 1600.0);
	Vec2 step = move * kUpdateInterval;
	//
	if (gameObject.canMove)
	{
		gameObject.velocity.x = gameObject.velocity.x + step.x * gameObject.direction.x;
		gameObject.velocity.y = gameObject.velocity.y + step.y * gameObject.direction.y;
	}
	//
	Vec2 minMovement = Vec2(-340.0, -340.0);
	Vec2 maxMovement = Vec2(340.0, 340.0);
	//
	gameObject.velocity.clamp(minMovement, maxMovement);
	//
	Vec2 stepVelocity = gameObject.velocity * kUpdateInterval;
	// 
	gameObject.desiredPosition = gameObject.getPosition() + stepVelocity;
}
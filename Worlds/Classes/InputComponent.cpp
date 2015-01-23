#include "AppGlobal.h"
#include "Constants.h"
#include "GameObject.h"
#include "InputComponent.h"

void PlayerInputComponent::update(GameObject& gameObject)
{
	auto global = AppGlobal::getInstance();

	if (global->states.LEFT)
		gameObject.setBearing(EAST);
	if (global->states.RIGHT)
		gameObject.setBearing(WEST);
	if (global->states.DOWN)
		gameObject.setBearing(SOUTH);
	if (global->states.UP)
		gameObject.setBearing(NORTH);
	if (global->states.STOP)
		gameObject.setBearing(STOP);

	//Vec2 newPlayerPos = gameObject.getPosition();
	//Vec2* direction = gameObject.getDirection();
	//EBearing bearing = gameObject.getBearing();

	////
	//newPlayerPos.x += kGameObjectVelocity.x * kUpdateInterval * direction[bearing].x;
	//newPlayerPos.y += kGameObjectVelocity.y * kUpdateInterval * direction[bearing].y;

	////
	//gameObject.setPosition(newPlayerPos);		
}
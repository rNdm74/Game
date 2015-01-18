#include "AppGlobal.h"
#include "Box2D.h"
#include "Constants.h"
#include "GameObject.h"
#include "GraphicsComponent.h"

void PlayerGraphicsComponent::update(GameObject &gameObject)
{
	b2Vec2 bodyPos = gameObject.getBody()->GetPosition();

	Vec2 nodePos = gameObject.getParent()->convertToNodeSpace(Vec2(bodyPos.x * kPixelsPerMeter, bodyPos.y * kPixelsPerMeter));

	gameObject.setPosition(nodePos);
}
#include "Constants.h"
#include "ContactListener.h"
#include "GameObject.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	GameObject* gameObject;

	if (isSensor(contact, gameObject))
	{
		gameObject->sensor = getSensorFixture(contact);

		return;
	}
	
	// ON begin contact
	// IF gameObject is touching ground
	// THEN gameObject is touching ground
	if (isOnGround(contact, gameObject))
	{
		gameObject->setTouchGround = true;
	}

	// ON begin contact
	// IF gameObject is touching ladder
	// THEN gameObject is touching ladder
	// FINALLY log ladder fixture
	if (isOnLadder(contact, gameObject))
	{
		gameObject->setTouchLadder = true;

		gameObject->ladder = getLadderFixture(contact);

		return;
	}
		
	// ON begin contact
	// IF gameObject is touching ground and ladder
	// IF gameObject is at top of ladder
	// THEN gameObject is at top of ladder
	// ELSE IF gameObject is at bottom of ladder
	// THEN gameObject is at bottom of ladder
	// ELSE gameObject is not on top and bottom of ladder
	if (gameObject->setTouchGround && gameObject->setTouchLadder)
	{		
		// get ladder fixture information
		if (isTopLadder(gameObject->ladder, *gameObject))
		{			
			// at top of ladder
			gameObject->atTopLadder = true;
			gameObject->atBottomLadder = false;
		}
		else if (isBottomLadder(gameObject->ladder, *gameObject))
		{
			// at bottom of ladder
			gameObject->atTopLadder = false;
			gameObject->atBottomLadder = true;
		}
	}
			
	/*log("################ Begin Contact ###################");
	log("touching ground:\t %i, touching ladder:\t %i", gameObject->setTouchGround, gameObject->setTouchLadder);
	log("top of ladder:\t\t %i, bottom of ladder:\t %i", gameObject->atTopLadder, gameObject->atBottomLadder);
	log("################################################## \n");	*/
}

void ContactListener::EndContact(b2Contact* contact)
{	
	// should be solid regardless
	//contact->SetEnabled(true);

	GameObject* gameObject;

	if (isSensor(contact, gameObject))
	{
		//log("left sensor");
		return;
	}
				
	// IF gameObject end contact is ground
	// THEN gameObject is not touching ground
	if (isOnGround(contact, gameObject))
	{
		gameObject->setTouchGround = false;	
	}

	// ON end contact
	// IF gameObject end contact is ladder
	// THEN gameObject is not touch ladder
	if (isOnLadder(contact, gameObject))
	{
		gameObject->setTouchLadder = false;
	}
	
	// ON end contact
	// IF gameObject is touching ground 
	// AND gameObject is not touching ladder
	// THEN gameObject is not at top or bottom of ladder
	if (gameObject->setTouchGround && gameObject->setTouchLadder == false)
	{		
		gameObject->atTopLadder = false;
		gameObject->atBottomLadder = false;
	}

	// ## GAMEOBJECT IS AT CLIMBING UP LADDER ##
	//
	// ON end contact
	// IF gameObject is not touching ground 
	// AND gameObject is touching the ladder
	// THEN gameObject is not at the bottom of a ladder
	if (gameObject->setTouchGround == false && gameObject->setTouchLadder)
	{
		// check that gameObject is not at the top of ladder
		if (isTopLadder(gameObject->ladder, *gameObject) == false)
		{			
			gameObject->atBottomLadder = false;
		}
	}

	// ## GAMEOBJECT IS AT TOP OF LADDER ##
	//
	// ON end contact
	// IF gameObject is not touching any ground
	// AND gameObject is not touching a ladder
	// THEN gameObject is at top of ladder
	//if (gameObject->setTouchGround == false && gameObject->setTouchLadder == false)
	//{
	//	// check to confirm gameObject is at top of ladder
	//	if (isTopLadder(ladder, *gameObject))
	//	{
	//		gameObject->climb = false;
	//		gameObject->atTopLadder = true;
	//		gameObject->atBottomLadder = false;
	//	}
	//}	
	
	/*log("################ End Contact #####################");
	log("touching ground:\t %i, touching ladder:\t %i", gameObject->setTouchGround, gameObject->setTouchLadder);
	log("top of ladder:\t\t %i, bottom of ladder:\t %i", gameObject->atTopLadder, gameObject->atBottomLadder);
	log("##################################################\n");*/	
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    
}

bool ContactListener::isSensor(b2Contact* contact, GameObject*& gameObject)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool isFixtureASensor = fixtureA->GetFilterData().categoryBits == kFilterCatagory::SENSOR;
	bool isFixtureBSensor = fixtureB->GetFilterData().categoryBits == kFilterCatagory::SENSOR;

	if (isFixtureASensor) // fixture B must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureB->GetBody()->GetUserData());
		return true;
	}
	else if (isFixtureBSensor) // fixtureA must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureA->GetBody()->GetUserData());
		return true;
	}

	return false;
}

bool ContactListener::isOnGround(b2Contact* contact, GameObject*& gameObject)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool isFixtureABoundary = fixtureA->GetFilterData().categoryBits == kFilterCatagory::BOUNDARY;
	bool isFixtureBBoundry = fixtureB->GetFilterData().categoryBits == kFilterCatagory::BOUNDARY;
	
	if (isFixtureABoundary) // fixture B must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureB->GetBody()->GetUserData());
		return true;
	}
	else if (isFixtureBBoundry) // fixtureA must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureA->GetBody()->GetUserData());
		return true;
	}

	return false;
}

bool ContactListener::isOnLadder(b2Contact* contact, GameObject*& gameObject)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool isFixtureALadder = fixtureA->GetFilterData().categoryBits == kFilterCatagory::LADDER;
	bool isFixtureBLadder = fixtureB->GetFilterData().categoryBits == kFilterCatagory::LADDER;

	if (isFixtureALadder) // fixture B must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureB->GetBody()->GetUserData());
		return true;
	}
	else if (isFixtureBLadder) // fixtureA must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureA->GetBody()->GetUserData());
		return true;
	}

	return false;
}

bool ContactListener::isLadder(b2Contact* contact, GameObject*& gameObject)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//make sure only one of the fixtures was a sensor
	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();
	
	if (!(sensorA ^ sensorB))
		return false;
		
	if (sensorA) // fixtureA must be gameObject
	{
		gameObject = static_cast<GameObject*>(fixtureA->GetBody()->GetUserData());
	}
	else // fixtureB must be gameObject
	{		
		gameObject = static_cast<GameObject*>(fixtureB->GetBody()->GetUserData());
	}

	return true;
}

b2Fixture* ContactListener::getSensorFixture(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (fixtureA->GetFilterData().categoryBits == kFilterCatagory::SENSOR)
		return fixtureA;
	else if (fixtureB->GetFilterData().categoryBits == kFilterCatagory::SENSOR)
		return fixtureB;
}

b2Fixture* ContactListener::getLadderFixture(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (fixtureA->GetFilterData().categoryBits == kFilterCatagory::LADDER) 
		return fixtureA;
	else if (fixtureB->GetFilterData().categoryBits == kFilterCatagory::LADDER) 
		return fixtureB;
}

bool ContactListener::isTopLadder(b2Fixture* ladder, GameObject& gameObject)
{
	auto top = ladder->GetBody()->GetWorldPoint(b2Vec2(0.5, 1)); // middle, top

	auto pos = gameObject.getBody()->GetWorldPoint(b2Vec2(0.5, 0)); // middle, bottom

	return pos.y > top.y;
}

bool ContactListener::isBottomLadder(b2Fixture* ladder, GameObject& gameObject)
{
	auto bottom = ladder->GetBody()->GetWorldPoint(b2Vec2(0.5, 0)); // middle, top

	auto pos = gameObject.getBody()->GetWorldPoint(b2Vec2(0.5, 0)); // middle, bottom

	return true;
}
#include "Components.h"
#include "GameObject.h"

USING_NS_CC;

ExtendedTMXTiledMap* ExtendedTMXTiledMap::create(const std::string& tmxFile)
{
	ExtendedTMXTiledMap *ret = new (std::nothrow) ExtendedTMXTiledMap();
	if (ret->initWithTMXFile(tmxFile))
	{
		ret->autorelease();
		ret->initGameObjects();

		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

ExtendedTMXTiledMap::ExtendedTMXTiledMap()
{
		
};

void ExtendedTMXTiledMap::update(float delta)
{
};

void ExtendedTMXTiledMap::setPositionOnPlayer(Rect collisionBox)
{
	//Vec2 v = this->getPlayer()->getVelocity();
	Rect r = collisionBox; // this->getPlayer()->getCollisionBox();
	/** Bottom middle of sprite **/
	Vec2 p = Vec2(r.origin.x + (r.size.width / 2), r.origin.y);
	//p.x += v.x * kUpdateInterval;

	float s = this->getParent()->getScale();

	Size m = _mapSize;
	Size t = _tileSize;
	Size w = Director::getInstance()->getWinSize() / s;

	float x = MAX(p.x, w.width / 2);
	float y = MAX(p.y, w.height / 2);
	x = MIN(x, (m.width * t.width) - w.width / 2);
	y = MIN(y, (m.height * t.height) - w.height / 2);

	Vec2 ap = Vec2(x, y);
	Vec2 cov = Vec2(w.width / 2, w.height / 2);
	Vec2 vp = cov - ap;

	this->setPosition(vp);
};

void ExtendedTMXTiledMap::initGameObjects()
{
	// loop over the object groups in this tmx file
	for (auto& objectGroup : this->getObjectGroups())
	{
		for (auto& object : objectGroup->getObjects())
		{
			auto properties = object.asValueMap();
			auto type = properties.at("type");

			if (type.isNull() == false)
			{
				if (initGameObject(type.asString(), properties))
				{
					_objectCount++;
				}
			}
		}
	}
};

bool ExtendedTMXTiledMap::initGameObject(std::string className, ValueMap& properties)
{
	// create the object
	IGameObject* o = GameObjectFactory::create(className, properties);

	// process the new object
	if (o != nullptr)
	{		
		this->addChild(o);

		return true;
	}

	return false;
};
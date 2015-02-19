#ifndef __FranticAlien__MenuComponent__
#define __FranticAlien__MenuComponent__

#include "cocos2d.h"

using namespace cocos2d;

class IGameObject;

class MenuComponentItem : public Sprite
{
public:
	static MenuComponentItem* createMenuWithFrameName(std::string frameName);

	void initListeners();
};

class IMenuComponent
{
public:
	//
	IMenuComponent(){};
	virtual ~IMenuComponent() {};

	//
	virtual bool addMenu(IGameObject& gameObject) = 0;
    virtual void showMenu(IGameObject& gameObject) = 0;
	virtual void hideMenu(IGameObject& gameObject) = 0;

	virtual void update(IGameObject& gameObject) = 0;
	
	virtual bool isActive() = 0;
};

class PlayerMenuComponent : public IMenuComponent
{
public:    
    // init methods
	bool addMenu(IGameObject& gameObject);
	void showMenu(IGameObject& gameObject);
	void hideMenu(IGameObject& gameObject);
    
	void update(IGameObject& gameObject){}

	inline bool isActive() { return _isActive; }

private:
	bool _isActive;
};

#endif /* defined(__FranticAlien__MenuComponent__) */

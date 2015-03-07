#ifndef __FranticAlien__MenuComponent__
#define __FranticAlien__MenuComponent__

class IGameObject;

class MenuComponentItem : public cocos2d::Sprite
{
public:
	static MenuComponentItem* createMenuWithFrameName(std::string frameName);
	void initListeners();
};

class IMenuComponent
{
public:
	IMenuComponent(){};
	virtual ~IMenuComponent(){};

	//
	virtual bool addMenu() = 0;
    virtual void showMenu() = 0;
	virtual void hideMenu() = 0;
	virtual void update() = 0;	
	virtual bool isActive() = 0;

protected:
	IGameObject* _gameObject;
};

class PlayerMenuComponent : public IMenuComponent
{
public: 
	//
	PlayerMenuComponent(IGameObject& gameObject){ _gameObject = &gameObject; };
	virtual ~PlayerMenuComponent() {};

    // init methods
	bool addMenu();
	void showMenu();
	void hideMenu();    
	void update(){};

	bool isActive() { return _isActive; }

private:
	bool _isActive;
};

#endif /* defined(__FranticAlien__MenuComponent__) */

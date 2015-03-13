#ifndef __FranticAlien__PATHFINDING_COMPONENT_H__
#define __FranticAlien__PATHFINDING_COMPONENT_H__

class IGameObject;
class IPathFinder;
class IPath;

class IPathfindingComponent
{
public:
	IPathfindingComponent(){};
	virtual ~IPathfindingComponent(){};
    
    virtual void update() = 0;
};

class PathfindingComponent : public IPathfindingComponent
{
	typedef IPathfindingComponent super;
	typedef PathfindingComponent self;

public:
	PathfindingComponent(IGameObject& gameObject);
	virtual ~PathfindingComponent()
    {
        delete _planetSurface;
        delete _gameObject;
        delete _pathFinder;
        delete _path;
    };

	/** Actions **/	
    void update() override;
    
protected:
    IParallaxTileMap* _planetSurface;
	IGameObject* _gameObject;
    IPathFinder* _pathFinder;
    IPath* _path;
};

class PlayerPathfindingComponent : public PathfindingComponent
{
	typedef PathfindingComponent super;
	typedef PlayerPathfindingComponent self;

public:
	PlayerPathfindingComponent(IGameObject& gameObject);
    virtual ~PlayerPathfindingComponent(){};
};

class NpcPathfindingComponent : public PathfindingComponent
{
	typedef PathfindingComponent super;
	typedef NpcPathfindingComponent self;

public:
	NpcPathfindingComponent(IGameObject& gameObject);
    virtual ~NpcPathfindingComponent(){};
};

#endif /* defined(__FranticAlien__PATHFINDING_COMPONENT_H__) */

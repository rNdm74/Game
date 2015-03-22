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
	virtual void setPath(IPath* newPath) = 0;
	virtual bool isPathActive() = 0;
};

class PathfindingComponent : public IPathfindingComponent
{
	typedef IPathfindingComponent super;
	typedef PathfindingComponent self;

public:
	PathfindingComponent(IGameObject& gameObject);
	virtual ~PathfindingComponent(){};

	/** Actions **/	
    void update() override;

	/** Setters **/
	virtual void setPath(IPath* newPath) override;
	virtual bool isPathActive() override;
    
protected:
	Vec2 getDistance();
	Vec2 getDirection();
	Vec2 getNextDirection();
	Vec2 getPathOrigin(Vec2 pathCoords);
	
	bool reachedPathCoords();

    IParallaxTileMap* _planetSurface;
	IGameObject* _gameObject;    
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

#ifndef __FranticAlien__WORLD_H__
#define __FranticAlien__WORLD_H__

#include "cocos2d.h"

using namespace cocos2d;

class World : public Node
{
public:	
	/** **/
	static World* create();
	/** **/
	World();
	virtual ~World(){};
	/** **/
	void update(float& delta);

private:
	class GamePlayer* player;
	class Level* level;
};





#endif /* defined(__FranticAlien__WORLD_H__) */

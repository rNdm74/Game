#ifndef __com_dotdat_World__PARALLAX_FOREGROUND_H__
#define __com_dotdat_World__PARALLAX_FOREGROUND_H__

#include "cocos2d.h"
#include <array>

using namespace cocos2d;

class ParallaxForeground : public ParallaxNode
{
public:
	// "creator" methods first	
	static ParallaxForeground* create(float width);

	// If applicable, then Constructors and the Destructor
	ParallaxForeground(float width);
	virtual ~ParallaxForeground();

	//
	void update(float delta);
			
	Sprite* getMountain();
	Sprite* getHill();
	Sprite* getCloud();
	Sprite* getShadowForNode(Sprite* sprite);
};

#endif /* defined(__com_dotdat_World__PARALLAX_FOREGROUND_H__) */

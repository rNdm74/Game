#ifndef __com_dotdat_World__PARALLAX_BACKGROUND_H__
#define __com_dotdat_World__PARALLAX_BACKGROUND_H__

#include "cocos2d.h"
#include <array>

using namespace cocos2d;

typedef std::array<Sprite*, 3> ParallaxBackgroundImages;

class ParallaxBackground : public ParallaxNode
{
public:
	// "creator" methods first	
	static ParallaxBackground* create(float width);

	// If applicable, then Constructors and the Destructor
	ParallaxBackground(float width);
	virtual ~ParallaxBackground();

	//
	void update(float delta);

	Sprite* getShadowForNode(Sprite* sprite);

	Sprite* getBackground();
	Sprite* getMountain();
	Sprite* getHill();
	Sprite* getCloud();

private:
	ParallaxBackgroundImages _clouds;
};

#endif /* defined(__com_dotdat_World__PARALLAX_BACKGROUND_H__) */

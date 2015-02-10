#ifndef __com_dotdat_World__PARALLAX_BACKGROUND_H__
#define __com_dotdat_World__PARALLAX_BACKGROUND_H__

#include "cocos2d.h"
#include <array>

using namespace cocos2d;

typedef std::array<Sprite*, 2> ParallaxBackgroundImages;

class ParallaxBackground : public ParallaxNode
{
public:
	// "creator" methods first	
	static ParallaxBackground* create();

	// If applicable, then Constructors and the Destructor
	ParallaxBackground();
	virtual ~ParallaxBackground();

	//
	void update(float delta);

private:
	ParallaxBackgroundImages _background;
	ParallaxBackgroundImages _midground;
	ParallaxBackgroundImages _foreground;
};

#endif /* defined(__com_dotdat_World__PARALLAX_BACKGROUND_H__) */

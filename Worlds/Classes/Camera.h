#ifndef __com_dotdat_World__CAMERA_H__
#define __com_dotdat_World__CAMERA_H__

#include "cocos2d.h"

using namespace cocos2d;

class Camera
{
public:
	// If the camera is performing any operation, return true.
	bool IsBusy();

	// Move/Zoom the Camera over time.
	void PanToPosition(const Vec2& position, float seconds);
	void ZoomToScale(float scale, float seconds);

	// Expand/Contract the displayed area without changing
	// the scale directly.
	void ExpandToSize(float size, float seconds);

	// Stop the current operation immediately.
	void Stop();

	// Called every frame to update the Camera state
	// and modify the Viewport.  The dt value may 
	// be actual or fixed in a fixed timestep
	// system.
	void Update(float delta);
};

#endif /* defined(__com_dotdat_World__CAMERA_H__) */

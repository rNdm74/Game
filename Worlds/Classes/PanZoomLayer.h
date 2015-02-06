
// Created By GrowingDever 21th January 2014

#ifndef _PAN_ZOOM_LAYER_H_
#define _PAN_ZOOM_LAYER_H_

#include "cocos2d.h"

using namespace cocos2d;

class PanZoomLayer : public LayerColor
{
private:
	Array* _touches;
	Vec2 _beganTouchPoint;
	Vec2 _endedTouchPoint;
	Vec2 _deltaSum;
	Vec2 _prevDeltaPoint;
	double _accelerationFactor;
	//cc_timeval _timeStamp;
	Rect _panBoundsRect;
	float _maxScale;
	float _minScale;
	float _productFactor;

	bool _isHolding;


public:
	PanZoomLayer();
	virtual ~PanZoomLayer();

	static PanZoomLayer* create();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void update( float dt );

	virtual void onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent);
	virtual void onTouchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent);
	virtual void onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent);
	
	void onMouseDown(Event* mEvent);
	void onMouseUp(Event* mEvent);
	void onMouseMove(Event* mEvent);
	void onMouseScroll(Event* mEvent);

	virtual void setPosition( Vec2 position );
	virtual void setScale( float scale );

	void SetPanBoundsRect( Rect rect );
	void SetMaxScale( float maxScale );
	float GetMaxScale();
	void SetMinScale( float minScale );
	float GetMinScale();

	void Holding();
	void UnHolding();

	void SetProductFactor( float v );

};


#endif
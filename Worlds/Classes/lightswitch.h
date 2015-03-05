#ifndef  __FranticAlien__LightSwitch_H__
#define  __FranticAlien__LightSwitch_H__

#include "fsm.h"

struct Event { int state; };

enum States { OFF, ON };

struct LightSwitch {
    /// List of states
    typedef LIST2(OFF, ON) StateList;
    /// default template version for the event function
	template <int> int event() {  return 0; }
	template <int> void enter() {  }
	template <int> void exit() {  };
};

/// implement the specializations for states that require behaviour
template <> void LightSwitch::enter<ON>() {  };
template <> void LightSwitch::enter<OFF>() {  };

template <> void LightSwitch::exit<ON>() {  };
template <> void LightSwitch::exit<OFF>() {  };

#endif // __FranticAlien__LightSwitch_H__

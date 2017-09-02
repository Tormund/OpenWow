#include "stdafx.h"
#include "Timer.h"

std::map<UINT_PTR, Timer::Callback*> Timer::_timers;

bool operator==(const UINT_PTR& timerId, const Timer& timer) {
	return timer.getId() == timerId;
}

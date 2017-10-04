#include "stdafx.h"

// General
#include "WowTime.h"

void WowTime::Set(uint32_t _seconds)
{
	m_GameTime = _seconds;
}

void WowTime::Set(uint32_t _hour, uint32_t _minute)
{
	m_GameTime = _hour * C_Game_SecondsInHour + _minute * C_Game_SecondsInMinute;
}

void WowTime::Set(uint32_t _day, uint32_t _hour, uint32_t _minute)
{
	m_GameTime = _day * C_Game_SecondsInDay +  _hour * C_Game_SecondsInHour + _minute * C_Game_SecondsInMinute;
}

void WowTime::Tick()
{
	m_GameTime += 1;
}
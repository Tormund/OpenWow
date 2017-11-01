#pragma once

class WowTime
{
public:
	inline void Set(uint32 _seconds)
	{
		m_GameTime = _seconds;
	}

	inline void Set(uint32 _hour, uint32 _minute)
	{
		m_GameTime = _hour * C_Game_SecondsInHour + _minute * C_Game_SecondsInMinute;
	}

	inline void Set(uint32 _day, uint32 _hour, uint32 _minute)
	{
		m_GameTime = _day * C_Game_SecondsInDay + _hour * C_Game_SecondsInHour + _minute * C_Game_SecondsInMinute;
	}

	inline uint32 GetDay() const { return m_GameTime / C_Game_SecondsInDay; }
	inline uint32 GetTime() const { return m_GameTime % C_Game_SecondsInDay; }
	inline uint32 GetHour() const { return (m_GameTime % C_Game_SecondsInDay) / C_Game_SecondsInHour; }
	inline uint32 GetMinute() const { return (m_GameTime % C_Game_SecondsInDay) % C_Game_SecondsInHour; }

	void Tick()
	{
		m_GameTime += 1;
	}

private:
	uint32 m_GameTime; // In game seconds

private: // Consts
	const uint32 C_Game_SecondsInMinute = 2;
	const uint32 C_Game_MinutesInHour = 60;
	const uint32 C_Game_HoursInDay = 24;

	const uint32 C_Game_SecondsInHour = (C_Game_SecondsInMinute * C_Game_MinutesInHour); // 120
	const uint32 C_Game_SecondsInDay = (C_Game_SecondsInMinute * C_Game_MinutesInHour * C_Game_HoursInDay); // 2880
};
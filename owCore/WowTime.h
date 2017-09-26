#pragma once

class WowTime
{
public:
	void Set(uint32_t _seconds);
	void Set(uint32_t _hour, uint32_t _minute);
	void Set(uint32_t _day, uint32_t _hour, uint32_t _minute);
	inline uint32_t GetDay() const { return m_GameTime / C_Game_SecondsInDay; }
	inline uint32_t GetTime() const { return m_GameTime % C_Game_SecondsInDay; }
	inline uint32_t GetHour() const { return (m_GameTime % C_Game_SecondsInDay) / C_Game_SecondsInHour; }
	inline uint32_t GetMinute() const { return (m_GameTime % C_Game_SecondsInDay) % C_Game_SecondsInHour; }

private:
	uint32_t m_GameTime; // In game seconds

private: // Consts
	const uint32_t C_Game_SecondsInMinute = 2;
	const uint32_t C_Game_MinutesInHour = 60;
	const uint32_t C_Game_HoursInDay = 24;

	const uint32_t C_Game_SecondsInHour = (C_Game_SecondsInMinute * C_Game_MinutesInHour); // 120
	const uint32_t C_Game_SecondsInDay = (C_Game_SecondsInMinute * C_Game_MinutesInHour * C_Game_HoursInDay); // 2880
};
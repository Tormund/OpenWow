#pragma once

enum PerfomanceTimer : uint8_t
{
	PERF_MAP_CHUNK = 0,
	PERF_MAP_LOWRES_TILES = 1,
	PERF_GLOBAL_WMO = 2,
	PERF_MAP_MDXs = 3,
	PERF_MAP_LIQUID = 4,
	PERF_MAP_MH20 = 5,

	PERF_WMOs = 10,
	PERF_WMOs_DOODADS = 11,
	PERF_WMOs_LIQUIDS = 12,

	PERF_FRUSTRUM = 20,

	PERF_TEST = 63,
};

#define TIMERS_COUNT 64

class Perfomance
{
	CLASS_INSTANCE(Perfomance);

	Perfomance()
	{
		FrameBegin();
	}

	inline void FrameBegin()
	{
		for (uint8_t i = 0; i < TIMERS_COUNT; i++)
		{
			m_Timer[i] = 0.0;
			m_TimerCurrent[i] = 0.0;
			m_DrawCount[i] = 0;
		}
	}

	// Timers

	inline void Start(PerfomanceTimer _timer)
	{
		m_TimerCurrent[_timer] = glfwGetTime();
	}

	inline void Stop(PerfomanceTimer _timer)
	{
		m_TimerCurrent[_timer] =  glfwGetTime() - m_TimerCurrent[_timer];

		m_Timer[_timer] += m_TimerCurrent[_timer];
	}

	inline string Get(PerfomanceTimer _timer)
	{
		return to_string(static_cast<uint32_t>(m_Timer[_timer] * 1000.0)) + ", c: " + to_string(m_DrawCount[_timer]);
	}

	inline string Sum()
	{
		double timerSum = 0.0;
		uint32_t drawSum = 0;

		for (uint8_t i = 0; i < TIMERS_COUNT; i++)
		{
			timerSum += m_Timer[i];
			drawSum += m_DrawCount[i];
		}

		return "t: [" + to_string(static_cast<uint32_t>(timerSum * 1000.0)) + "] c: [" + to_string(drawSum) + "]";
	}

	// Statistics

	inline void Inc(PerfomanceTimer _timer)
	{
		m_DrawCount[_timer] += 1;
	}

private:
	double m_Timer[TIMERS_COUNT];
	double m_TimerCurrent[TIMERS_COUNT];
	uint32_t m_DrawCount[TIMERS_COUNT];
};

#define _Perfomance Perfomance::instance()
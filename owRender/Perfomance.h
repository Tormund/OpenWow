#pragma once

enum PerfomanceTimer : uint8_t
{
	// Map perfomance
	PERF_MAP = 0,
	PERF_MAP_CHUNK_GEOMETRY = 1,
	PERF_MAP_CHUNK_LIQUID = 2,
	PERF_MAP_CHUNK_MH20 = 3,
	PERF_MAP_LOWRESOLUTION = 4,
	
	// Global WMO
	PERF_MAP_MODELS_WMO_GLOBAL = 20,

	// WMOs
	PERF_MAP_MODELS_WMOs = 30,
	PERF_MAP_MODELS_WMOs_GEOMETRY = 31,
	PERF_MAP_MODELS_WMOs_DOODADS = 32,
	PERF_MAP_MODELS_WMOs_LIQUIDS = 33,

	// MDX (Map)
	PERF_MAP_MODELS_MDXs = 40,
	
	//

	PERF_FRUSTRUM = 60,
};

#define PERF_TIMERS_ENABLED
#define PERF_COUNTERS_ENABLED
#define PERF_TIMERS_COUNT 64
#define PERF_DELIM (static_cast<PerfomanceTimer>(PERF_TIMERS_COUNT + 1))

class Perfomance
{
	CLASS_INSTANCE(Perfomance);

	Perfomance()
	{
		FrameBegin();
	}

	inline void FrameBegin()
	{
		for (uint8_t i = 0; i < PERF_TIMERS_COUNT; i++)
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

	inline string GetTimer(PerfomanceTimer _timer)
	{
		return to_string(static_cast<uint32_t>(m_Timer[_timer] * 1000.0));
	}

	inline string GetInc(PerfomanceTimer _timer)
	{
		return to_string(m_DrawCount[_timer]);
	}

	inline string Sum()
	{
		double timerSum = 0.0;
		uint32_t drawSum = 0;

		for (uint8_t i = 0; i < PERF_TIMERS_COUNT; i++)
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

	// Draw
	void Draw(vec2 _startPoint);

private:
	double m_Timer[PERF_TIMERS_COUNT];
	double m_TimerCurrent[PERF_TIMERS_COUNT];
	uint32_t m_DrawCount[PERF_TIMERS_COUNT];
};

#define _Perfomance Perfomance::instance()

// Macros

#ifdef PERF_TIMERS_ENABLED
#define PERF_START(timer) _Perfomance->Start(timer);
#define PERF_STOP(timer) _Perfomance->Stop(timer);
#else
#define PERF_START(timer)
#define PERF_STOP(timer)
#endif

#ifdef PERF_COUNTERS_ENABLED
#define PERF_INC(timer) _Perfomance->Inc(timer);
#else
#define PERF_INC(timer)
#endif
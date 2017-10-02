#include "stdafx.h"

// General
#include "Perfomance.h"

// Additional
#include "Render.h"

struct PerfomanceDebugOutput
{
	PerfomanceTimer what;
	const char* descr;
	bool showInc;
};

const PerfomanceDebugOutput Messages[] =
{
	{PERF_MAP, "Map", false},

	{PERF_DELIM, "", false},

	{PERF_MAP_CHUNK_GEOMETRY, "Map Geom", true},
	{PERF_MAP_CHUNK_LIQUID, "Map Liquid", true},
	{PERF_MAP_CHUNK_MH20, "Map MH2O", true},
	{PERF_MAP_LOWRESOLUTION, "Map Low-res", true},

	{PERF_DELIM, "", false},

	// Global WMO
	{PERF_MAP_MODELS_WMO_GLOBAL, "Map Global WMO", true},

	{PERF_DELIM, "", false},

	// WMOs
	{PERF_MAP_MODELS_WMOs, "Map WMOs", true},
	{PERF_MAP_MODELS_WMOs_GEOMETRY, "Map WMOs Geom", true},
	{PERF_MAP_MODELS_WMOs_DOODADS, "Map WMOs Doodads", true},
	{PERF_MAP_MODELS_WMOs_LIQUIDS, "Map WMOs Liquids", true},

	{PERF_DELIM, "", false},

	// MDX (Map)
	{PERF_MAP_MODELS_MDXs, "Map MDXs", true}
};

void Perfomance::Draw(vec2 _startPoint)
{
	vec2 point = _startPoint;
	float diff = 15.0f;

	for (uint8_t i = 0; i < ARRAY_SIZE_IN_ELEMENTS(Messages); i++)
	{
		if (Messages[i].what != PERF_DELIM)
		{
			_Render->RenderText(point, Messages[i].descr + string(": t[") + _Perfomance->GetTimer(Messages[i].what) + (Messages[i].showInc ? "], c[" + _Perfomance->GetInc(Messages[i].what) + "]" : "]"));
		}

		point.y += diff;
	}

	_Render->RenderText(point, "SUMMA: " + _Perfomance->Sum());
}
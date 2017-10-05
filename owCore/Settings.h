#pragma once

class Settings
{
public:
	static void Init();

public:
	static inline void CalculateSquareDistances()
	{
		highresdistance2 = highresdistance * highresdistance;
		mapdrawdistance2 = mapdrawdistance * mapdrawdistance;
		modeldrawdistance2 = modeldrawdistance * modeldrawdistance;
		doodaddrawdistance2 = doodaddrawdistance * doodaddrawdistance;
		culldistance2 = culldistance * culldistance;
	}

	static inline void CalculateAspectFactor()
	{
		aspectFactor = 1.0f;
		aspectRatio = windowSizeX / windowSizeY;
		if (aspectRatio >= 1.0f)
		{
			aspectFactor = aspectRatio;
		}
	}

	template<typename T>
	static inline void SetWindowSize(T _x, T _y)
	{
		windowSizeX = static_cast<float>(_x);
		windowSizeY = static_cast<float>(_y);
		CalculateAspectFactor();
	}

	static inline vec2 GetWindowSize()
	{
		return vec2(windowSizeX, windowSizeY);
	}

	//

public:
	// Render settings
	static float windowSizeX;
	static float windowSizeY;
	static float aspectRatio;
	static float aspectFactor;

	// Distances
	static float mapdrawdistance;
	static float modeldrawdistance;
	static float doodaddrawdistance;
	static float highresdistance;
	static float culldistance;

	static float mapdrawdistance2;
	static float modeldrawdistance2;
	static float doodaddrawdistance2;
	static float highresdistance2;
	static float culldistance2;

	static float fogdistance;

	// Drawing
	static bool draw_map_chunk;
	static bool draw_map_wmo;
	static bool draw_map_wmo_doodads;
	static bool draw_map_mdx;

	static bool loading;
	static bool drawhighres;
	static bool drawfog;
	static bool drawColors;

	// Quality
	static bool uselowlod;
	static bool enableMCCV;
	static bool enableMCLV;

	// Perfomance
	static bool disable_pipeline;
};
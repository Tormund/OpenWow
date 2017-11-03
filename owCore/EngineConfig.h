#pragma once

struct EngineOptions
{
	enum List
	{
		WindowsWidth = 0,
		WindowsHeight,
		AspectRatio,
		AspectFactor,

		MapDrawDistance,
		WMODrawDistance,
		WMODoodadDrawDistance,
		MDXDrawDistance,
		modeldrawdistance
	};
};

class EngineConfig
{
public:
	EngineConfig();
	~EngineConfig();

public:
	inline void CalculateSquareDistances()
	{
		highresdistance2 = highresdistance * highresdistance;
		mapdrawdistance2 = mapdrawdistance * mapdrawdistance;
		modeldrawdistance2 = modeldrawdistance * modeldrawdistance;
		doodaddrawdistance2 = doodaddrawdistance * doodaddrawdistance;
		culldistance2 = culldistance * culldistance;
	}

	inline void CalculateAspectFactor()
	{
		aspectFactor = 1.0f;
		aspectRatio = windowSizeX / windowSizeY;
		if (aspectRatio >= 1.0f)
		{
			aspectFactor = aspectRatio;
		}
	}

	//

	template<typename T>
	inline void SetWindowSize(T _x, T _y)
	{
		windowSizeX = static_cast<float>(_x);
		windowSizeY = static_cast<float>(_y);
		CalculateAspectFactor();
	}

	inline vec2 GetWindowSize()
	{
		return vec2(windowSizeX, windowSizeY);
	}

	//

public:
	// Render settings
	float windowSizeX;
	float windowSizeY;
	float aspectRatio;
	float aspectFactor;

	// Distances
	float mapdrawdistance;
	float modeldrawdistance;
	float doodaddrawdistance;
	float highresdistance;
	float culldistance;

	float mapdrawdistance2;
	float modeldrawdistance2;
	float doodaddrawdistance2;
	float highresdistance2;
	float culldistance2;

	float fogdistance;

	// Drawing
	bool draw_map_chunk;
	bool draw_map_wmo;
	bool draw_map_wmo_doodads;
	bool draw_map_mdx;

	bool loading;
	bool drawhighres;
	bool drawfog;
	bool drawColors;

	// Quality
	bool uselowlod;
	bool enableMCCV;
	bool enableMCLV;

	// Perfomance

};
#pragma once

class gLightDBRecord;

class SkyColor
{
public:
	SkyColor(int _time, uint32_t _color)
	{
		time = _time;
		color = fromRGB(_color);
	}

public:
	vec3 color;
	int time;
};

//

class Sky
{
public:
	vec3 position;
	float radiusInner;
	float radiusOuter;

	Sky(gLightDBRecord* data);

	vector<SkyColor> colorRows[36];
	int mmin[36];

	vec3 colorFor(int r, int t) const;

	float weight;
	bool global;

	bool operator<(const Sky& s) const
	{
		if (global)
			return false;
		else if (s.global)
			return true;
		else
			return radiusOuter < s.radiusOuter;
	}
};

enum SkyColorNames
{
	LIGHT_GLOBAL_DIFFUSE,
	LIGHT_GLOBAL_AMBIENT,
	SKY_COLOR_0,
	SKY_COLOR_1,
	SKY_COLOR_2,
	SKY_COLOR_3,
	SKY_COLOR_4,
	FOG_COLOR,
	SKY_UNKNOWN_1,
	SUN_COLOR,
	SUN_HALO_COLOR,
	SKY_UNKNOWN_2,
	CLOUD_COLOR,
	SKY_UNKNOWN_3,
	SKY_UNKNOWN_4,
	WATER_COLOR_DARK,
	WATER_COLOR_LIGHT,
	SHADOW_COLOR
};







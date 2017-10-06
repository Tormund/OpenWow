#pragma once

class SkyColor
{
public:
	SkyColor(int _time, uint32 _color)
	{
		time = _time;
		color = fromRGB(_color);
	}

public:
	vec3 color;
	int time;
};

//

#include "../DBC__Storage.h"

class Sky
{
public:
	vec3 position;
	float radiusInner;
	float radiusOuter;

	Sky(DBC_LightRecord* data);

	vector<SkyColor> colorRows[18];
	int mmin[18];

	vec3 colorFor(int r, int t) const;

	float weight;
	bool global;

	inline bool operator<(const Sky& s) const
	{
		if (global)
			return false;
		else if (s.global)
			return true;
		else
			return radiusOuter < s.radiusOuter;
	}
};









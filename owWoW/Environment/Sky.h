#pragma once

#define SKY_COLORSCOUNT 18

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
    Sky(DBC_LightRecord* data);
    vec3 colorFor(int r, int t) const;

public:
	vec3 position;
	float radiusInner;
	float radiusOuter;

	vector<SkyColor> colorRows[18];
	int mmin[18];

	float weight;
	bool global;
};









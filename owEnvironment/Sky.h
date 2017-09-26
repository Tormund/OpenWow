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









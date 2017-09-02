#pragma once

#include "model.h"

class SkyColor {
public:
	SkyColor(int _time, int _color) {
		time = _time;
		color.z = ((_color & 0x0000ff)) / 255.0f;
		color.y = ((_color & 0x00ff00) >> 8) / 255.0f;
		color.x = ((_color & 0xff0000) >> 16) / 255.0f;
	}

	vec3 color;
	int time;
};



class Sky {
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

	bool operator<(const Sky& s) const {
		if(global)
			return false;
		else if(s.global)
			return true;
		else
			return radiusOuter < s.radiusOuter;
	}
};

enum SkyColorNames {
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

class MapSkies {
	vector<Sky*> skies;
	Model* stars;

public:
	vec3 colorSet[18];

	void CalculateSkiesWeights(cvec3 pos);
	void initSky(cvec3 pos, int t);

	void draw();

	bool drawSky(cvec3 pos);
	bool hasSkies() { return !skies.empty(); }

public:
	MapSkies(int mapid);
	~MapSkies();
};





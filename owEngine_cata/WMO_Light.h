#pragma once

class Vec4D;

struct WMOLight {
	uint8_t LightType;
	uint8_t type;
	uint8_t useAtten;
	uint8_t pad;
	uint32_t color;
	vec3 pos;
	float intensity;
	float attenStart;
	float attenEnd;
	float unk[4];

	// struct read stop
	Vec4D fcolor;

	void init(File &f);
	void setup(GLint light);

	static void setupOnce(GLint light, vec3 dir, Vec4D lcol);
};
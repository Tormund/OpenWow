#pragma once

class Model;

class DoodadInstance
{
public:
	DoodadInstance(Model* m, File& f);
	void Draw(cvec3 ofs, float roll);

public: // TODO private
	Model* model;

	int id;

	vec3 pos, dir;
	uint32_t scale;

	float frot, w, sc;

	int light;
	vec3 ldir;
	vec4 lcol;
};
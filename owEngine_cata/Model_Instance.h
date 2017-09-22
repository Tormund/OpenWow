#pragma once

class ModelInstance
{
public:
	ModelInstance() {}
	ModelInstance(Model *m, File& f);

	void draw();

	void InitAsDoodad(Model * m, File& f);
	void DrawAsDoodad(cvec3 ofs, float roll);

public: // TODO private
	Model *model;

	int id;

	vec3 pos, dir;
	uint32_t scale;

	float frot, w, sc;

	int light;
	vec3 ldir;
	vec4 lcol;
};
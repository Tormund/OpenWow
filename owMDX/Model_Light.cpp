#include "stdafx.h"

// General
#include "Model_Light.h"

// Additional
#include "ModelsUtils.h"

void ModelLight::init(File &f, ModelLightDef &mld, uint32_t *global)
{
	tpos = pos = fixCoordSystem(mld.pos);
	tdir = dir = vec3(0, 1, 0); // no idea
	type = mld.type;
	parent = mld.bone;
	ambColor.init(mld.ambColor, f, global);
	ambIntensity.init(mld.ambIntensity, f, global);
	diffColor.init(mld.color, f, global);
	diffIntensity.init(mld.intensity, f, global);
}

void ModelLight::setup(int time, GLuint l)
{
	vec4 ambcol(ambColor.getValue(0, time) * ambIntensity.getValue(0, time), 1.0f);
	vec4 diffcol(diffColor.getValue(0, time) * diffIntensity.getValue(0, time), 1.0f);
	vec4 p;

	if (type == MODELLIGHT_DIRECTIONAL)
	{
		// directional
		p = vec4(tdir, 0.0f);
	}
	else if (type == MODELLIGHT_POINT)
	{
		// point
		p = vec4(tpos, 1.0f);
	}
	else
	{
		p = vec4(tpos, 1.0f);
		Debug::Info("Error: Light type %d is unknown.", type);
	}

	//Debug::Info("Light %d (%f,%f,%f) (%f,%f,%f) [%f,%f,%f]", l-GL_LIGHT4, ambcol.x, ambcol.y, ambcol.z, diffcol.x, diffcol.y, diffcol.z, p.x, p.y, p.z);
	glLightfv(l, GL_POSITION, glm::value_ptr(p));
	glLightfv(l, GL_DIFFUSE, glm::value_ptr(diffcol));
	glLightfv(l, GL_AMBIENT, glm::value_ptr(ambcol));
	glEnable(l);
}
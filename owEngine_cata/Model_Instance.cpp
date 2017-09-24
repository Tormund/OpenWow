#include "stdafx.h"

// General
#include "Model_Instance.h"

// Additional
#include "World.h"

ModelInstance::ModelInstance(Model* m, File& f) : model(m)
{
	//uint32_t d1;
	f.SeekRelative(4);

	float ff[3]; 

	f.ReadBytes(ff, 12);// position
	pos = vec3(ff[0], ff[1], ff[2]);

	f.ReadBytes(ff, 12); // direction
	dir = vec3(ff[0], ff[1], ff[2]);

	f.ReadBytes(&scale, 4);
	// scale factor - divide by 1024. blizzard devs must be on crack, why not just use a float?
	sc = scale / 1024.0f;
}

void ModelInstance::draw()
{
	float dist = glm::length(pos - _Camera->Position) - model->rad;

	if (dist > _WowSettings->modeldrawdistance)
	{
		return;
	}

	if (!_Render->frustum.intersectsSphere(pos, model->rad*sc))
	{
		return;
	}

	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, pos.z);

		glRotatef(dir.y - 90.0f, 0, 1, 0);
		glRotatef(-dir.x, 0, 0, 1);
		glRotatef(dir.z, 1, 0, 0);

		glScalef(sc, sc, sc);

		model->draw();
	}
	glPopMatrix();
}

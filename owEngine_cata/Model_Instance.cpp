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

	if (!_World->frustum.intersectsSphere(pos, model->rad*sc))
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

void ModelInstance::InitAsDoodad(Model * m, File& f)
{
	// MODD
	model = m;

	float ff[3], temp;
	f.ReadBytes(ff, 12); // Position (X,Z,-Y)

	pos = vec3(ff[0], ff[1], ff[2]);
	temp = pos.z;
	pos.z = -pos.y;
	pos.y = temp;

	f.ReadBytes(&w, 4); // W component of the orientation quaternion
	f.ReadBytes(ff, 12); // X, Y, Z components of the orientaton quaternion

	dir = vec3(ff[0], ff[1], ff[2]);
	f.ReadBytes(&sc, 4); // Scale factor
	uint32_t d1;
	f.ReadBytes(&d1, 4); // (B,G,R,A) Lightning-color. 
	lcol = fromARGB(d1);
}

void ModelInstance::DrawAsDoodad(cvec3 ofs, float roll)
{
	vec3 tpos(ofs + pos);
	rotate(ofs.x, ofs.z, &tpos.x, &tpos.z, roll * PI / 180.0f);

	if (glm::length2(tpos - _Camera->Position) > (_WowSettings->doodaddrawdistance2 * model->rad * sc))
	{
		return;
	}

	if (!_World->frustum.intersectsSphere(tpos, model->rad*sc))
	{
		return;
	}

	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, pos.z);

		vec3 vdir(-dir.z, dir.x, dir.y);
		glQuaternionRotate(vdir, w);

		glScalef(sc, -sc, -sc);

		model->draw();
	}
	glPopMatrix();
}



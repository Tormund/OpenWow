#include "stdafx.h"

// General
#include "DoodadInstance.h"

DoodadInstance::DoodadInstance(Model * m, File& f)
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

void DoodadInstance::Draw(cvec3 ofs, float roll)
{
	vec3 tpos(ofs + pos);
	rotate(ofs.x, ofs.z, &tpos.x, &tpos.z, roll * PI / 180.0f);

	if (glm::length2(tpos - _Camera->Position) > (_Settings->doodaddrawdistance2 * model->rad * sc))
	{
		return;
	}

	if (!_Render->frustum.intersectsSphere(tpos, model->rad*sc))
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



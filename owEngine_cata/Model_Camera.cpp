#include "stdafx.h"

// General
#include "Model_Camera.h"

// Additional
#include "ModelsUtils.h"

void ModelCamera::init(File& f, ModelCameraDef& mcd, uint32_t * global)
{
	ok = true;
	nearclip = mcd.nearclip;
	farclip = mcd.farclip;
	//fov = mcd.fov;
	pos = fixCoordSystem(mcd.pos);
	target = fixCoordSystem(mcd.target);
	tPos.init(mcd.transPos, f, global);
	tTarget.init(mcd.transTarget, f, global);
	rot.init(mcd.rot, f, global);
	tPos.fix(fixCoordSystem);
	tTarget.fix(fixCoordSystem);
}

void ModelCamera::setup(int time)
{
	if (!ok) return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov * 34.5f, _Render->GetAspectRatio(), nearclip, farclip);

	vec3 p = pos + tPos.getValue(0, time);
	vec3 t = target + tTarget.getValue(0, time);

	vec3 u(0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(p.x, p.y, p.z, t.x, t.y, t.z, u.x, u.y, u.z);
	//float roll = rot.getValue(0, time) / PI * 180.0f;
	//glRotatef(roll, 0, 0, 1);
}
#include "stdafx.h"

// General
#include "Model_Camera.h"

// Additional
#include "ModelsUtils.h"

void ModelCamera::init(File& f, M2Camera& mcd, uint32_t* global)
{
	nearclip = mcd.near_clip;
	farclip = mcd.far_clip;

	pos = fixCoordSystem(mcd.position_base);
	target = fixCoordSystem(mcd.target_position_base);

	tPos.init(mcd.positions, f, global);
	tPos.fix(fixCoordSystem);

	tTarget.init(mcd.target_position, f, global);
	tTarget.fix(fixCoordSystem);

	tRoll.init(mcd.roll, f, global);
	tFov.init(mcd.FoV, f, global);
}

void ModelCamera::setup(int time)
{
	fov = tFov.getValue(0, time) * 34.5f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, _Render->GetAspectRatio(), nearclip, farclip);

	vec3 p = pos + tPos.getValue(0, time);
	vec3 t = target + tTarget.getValue(0, time);
	vec3 u(0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(p.x, p.y, p.z, t.x, t.y, t.z, u.x, u.y, u.z);

	roll = tRoll.getValue(0, time) / PI * 180.0f;
	glRotatef(roll, 0, 0, 1);
}
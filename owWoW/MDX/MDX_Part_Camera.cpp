#include "../stdafx.h"

// General
#include "MDX_Part_Camera.h"

void MDX_Part_Camera::init(File& f, M2Camera& mcd, uint32* global)
{
	nearclip = mcd.near_clip;
	farclip = mcd.far_clip;

	pos = From_XYZ_To_XZminusY_RET(mcd.position_base);
	target = From_XYZ_To_XZminusY_RET(mcd.target_position_base);

	tPos.init(mcd.positions, f, global);
	tPos.fix(From_XYZ_To_XZminusY_RET);

	tTarget.init(mcd.target_position, f, global);
	tTarget.fix(From_XYZ_To_XZminusY_RET);

	tRoll.init(mcd.roll, f, global);
	tFov.init(mcd.FoV, f, global);
}

void MDX_Part_Camera::setup(int time)
{
	fov = tFov.getValue(0, time) * 34.5f;

	vec3 p = pos + tPos.getValue(0, time);
	vec3 t = target + tTarget.getValue(0, time);
	vec3 u(0, 1, 0);

	roll = tRoll.getValue(0, time) / PI * 180.0f;

	camera.setupViewParams(fov, _Config.aspectRatio, nearclip, farclip);

	camera.Position = p;
	camera.Direction = t;
	camera.CameraUp = u;

    camera.Update(true);
}
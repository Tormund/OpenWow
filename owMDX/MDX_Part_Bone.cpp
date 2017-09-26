#include "stdafx.h"

// General
#include "MDX_Part_Bone.h"

void MDX_Part_Bone::init(File& f, M2CompBone& b, uint32_t* global, File* animfiles)
{
	parent = b.parent_bone;
	pivot = From_XYZ_To_XZminusY_RET(b.pivot);
	billboard = (b.flags.spherical_billboard);

	trans.init(b.translation, f, global, animfiles);
	roll.init(b.rotation, f, global, animfiles);
	scale.init(b.scale, f, global, animfiles);

	trans.fix(From_XYZ_To_XZminusY_RET);
	roll.fix(fixCoordSystemQuat);
	scale.fix(fixCoordSystem2);

	/*if (b.flags.spherical_billboard)
		Debug::Error("SPHERE!!!!!!");

	if (b.flags.cylindrical_billboard_lock_x)
	{
		Debug::Error("b.flags.cylindrical_billboard_lock_x");

		if (b.flags.spherical_billboard)
			Debug::Error("SHEEETT!!!!");
	}

	if (b.flags.cylindrical_billboard_lock_y)
	{
		Debug::Error("b.flags.cylindrical_billboard_lock_y");

		if (b.flags.spherical_billboard)
			Debug::Error("SHEEETT!!!!");
	}

	if (b.flags.cylindrical_billboard_lock_z)
	{
		Debug::Error("b.flags.cylindrical_billboard_lock_z");

		if (b.flags.spherical_billboard)
			Debug::Error("SHEEETT!!!!");
	}*/
}

void MDX_Part_Bone::calcMatrix(MDX_Part_Bone* allbones, int anim, int time)
{
	if (calc)
	{
		return;
	}

	Matrix m;
	Quaternion q;

	bool tr = roll.uses(anim) || scale.uses(anim) || trans.uses(anim) || billboard;
	if (tr)
	{
		m.translation(pivot);

		if (trans.uses(anim))
		{
			vec3 tr = trans.getValue(anim, time);
			m *= Matrix::newTranslation(tr);
		}

		if (roll.uses(anim))
		{
			q = roll.getValue(anim, time);
			m *= Matrix::newQuatRotate(q);
		}

		if (scale.uses(anim))
		{
			vec3 sc = scale.getValue(anim, time);
			m *= Matrix::newScale(sc);
		}

		if (billboard)
		{
			mat4 res = (*_Pipeline->GetVW());

			vec3 vRight = vec3(res[0][0], res[1][0], res[2][0]);
			vec3 vUp = vec3(res[0][1], res[1][1], res[2][1]);
			vec3 vForward = vec3(res[0][2], res[1][2], res[2][2]);
			
			vRight = vRight * -1.0f;

			m.m[0][1] = vUp.x;
			m.m[1][1] = vUp.y;
			m.m[2][1] = vUp.z;


			m.m[0][2] = vRight.x;
			m.m[1][2] = vRight.y;
			m.m[2][2] = vRight.z;
		}

		m *= Matrix::newTranslation(pivot * -1.0f);
	}
	else
	{
		m.unit();
	}

	if (parent >= 0)
	{
		allbones[parent].calcMatrix(allbones, anim, time);
		mat = allbones[parent].mat * m;
	}
	else
	{
		mat = m;
	}

	// transform matrix for normal vectors ... ??
	if (roll.uses(anim))
	{
		if (parent >= 0)
		{
			mrot = allbones[parent].mrot * Matrix::newQuatRotate(q);
		}
		else mrot = Matrix::newQuatRotate(q);
	}
	else
	{
		mrot.unit();
	}

	transPivot = mat * pivot;

	calc = true;
}

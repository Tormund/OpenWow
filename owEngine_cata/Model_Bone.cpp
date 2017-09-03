#include "stdafx.h"

// General
#include "Model_Bone.h"

// Additional
#include "ModelsUtils.h"

void Bone::init(File &f, ModelBoneDef &b, uint32_t *global, File *animfiles)
{
	parent = b.parent;
	pivot = fixCoordSystem(b.pivot);
	billboard = (b.flags & MODELBONE_BILLBOARD) != 0;

	trans.init(b.translation, f, global, animfiles);
	rot.init(b.rotation, f, global, animfiles);
	scale.init(b.scaling, f, global, animfiles);

	trans.fix(fixCoordSystem);
	rot.fix(fixCoordSystemQuat);
	scale.fix(fixCoordSystem2);
}

void Bone::calcMatrix(Bone* allbones, int anim, int time)
{
	if (calc)
	{
		return;
	}

	Matrix m;
	Quaternion q;

	bool tr = rot.uses(anim) || scale.uses(anim) || trans.uses(anim) || billboard;
	if (tr)
	{
		m.translation(pivot);

		if (trans.uses(anim))
		{
			vec3 tr = trans.getValue(anim, time);
			m *= Matrix::newTranslation(tr);
		}
		if (rot.uses(anim))
		{
			q = rot.getValue(anim, time);
			m *= Matrix::newQuatRotate(q);
		}
		if (scale.uses(anim))
		{
			vec3 sc = scale.getValue(anim, time);
			m *= Matrix::newScale(sc);
		}
		if (billboard)
		{
			float modelview[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

			vec3 vRight = vec3(modelview[0], modelview[4], modelview[8]);
			vec3 vUp = vec3(modelview[1], modelview[5], modelview[9]); // Spherical billboarding
																	   //vec3 vUp = vec3(0,1,0); // Cylindrical billboarding
			vRight = vRight * -1.0f;
			m.m[0][2] = vRight.x;
			m.m[1][2] = vRight.y;
			m.m[2][2] = vRight.z;
			m.m[0][1] = vUp.x;
			m.m[1][1] = vUp.y;
			m.m[2][1] = vUp.z;
		}

		m *= Matrix::newTranslation(pivot*-1.0f);

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
	if (rot.uses(anim))
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

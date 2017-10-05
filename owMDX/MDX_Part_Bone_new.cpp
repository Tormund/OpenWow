#include "stdafx.h"

// General
#include "MDX_Part_Bone.h"

void MDX_Part_Bone::init(File& f, M2CompBone &b, uint32 *global, File *animfiles)
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
}

void MDX_Part_Bone::calcMatrix(MDX_Part_Bone* allbones, uint32 _animationIndex, int time)
{
	if (calc)
	{
		return;
	}

	mat4 mNew;
	quat q;

	bool tr = roll.uses(_animationIndex) || scale.uses(_animationIndex) || trans.uses(_animationIndex) || billboard;
	if (tr)
	{
		mNew = glm::translate(pivot);

		if (trans.uses(_animationIndex))
		{
			vec3 tr = trans.getValue(_animationIndex, time);
			mNew *= glm::translate(tr);
		}

		if (roll.uses(_animationIndex))
		{
			q = roll.getValue(_animationIndex, time);
			//mNew *= glm::toMat4(q);

			vec3 eulAngle = glm::eulerAngles(q);

			mNew *= glm::rotate(glm::eulerAngles(q).x, vec3(1.0f, 0.0f, 0.0f));
			mNew *= glm::rotate(glm::eulerAngles(q).y, vec3(0.0f, 1.0f, 0.0f));
			mNew *= glm::rotate(glm::eulerAngles(q).z, vec3(0.0f, 0.0f, 1.0f));
		}

		if (scale.uses(_animationIndex))
		{
			vec3 sc = scale.getValue(_animationIndex, time);
			mNew *= glm::scale(sc);
		}

		if (billboard)
		{
			//float modelview[16];
			//glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

			float modelview[16];
			memcpy(&modelview, glm::value_ptr(_PipelineGlobal->GetWorld()), 16 * sizeof(float));

			vec3 vRight = vec3(modelview[0], modelview[4], modelview[8]);
			vec3 vUp = vec3(modelview[1], modelview[5], modelview[9]); // Spherical billboarding
																	   //vec3 vUp = vec3(0,1,0); // Cylindrical billboarding
			vRight = vRight * -1.0f;

			mNew[0][2] = vRight.x;
			mNew[1][2] = vRight.y;
			mNew[2][2] = vRight.z;

			mNew[0][1] = vUp.x;
			mNew[1][1] = vUp.y;
			mNew[2][1] = vUp.z;
		}

		mNew *= glm::translate(pivot * -1.0f);

	}
	else
	{
		mNew = glm::mat4(1.0f);
	}

	if (parent >= 0)
	{
		allbones[parent].calcMatrix(allbones, _animationIndex, time);
		mat = allbones[parent].mat * mNew;
	}
	else
	{
		mat = mNew;
	}

	//-----------------------------

	// transform matrix for normal vectors ... ??
	if (roll.uses(_animationIndex))
	{
		if (parent >= 0)
		{
			mRot = allbones[parent].mRot * glm::toMat4(q);
		}
		else
		{
			mRot = glm::toMat4(q);
		}
	}
	else
	{
		mRot = glm::mat4(1.0f);
	}

	//------------------------------

	transPivot = mat * pivot;

	calc = true;
}

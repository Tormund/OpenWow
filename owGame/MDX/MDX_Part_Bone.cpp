#include "../stdafx.h"

// General
#include "MDX_Part_Bone.h"

void MDX_Part_Bone::init(File& f, M2CompBone& b, uint32* global, File* animfiles)
{
	parent = b.parent_bone;
	pivot = From_XYZ_To_XZminusY_RET(b.pivot);
	billboard = (b.flags.spherical_billboard);

	trans.init(b.translation, f, global, animfiles);
	roll.init(b.rotation, f, global, animfiles);
	scale.init(b.scale, f, global, animfiles);

	trans.fix(From_XYZ_To_XZminusY_RET);
	roll.fix(From_XYZW_To_XZminusYW_RET);
	scale.fix(From_XYZ_To_XZY_RET);
}

void MDX_Part_Bone::calcMatrix(MDX_Part_Bone* allbones, int anim, int time)
{
	if (m_IsCalculated)
	{
		return;
	}

	mat4 m;

	bool tr = roll.uses(anim) || scale.uses(anim) || trans.uses(anim) || billboard;
	if (tr)
	{
		m.translate(pivot);

		if (trans.uses(anim))
		{
			vec3 tr = trans.getValue(anim, time);
			m.translate(tr);
		}

		if (roll.uses(anim))
		{
			quat q = roll.getValue(anim, time);
			m.rotate(q);

			if (parent >= 0)
			{
				m_RotationMatrix = allbones[parent].m_RotationMatrix * mat4::RotMat(q);
			}
			else
			{
				m_RotationMatrix = mat4::RotMat(q);
			}
		}

		if (scale.uses(anim))
		{
			vec3 sc = scale.getValue(anim, time);
			m.scale(sc);
		}

		if (billboard) // TODO
		{
			//float modelview[16];
			//memcpy(&modelview, &_PipelineGlobal->GetVW().x[0], 16 * sizeof(float));

			vec3 vRight = vec3(_PipelineGlobal->GetVW().x[0], _PipelineGlobal->GetVW().x[4], _PipelineGlobal->GetVW().x[8]);
			vec3 vUp = vec3(_PipelineGlobal->GetVW().x[1], _PipelineGlobal->GetVW().x[5], _PipelineGlobal->GetVW().x[9]);
																	   //vec3 vUp = vec3(0,1,0); // Cylindrical billboarding
			vRight = vRight * -1.0f;

			m.c[1][0] = vUp.x;
			m.c[1][1] = vUp.y;
			m.c[1][2] = vUp.z;

			m.c[2][0] = vRight.x;
			m.c[2][1] = vRight.y;
			m.c[2][2] = vRight.z;
		}

		m.translate(pivot * -1.0f);
	}

	if (parent >= 0)
	{
		allbones[parent].calcMatrix(allbones, anim, time);
		m_TransformMatrix = allbones[parent].m_TransformMatrix * m;
	}
	else
	{
		m_TransformMatrix = m;
	}

	transPivot = m_TransformMatrix * pivot;

	m_IsCalculated = true;
}

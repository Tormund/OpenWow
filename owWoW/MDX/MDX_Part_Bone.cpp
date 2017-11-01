#include "../stdafx.h"

// General
#include "MDX_Part_Bone.h"

void MDX_Part_Bone::init(File& f, M2CompBone& b, uint32* global, File* animfiles)
{
	parent = b.parent_bone;
	pivot = From_XYZ_To_XZminusY_RET(b.pivot);

	m_BillboardType = BillboardType::BILLBOARD_DISABLED;

	//if (b.translation.interpolation_type || b.rotation.interpolation_type || b.scale.interpolation_type)
	{
		if (b.flags.spherical_billboard)
		{
			m_BillboardType = BillboardType::BILLBOARD_SPHERICAL;
		}
		else if (b.flags.cylindrical_billboard_lock_x)
		{
			m_BillboardType = BillboardType::BILLBOARD_CYLINDRICAL_X;
		}
		else if (b.flags.cylindrical_billboard_lock_y)
		{
			m_BillboardType = BillboardType::BILLBOARD_CYLINDRICAL_Y;
		}
		else if (b.flags.cylindrical_billboard_lock_z)
		{
			m_BillboardType = BillboardType::BILLBOARD_CYLINDRICAL_Z;
		}
	}

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

	if (roll.uses(anim) || scale.uses(anim) || trans.uses(anim) || (m_BillboardType))
	{
		m.translate(pivot);

		if (trans.uses(anim))
		{
			m.translate(trans.getValue(anim, time));
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
			m.scale(scale.getValue(anim, time));
		}

		if (m_BillboardType)
		{
			if (parent != -1)
			{
				if (allbones[parent].m_BillboardType)
				{
					goto xxx;
				}
			}

			mat4 worldPrevMatrix = m * _PipelineGlobal->GetWorld();

			bool shitMatrix = false;
			float scaleX = vec3(worldPrevMatrix.x[0], worldPrevMatrix.x[1], worldPrevMatrix.x[2]).length();
			float scaleY = vec3(worldPrevMatrix.x[4], worldPrevMatrix.x[5], worldPrevMatrix.x[6]).length();
			float scaleZ = vec3(worldPrevMatrix.x[8], worldPrevMatrix.x[9], worldPrevMatrix.x[10]).length();

			mat4 matrix = _PipelineGlobal->GetView();
			if (!shitMatrix)
			{
				matrix *= worldPrevMatrix;
			}

			// Set vectors
			vec3 vUp;
			vec3 vRight = (vec3(matrix.c[0][0], matrix.c[1][0], matrix.c[2][0]) / scaleX) * -1.0f;

			// Calc billboard type
			switch (m_BillboardType)
			{
				case BillboardType::BILLBOARD_SPHERICAL:
				vUp = vec3(matrix.c[0][1], matrix.c[1][1], matrix.c[2][1]) / scaleY;
				break;

				case BillboardType::BILLBOARD_CYLINDRICAL_X:
				vUp = vec3(1, 0, 0);
				break;

				case BillboardType::BILLBOARD_CYLINDRICAL_Y:
				vUp = vec3(0, 0, 1);
				break;

				case BillboardType::BILLBOARD_CYLINDRICAL_Z:
				vUp = vec3(0, 1, 0);
				break;

				//default: vUp = vec3(0, 1, 0);
			}


			//vec3 vForward = vec3(matrix.c[0][2], matrix.c[1][2], matrix.c[2][2]) / scaleZ;
			//m.c[0][0] = vForward.x;
			//m.c[0][1] = vForward.y;
			//m.c[0][2] = vForward.z;

			m.c[1][0] = vUp.x;
			m.c[1][1] = vUp.y;
			m.c[1][2] = vUp.z;

			m.c[2][0] = vRight.x;
			m.c[2][1] = vRight.y;
			m.c[2][2] = vRight.z;
		}
xxx:

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

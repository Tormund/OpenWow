#include "stdafx.h"

// General
#include "Frustum.h"

// Additional
#include "Perfomance.h"
#include "Pipeline.h"


enum Directions
{
	FRIGHT, 
	FLEFT, 
	FBOTTOM, 
	FTOP, 
	FBACK, 
	FFRONT
};

void Frustum::retrieve()
{
	_Perfomance->Start(PERF_FRUSTRUM);

	float mat[16];
	memcpy(&mat, glm::value_ptr(_PipelineGlobal->GetPV()), 16 * sizeof(float));

	planes[FRIGHT].a = mat[3] - mat[0];
	planes[FRIGHT].b = mat[7] - mat[4];
	planes[FRIGHT].c = mat[11] - mat[8];
	planes[FRIGHT].d = mat[15] - mat[12];
	planes[FRIGHT].normalize();

	planes[FLEFT].a = mat[3] + mat[0];
	planes[FLEFT].b = mat[7] + mat[4];
	planes[FLEFT].c = mat[11] + mat[8];
	planes[FLEFT].d = mat[15] + mat[12];
	planes[FLEFT].normalize();

	planes[FBOTTOM].a = mat[3] + mat[1];
	planes[FBOTTOM].b = mat[7] + mat[5];
	planes[FBOTTOM].c = mat[11] + mat[9];
	planes[FBOTTOM].d = mat[15] + mat[13];
	planes[FBOTTOM].normalize();

	planes[FTOP].a = mat[3] - mat[1];
	planes[FTOP].b = mat[7] - mat[5];
	planes[FTOP].c = mat[11] - mat[9];
	planes[FTOP].d = mat[15] - mat[13];
	planes[FTOP].normalize();

	planes[FBACK].a = mat[3] - mat[2];
	planes[FBACK].b = mat[7] - mat[6];
	planes[FBACK].c = mat[11] - mat[10];
	planes[FBACK].d = mat[15] - mat[14];
	planes[FBACK].normalize();

	planes[FFRONT].a = mat[3] + mat[2];
	planes[FFRONT].b = mat[7] + mat[6];
	planes[FFRONT].c = mat[11] + mat[10];
	planes[FFRONT].d = mat[15] + mat[14];
	planes[FFRONT].normalize();

	_Perfomance->Stop(PERF_FRUSTRUM);
}

bool Frustum::contains(cvec3 v) const
{
	_Perfomance->Start(PERF_FRUSTRUM);

	for (uint8_t i = 0; i < 6; i++)
	{
		if (!planes[i].Contains(v))
		{
			_Perfomance->Stop(PERF_FRUSTRUM);
			return false;
		}
	}

	_Perfomance->Stop(PERF_FRUSTRUM);
	return true;
}

bool Frustum::intersects(cvec3 v1, cvec3 v2) const
{
	//return true; // BOUZI

	_Perfomance->Start(PERF_FRUSTRUM);

	vec3 points[8];
	points[0] = vec3(v1.x, v1.y, v1.z);
	points[1] = vec3(v1.x, v1.y, v2.z);
	points[2] = vec3(v1.x, v2.y, v1.z);
	points[3] = vec3(v1.x, v2.y, v2.z);
	points[4] = vec3(v2.x, v1.y, v1.z);
	points[5] = vec3(v2.x, v1.y, v2.z);
	points[6] = vec3(v2.x, v2.y, v1.z);
	points[7] = vec3(v2.x, v2.y, v2.z);

	for (uint8_t i = 0; i < 6; i++)
	{
		uint8_t numIn = 0;

		for (uint8_t k = 0; k < 8; k++)
		{
			if (planes[i].Contains(points[k]))
			{
				numIn++;
			}
		}

		if (numIn == 0)
		{
			_Perfomance->Stop(PERF_FRUSTRUM);
			return false;
		}
	}

	_Perfomance->Stop(PERF_FRUSTRUM);
	return true;
}

bool Frustum::intersects2(cvec3 v1, cvec3 v2) const
{
	return contains(v1) || contains(v2);
}

bool Frustum::intersectsSphere(cvec3 v, const float rad) const
{
	//return true; // BOUZI

	_Perfomance->Start(PERF_FRUSTRUM);

	for (uint8_t i = 0; i < 6; ++i)
	{
		if (planes[i].Distance(v) < -rad)
		{
			_Perfomance->Stop(PERF_FRUSTRUM);
			return false;
		}
	}

	_Perfomance->Stop(PERF_FRUSTRUM);
	return true;
}


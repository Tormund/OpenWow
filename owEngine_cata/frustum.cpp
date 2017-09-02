#include "stdafx.h"

// General
#include "frustum.h"

void Plane::normalize()
{
	float len;
	len = sqrtf(a*a + b*b + c*c);
	a /= len;
	b /= len;
	c /= len;
	d /= len;
}

enum Directions
{
	FRIGHT, FLEFT, FBOTTOM, FTOP, FBACK, FFRONT
};

void Frustum::retrieve()
{
	float mat[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glMultMatrixf(mat);
	glGetFloatv(GL_PROJECTION_MATRIX, mat);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

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

}

bool Frustum::contains(const vec3 &v) const
{
	for (int i = 0; i < 6; i++)
	{
		if ((planes[i].a*v.x + planes[i].b*v.y + planes[i].c*v.z + planes[i].d) <= 0)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::intersects(const vec3 &v1, const vec3 &v2) const
{
	return true; // BOUZI

	vec3 points[8];
	points[0] = vec3(v1.x, v1.y, v1.z);
	points[1] = vec3(v1.x, v1.y, v2.z);
	points[2] = vec3(v1.x, v2.y, v1.z);
	points[3] = vec3(v1.x, v2.y, v2.z);
	points[4] = vec3(v2.x, v1.y, v1.z);
	points[5] = vec3(v2.x, v1.y, v2.z);
	points[6] = vec3(v2.x, v2.y, v1.z);
	points[7] = vec3(v2.x, v2.y, v2.z);

	for (int i = 0; i < 6; i++)
	{
		int numIn = 0;

		for (int k = 0; k < 8; k++)
		{
			if ((planes[i].a*points[k].x + planes[i].b*points[k].y + planes[i].c*points[k].z + planes[i].d) > 0)
			{
				numIn++;
			}
		}

		if (numIn == 0) return false;
	}

	return true;
}


bool Frustum::intersectsSphere(cvec3 v, const float rad) const
{
	return true;

	for (int i = 0; i < 6; ++i)
	{
		float distance = (planes[i].a * v.x + planes[i].b * v.y + planes[i].c * v.z + planes[i].d);
		if (distance < -rad)
			return false;
		if (fabs(distance) < rad)
			return true;
	}
	return true;
}


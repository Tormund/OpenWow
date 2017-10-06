#pragma once

#include "MDX_Headers.h"

class MDX_Part_Bone
{
public:
	void init(File& f, M2CompBone& b, uint32* global, File* animfiles);
	void calcMatrix(MDX_Part_Bone* allbones, int anim, int time);

private:
	vec3 pivot, transPivot;
	int16 parent;

	bool billboard;
	Matrix4f m_TransformMatrix;
	Matrix4f m_RotationMatrix;

private:
	bool m_IsCalculated;

	Animated<vec3> trans;
	Animated<quat, M2CompQuat, Quat16ToQuat32> roll;
	Animated<vec3> scale;

	//

	friend class MDX;
	friend class PlaneParticleEmitter;
	friend class SphereParticleEmitter;
	friend class RibbonEmitter;
};



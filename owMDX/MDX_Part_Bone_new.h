#pragma once

class MDX_Part_Bone
{
public:
	void init(File& f, M2CompBone& b, uint32_t* global, File* animfiles);
	void calcMatrix(MDX_Part_Bone* allbones, uint32_t _animationIndex, int time);

private:
	Animated<vec3> trans;
	Animated<Quaternion, M2CompQuat, Quat16ToQuat32> roll;
	Animated<vec3> scale;

public:
	vec3 pivot, transPivot;
	int16_t parent;

	bool billboard;

	//Matrix mat;
	//Matrix mrot;
	mat4 mat;
	mat4 mRot;

	bool calc;
};
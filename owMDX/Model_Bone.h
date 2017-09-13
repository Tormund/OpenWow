#pragma once

#define	MODELBONE_BILLBOARD	8
#define	MODELBONE_TRANSFORM	512

struct ModelBoneDef
{
	int32_t keyBoneId;
	int32_t flags;
	int16_t parent; // parent bone index
	int16_t geoid;
	int32_t unknown; // new int added to the bone definitions.  Added in WoW 2.0
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	vec3 pivot;
};

class Bone
{
public:
	OW_MDX_DLL_API void init(File& f, ModelBoneDef& b, uint32_t* global, File* animfiles);
	OW_MDX_DLL_API void calcMatrix(Bone* allbones, int anim, int time);

private:
	Animated<vec3> trans;
	Animated<Quaternion, PACK_QUATERNION, Quat16ToQuat32> roll;
	Animated<vec3> scale;

public:
	vec3 pivot, transPivot;
	int16_t parent;

	bool billboard;
	Matrix mat;
	Matrix mrot;

	bool calc;
};
#pragma once

/*struct ModelCameraDef
{
	int32 type;                 // 0 is potrait camera, 1 characterinfo camera; -1 if none; referenced in CamLookup_Table
	float farclip;                // Where it stops to be drawn.
	float nearclip;               // Far and near. Both of them.
	AnimationBlock transPos;      // (vec3) How the cameras position moves. Should be 3*3 floats. (? WoW parses 36 bytes = 3*3*sizeof(float))
	vec3 pos;                     // float, Where the camera is located.
	AnimationBlock transTarget;   // (vec3) How the target moves. Should be 3*3 floats. (?)
	vec3 target;                  // float, Where the camera points to.
	AnimationBlock roll;          // (Quat) The camera can have some roll-effect. Its 0 to 2*Pi. 3 Floats!
	AnimationBlock FoV;           // (Float) One Float. cataclysm
};*/

class MDX_Part_Camera
{
public:
	void init(File& f, M2Camera& mcd, uint32* global);
	void setup(int time = 0);

private:
	float farclip;
	float nearclip;

	Animated<vec3> tPos;
	vec3 pos;

	Animated<vec3> tTarget;
	vec3 target;

	Animated<float> tRoll;
	float roll;

	Animated<float> tFov;
	float fov;

private:
	Camera* camera;
};
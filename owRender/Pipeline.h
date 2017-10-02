#pragma once

// Include 
#include "Camera.h"
#include "PipelineWorldTransformation.h"

class Pipeline
{
	CLASS_INSTANCE(Pipeline);

	Pipeline() : camera(nullptr), cameraTest(nullptr), useMainCamera(true) {}

	void SetProjection(float _fov, float _aspectRatio, float _near, float _far);
	void SetProjection(cmat4 _projectionMatrix);

	void SetCamera(Camera* _camera);
	void SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up = vec3(0.0f, 1.0f, 0.0f));
	
	//

	inline const mat4& GetProjection() 
	{ 
		return projection; 
	}
	inline const mat4& GetView()
	{
		return *camera->GetViewMatrix(); 
	}
	inline const mat4& GetWorld()
	{ 
		return worldTransformation.GetWorld();
	}

	inline const mat4 GetPVM()
	{
		return GetProjection() * GetView() * GetWorld();
	}
	inline const mat4 GetVW()
	{
		return GetView() * GetWorld();
	}
	inline const mat4 GetPV()
	{
		return GetProjection()  * GetView();
	}

	//

	inline PipelineWorldTransformation* GetWorldTransformation()
	{
		return &worldTransformation;
	}
	inline PipelineWorldTransformation* GetWorldTransformationTest()
	{
		return &worldTransformationTest;
	}

	//

	inline Camera* GetCamera() { return camera; }
	void RenderCamera(Camera* _camera = nullptr);

private: // Matrices
	mat4 projection;
	mat4 view;
	PipelineWorldTransformation worldTransformation;
	PipelineWorldTransformation worldTransformationTest;

	Camera* camera;
	Camera* cameraTest;
	bool useMainCamera;


private:
	float nh;
	float nw;

	float fh;
	float fw;

	float nearDist;
	float farDist;
};

#define _PipelineGlobal Pipeline::instance()

#define _Pipeline Pipeline::instance()->GetWorldTransformation()
#define _PipelineTest Pipeline::instance()->GetWorldTransformationTest()

#define _Camera  Pipeline::instance()->GetCamera()
#define _Camera2 Pipeline::instance()->GetCamera2()
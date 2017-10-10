#pragma once

// Include 
#include "Camera.h"
#include "PipelineWorldTransformation.h"

class Pipeline
{
	CLASS_INSTANCE(Pipeline);

	Pipeline() : camera(nullptr) {}

	void SetCamera(Camera* _camera);
	void SetCameraFrustum(Camera* _camera);
//	void SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up = vec3(0.0f, 1.0f, 0.0f));
	
	//

	inline const mat4& GetProjection() 
	{ 
		return camera->getProjMat();
	}
	inline const mat4& GetView()
	{
		return camera->getViewMat(); 
	}
	inline const mat4& GetWorld()
	{ 
		return worldTransformation.GetWorld();
	}

	inline mat4 GetPVM()
	{
		return GetProjection() * GetView() * GetWorld();
	}
	inline mat4 GetVW()
	{
		return GetView() * GetWorld();
	}
	inline mat4 GetPV()
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
	inline Camera* GetCameraFrustum() { return cameraFrustum; }

	void RenderCamera(Camera* _camera = nullptr);

private: // Matrices
	PipelineWorldTransformation worldTransformation;
	PipelineWorldTransformation worldTransformationTest;

	Camera* camera;
	Camera* cameraFrustum;

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
#define _CameraFrustum   Pipeline::instance()->GetCameraFrustum()
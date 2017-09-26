#pragma once

// Include 
#include "Camera.h"

class Pipeline
{
	CLASS_INSTANCE(Pipeline);

	Pipeline() : camera(nullptr) {}

	inline void Clear();
	inline void Clear2();

	void SetProjection(float _fov, float _aspectRatio, float _near, float _far);
	void SetProjection(cmat4 _projectionMatrix);

	void SetCamera(Camera* _camera);
	void SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up = vec3(0.0f, 1.0f, 0.0f));

#pragma region World transformation

	inline void Mult(cmat4 mat);

	inline void Translate(float x, float y, float z);
	inline void Translate(const vec3& _translate);

	inline void Quat(cQuaternion _quat);
	inline void Quat(float w, float x, float y, float z);

	inline void RotateX(float _yaw);
	inline void RotateY(float _pitch);
	inline void RotateZ(float _roll);

	inline void Scale(float _scale);
	inline void Scale(float _scaleX, float _scaleY, float _scaleZ);
	inline void Scale(const vec3& _scale);


	inline void Mult2(cmat4 mat);

	inline void Translate2(float x, float y, float z);
	inline void Translate2(const vec3& _translate);

	inline void Quat2(cQuaternion _quat);
	inline void Quat2(float w, float x, float y, float z);

	inline void RotateX2(float _yaw);
	inline void RotateY2(float _pitch);
	inline void RotateZ2(float _roll);

	inline void Scale2(float _scale);
	inline void Scale2(float _scaleX, float _scaleY, float _scaleZ);
	inline void Scale2(const vec3& _scale);

#pragma endregion

	inline const mat4* GetProjection() 
	{ 
		return &projection; 
	}
	inline const mat4* GetView() 
	{ 
		return camera->GetViewMatrix(); 
	}
	inline const mat4* GetWorld() 
	{ 
		return &worldTransformation; 
	}

	inline const mat4* GetPVM()
	{
		if (camera != nullptr)
		{
			view = *camera->GetViewMatrix();
		}

		m_WVP = projection * view * worldTransformation;

		return &m_WVP;
	}

	inline const mat4* GetVW()
	{
		if (camera != nullptr)
		{
			view = *camera->GetViewMatrix();
		}

		m_VW = view * worldTransformation;

		return &m_VW;
	}

	inline const mat4* GetPV()
	{
		if (camera != nullptr)
		{
			view = *camera->GetViewMatrix();
		}

		m_PV = projection * view;

		return &m_PV;
	}


	//

	inline const mat4* GetPVM2()
	{
		if (camera != nullptr)
		{
			view = *camera->GetViewMatrix();
		}

		m_WVP = projection * view * worldTransformation2;

		return &m_WVP;
	}
	inline const mat4* GetVW2()
	{
		if (camera != nullptr)
		{
			view = *camera->GetViewMatrix();
		}

		m_VW = view * worldTransformation2;

		return &m_VW;
	}

	//

	inline Camera* GetCamera() { return camera; }

private: // Matrices
	mat4 projection;
	mat4 view;
	mat4 worldTransformation;
	mat4 worldTransformation2;

	Camera* camera;

private: // General
	mat4 m_WVP;
	mat4 m_VW;
	mat4 m_PM;
	mat4 m_PV;
};

#include "Pipeline.inl"

#define _Pipeline Pipeline::instance()
#define _Camera _Pipeline->GetCamera()
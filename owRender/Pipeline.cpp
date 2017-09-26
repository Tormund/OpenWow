#include "stdafx.h"

// General
#include "Pipeline.h"

void Pipeline::SetProjection(float _fov, float _aspectRatio, float _near, float _far)
{
	projection = glm::perspective(_fov, _aspectRatio, _near, _far);
}

void Pipeline::SetProjection(cmat4 _projectionMatrix)
{
	projection = _projectionMatrix;
}

//

void Pipeline::SetCamera(Camera* _camera)
{
	if (_camera == nullptr)
	{
		Debug::Fatal("Pipeline error", "Pipeline[]: Can't set 'nullptr' camera.");
		return;
	}

	view = mat4();
	camera = _camera;
}

void Pipeline::SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up)
{
	vec3 CameraRight = glm::normalize(glm::cross(_rotation, _up));
	vec3 CameraUp = glm::normalize(glm::cross(CameraRight, _rotation));

	view = glm::lookAt(_position, _position + _rotation, CameraUp);
	camera = nullptr;
}
#include "stdafx.h"

// Include
#include "Camera.h"

// General
#include "Pipeline.h"

void Pipeline::Clear() {
	position = vec3(0.0f, 0.0f, 0.0f);
	rotation = vec3(0.0f, 0.0f, 0.0f);
	scale = vec3(1.0f, 1.0f, 1.0f);

	UpdateWorldTransformation();
}

void Pipeline::SetProjection(float _fov, float _aspectRatio, float _near, float _far) {
	projection = glm::perspective(_fov, _aspectRatio, _near, _far);
}

void Pipeline::SetProjection(cmat4 _projectionMatrix) {
	projection = _projectionMatrix;
}

void Pipeline::SetCamera(Camera* _camera) {
	if(_camera == nullptr) {
		Debug::Error("Pipeline[]: Can't set 'nullptr' camera.");
		return;
	}

	view = mat4();
	camera = _camera;
}

void Pipeline::SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up) {
	vec3 CameraRight = glm::normalize(glm::cross(_rotation, _up));
	vec3 CameraUp = glm::normalize(glm::cross(CameraRight, _rotation));

	view = glm::lookAt(_position, _position + _rotation, CameraUp);
	camera = nullptr;
}

#pragma region World position

// Translate Set
void Pipeline::SetTranslate(const vec3& _translate) {
	position = _translate;

	UpdateWorldTransformation();
}
void Pipeline::AddTranslate(const vec3& _translate) {
	position += _translate;

	UpdateWorldTransformation();
}

// Rotation
void Pipeline::SetRotation(const vec3& _rotation) {
	rotation = _rotation;

	UpdateWorldTransformation();
}
void Pipeline::SetYaw(float _yaw) {
	rotation.x = _yaw;

	UpdateWorldTransformation();
}
void Pipeline::SetPitch(float _pitch) {
	rotation.y = _pitch;

	UpdateWorldTransformation();
}
void Pipeline::SetRoll(float _roll) { 
	rotation.z = _roll;

	UpdateWorldTransformation();
}
void Pipeline::AddRotation(const vec3& _rotation) {
	rotation += _rotation;

	UpdateWorldTransformation();
}
void Pipeline::AddYaw(float _yaw) {
	rotation.x += _yaw;

	UpdateWorldTransformation();
}
void Pipeline::AddPitch(float _pitch) {
	rotation.y += _pitch;

	UpdateWorldTransformation();
}
void Pipeline::AddRoll(float _roll) {
	rotation.z += _roll;

	UpdateWorldTransformation();
}

// Scale
void Pipeline::SetScale(const vec3& _scale) {
	scale = _scale;

	UpdateWorldTransformation();
}
void Pipeline::AddScale(const vec3& _scale) {
	scale += _scale;

	UpdateWorldTransformation();
}

#pragma endregion

const mat4* Pipeline::GetPVM() {
	if(camera != nullptr)
		view = *camera->GetViewMatrix();

	m_WVP =	projection * view * worldTransformation;

	return &m_WVP;
}

//-------------------------------------------

void Pipeline::UpdateWorldTransformation() {
	worldTransformation = 
		glm::translate(position) *							   // World: Translate
		glm::rotate(rotation.x, vec3(1.0f, 0.0f, 0.0f)) * // World: Rotate Yaw (X)
		glm::rotate(rotation.y, vec3(0.0f, 1.0f, 0.0f)) * // World: Rotate Pitch (Y)
		glm::rotate(rotation.z, vec3(0.0f, 0.0f, 1.0f)) * // World: Rotate Roll (Z)
		glm::scale(scale);									   // World: Scale
}

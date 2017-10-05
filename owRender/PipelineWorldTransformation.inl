#pragma once

// Set Translate

inline void PipelineWorldTransformation::Translate(float x, float y, float z)
{
	worldTransformation *= glm::translate(vec3(x, y, z));
}
inline void PipelineWorldTransformation::Translate(cvec3 _translate)
{
	worldTransformation *= glm::translate(_translate);
}

// Set Rotation

inline void PipelineWorldTransformation::RotateX(float _yaw)
{
	worldTransformation *= glm::rotate(_yaw, vec3(1.0f, 0.0f, 0.0f));
}
inline void PipelineWorldTransformation::RotateY(float _pitch)
{
	worldTransformation *= glm::rotate(_pitch, vec3(0.0f, 1.0f, 0.0f));
}
inline void PipelineWorldTransformation::RotateZ(float _roll)
{
	worldTransformation *= glm::rotate(_roll, vec3(0.0f, 0.0f, 1.0f));
}
inline void PipelineWorldTransformation::Rotate(float _yaw, float _pitch, float _roll)
{
	worldTransformation *= glm::rotate(_yaw, vec3(1.0f, 0.0f, 0.0f));
	worldTransformation *= glm::rotate(_pitch, vec3(0.0f, 1.0f, 0.0f));
	worldTransformation *= glm::rotate(_roll, vec3(0.0f, 0.0f, 1.0f));
}
inline void PipelineWorldTransformation::Rotate(cvec3 _rotation)
{
	worldTransformation *= glm::rotate(_rotation.x, vec3(1.0f, 0.0f, 0.0f));
	worldTransformation *= glm::rotate(_rotation.y, vec3(0.0f, 1.0f, 0.0f));
	worldTransformation *= glm::rotate(_rotation.z, vec3(0.0f, 0.0f, 1.0f));
}

// Set Scale

inline void PipelineWorldTransformation::Scale(float _scale)
{
	worldTransformation *= glm::scale(vec3(_scale, _scale, _scale));
}
inline void PipelineWorldTransformation::Scale(float _scaleX, float _scaleY, float _scaleZ)
{
	worldTransformation *= glm::scale(vec3(_scaleX, _scaleY, _scaleZ));
}
inline void PipelineWorldTransformation::Scale(cvec3 _scale)
{
	worldTransformation *= glm::scale(_scale);
}

// Pure

inline void PipelineWorldTransformation::Quat(cquat _quat)
{
	worldTransformation *= glm::toMat4(_quat);
}
inline void PipelineWorldTransformation::Quat(float w, float x, float y, float z)
{
	worldTransformation *= glm::toMat4(glm::quat(w, x, y, z));
}

inline void PipelineWorldTransformation::Mult(cmat4 mat)
{
	worldTransformation *= mat;
}

// Push / Pop

inline void PipelineWorldTransformation::SetWorld(const mat4 _world)
{
	worldTransformation = _world;
}

inline const mat4& PipelineWorldTransformation::GetWorld()
{
	return worldTransformation;
}

inline void PipelineWorldTransformation::Push()
{
	assert1(!m_IsMatrixPushed);
	m_PushedWorldTranformation = worldTransformation;
	m_IsMatrixPushed = true;
}

inline void PipelineWorldTransformation::Pop()
{
	assert1(m_IsMatrixPushed);
	worldTransformation = m_PushedWorldTranformation;
	m_IsMatrixPushed = false;
}
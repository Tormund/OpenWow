#pragma once

// Set Translate

inline void PipelineWorldTransformation::Translate(float x, float y, float z)
{
	worldTransformation.translate(x, y, z);
}
inline void PipelineWorldTransformation::Translate(cvec3 _translate)
{
	worldTransformation.translate(_translate.x, _translate.y, _translate.z);
}

// Set Rotation

inline void PipelineWorldTransformation::RotateX(float _yaw)
{
	worldTransformation.rotate(_yaw, 0.0f, 0.0f);
}
inline void PipelineWorldTransformation::RotateY(float _pitch)
{
	worldTransformation.rotate(0.0f, _pitch, 0.0f);
}
inline void PipelineWorldTransformation::RotateZ(float _roll)
{
	worldTransformation.rotate(0.0f, 0.0f, _roll);
}
inline void PipelineWorldTransformation::Rotate(float _yaw, float _pitch, float _roll)
{
	worldTransformation.rotate(_yaw, _pitch, _roll);
}
inline void PipelineWorldTransformation::Rotate(cvec3 _rotation)
{
	worldTransformation.rotate(_rotation);
}

// Set Scale

inline void PipelineWorldTransformation::Scale(float _scale)
{
	worldTransformation.scale(_scale, _scale, _scale);
}
inline void PipelineWorldTransformation::Scale(float _scaleX, float _scaleY, float _scaleZ)
{
	worldTransformation.scale(_scaleX, _scaleY, _scaleZ);
}
inline void PipelineWorldTransformation::Scale(cvec3 _scale)
{
	worldTransformation.scale(_scale.x, _scale.y, _scale.z);
}

// Pure

inline void PipelineWorldTransformation::Quat(cquat _quat)
{
	worldTransformation = worldTransformation * _quat;
}
inline void PipelineWorldTransformation::Quat(float w, float x, float y, float z)
{
	worldTransformation = worldTransformation * (Quaternion(x, y, z, w));
}

inline void PipelineWorldTransformation::Mult(cmat4 mat)
{
	worldTransformation = worldTransformation * mat;
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
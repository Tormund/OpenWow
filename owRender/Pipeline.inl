#pragma once

inline void Pipeline::Clear()
{
	worldTransformation = glm::mat4(1.0f);
}

inline void Pipeline::Mult(cmat4 mat)
{
	worldTransformation *= mat;
}

// Set Translate

inline void Pipeline::Translate(float x, float y, float z)
{
	worldTransformation *= glm::translate(vec3(x, y, z));
}
inline void Pipeline::Translate(const vec3& _translate)
{
	worldTransformation *= glm::translate(_translate);
}

// Set Rotation

inline void Pipeline::Quat(cQuaternion _quat)
{
	worldTransformation *= glm::toMat4(_quat);
}

inline void Pipeline::Quat(float w, float x, float y, float z)
{
	worldTransformation *= glm::toMat4(glm::quat(w, x, y, z));
}

inline void Pipeline::RotateX(float _yaw)
{
	worldTransformation *= glm::rotate(_yaw, vec3(1.0f, 0.0f, 0.0f));
}
inline void Pipeline::RotateY(float _pitch)
{
	worldTransformation *= glm::rotate(_pitch, vec3(0.0f, 1.0f, 0.0f));
}
inline void Pipeline::RotateZ(float _roll)
{
	worldTransformation *= glm::rotate(_roll, vec3(0.0f, 0.0f, 1.0f));
}

// Set Scale

inline void Pipeline::Scale(float _scale)
{
	worldTransformation *= glm::scale(vec3(_scale, _scale, _scale));
}
inline void Pipeline::Scale(float _scaleX, float _scaleY, float _scaleZ)
{
	worldTransformation *= glm::scale(vec3(_scaleX, _scaleY, _scaleZ));
}
inline void Pipeline::Scale(const vec3& _scale)
{
	worldTransformation *= glm::scale(_scale);
}
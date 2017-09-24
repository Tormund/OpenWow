#pragma once

class Pipeline
{
	CLASS_INSTANCE_API(Pipeline, );

public:
	Pipeline() : camera(nullptr) {}

	 void Clear();

	 void SetProjection(float _fov, float _aspectRatio, float _near, float _far);
	 void SetProjection(cmat4 _projectionMatrix);

	 void SetCamera(Camera* _camera);
	 void SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up = vec3(0.0f, 1.0f, 0.0f));

#pragma region World transformation

	// Translate Set and Add
	inline void SetTranslate(float x, float y, float z) { SetTranslate(vec3(x, y, z)); }
	 void SetTranslate(const vec3& _translate);

	inline void AddTranslate(float x, float y, float z) { AddTranslate(vec3(x, y, z)); }
	 void AddTranslate(const vec3& _translate);

	// Rotation Set and Add
	inline void SetRotation(float _yaw, float _pitch, float _roll) { SetRotation(vec3(_yaw, _pitch, _roll)); }
	 void SetRotation(const vec3& _rotation);
	 void SetYaw(float _yaw);
	 void SetPitch(float _pitch);
	 void SetRoll(float _roll);

	inline void AddRotation(float _yaw, float _pitch, float _roll) { AddRotation(vec3(_yaw, _pitch, _roll)); }
	 void AddRotation(const vec3& _rotation);
	 void AddYaw(float _yaw);
	 void AddPitch(float _pitch);
	 void AddRoll(float _roll);

	// Scale Set and Add
	inline void SetScale(float _scale) { SetScale(vec3(_scale, _scale, _scale)); }
	inline void SetScale(float _scaleX, float _scaleY, float _scaleZ) { SetScale(vec3(_scaleX, _scaleY, _scaleZ)); }
	 void SetScale(const vec3& _scale);

	inline void AddScale(float _scale) { AddScale(vec3(_scale, _scale, _scale)); }
	inline void AddScale(float _scaleX, float _scaleY, float _scaleZ) { AddScale(vec3(_scaleX, _scaleY, _scaleZ)); }
	 void AddScale(const vec3& _scale);

#pragma endregion

	inline const mat4* GetProjection() { return &projection; }
	inline const mat4* GetView() { return camera->GetViewMatrix(); }
	inline const mat4* GetWorld() { return &worldTransformation; }

	 const mat4* GetPVM();

	inline Camera* GetCamera() { return camera; }

private:
	void UpdateWorldTransformation();

private: // Vectors
	vec3 position;
	vec3 rotation;
	vec3 scale;

private: // Matrices
	mat4 projection;
	mat4 view;
	mat4 worldTransformation;

	Camera* camera;

private: // General
	mat4 m_WVP;
};

#define _Pipeline Pipeline::instance()
#define _Camera _Pipeline->GetCamera()
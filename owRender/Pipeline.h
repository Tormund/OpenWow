#pragma once

class Pipeline {
public:
	static OW_RENDER_DLL_API Pipeline* instance() {
		static Pipeline instance;
		return &instance;
	}

	Pipeline() : camera(nullptr) { }
	
	OW_RENDER_DLL_API void Clear();

	OW_RENDER_DLL_API void SetProjection(float _fov, float _aspectRatio, float _near, float _far);
	OW_RENDER_DLL_API void SetProjection(cmat4 _projectionMatrix);

	OW_RENDER_DLL_API void SetCamera(Camera* _camera);
	OW_RENDER_DLL_API void SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up = vec3(0.0f, 1.0f, 0.0f));
	
#pragma region World transformation

	// Translate Set and Add
	inline void SetTranslate(float x, float y, float z) { SetTranslate(vec3(x, y, z)); }
	OW_RENDER_DLL_API void SetTranslate(const vec3& _translate);
	
	inline void AddTranslate(float x, float y, float z) { AddTranslate(vec3(x, y, z)); }
	OW_RENDER_DLL_API void AddTranslate(const vec3& _translate);
	
	// Rotation Set and Add
	inline void SetRotation(float _yaw, float _pitch, float _roll) { SetRotation(vec3(_yaw, _pitch, _roll)); }
	OW_RENDER_DLL_API void SetRotation(const vec3& _rotation);
	OW_RENDER_DLL_API void SetYaw(float _yaw);
	OW_RENDER_DLL_API void SetPitch(float _pitch);
	OW_RENDER_DLL_API void SetRoll(float _roll);

	inline void AddRotation(float _yaw, float _pitch, float _roll) { AddRotation(vec3(_yaw, _pitch, _roll)); }
	OW_RENDER_DLL_API void AddRotation(const vec3& _rotation);
	OW_RENDER_DLL_API void AddYaw(float _yaw);
	OW_RENDER_DLL_API void AddPitch(float _pitch);
	OW_RENDER_DLL_API void AddRoll(float _roll);

	// Scale Set and Add
	inline void SetScale(float _scale) { SetScale(vec3(_scale, _scale, _scale)); }
	inline void SetScale(float _scaleX, float _scaleY, float _scaleZ) { SetScale(vec3(_scaleX, _scaleY, _scaleZ)); }
	OW_RENDER_DLL_API void SetScale(const vec3& _scale);
	
	inline void AddScale(float _scale) { AddScale(vec3(_scale, _scale, _scale)); }
	inline void AddScale(float _scaleX, float _scaleY, float _scaleZ) { AddScale(vec3(_scaleX, _scaleY, _scaleZ)); }
	OW_RENDER_DLL_API void AddScale(const vec3& _scale);

#pragma endregion
	
	inline const mat4* GetProjection() { return &projection; }
	inline const mat4* GetView() { return camera->GetViewMatrix(); }
	inline const mat4* GetWorld() { return &worldTransformation; }

	OW_RENDER_DLL_API const mat4* GetPVM();

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
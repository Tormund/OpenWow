#pragma once

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVTY = 20.1f;

class Camera
{
public:
	 Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	 Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	//

	void Update();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	void setupViewportSize(float x, float y, float w, float h);
	void setupViewParams(float fov, float aspect, float nearPlane, float farPlane);
	void setProjectionMatrix(float* projMat);
	void onPostUpdate();

	const Frustum& getFrustum() const { return _frustum; }
	const Matrix4f& getViewMat() const { return _viewMat; }
	const Matrix4f& getProjMat() const { return _projMat; }

public:
	// Camera Attributes
	vec3 Position;
	vec3 Direction;

	vec3 CameraUp;
	vec3 CameraRight;

	// Eular Angles
	float Roll;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;

public:
	Matrix4f            _viewMat, _projMat;
	Frustum             _frustum;
	int					_vpX, _vpY, _vpWidth, _vpHeight;

	float               _frustLeft, _frustRight, _frustBottom, _frustTop;
	float               _frustNear, _frustFar;

	bool                _orthographic;  // Perspective or orthographic frustum?
	bool                _manualProjMat; // Projection matrix manually set?
};
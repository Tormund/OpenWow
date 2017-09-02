#pragma once

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVTY = 20.1f;

class Camera {
public:
	OW_RENDER_DLL_API Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	OW_RENDER_DLL_API Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	void Update() {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMultMatrixf(&viewMatrix[0][0]);
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	inline const glm::mat4* GetViewMatrix() {
		return &viewMatrix;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	OW_RENDER_DLL_API void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	OW_RENDER_DLL_API void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
	void updateCameraVectors();

public:
	// Camera Attributes
	vec3 Position;
	vec3 Direction;

	vec3 CameraUp;
	vec3 CameraRight;

	vec3 WorldUp;

	// Eular Angles
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;

private:
	mat4 viewMatrix;
};
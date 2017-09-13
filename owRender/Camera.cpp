#include "stdafx.h"

// General
#include "Camera.h"

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch) :
	Direction(vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVTY)
{

	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	Direction(vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVTY)
{

	Position = vec3(posX, posY, posZ);
	WorldUp = vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVectors();
}

void Camera::Update()
{
	updateCameraVectors();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixf(glm::value_ptr(viewMatrix));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
	{
		Position += Direction * velocity;
	}
	if (direction == BACKWARD)
	{
		Position -= Direction * velocity;
	}
	if (direction == LEFT)
	{
		Position -= CameraRight * velocity;
	}
	if (direction == RIGHT)
	{
		Position += CameraRight * velocity;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch /*= true*/)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}

		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Direction = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	CameraRight = glm::normalize(glm::cross(Direction, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	CameraUp = glm::normalize(glm::cross(CameraRight, Direction));

	// View matrix
	viewMatrix = glm::lookAt(Position, Position + Direction, CameraUp);
}

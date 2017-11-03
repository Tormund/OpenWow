#include "stdafx.h"

// General
#include "Camera.h"

Camera::Camera(vec3 position, vec3 up, float roll, float pitch) :
	Direction(vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVTY)
{

	Position = position;
	//WorldUp = up;
	Roll = roll;
	Pitch = pitch;

	Update();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float roll, float pitch) :
	Direction(vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVTY)
{

	Position = vec3(posX, posY, posZ);

	Roll = roll;
	Pitch = pitch;

	Update();
}

void Camera::Update(bool _dontUseAngles)
{
    if (!_dontUseAngles)
    {
        // Calculate the new Front vector
        vec3 front;
        /*front.x = -(sinf(degToRad(Roll)) * cos(degToRad(Pitch)));
        front.y = sin(degToRad(Pitch));
        front.z = -(cosf(degToRad(Roll)) * cos(degToRad(Pitch)));*/

        front.x = cos(degToRad(Roll)) * cos(degToRad(Pitch));
        front.y = sin(degToRad(Pitch));
        front.z = sin(degToRad(Roll)) * cos(degToRad(Pitch));

        Direction = front.normalized();
    }

    Direction = Direction.normalized();

	// Also re-calculate the Right and Up vector
	CameraRight = Direction.cross(Vec3f(0.0f, 1.0f, 0.0f)).normalized();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	CameraUp = CameraRight.cross(Direction);

	_viewMat = Matrix4f::lookAtRH(Position, Position + Direction, CameraUp);

	// View matrix
	//setTransform(Position, vec3(Pitch, Roll, 0.0), vec3(1.0f, 1.0f, 1.0f));

	onPostUpdate();
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

	Update();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Roll += xoffset;
	Pitch += yoffset;

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

	Update();
}

//

void Camera::setupViewportSize(float x, float y, float w, float h)
{
	_vpX = x;
	_vpY = y;
	_vpWidth = w;
	_vpHeight = h;
}

void Camera::setupViewParams(float fov, float aspect, float nearPlane, float farPlane)
{
	float ymax = nearPlane * tanf(degToRad(fov / 2.0f));
	float xmax = ymax * aspect;

	_frustLeft = -xmax;
	_frustRight = xmax;

	_frustBottom = -ymax;
	_frustTop = ymax;

	_frustNear = nearPlane;
	_frustFar = farPlane;

	// setting view params implicitly disables the manual projection matrix 
	_manualProjMat = false;
	_orthographic = false;
}

void Camera::setProjectionMatrix(float* projMat)
{
	memcpy(_projMat.x, projMat, 16 * sizeof(float));
	_manualProjMat = true;
}

void Camera::onPostUpdate()
{
	// Calculate projection matrix if not using a manually set one
	if (!_manualProjMat)
	{
		if (!_orthographic)
		{
			_projMat = Matrix4f::PerspectiveMat(_frustLeft, _frustRight, _frustBottom, _frustTop, _frustNear, _frustFar);
		}
		else
		{
			_projMat = Matrix4f::OrthoMat(_frustLeft, _frustRight, _frustBottom, _frustTop, _frustNear, _frustFar);
		}
	}

	// Update frustum
	_frustum.buildViewFrustum(_viewMat, _projMat);
}

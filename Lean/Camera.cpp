////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	// Setup where the camera is looking by default
	m_LookAtX = 0.0f;
	m_LookAtY = 0.0f;
	m_LookAtZ = 1.0f;
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void Camera::GetPosition(D3DXVECTOR3& pos)
{
	pos = D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void Camera::GetRotation(D3DXVECTOR3& rot)
{
	rot = D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::SetTargetToLookAt(float x, float y, float z)
{
	D3DXVECTOR3 betweenCameraAndTarget(x - m_positionX, y - m_positionY, z - m_positionZ);
	D3DXVECTOR3 lookAtNormalizedVector;
	D3DXVec3Normalize(&lookAtNormalizedVector, &betweenCameraAndTarget);

	m_LookAtX = lookAtNormalizedVector.x;
	m_LookAtY = lookAtNormalizedVector.y;
	m_LookAtZ = lookAtNormalizedVector.z;
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking.
	lookAt.x = m_LookAtX;
	lookAt.y = m_LookAtY;
	lookAt.z = m_LookAtZ;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}




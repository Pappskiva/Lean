////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

Camera::Camera()
{
	/*S�tt alla variabler till 0 eller standard/testv�rden*/
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

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
	/*S�tt positionen*/
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void Camera::GetPosition(v3& pos)
{
	/*F� positionen*/
	pos = v3(m_positionX, m_positionY, m_positionZ);
}

void Camera::SetRotation(float x, float y, float z)
{
	/*S�tt rotationen*/
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void Camera::GetRotation(v3& rot)
{
	/*F� rotationen*/
	rot = v3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::SetTargetToLookAt(float x, float y, float z)
{
	/*S�tt vilken riktning som kameran ska titta �t. Positionen p� det man vill titta mot l�ggs in, och en normaliserad vector mellan kamerans och objektets position f�s fram*/
	v3 betweenCameraAndTarget(x - m_positionX, y - m_positionY, z - m_positionZ);
	betweenCameraAndTarget.Normalize();

	m_LookAtX = betweenCameraAndTarget.x;
	m_LookAtY = betweenCameraAndTarget.y;
	m_LookAtZ = betweenCameraAndTarget.z;
}

void Camera::Render()
{
	v3 up, position, lookAt;
	float yaw, pitch, roll;
	m4 rotationMatrix;

	/*Skapa en vektor som pekar upp�t*/
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	/*Ta kamerans position*/
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	/*Ta riktningen den tittar mot*/
	lookAt.x = m_LookAtX;
	lookAt.y = m_LookAtY;
	lookAt.z = m_LookAtZ;

	/*Best�m pitch, yaw och roll i radianer*/
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	/*Skapa rotationsmatrisen baserat p� rotation*/
	rotationMatrix = m4::CreateYawPitchRoll(yaw, pitch, roll);

	/*�ndra lookAt och up vektorerna baserat p� rotationsmatrisen s� de �r r�tt*/ 
	lookAt = rotationMatrix.Transform(lookAt);
	up = rotationMatrix.Transform(up);

	/*�ndra s� att lookAt blir r�tt baserat p� kamerans position*/
	lookAt = position + lookAt;

	/*Skapa view matrisen fr�n de tre vektorerna*/
	m_viewMatrix.ViewAtLH(position, lookAt, up);
	
	return;
}

void Camera::GetViewMatrix(m4& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}




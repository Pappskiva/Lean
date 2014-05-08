////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

Camera::Camera()
{
	position = v3(0.0f);

	xRotation = 0.0f;
	yRotation = 0.0f;

	orientation.Identity();

	minDepth = 0.0f;
	maxDepth = 1.0f;

	zoom = 1.0f;
	//direction = v3(1.0, 0.0f, 0.0f);
}

void Camera::Update()
{
	if (xRotation < -1.5707964f)
		xRotation = -1.5707964f;

	if (xRotation > 1.5707962f)
		xRotation = 1.5707962f;

}

void Camera::RotateY(const float rotation)
{
	yRotation += rotation;

	orientation = m3::CreateRotateX(xRotation) * m3::CreateRotateY(yRotation);
}

void Camera::RotateX(const float rotation)
{
	xRotation += rotation;

	orientation = m3::CreateRotateX(xRotation) * m3::CreateRotateY(yRotation);
}

void Camera::LookAt(const v3 &at)
{
	orientation.FromDir(at - position);
}

void Camera::LookAlong(const v3 &dir)
{
	orientation.FromDir(dir);
}

void Camera::SetPosition(const v3 &position)
{
	this->position = position;
}

void Camera::GeneratePerspectiveProjectionMatrix()
{
	projection.Projection(fieldOfView, nearPlane, farPlane, (float)screenWidth / (float)screenHeight);
}

void Camera::GenerateOrthoProjectionMatrix()
{
	projection = m4::CreateOrthographicLH((float)screenWidth, (float)screenHeight, nearPlane, farPlane);
}

void Camera::Generate2DViewMatrix()
{
	float doubleZoom = zoom * 2.0f;
	view = m4::CreateScale(v3(doubleZoom / screenWidth, doubleZoom / screenHeight, 1.0f));
	view._41 = (doubleZoom * -position.x) / (float)screenWidth;
	view._42 = (doubleZoom * -position.y) / (float)screenHeight;
	view._43 = 0.0f;
}

void Camera::Generate3DViewMatrix()
{
	view.ViewAlongLH(position, orientation.GetZDir());
}

void Camera::GenerateDirectionalLightCamera()
{
	//http://www.gamedev.net/topic/505893-orthographic-projection-for-shadow-mapping/
}

Ray Camera::GenerateRay(const v2 &screenCoordinate) const
{
	Ray ray;
	m4 invVP;

	invVP = view.Inverse();

	float screenX = ((2.0f * ((float)screenCoordinate.x / screenWidth)) - 1.0f) / projection._11;
	float screenY = -((2.0f * ((float)screenCoordinate.y / screenHeight)) - 1.0f) / projection._22;

	ray.pos.x = screenX * invVP._11 + screenY * invVP._21 + invVP._31 + invVP._41;
	ray.pos.y = screenX * invVP._12 + screenY * invVP._22 + invVP._32 + invVP._42;
	ray.pos.z = screenX * invVP._13 + screenY * invVP._23 + invVP._33 + invVP._43;

	ray.dir = ray.pos - position;
	ray.dir.Normalize();

	return ray;
}

m4& Camera::GetViewMatrix(m4 &view)
{
	view = this->view;

	return view;
}

m4& Camera::GetProjectionMatrix(m4 &proj)
{
	proj = projection;

	return projection;
}
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

void Camera::GenerateDirectionalLightCamera(const v3 &lightDir, const v3 boundaryBoxCorners[8], const v3 &boxOffset)
{
	//http://www.gamedev.net/topic/505893-orthographic-projection-for-shadow-mapping/

	v3 center(0.0f);
	for (int i = 0; i < 8; ++i)
		center += boundaryBoxCorners[i];


	v3 newOffset(0);// = boxOffset;
	newOffset.y = 0;
	center.x /= 8;
	center.y /= 8;
	center.z /= 8;
	//center += newOffset;
	v3 viewFrustumCorners[8];
	view.ViewAtLH(center * -lightDir * farPlane, center, v3(0, 1, 0));

	viewFrustumCorners[0] = view.Transform(boundaryBoxCorners[0] + newOffset);
	viewFrustumCorners[1] = view.Transform(boundaryBoxCorners[1] + newOffset);
	viewFrustumCorners[2] = view.Transform(boundaryBoxCorners[2] + newOffset);
	viewFrustumCorners[3] = view.Transform(boundaryBoxCorners[3] + newOffset);
	viewFrustumCorners[4] = view.Transform(boundaryBoxCorners[4] + newOffset);
	viewFrustumCorners[5] = view.Transform(boundaryBoxCorners[5] + newOffset);
	viewFrustumCorners[6] = view.Transform(boundaryBoxCorners[6] + newOffset);
	viewFrustumCorners[7] = view.Transform(boundaryBoxCorners[7] + newOffset);

	v3 min, max;

	for (int i = 0; i < 8; ++i)
	{
		if (viewFrustumCorners[i].x > max.x)
			max.x = viewFrustumCorners[i].x;
		else if (viewFrustumCorners[i].x < min.x)
			min.x = viewFrustumCorners[i].x;

		if (viewFrustumCorners[i].y > max.y)
			max.y = viewFrustumCorners[i].y;
		else if (viewFrustumCorners[i].y < min.y)
			min.y = viewFrustumCorners[i].y;

		if (viewFrustumCorners[i].z > max.z)
			max.z = viewFrustumCorners[i].z;
		else if (viewFrustumCorners[i].z < min.z)
			min.z = viewFrustumCorners[i].z;
	}

	projection = m4::CreateOrthoOffCenterLH(min.x, max.x, min.y, max.y, min.z, max.z);
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
	return view = this->view;;
}

m4& Camera::GetProjectionMatrix(m4 &proj)
{
	return proj = this->projection;
}
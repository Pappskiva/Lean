////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////

/*
Camera klassen är kameran som används för att få view matrisen. Den har position och rotation, samt vilken riktning den är riktad mot
*/

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "DuckRenderer\DMath.h"
#include "DuckRenderer\DCollision.h"

class Camera
{
public:
	v3		position;
	float	xRotation, yRotation;
	m3		orientation;
	m4		projection;
	m4		view;

	float	nearPlane;
	float	farPlane;
	float	fieldOfView;
	int		screenWidth;
	int 	screenHeight;

	float	minDepth;
	float	maxDepth;

	//2D
	float	zoom;

public:

	Camera();
	void	Update();

	void	RotateY(const float rotation);
	void	RotateX(const float rotation);

	void	LookAt(const v3 &at);
	void	LookAlong(const v3 &dir);

	void	SetNearPlane(const float nearPlane);
	void	SetFarPlane(const float farPlane);
	void	SetFieldOfView(const float fov);
	void	SetScreenSize(const int screenWidth, const int screenHeight);
	void	SetMinDepth(const float minDepth);
	void	SetMaxDepth(const float maxDepth);

	void	SetPosition(const v3 &position);
	v3		GetPosition() const;

	void	GenerateOrthoProjectionMatrix();
	void	GeneratePerspectiveProjectionMatrix();
	void	Generate2DViewMatrix();
	void	Generate3DViewMatrix();
	void	GenerateDirectionalLightCamera(const v3 &lightDir, const v3 boundaryBoxCorners[8], const v3 &boxOffset = v3(0.0f));

	Ray		GenerateRay(const v2 &screenCoordinate) const;
	m3		GetOrientation() const;
	m4&		GetViewMatrix(m4 &view);
	m4&		GetProjectionMatrix(m4 &proj);

	friend class Renderer;
};

inline void Camera::SetNearPlane(const float nearPlane)
{
	this->nearPlane = nearPlane;
}

inline void Camera::SetFarPlane(const float farPlane)
{
	this->farPlane = farPlane;
}

inline void Camera::SetFieldOfView(const float fov)
{
	this->fieldOfView = fov;
}

inline void Camera::SetScreenSize(const int screenWidth, const int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

inline void Camera::SetMinDepth(const float minDepth)
{
	this->minDepth = minDepth;
}

inline void Camera::SetMaxDepth(const float maxDepth)
{
	this->maxDepth = maxDepth;
}

inline v3 Camera::GetPosition() const
{
	return position;
}

inline m3 Camera::GetOrientation() const
{
	return orientation;
}
#endif
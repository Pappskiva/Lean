////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "DuckRenderer\DMath.h"

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void		SetPosition(float, float, float);
	void		GetPosition(v3&);

	void		SetRotation(float, float, float);
	void		GetRotation(v3&);
	
	void		SetTargetToLookAt(float, float, float);

	void		Render();//what
	void		GetViewMatrix(m4&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_LookAtX, m_LookAtY, m_LookAtZ;
	m4 m_viewMatrix;
};

#endif
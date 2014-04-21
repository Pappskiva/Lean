////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "..\\Include\\d3dx10math.h"

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void		SetPosition(float, float, float);
	void		GetPosition(D3DXVECTOR3&);

	void		SetRotation(float, float, float);
	void		GetRotation(D3DXVECTOR3&);

	void		SetTargetToLookAt(float, float, float);
	
	void		Render();
	void		GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_LookAtX, m_LookAtY, m_LookAtZ;
	D3DXMATRIX m_viewMatrix;
};

#endif
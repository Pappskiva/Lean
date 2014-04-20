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
	
	void		Render();
	void		GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif
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

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	/*Sätt och få position*/
	void		SetPosition(float, float, float);
	void		GetPosition(v3&);

	/*Sätt och få rotation*/
	void		SetRotation(float, float, float);
	void		GetRotation(v3&);
	
	/*Sätt vart den ska titta mot, t.ex. ett objekt*/
	void		SetTargetToLookAt(float, float, float);

	/*Rendera en viewmatris baserat på position, rotation och riktning*/
	void		Render();
	/*Få viewmatrisen*/
	void		GetViewMatrix(m4&);

private:
	/*Position*/
	float m_positionX, m_positionY, m_positionZ;
	/*Rotation*/
	float m_rotationX, m_rotationY, m_rotationZ;
	/*Riktning*/
	float m_LookAtX, m_LookAtY, m_LookAtZ;
	/*Viewmatris*/
	m4 m_viewMatrix;
};

#endif
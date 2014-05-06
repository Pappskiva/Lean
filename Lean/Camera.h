////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////

/*
Camera klassen �r kameran som anv�nds f�r att f� view matrisen. Den har position och rotation, samt vilken riktning den �r riktad mot
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

	/*S�tt och f� position*/
	void		SetPosition(float, float, float);
	void		GetPosition(v3&);

	/*S�tt och f� rotation*/
	void		SetRotation(float, float, float);
	void		GetRotation(v3&);
	
	/*S�tt vart den ska titta mot, t.ex. ett objekt*/
	void		SetTargetToLookAt(float, float, float);

	/*Rendera en viewmatris baserat p� position, rotation och riktning*/
	void		Render();
	/*F� viewmatrisen*/
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
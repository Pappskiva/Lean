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
	void		SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void		Render();
	void		BaseViewRender();
	void		GetViewMatrix(D3DXMATRIX&);



private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif
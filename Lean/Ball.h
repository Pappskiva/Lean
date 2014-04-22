////////////////////////////////////////////////////////////////////////////////
// Filename: Ball.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BALL_H
#define BALL_H

#include "D3D.h"
#include "DuckRenderer\DMath.h"
#include "Mesh.h"
#include <fstream>

#include "Texture.h"

class Ball
{
protected:
public:
	Ball();
	Ball(const Ball&);
	~Ball();

	bool Initialize(D3D*, WCHAR*);
	void Shutdown();
	void Update(float);
	void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	void	SetRotation(float, float, float);
	void	GetRotation(v3&);


	void	SetRadius(float);
	float	GetRadius();

	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

private:
	void	ShutdownBuffers();
	void	RenderBuffers(ID3D11DeviceContext*);


private:

	Mesh			*mesh;

	Texture*	m_Texture;

	v3 m_position;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_radius;

	m4 worldMatrix;
};

inline void	Ball::SetMesh(const Mesh *mesh)
{
	this->mesh = (Mesh *)mesh;
}

inline Mesh *	Ball::GetMesh()
{
	return mesh;
}
#endif
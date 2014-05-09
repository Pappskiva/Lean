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

	void		SetVelocity(const v3 &velocity);
	const v3&	GetVelocity() const;

	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);
	void	SetWorldMatrix(m4 world);

	void	SetShader(const Shader *shader);

	void	MakeSlippery(); // För vattenhindret
	float	GetFriction();
	void	AddForce(v3); // För trampolin och magnet
	v3		GetAndZeroAddedForce(); // OBS: Nollställer AddedForce

private:
	void	ShutdownBuffers();
	void	RenderBuffers(ID3D11DeviceContext*);


private:

	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	v3 m_position;
	v3 m_velocity;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_radius;
	float m_friction;
	v3 m_addedForce; // För kraftpåverkningar som studsmatta och magnet

	m4 worldMatrix;
};

inline void	Ball::SetMesh(const Mesh *mesh)
{
	this->m_mesh = (Mesh *)mesh;
}

inline Mesh* Ball::GetMesh()
{
	return m_mesh;
}

inline void Ball::SetVelocity(const v3 &velocity)
{
	this->m_velocity = velocity;
}

inline const v3& Ball::GetVelocity() const
{
	return m_velocity;
}

inline void Ball::SetShader(const Shader *shader)
{
	this->m_shader = (Shader *)shader;
}

#endif
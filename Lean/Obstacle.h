////////////////////////////////////////////////////////////////////////////////
// Filename: Obstacle.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "D3D.h"
#include "DuckRenderer\DMath.h"
#include "Mesh.h"
#include <fstream>

#include "Texture.h"

//För stunden är det tänkt att detta ska vara en basklass till alla hindren. Detta kan ändras på behov om så behövs.
//I nuläget är klassen inte abstrakt

enum Type //Jag är inte säker på om den behövs, men det kanske är bättre att låta objektet veta det snarare än att behöva kolla typen på objektet eller något.	
{
	WATER,
	LEAVES,
	MAGNET,
	TRAMPOLINE
};


class Obstacle
{
public:
	Obstacle();
	Obstacle(const Obstacle&);
	virtual ~Obstacle() = 0;

	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	virtual void Update(float, float, float, float, float);
	virtual void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	void	SetRotation(float, float, float);
	void	GetRotation(v3&);

	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	Type	GetType();

	void	SetShader(const Shader *shader);

protected:
	virtual bool 	InitalizeBuffers(D3D*);
	//void	ShutdownBuffers();



protected:

	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	Type m_Type;

	v3 m_position;
	v3 m_velocity;
	float m_rotationX, m_rotationY, m_rotationZ;

	m4 worldMatrix;
};

inline void	Obstacle::SetMesh(const Mesh *mesh)
{
	this->m_mesh = (Mesh *)mesh;
}

inline Mesh* Obstacle::GetMesh()
{
	return m_mesh;
}

inline void Obstacle::SetShader(const Shader *shader)
{
	this->m_shader = (Shader *)shader;
}

#endif
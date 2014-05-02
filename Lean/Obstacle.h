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

//F�r stunden �r det t�nkt att detta ska vara en basklass till alla hindren. Detta kan �ndras p� behov om s� beh�vs.
//I nul�get �r klassen inte abstrakt

enum Type //Jag �r inte s�ker p� om den beh�vs, men det kanske �r b�ttre att l�ta objektet veta det snarare �n att beh�va kolla typen p� objektet eller n�got.	
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
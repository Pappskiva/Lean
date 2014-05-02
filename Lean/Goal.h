////////////////////////////////////////////////////////////////////////////////
// Filename: Goal.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GOAL_H
#define GOAL_H

#include "D3D.h"
#include "DuckRenderer\DMath.h"
#include "Mesh.h"
#include <fstream>

#include "Texture.h"

class Goal
{
public:
	Goal();
	Goal(const Goal&);
	~Goal();

	bool Initialize(D3D*, WCHAR*);
	void Shutdown();
	void Update(float, float, float);
	void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	void	SetRotation(float, float, float);
	void	GetRotation(v3&);

	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	int		GetNextLevelNumber();

	void	SetShader(const Shader *shader);

private:

	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	int m_nextLevelNumber;

	v3 m_position;
	v3 m_velocity;
	float m_rotationX, m_rotationY, m_rotationZ;

	m4 worldMatrix;
};

inline void	Goal::SetMesh(const Mesh *mesh)
{
	this->m_mesh = (Mesh *)mesh;
}

inline Mesh* Goal::GetMesh()
{
	return m_mesh;
}

inline void Goal::SetShader(const Shader *shader)
{
	this->m_shader = (Shader *)shader;
}

#endif
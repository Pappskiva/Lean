////////////////////////////////////////////////////////////////////////////////
// Filename: Box.h
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef BOX_H
#define BOX_H

#include "D3D.h"
#include "DuckRenderer\DMath.h"
#include "Mesh.h"
#include <fstream>

#include "Texture.h"

class Box
{
public:
	Box();
	Box(const Box&);
	~Box();

	/*Initialisering, updatering, rendering, shutdown, de grundl�ggande funktionerna*/
	bool Initialize(D3D*, WCHAR*);
	void Shutdown();
	void Update(float, float, float);
	void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	/*Funktioner f�r att f� och s�tta meshen*/
	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	/*F� och s�tta position p� m�let*/
	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	/*F� och s�tta rotationen p� m�let*/
	void	SetRotation(float, float, float);
	void	GetRotation(v3&);

	//S�tt skalan
	void SetScale(float);

	/*Uppdatera worldmatrisen och f� den. UpdateWorldMatrix kallas i Render funktionen*/
	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	/*S�tt vilken shader som anv�nds*/
	void	SetShader(const Shader *shader);

private:

	/*Mesh, vilken shader som anv�nds och objektets textur*/
	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	/*Position p� objeketet samt rotationen och skalan*/
	v3 m_position;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_scale;

	const float HEIGHT = 9.0f;

	/*V�rldsmatrisen*/
	m4 worldMatrix;
};

inline void	Box::SetMesh(const Mesh *mesh)
{
	this->m_mesh = (Mesh *)mesh;
}

inline Mesh* Box::GetMesh()
{
	return m_mesh;
}

inline void Box::SetShader(const Shader *shader)
{
	this->m_shader = (Shader *)shader;
}

#endif
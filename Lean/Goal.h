////////////////////////////////////////////////////////////////////////////////
// Filename: Goal.h
////////////////////////////////////////////////////////////////////////////////

/*
Goal klassen ska användas för ett mål objekt i spelet.
Målet i fråga har position, rotation och ett sätt att få dess worldmatris.
Klassen har också ett mesh objekt, alltså det objekt som ska renderas ut.

Goal har ett värde i sig, m_nextLevelNumber som är en integer som ska användas för att veta till vilken nivå som målet ska leda till.
*/

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

	/*Initialisering, updatering, rendering, shutdown, de grundläggande funktionerna*/
	bool Initialize(D3D*, WCHAR*);
	void Shutdown();
	void Update(float, float, float);
	void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	/*Funktioner för att få och sätta meshen*/
	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	/*Få och sätta position på målet*/
	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	/*Få och sätta rotationen på målet*/
	void	SetRotation(float, float, float);
	void	GetRotation(v3&);

	/*Uppdatera worldmatrisen och få den. UpdateWorldMatrix kallas i Render funktionen*/
	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	/*Hämta eller få vilken bana som målet ska leda till*/
	int		GetNextLevelNumber();
	void	SetNextLevelNumber(int);

	/*Sätt vilken shader som används*/
	void	SetShader(const Shader *shader);

private:

	/*Mesh, vilken shader som används och objektets textur*/
	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	/*Banan som målet ska leda till*/
	int m_nextLevelNumber;

	/*Position på objeketet samt rotationen*/
	v3 m_position;
	float m_rotationX, m_rotationY, m_rotationZ;

	/*Världsmatrisen*/
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
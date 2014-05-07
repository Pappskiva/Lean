////////////////////////////////////////////////////////////////////////////////
// Filename: Goal.h
////////////////////////////////////////////////////////////////////////////////

/*
Goal klassen ska anv�ndas f�r ett m�l objekt i spelet.
M�let i fr�ga har position, rotation och ett s�tt att f� dess worldmatris.
Klassen har ocks� ett mesh objekt, allts� det objekt som ska renderas ut.

Goal har ett v�rde i sig, m_nextLevelNumber som �r en integer som ska anv�ndas f�r att veta till vilken niv� som m�let ska leda till.
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

	/*Uppdatera worldmatrisen och f� den. UpdateWorldMatrix kallas i Render funktionen*/
	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	/*H�mta eller f� vilken bana som m�let ska leda till*/
	int		GetNextLevelNumber();
	void	SetNextLevelNumber(int);

	/*S�tt vilken shader som anv�nds*/
	void	SetShader(const Shader *shader);

private:

	/*Mesh, vilken shader som anv�nds och objektets textur*/
	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	/*Banan som m�let ska leda till*/
	int m_nextLevelNumber;

	/*Position p� objeketet samt rotationen*/
	v3 m_position;
	float m_rotationX, m_rotationY, m_rotationZ;

	/*V�rldsmatrisen*/
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
////////////////////////////////////////////////////////////////////////////////
// Filename: Obstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
Obstacle �r basklassen som anv�nds med alla hindren i spelet. F�rutom mesh, position, textur, rotation och liknande s� har obstacle �ven
ett enum v�rde Type som kan vara WATER, LEAF, TRAMPOLINE, MAGNET s� att man enkelt kan f� ut vilken typ av objekt det �r

Objektet i sig �r billboardat s� att det alltid �r v�nt mot kameran
*/

#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "D3D.h"
#include "DuckRenderer\DMath.h"
#include "Mesh.h"
#include <fstream>

#include "Texture.h"
#include "Ball.h"

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
	/*Klassen �r abstrakt*/
	virtual ~Obstacle() = 0;

	/*Initalisering, shutdown, uppdatering och render funktioner, alla virtual beroende p� vad som beh�vs*/
	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	/*Update tar deltatid, position x och z p� kameran och �ven rotation x och y p� planet*/
	virtual void Update(float, float, float, float, float, Ball*);
	virtual void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	/*S�tt och f� meshen*/
	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	/*S�tt och f� positionen*/
	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	/*S�tt och f� rotiationen*/
	void	SetRotation(float, float, float);
	void	GetRotation(v3&);

	/*Uppdatera och f� worldmatrisen*/
	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	/*F� ut vilken typ det �r*/
	Type	GetType();

	/*Best�m vilken shader som anv�nds vid rendering*/
	void	SetShader(const Shader *shader);

protected:
	/*Initialisering av buffrarna, och meshen.*/
	virtual bool 	InitalizeBuffers(D3D*);
	//void	ShutdownBuffers();



protected:

	/*Mesh, vilken shader som anv�nds och objektets textur*/
	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	/*Vilken typ som hindret �r*/
	Type m_Type;

	/*Position och rotation*/
	v3 m_position;
	float m_rotationX, m_rotationY, m_rotationZ;

	/*Worldmatris*/
	m4 worldMatrix;

	float cooldown; // Detta beh�vs fr�mst f�r studsmattan
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
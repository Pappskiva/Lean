////////////////////////////////////////////////////////////////////////////////
// Filename: Obstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
Obstacle är basklassen som används med alla hindren i spelet. Förutom mesh, position, textur, rotation och liknande så har obstacle även
ett enum värde Type som kan vara WATER, LEAF, TRAMPOLINE, MAGNET så att man enkelt kan få ut vilken typ av objekt det är

Objektet i sig är billboardat så att det alltid är vänt mot kameran
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
	/*Klassen är abstrakt*/
	virtual ~Obstacle() = 0;

	/*Initalisering, shutdown, uppdatering och render funktioner, alla virtual beroende på vad som behövs*/
	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	/*Update tar deltatid, position x och z på kameran och även rotation x och y på planet*/
	virtual void Update(float, float, float, float, float, Ball*);
	virtual void Render(D3D *direct3D);

	ID3D11ShaderResourceView* GetTexture();

	/*Sätt och få meshen*/
	void	SetMesh(const Mesh *mesh);
	Mesh *	GetMesh();

	/*Sätt och få positionen*/
	void	SetPosition(float, float, float);
	void	GetPosition(v3&);

	/*Sätt och få rotiationen*/
	void	SetRotation(float, float, float);
	void	GetRotation(v3&);

	/*Uppdatera och få worldmatrisen*/
	void	UpdateWorldMatrix();
	void	GetWorldMatrix(m4&);

	/*Få ut vilken typ det är*/
	Type	GetType();

	/*Bestäm vilken shader som används vid rendering*/
	void	SetShader(const Shader *shader);

protected:
	/*Initialisering av buffrarna, och meshen.*/
	virtual bool 	InitalizeBuffers(D3D*);
	//void	ShutdownBuffers();



protected:

	/*Mesh, vilken shader som används och objektets textur*/
	Mesh			*m_mesh;
	Shader			*m_shader;
	Texture			*m_Texture;

	/*Vilken typ som hindret är*/
	Type m_Type;

	/*Position och rotation*/
	v3 m_position;
	float m_rotationX, m_rotationY, m_rotationZ;

	/*Worldmatris*/
	m4 worldMatrix;

	float cooldown; // Detta behövs främst för studsmattan
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
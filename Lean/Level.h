////////////////////////////////////////////////////////////////////////////////
// Filename: Level.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <d3d11.h>
#include <fstream>

#include "D3D.h"
#include "DuckRenderer\DMath.h"

#include "Texture.h"

class Level
{
protected:

public:
	Level();
	Level(const Level&);
	~Level();

	bool Initialize(D3D*, WCHAR*);
	void Shutdown();
	void Update(float);
	void Render(D3D*);
	int	 GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float, float, float);
	void GetPosition(v3&);

	void SetRotation(float, float, float);
	void GetRotation(v3&);

	void SetRotationX(float);
	float GetRotationX();

	void SetRotationZ(float);
	float GetRotationZ();

	void SetScale(float);
	float GetScale();

	void GetNormal(v3&);

	void UpdateWorldMatrix();
	void GetWorldMatrix(m4&);

private:
	bool	InitializeBuffers(D3D*);
	void	ShutdownBuffers();

private:
	Mesh *m_mesh;

	Texture*	m_Texture;

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_scale;

	float m_MaximumRotationValue;

	m4 worldMatrix;
	v3 m_normal;

};
#endif
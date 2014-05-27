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
#include "DuckRenderer\DCollision.h"
#include "LevelLoaderClass.h"
#include "Texture.h"

class Level
{
protected:

public:
	Level();
	Level(const Level&);
	~Level();

	bool Initialize(D3D*, WCHAR*, WCHAR*);
	void Shutdown();
	void Update(float);
	void Render(D3D*);
	int	 GetIndexCount();

	ID3D11ShaderResourceView* GetTexture(int);

	void LoadLevel(const uint levelIndex, D3D* direct3D, LevelLoaderClass::ObstacleType *&obstacles, v3 &startPos, v3 &goalPos, int &nrOfObst);

	void SetPosition(float, float, float);
	void GetPosition(v3&);

	void SetRotation(float, float, float);
	void GetRotation(v3&);

	void SetRotationX(float);
	float GetRotationX();

	void SetRotationZ(float);
	float GetRotationZ();

	float GetMaxHeight() const;

	void SetScale(float);
	float GetScale();

	void GetNormal(v3&);

	void UpdateWorldMatrix();
	void GetWorldMatrix(m4&);

	uint GetWidth();
	uint GetLength();

	float SphereHeightmapIntersection(const v3 &center, const float radius, const v3 &velocity);

	void SetShader(const Shader *shader);

	void SetWorldMatrix(m4& world);
	float* GetHeighMapData();

private:
	bool	InitializeBuffers(D3D*);
	void	ShutdownBuffers();

	void	SetTextureBasedOnNumber(int, D3D*);

	void	_FilterHeightMap(float *floatArray, const uint arrayHeight, const uint arrayWidth);

private:
	Mesh *m_mesh;
	Shader *m_shader;
	Texture *m_Texture1, *m_Texture2, *m_mask;

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_scale;

	float m_MaximumRotationValue;

	//maybe for collision detection!
	float *heightmap;
	float *heightmapToPhys;
	uint triangleCount;
	Triangle *collisionTriangles;

	m4 worldMatrix;
	v3 m_normal;

	uint m_width;
	uint m_length;

	const float LEVEL_MAX_HEIGHT = 3.7f;

};

inline void Level::SetShader(const Shader *shader)
{
	this->m_shader = (Shader *)shader;
}

#endif
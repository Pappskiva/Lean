#pragma once
#include "btBulletCollisionCommon.h"
#include "Bullet\src\BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"
#include "D3D.h"



class Heightfield : public btHeightfieldTerrainShape
{
public:
	Heightfield(int m_HeightStickWidth, int m_HeightStickLeangth, const void * m_HeightMapData, btScalar m_HeightScale, btScalar m_MinHeight, btScalar m_MaxHeight, int m_UpAxis, PHY_ScalarType m_HeightDataType, bool m_FlipQuadEdges);

	~Heightfield();

	void GenerateDebugHeightmap(D3D* d3d);
	Mesh* GetDebugMesh();
private:
	Mesh* debug;
};


#include "Heightfield.h"



Heightfield::Heightfield(int m_HeightStickWidth, int m_HeightStickLeangth, const void * m_HeightMapData, btScalar m_HeightScale, btScalar m_MinHeight, btScalar m_MaxHeight, int m_UpAxis, PHY_ScalarType m_HeightDataType, bool m_FlipQuadEdges)
:btHeightfieldTerrainShape(m_HeightStickWidth, m_HeightStickLeangth, m_HeightMapData, m_HeightScale, m_MinHeight, m_MaxHeight, m_UpAxis, m_HeightDataType, m_FlipQuadEdges)
{
	m_width = m_HeightStickWidth;
	m_length = m_HeightStickLeangth;
}


Heightfield::~Heightfield()
{
}

void Heightfield::GenerateDebugHeightmap(D3D* d3d)
{
	uint vertexCount = 6 * (m_heightStickWidth)*(m_heightStickLength);

	struct Vertex
	{
		v3 position;
		v3 normal;
		float u, v;
	} *vertices = new Vertex[vertexCount];

	for (int i = 0; i < m_heightStickLength - 1; i++)
	{
		for (int j = 0; j < m_heightStickWidth - 1; j++)
		{
			int index = 6 * m_heightStickWidth*i + 6 * j;
			btVector3 temp;
			//Triangle 1
			// v1
			getVertex(j, i, temp);
			vertices[index].position = v3(temp.x(), temp.y(), temp.z());
			vertices[index].normal = v3(0, 1, 0);
			vertices[index].u = 0;
			vertices[index].v = 0;

			// v2
			getVertex(j, i + 1, temp);
			vertices[index + 1].position = v3(temp.x(), temp.y(), temp.z());
			vertices[index + 1].normal = v3(0, 1, 0);
			vertices[index + 1].u = 0;
			vertices[index + 1].v = 1;

			// v3
			getVertex(j + 1, i, temp);
			vertices[index + 2].position = v3(temp.x(), temp.y(), temp.z());
			vertices[index + 2].normal = v3(0, 1, 0);
			vertices[index + 2].u = 1;
			vertices[index + 2].v = 0;

			//Triangle 2
			// v1
			vertices[index + 3].position = v3(temp.x(), temp.y(), temp.z());
			vertices[index + 3].normal = v3(0, 1, 0);
			vertices[index + 3].u = 1;
			vertices[index + 3].v = 0;

			// v2
			getVertex(j, i + 1, temp);
			vertices[index + 4].position = v3(temp.x(), temp.y(), temp.z());
			vertices[index + 4].normal = v3(0, 1, 0);
			vertices[index + 4].u = 0;
			vertices[index + 4].v = 1;

			// v3
			getVertex(j + 1, i + 1, temp);
			vertices[index + 5].position = v3(temp.x(), temp.y(), temp.z());
			float test = vertices[index + 5].position.y;
			if (test > 100)
				test = 0;
			vertices[index + 5].normal = v3(0, 1, 0);
			vertices[index + 5].u = 1;
			vertices[index + 5].v = 1;
		}
	}

	debug = d3d->CreateMeshFromRam(vertices, sizeof(Vertex), vertexCount);

	delete[] vertices;
}

Mesh* Heightfield::GetDebugMesh()
{
	return debug;
}
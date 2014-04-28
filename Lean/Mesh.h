#pragma once

#include "D3D Includes.h"

#include <fstream>
#include <string>

typedef unsigned int uint;

struct MeshSubset
{
	uint startVertex;
	uint length;
};

class Mesh
{
protected:
	uint				vertexSize;
	uint				nrVertices;
	void				*vertices;
	
	uint				nrIndices;
	uint				*indices;

	uint				meshID;
	bool				gpuHasCopy;

	MeshSubset			*subsets;
	uint				nrSubsets;

	ID3D11Buffer		*vertexBuffer;
	ID3D11Buffer		*indexBuffer;
	
public:
	
						Mesh();
				
	void				Flush();
	void				ReleaseFromGPU();
	void				DeleteFromRam();

	void				Initialize(void *vertexData, const uint vertexSize, const uint nrVertices, uint *indices, const uint nrIndices);

	void				SaveAsDMesh(std::string filePath);
	uint				GetMeshID();
	//void				GetTriangles(Triangle *&triangleArray, uint &numTriangles) const;//wrongo, Måste fixa en vertexlayout. //test

	const MeshSubset&	GetMeshSubset(const uint index) const;
	uint				GetNumberOfSubsets() const;

	void const *		GetRamVerticesPointer() const;
	uint				GetVertexSize();

	friend class D3D;

private: 
	bool				LoadMeshFromObjToRam(std::string filePath);
};

inline uint Mesh::GetMeshID()
{
	return meshID;
}

inline uint Mesh::GetNumberOfSubsets() const
{
	return nrSubsets;
}

inline void const * Mesh::GetRamVerticesPointer() const
{
	return vertices;
}

inline uint Mesh::GetVertexSize()
{
	return vertexSize;
}
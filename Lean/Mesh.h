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
//The Mesh class contains all the required data to render a "model" onto the screen!
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

	//Vertex data is a must to pass, but not necessary the index data. It depends on the topology!
	void				Initialize(void *vertexData, const uint vertexSize, const uint nrVertices, uint *indices = nullptr, const uint nrIndices = 0);

	void				SaveAsDMesh(std::string filePath);
	uint				GetMeshID();

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
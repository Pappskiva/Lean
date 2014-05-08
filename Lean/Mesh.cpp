#include "Mesh.h"
#include "DuckRenderer\Array.h"
#include "DuckRenderer\DMath.h"

#define			WBOX(x) MessageBox(NULL, x, L"Mesh Error!!", MB_OK | MB_ICONASTERISK);
#define			SAFE_DELETE_ARRAY(x)	{ if(x){delete[] x; x = nullptr;}}
#define			SAFE_RELEASE(x)			{ if(x){x->Release(); x = nullptr;}}

Mesh::Mesh()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	vertices = nullptr;
	indices = nullptr;
	nrIndices = NULL;
	nrVertices = NULL;
	nrSubsets = NULL;
	subsets = NULL;
	meshID = -1;
	gpuHasCopy = false;
}

void Mesh::Flush()
{
	ReleaseFromGPU();
	DeleteFromRam();
}

void Mesh::ReleaseFromGPU()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	gpuHasCopy = false;
}

void Mesh::DeleteFromRam()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(subsets);
}
/*
void Mesh::ParseMTLFile(std::string path, Material &materials)
{
std::string buffer;
std::fstream mtlFile(path, std::ios::in);
gpuHasCopy = false;
bool materialFound = false;


if (!mtlFile.is_open())
return;
while(!mtlFile.eof())
{
mtlFile >> buffer;

if (buffer == "newmtl")
{
if (materialFound)
return;
else
materialFound = true;

mtlFile >> buffer;
materials.name = buffer.data();
}

if (buffer == "map_Kd")
{
mtlFile >> buffer;

Texture *temp = *materials.textures.Append(new Texture);

temp->name = buffer.data();
temp->path = ("Data//Textures//" + buffer).data();
materials.flags.Append(SVF_DIFFUSEMAP | SVF_INDEX_0);
}
}

}
*/
bool Mesh::LoadMeshFromObjToRam(std::string filePath)
{
	struct UV
	{
		float u, v;
	};

	struct Vertex
	{
		v3 pos;
		v3 normal;
		UV uv;
	};

	Array<v3> positions;
	Array<UV> uvs;
	Array<v3> normals;
	Array<Vertex> vertices;

	std::fstream file(filePath, std::ios::in | std::ios::ate);
	std::string buffer;
	uint endOfFile = file.tellg();
	file.seekg(0);
	char floatingPoint[16];
	uint offset = 0;
	int nrFaces = 0;
	int group = 0;

	Array<MeshSubset> tempSubsets(8);

	uint filePos = 0;
	if (!file.is_open())
		return false;

	while (!file.eof())
	{
		//file.getline(buffer, 64);
		file >> buffer;
		filePos = (uint)file.tellg();

		if (filePos == ((uint)-1))
			break;

		if (buffer == "v")
		{
			v3 pos;
			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			pos.v[0] = (float)atof(floatingPoint);

			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			pos.v[1] = (float)atof(floatingPoint);

			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			pos.v[2] = (float)atof(floatingPoint);

			positions.Append(pos);
		}
		else if (buffer == "vt")
		{
			UV uv;

			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			uv.u = (float)atof(floatingPoint);

			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			uv.v = (float)atof(floatingPoint);

			uvs.Append(uv);
		}
		else if (buffer == "vn")
		{
			v3 normal;
			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			normal.v[0] = (float)atof(floatingPoint);

			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			normal.v[1] = (float)atof(floatingPoint);

			memset(floatingPoint, 0, 16);
			file >> floatingPoint;
			normal.v[2] = (float)atof(floatingPoint);

			normals.Append(normal);
		}
		else if (buffer == "f")
		{
			Vertex vertex;
			int index = 0;
			++nrFaces;

			for (uint i = 0; i < 3; ++i)
			{
				file.getline(floatingPoint, 16, '/');
				index = atoi(floatingPoint);
				if (index == 0)
					break;

				vertex.pos = positions[index - 1];

				file.getline(floatingPoint, 16, '/');
				index = atoi(floatingPoint);
				vertex.uv = uvs[index - 1];

				if (i == 2)
					file.getline(floatingPoint, 16, '\n');
				else
					file.getline(floatingPoint, 16, ' ');

				index = atoi(floatingPoint);
				vertex.normal = normals[index - 1];

				vertices.Append(vertex);
			}
		}
		else if (buffer == "g")
		{
			file >> buffer;

			if (buffer == "default")
				continue;

			if (tempSubsets.Length() == 0)
			{
				MeshSubset temp;
				temp.length = 0;
				temp.startVertex = 0;

				tempSubsets.Append(temp);
			}
			else
			{
				MeshSubset temp;
				temp.startVertex = vertices.Length();

				tempSubsets.GetLast().length = temp.startVertex - tempSubsets.GetLast().startVertex;
				tempSubsets.Append(temp);
			}
		}
		else if (buffer == "mtllib")
		{
			std::string path = filePath;
			path.resize(filePath.find_last_of("//") + 1);

			file >> buffer;

			//ParseMTLFile(path + buffer, *material);

		}
		else if (buffer == "usemtl")
		{
			if (!tempSubsets.Length())
				WBOX(L"Group Error In .Obj File");

			file >> buffer;

			/*	HString temp = buffer.data();

			for(uint i = 0; i < materials.Length(); ++i)
			{
			if (materials[i]->name == temp)
			{
			uint index = tempSubsets.Length() - 1;
			tempSubsets[index].material = materials[i];
			break;
			}
			}*/
		}
	}
	file.close();

	if (tempSubsets.Length())
	{
		tempSubsets.GetLast().length = vertices.Length() - tempSubsets.GetLast().startVertex;
		nrSubsets = tempSubsets.Length();
		subsets = new MeshSubset[nrSubsets];
		memcpy(subsets, tempSubsets.Pointer(), sizeof(MeshSubset) * nrSubsets);
	}
	else
	{
		nrSubsets = 1;
		subsets = new MeshSubset[nrSubsets];
		subsets[0].length = vertices.Length();
		subsets[0].startVertex = 0;
	}

	this->vertexSize = sizeof(Vertex);
	this->vertices = new Vertex[sizeof(Vertex) * vertices.Length()];
	memcpy(this->vertices, vertices.Pointer(), sizeof(Vertex) * vertices.Length());
	this->nrVertices = vertices.Length();

	this->nrIndices = 0;
	this->indices = NULL;

	return true;


	return false;
}

void Mesh::SaveAsDMesh(std::string filePath)
{
	std::fstream file(filePath, std::ios::out | std::ios::binary);

	if (!vertices)
		return;

	file.write((char *)&nrVertices, sizeof(uint));
	file.write((char *)vertices, vertexSize * nrVertices);

	file.write((char *)&nrIndices, sizeof(uint));
	if (indices)
		file.write((char *)indices, sizeof(uint) * nrIndices);

	file.close();
}

void Mesh::Initialize(void *vertexData, const uint vertexSize, const uint nrVertices, uint *indices, const uint nrIndices)
{
	vertices = new uchar[vertexSize * nrVertices];
	memcpy(vertices, vertexData, vertexSize * nrVertices);
	this->nrVertices = nrVertices;
	this->vertexSize = vertexSize;

	if (indices)
	{
		this->indices = new uint[sizeof(uint) * nrIndices];
		memcpy(this->indices, indices, sizeof(uint) * nrIndices);
		this->nrIndices = nrIndices;
	}

	nrSubsets = 1;
	subsets = new MeshSubset[nrSubsets];
	subsets[0].startVertex = 0;
	subsets[0].length = nrVertices;
}
/*
void Mesh::GetTriangles(Triangle *&triangleArray, uint &numTriangles) const
{
if (triangleArray)
return;

if (nrIndices)
{

numTriangles = nrIndices / 3;
triangleArray = new Triangle[numTriangles];

struct tempVertex
{
v3 pos;
v3 normal;
float u, v;
}*vertexPointer = (tempVertex *) vertices;

for(uint i = 0, indicesIndex = 0; i < numTriangles; ++i)
{
triangleArray[i].points[0] = vertexPointer[indices[indicesIndex++]].pos;
triangleArray[i].points[1] = vertexPointer[indices[indicesIndex++]].pos;
triangleArray[i].points[2] = vertexPointer[indices[indicesIndex++]].pos;
}
}
else
{
numTriangles = nrVertices / 3;
triangleArray = new Triangle[numTriangles];

struct tempVertex
{
v3 pos;
v3 normal;
float u, v;
}*vertexPointer = (tempVertex *) vertices;

for(uint i = 0, indicesIndex = 0; i < numTriangles; ++i)
{
triangleArray[i].points[0] = vertexPointer[indicesIndex++].pos;
triangleArray[i].points[1] = vertexPointer[indicesIndex++].pos;
triangleArray[i].points[2] = vertexPointer[indicesIndex++].pos;
}
}
}
*/
const MeshSubset& Mesh::GetMeshSubset(const uint index) const
{
	return subsets[index];
}
////////////////////////////////////////////////////////////////////////////////
// Filename: LeafObstacle.cpp
////////////////////////////////////////////////////////////////////////////////
#include "LeafObstacle.h"

LeafObstacle::LeafObstacle() : Obstacle()
{
	m_Type = LEAVES;
}

LeafObstacle::~LeafObstacle()
{

}

bool LeafObstacle::Initialize(D3D* direct3D)
{
	bool result = true;

	/*Detta utkommenterade kodstycker är för framtiden, ifall vi önskar ladda in en mesh för objektet*/
	/*m_mesh = direct3D->LoadMeshFromOBJ(".obj");
	if (!m_mesh)
	return false;*/

	/*En lövtextur läses och laddas in till hindret*/
	result = InitalizeBuffers(direct3D);
	if (!result)
	{
		return false;
	}

	WCHAR* textureFileName = L"data//leaf.png";
	//Ladda in texturen till föremålet.	
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
	{
		return false;
	}

	return true;
}
void LeafObstacle::Shutdown()
{

}
//void LeafObstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ)
//{
//	m_rotationY = -atan2(m_position.x - cameraPosX, m_position.z - cameraPosZ) * 57.2957795131f; //I grader, då vi gör det till radianer igen i updateworldmatrix funktionen
//}

/*
bool LeafObstacle::InitalizeBuffers(D3D* direct3D)
{
uint vertexCount = 6;

struct Vertex
{
v3 position;
v3 normal;
float u, v;
} *vertices = new Vertex[vertexCount];

//En quad för enkelt trianglelist testning. En placeholder tills vi bestämt mer om hinder
// Fyller vertex arrayen med data

const float quadSize = .5f;

vertices[0].position = v3(quadSize, 0.0, 0.0f);
vertices[0].normal = v3(0, 1, 0);
vertices[0].u = 1.0f;
vertices[0].v = 0.0f;

vertices[1].position = v3(-quadSize, 0.0, 0.0f);
vertices[1].normal = v3(0, 1, 0);
vertices[1].u = 0.0f;
vertices[1].v = 0.0f;

vertices[2].position = v3(quadSize, quadSize * 2.0f, 0.0f);
vertices[2].normal = v3(0, 1, 0);
vertices[2].u = 1.0f;
vertices[2].v = 1.0f;

vertices[3].position = v3(quadSize, quadSize * 2.0f, 0.0f);
vertices[3].normal = v3(0, 1, 0);
vertices[3].u = 1.0f;
vertices[3].v = 1.0f;

vertices[4].position = v3(-quadSize, 0.0, 0.0f);
vertices[4].normal = v3(0, 1, 0);
vertices[4].u = 0.0f;
vertices[4].v = 0.0f;

vertices[5].position = v3(-quadSize, quadSize * 2.0f, 0.0f);
vertices[5].normal = v3(0, 1, 0);
vertices[5].u = 0.0f;
vertices[5].v = 1.0f;

// Release the arrays now that the vertex and index buffers have been created and loaded.

m_mesh = direct3D->CreateMeshFromRam(vertices, sizeof(Vertex), vertexCount);

delete[] vertices;

return true;
}
*/

void LeafObstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ, float planeRotationX, float planeRotationZ, Ball *ball, ParticleHandler *particles)
{
	Obstacle::Update(deltaTime, cameraPosX, cameraPosZ, planeRotationX, planeRotationZ, ball, particles);

	// Räknar ut avstånd till boll
	v3 ballPos;
	ball->GetPosition(ballPos);
	v3 vectorToBall = ballPos - m_position;
	float squaredDistance = vectorToBall.x * vectorToBall.x + vectorToBall.z * vectorToBall.z;

	if (squaredDistance < 10 && cooldown <= 0)
	{
		particles->AddParticles(1);
		cooldown = 31;
	}
}
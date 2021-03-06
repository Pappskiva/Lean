////////////////////////////////////////////////////////////////////////////////
// Filename: TrampolineObstacle.cpp
////////////////////////////////////////////////////////////////////////////////

#include "TrampolineObstacle.h"

TrampolineObstacle::TrampolineObstacle() : Obstacle()
{
	m_Type = TRAMPOLINE;
}

TrampolineObstacle::~TrampolineObstacle()
{

}

bool TrampolineObstacle::Initialize(D3D* direct3D, HWND hwnd)
{
	bool result = true;

	/*Detta utkommenterade kodstycker �r f�r framtiden, ifall vi �nskar ladda in en mesh f�r objektet*/
	/*m_mesh = direct3D->LoadMeshFromOBJ(".obj");
	if (!m_mesh)
	return false;*/

	result = InitalizeBuffers(direct3D);
	if (!result)
	{
		return false;
	}

	/*En trampolintextur l�ses och laddas in till hindret*/

	WCHAR* textureFileName = L"data//trampoline.png";
	//Ladda in texturen till f�rem�let.	
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
	{
		return false;
	}

	m_Sound = new Sound;
	m_Sound->Initialize2DSound(hwnd, "data/musik/StudsmattaHindret/SpringMono.wav", 0, 0, 0);

	return true;
}
void TrampolineObstacle::Shutdown()
{
	m_Sound->Shutdown();
	delete m_Sound;
	m_Sound = 0;
}
//void TrampolineObstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ)
//{
//	m_rotationY = -atan2(m_position.x - cameraPosX, m_position.z - cameraPosZ) * 57.2957795131f; //I grader, d� vi g�r det till radianer igen i updateworldmatrix funktionen
//}
/*
bool TrampolineObstacle::InitalizeBuffers(D3D* direct3D)
{
uint vertexCount = 6;

struct Vertex
{
v3 position;
v3 normal;
float u, v;
} *vertices = new Vertex[vertexCount];

//En quad f�r enkelt trianglelist testning. En placeholder tills vi best�mt mer om hinder
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
}*/
void TrampolineObstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ, float planeRotationX, float planeRotationZ, Ball *ball, ParticleHandler *particles)
{
	Obstacle::Update(deltaTime, cameraPosX, cameraPosZ, planeRotationX, planeRotationZ, ball, particles);

	// R�knar ut avst�nd till boll
	v3 ballPos;
	ball->GetPosition(ballPos);
	v3 vectorToBall = ballPos - m_position;
	float squaredDistance = vectorToBall.x * vectorToBall.x + vectorToBall.z * vectorToBall.z;

	if (squaredDistance < 9 && cooldown <= 0)
	{
		m_Sound->PlayOnce();
		ball->AddForce(v3(0, 75, 0));
		cooldown = 1;
	}
}
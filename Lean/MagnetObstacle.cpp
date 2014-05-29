////////////////////////////////////////////////////////////////////////////////
// Filename: -MagnetObstacle.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MagnetObstacle.h"

MagnetObstacle::MagnetObstacle() : Obstacle()
{
	m_Type = MAGNET;
}

MagnetObstacle::~MagnetObstacle()
{

}

bool MagnetObstacle::Initialize(D3D* direct3D, HWND hwnd)
{
	bool result = true;
	
	/*Detta utkommenterade kodstycker är för framtiden, ifall vi önskar ladda in en mesh för objektet*/
	/*m_mesh = direct3D->LoadMeshFromOBJ(".obj");
	if (!m_mesh)
	return false;*/

	result = InitalizeBuffers(direct3D);
	if (!result)
	{
		return false;
	}

	/*En magnettextur läses och laddas in till hindret*/
	WCHAR* textureFileName = L"data//magnet.png";
	//Ladda in texturen till föremålet.	
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
	{
		return false;
	}
	m_Sound = new Sound;
	m_Sound->Initialize2DSound(hwnd, "data/musik/MagnetHinder/MagnetMono2.wav", m_position.x, m_position.y, m_position.z);

	return true;
}
void MagnetObstacle::Shutdown()
{
	m_Sound->Shutdown();
	delete m_Sound;
	m_Sound = 0;
}
//void MagnetObstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ)
//{
//	m_rotationY = -atan2(m_position.x - cameraPosX, m_position.z - cameraPosZ) * 57.2957795131f; //I grader, då vi gör det till radianer igen i updateworldmatrix funktionen
//}

/*
bool MagnetObstacle::InitalizeBuffers(D3D* direct3D)
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
void MagnetObstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ, float planeRotationX, float planeRotationZ, Ball *ball, ParticleHandler *particles)
{
	Obstacle::Update(deltaTime, cameraPosX, cameraPosZ, planeRotationX, planeRotationZ, ball, particles);
	// Räknar ut avstånd till boll
	v3 ballPos;
	ball->GetPosition(ballPos);
	v3 vectorToBall = (ballPos - m_position) * 0.7; // Vill öka hindrets range
	vectorToBall.y = 0;

	v3 soundV;
	soundV.x = ballPos.x + 1;
	soundV.y = ballPos.y;
	soundV.z = ballPos.z - 1;

	m_Sound->UpdateListener(ballPos.x, ballPos.y, ballPos.z, soundV);

	float squaredDistance = vectorToBall.x * vectorToBall.x + vectorToBall.z * vectorToBall.z;
	if (squaredDistance < 100) // Använder inte cooldown
	{

		if (squaredDistance > 1)
		{
			v3 forceToAdd = (-vectorToBall / squaredDistance) * deltaTime * 75; // drar bollen till sig
			ball->AddForce(forceToAdd);
			m_Sound->Play3DSound();
		}
		else //if(squaredDistance >= 0.2f)
		{
			//float konstant = (-vectorToBall / squaredDistance).Length() * 75 + vectorToBall.Length(); // Konstant för att få samma värde i övergång mellan zoner

			float length = vectorToBall.Length();
			vectorToBall.Normalize();
			v3 forceToAdd = (-vectorToBall) * (76.438 - length) * deltaTime;
			ball->AddForce(forceToAdd);
			m_Sound->Play3DSound();
		}
	}
}
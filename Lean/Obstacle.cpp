////////////////////////////////////////////////////////////////////////////////
// Filename: Obstacle.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Obstacle.h"

Obstacle::Obstacle()
{
	/*Sätt alla variabler till 0 eller standard/testvärden*/
	m_Texture = 0;

	m_position.x = 3.0f;
	m_position.y = 1.0f;
	m_position.z = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	worldMatrix = m4::IDENTITY;
}

Obstacle::Obstacle(const Obstacle& other)
{

}

Obstacle::~Obstacle()
{

}

bool Obstacle::Initialize(D3D* direct3D)
{
	/*Eftersom klassen är virtual så bör detta aldrig faktiskt hända*/
	bool result = true;

	/*Meshen skapas*/
	result = InitalizeBuffers(direct3D);
	if (!result)
	{
		return false;
	}

	/*Hämta och ladda in texturen till föremålet*/
	WCHAR* textureFileName = L"data//testTexture.png";
	//Ladda in texturen till föremålet.	
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
	{
		return false;
	}

	return true;
}

void Obstacle::Shutdown()
{
	// Release vertex och index arrayen
	//ShutdownBuffers();

	return;
}

void Obstacle::Update(float deltaTime, float cameraPosX, float cameraPosZ, float planeRotationX, float planeRotationZ)
{
	/*Uppdatera hindret så det ligger utmed banan rätt*/
	m_rotationX = planeRotationX;
	m_rotationZ = planeRotationZ;
	/*billboarding rotation*/
	m_rotationY = -atan2(m_position.x - cameraPosX, m_position.z - cameraPosZ) * 57.2957795131f; //I grader, då vi gör det till radianer igen i updateworldmatrix funktionen
}

void Obstacle::Render(D3D *direct3D)
{
	//direct3D->SetTexture(blabla)
	UpdateWorldMatrix();

	direct3D->SetShader(m_shader);
	m_shader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));
	direct3D->ApplyConstantBuffers();
	direct3D->ApplyTexture(m_Texture, 0);

	direct3D->RenderMesh(m_mesh);

	return;
}


ID3D11ShaderResourceView* Obstacle::GetTexture()
{
	return m_Texture->GetTexture();
}

void Obstacle::SetPosition(float positionX, float positionY, float positionZ)
{
	/*Sätt positionen*/
	m_position.x = positionX;
	m_position.y = positionY;
	m_position.z = positionZ;
}

void Obstacle::GetPosition(v3& pos)
{
	/*Få positionen*/
	pos = m_position;
}

void Obstacle::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	/*Sätt rotationen*/
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Obstacle::GetRotation(v3& rot)
{
	/*Få rotationen*/
	rot = v3(m_rotationX, m_rotationY, m_rotationZ);
}

Type Obstacle::GetType()
{
	return m_Type;
}

void Obstacle::UpdateWorldMatrix()
{
	/*Ändra worldmatrisen baserat på postion, rotation och skala.*/

	/*Eftersom det finns två typer av rotation, billboarding och utmed planet så behövs två matriser skapas*/
	m4 billBoardRotationMatrix;
	billBoardRotationMatrix = m4::CreateYawPitchRoll(m_rotationY * 0.0174532925f, 0.0f, 0.0f);
	m4 worldRotationMatrix;
	worldRotationMatrix = m4::CreateYawPitchRoll(0.0f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	m4 scaleMatrix;
	scaleMatrix = m4::CreateScale(v3(1.0f, 1.0f, 1.0f));
	m4 localSpace;
	localSpace = m4::CreateTranslation(m_position);

	/*Matriserna multipliceras ihop*/
	worldMatrix = scaleMatrix * billBoardRotationMatrix * localSpace * worldRotationMatrix;
}

void Obstacle::GetWorldMatrix(m4& worldMatrix)
{
	//Få worldmatrisen
	worldMatrix = this->worldMatrix;
}

//void Obstacle::ShutdownBuffers()
//{
//	// Release indexbuffern
//	return;
//}

bool Obstacle::InitalizeBuffers(D3D* direct3D)
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
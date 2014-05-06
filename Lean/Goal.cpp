////////////////////////////////////////////////////////////////////////////////
// Filename: Goal.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Goal.h"

Goal::Goal()
{
	m_Texture = 0;

	m_position.x = 3.0f;
	m_position.y = 1.0f;
	m_position.z = 2.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_nextLevelNumber = 0;

	worldMatrix = m4::IDENTITY;
}

Goal::Goal(const Goal& other)
{

}

Goal::~Goal()
{

}

bool Goal::Initialize(D3D* direct3D, WCHAR* textureFileName)
{
	bool result = true;

	//Load in a sphere, something round.
	m_mesh = direct3D->LoadMeshFromOBJ("data//Sphere.obj");
	if (!m_mesh)
	return false;

	//Ladda in texturen till föremålet.	
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
	{
		return false;
	}

	return true;
}

void Goal::Shutdown()
{
	// Release vertex och index arrayen
	//ShutdownBuffers();

	return;
}

void Goal::Update(float deltaTime, float planeRotationX, float planeRotationZ)
{
	//Uppdatera målet
	m_rotationX = planeRotationX;
	m_rotationZ = planeRotationZ;
}

void Goal::Render(D3D *direct3D)
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


ID3D11ShaderResourceView* Goal::GetTexture()
{
	return m_Texture->GetTexture();
}

void Goal::SetPosition(float positionX, float positionY, float positionZ)
{
	//Sätt positionen
	m_position.x = positionX;
	m_position.y = positionY;
	m_position.z = positionZ;
}

void Goal::GetPosition(v3& pos)
{
	//Få positionen
	pos = m_position;
}

void Goal::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	//Sätt rotationen
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Goal::GetRotation(v3& rot)
{
	//Få rotationen
	rot = v3(m_rotationX, m_rotationY, m_rotationZ);
}

int Goal::GetNextLevelNumber()
{
	return m_nextLevelNumber;
}

void Goal::UpdateWorldMatrix()
{
	//Ändra worldmatrisen baserat på postion, rotation och skala.
	m4 rotationMatrix;
	rotationMatrix = m4::CreateYawPitchRoll(m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	m4 scaleMatrix;
	scaleMatrix = m4::CreateScale(v3(1.0f, 0.005f, 1.0f));
	m4 localSpace;
	localSpace = m4::CreateTranslation(m_position);

	worldMatrix = scaleMatrix * localSpace * rotationMatrix;
}

void Goal::GetWorldMatrix(m4& worldMatrix)
{
	//Få worldmatrisen
	worldMatrix = this->worldMatrix;
}

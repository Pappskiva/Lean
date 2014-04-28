////////////////////////////////////////////////////////////////////////////////
// Filename: Ball.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Ball.h"

Ball::Ball()
{
	m_Texture = 0;

	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 5.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_radius = 0.5f;

	m_velocity = v3(0.0f);

	worldMatrix = m4::IDENTITY;
}

Ball::Ball(const Ball& other)
{

}

Ball::~Ball()
{

}

bool Ball::Initialize(D3D* direct3D, WCHAR* textureFileName)
{
	bool result = true;

	m_mesh = direct3D->LoadMeshFromOBJ("Sphere.obj");
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

void Ball::Shutdown()
{
	// Release vertex och index arrayen
	ShutdownBuffers();

	return;
}

void Ball::Update(float deltaTime)
{
	//Uppdatera bollen

	m_position += m_velocity * deltaTime;

	//Testrotation
	this->m_rotationY = this->m_rotationY + 20.0f * deltaTime;
}

void Ball::Render(D3D *direct3D)
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


ID3D11ShaderResourceView* Ball::GetTexture()
{
	return m_Texture->GetTexture();
}

void Ball::SetPosition(float positionX, float positionY, float positionZ)
{
	//Sätt positionen
	m_position.x = positionX;
	m_position.y = positionY;
	m_position.z = positionZ;
}

void Ball::GetPosition(v3& pos)
{
	//Få positionen
	pos = m_position;
}

void Ball::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	//Sätt rotationen
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Ball::GetRotation(v3& rot)
{
	//Få rotationen
	rot = v3(m_rotationX, m_rotationY, m_rotationZ);
}

void Ball::SetRadius(float radius)
{
	//Bestäm radien, skalan blir det dubbla av radien
	m_radius = radius;
}

float Ball::GetRadius()
{
	//Få radien
	return m_radius;
}

void Ball::UpdateWorldMatrix()
{
	//Ändra worldmatrisen baserat på postion, rotation och skala.
	m4 rotationMatrix;
	rotationMatrix = m4::CreateYawPitchRoll(m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	m4 scaleMatrix;
	scaleMatrix = m4::CreateScale(v3(m_radius, m_radius, m_radius));
	m4 localSpace;
	localSpace = m4::CreateTranslation(m_position);

	worldMatrix = scaleMatrix * rotationMatrix * localSpace;
}

void Ball::GetWorldMatrix(m4& worldMatrix)
{
	//Få worldmatrisen
	worldMatrix = this->worldMatrix;
}

void Ball::ShutdownBuffers()
{
	// Release indexbuffern
	return;
}
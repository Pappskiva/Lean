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
	m_friction = 1;

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

void Ball::Shutdown()
{
	// Release vertex och index arrayen
	ShutdownBuffers();

	return;
}

void Ball::Update(float deltaTime)
{
	// Position och rotation uppdateras i PhysicsBridge
	////Uppdatera bollen
	//m_position += m_velocity * deltaTime;

	////Testrotation
	//this->m_rotationY = this->m_rotationY + 20.0f * deltaTime;

	if (m_friction < 1)
	{
		m_friction += 0.07 * deltaTime;
	}
}

void Ball::Render(D3D *direct3D)
{
	direct3D->SetShader(m_shader);
	Shader *currentShader = direct3D->GetCurrentShader();

	m4 newWorld = worldMatrix;
	static const v3 offset(0, -0.2f, 0);
	newWorld.Translate(offset);
	currentShader->SetVariable("worldMatrix", &newWorld, sizeof(m4));
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

void Ball::SetPosition(const v3 &pos)
{
	this->m_position = pos;
}

const v3& Ball::GetPosition() const
{
	return m_position;
}

void Ball::SetFlatPosition(float positionX, float positionY, float positionZ)
{
	//Sätt positionen
	m_flatPosition.x = positionX;
	m_flatPosition.y = positionY;
	m_flatPosition.z = positionZ;
}

void Ball::GetFlatPosition(v3& pos)
{
	//Få positionen
	pos = m_flatPosition;
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
void Ball::SetWorldMatrix(m4 world)
{
	worldMatrix = world;
}

void Ball::MakeSlippery()
{
	m_friction = 0.01;
}
float Ball::GetFriction()
{
	return m_friction;
}
void Ball::SetFriction(float friction)
{
	m_friction = friction;
}
void Ball::AddForce(v3 force)
{
	m_addedForce += force;
}
v3 Ball::GetAndZeroAddedForce()
{
	if (m_addedForce.Length() != 0)
	{
		v3 returnForce = m_addedForce;
		m_addedForce = v3(0, 0, 0);
		return returnForce;
	}
	else
	{
		return 0;
	}
}

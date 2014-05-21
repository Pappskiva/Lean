////////////////////////////////////////////////////////////////////////////////
// Filename: Box.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Box.h"

Box::Box()
{
	/*Alla privata variabler sätts till 0 eller standard/testvärden*/

	m_Texture = 0;

	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_scale = 1.0f;

	worldMatrix = m4::IDENTITY;
}

Box::Box(const Box& other)
{

}

Box::~Box()
{

}

bool Box::Initialize(D3D* direct3D, WCHAR* textureFileName)
{
	bool result = true;

	/*Ladda in en sfär*/
	m_mesh = direct3D->LoadMeshFromOBJ("data//box.obj");
	if (!m_mesh)
		return false;

	/*Ladda in texturen till föremålet.*/
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
	{
		return false;
	}

	return true;
}

void Box::Shutdown()
{
	// Release vertex och index arrayen
	//ShutdownBuffers();

	return;
}

void Box::Update(float deltaTime, float planeRotationX, float planeRotationZ)
{
	/*Uppdatera målet. Vi får även rotationen på banan så att målet ligger rätt utmed det.*/
	//m_rotationX = planeRotationX;
	//m_rotationZ = planeRotationZ;
}

void Box::Render(D3D *direct3D)
{
	/*Worldmatrisen uppdateras baserat på position och rotation*/
	UpdateWorldMatrix();

	direct3D->SetShader(m_shader);
	m_shader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));
	direct3D->ApplyConstantBuffers();
	direct3D->ApplyTexture(m_Texture, 0);

	direct3D->RenderMesh(m_mesh);

	return;
}


ID3D11ShaderResourceView* Box::GetTexture()
{
	return m_Texture->GetTexture();
}

void Box::SetPosition(float positionX, float positionY, float positionZ)
{
	/*Sätt positionen*/
	m_position.x = positionX;
	m_position.y = positionY;
	m_position.z = positionZ;
}

void Box::GetPosition(v3& pos)
{
	/*Få positionen*/
	pos = m_position;
}

void Box::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	/*Sätt rotationen*/
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Box::GetRotation(v3& rot)
{
	/*Få rotationen*/
	rot = v3(m_rotationX, m_rotationY, m_rotationZ);
}

void Box::SetScale(float scale)
{
	m_scale = scale;
}

void Box::UpdateWorldMatrix()
{
	/*Ändra worldmatrisen baserat på postion, rotation och skala.*/
	m4 rotationMatrix;
	rotationMatrix = m4::CreateYawPitchRoll(m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	m4 scaleMatrix;
	scaleMatrix = m4::CreateScale(v3(m_scale, HEIGHT, m_scale));
	m4 localSpace;
	localSpace = m4::CreateTranslation(m_position);

	worldMatrix = scaleMatrix * localSpace * rotationMatrix;
}

void Box::GetWorldMatrix(m4& worldMatrix)
{
	/*Få worldmatrisen*/
	worldMatrix = this->worldMatrix;
}

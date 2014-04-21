////////////////////////////////////////////////////////////////////////////////
// Filename: Ball.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Ball.h"

Ball::Ball()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;

	m_scale = 1.0f;
	m_radius = m_scale / 2.0f;

	m_positionX = 2.0f;
	m_positionY = 1.0f + m_radius;
	m_positionZ = 2.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	D3DXMatrixIdentity(&worldMatrix);
}

Ball::Ball(const Ball& other)
{

}

Ball::~Ball()
{

}

bool Ball::Initialize(ID3D11Device* device, WCHAR* textureFileName)
{
	bool result;

	// Initialisera vertex och index buffrarna för att skapa föremålets geometri.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//Ladda in texturen till föremålet.
	result = LoadTexture(device, textureFileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void Ball::Shutdown()
{
	// Release texturen
	ReleaseTexture();
	// Release vertex och index arrayen
	ShutdownBuffers();

	return;
}

void Ball::Update(float deltaTime)
{	
	//Testrotation
	//this->m_rotationY = this->m_rotationY + 20.0f * deltaTime;
	//this->m_rotationX = this->m_rotationX + 30.0f * deltaTime;
	//this->m_rotationZ = this->m_rotationZ + 10.0f * deltaTime;
}

void Ball::Render(ID3D11DeviceContext* deviceContext)
{
	// Lägg in vertex och index buffrarna på grafikpipelinen för att förbereda dem för rendering.
	RenderBuffers(deviceContext);

	return;
}

int Ball::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Ball::GetTexture()
{
	return m_Texture->GetTexture();
}

void Ball::SetPosition(float positionX, float positionY, float positionZ)
{
	//Sätt positionen
	m_positionX = positionX;
	m_positionY = positionY;
	m_positionZ = positionZ;
}

void Ball::GetPosition(D3DXVECTOR3& pos)
{
	//Få positionen
	pos = D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

void Ball::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	//Sätt rotationen
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Ball::GetRotation(D3DXVECTOR3& rot)
{
	//Få rotationen
	rot = D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Ball::SetScale(float scale)
{
	//Sätt skalan, radien blir hälften av skalan
	m_scale = scale;
	m_radius = m_scale / 2;
}

float Ball::GetScale()
{
	//Få skalan
	return m_scale;
}

void Ball::SetRadius(float radius)
{
	//Bestäm radien, skalan blir det dubbla av radien
	m_radius = radius;
	m_scale = radius * 2.0f;
}

float Ball::GetRadius()
{
	//Få radien
	return m_radius;
}

void Ball::UpdateWorldMatrix()
{
	//Ändra worldmatrisen baserat på postion, rotation och skala.
	D3DXMATRIX rotationSphereMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationSphereMatrix, m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	D3DXMATRIX scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix, m_scale, m_scale, m_scale);
	D3DXMATRIX localSpace;
	D3DXMatrixTranslation(&localSpace, m_positionX, m_positionY, m_positionZ);

	D3DXMatrixMultiply(&worldMatrix, &rotationSphereMatrix, &localSpace);
	D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);
}

void Ball::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	//Få worldmatrisen
	worldMatrix = this->worldMatrix;
}

bool Ball::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Bestäm antalet vertexer i vertex arrayen
	m_vertexCount = 300;

	// Bestäm antalet index i index arrayen
	m_indexCount = 300;

	// Skapa vertex arrayen
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Skapa index arrayen
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//LoadSphere(vertices, indices);
	

	//Väldigt enkel pointlist sfär för debug och tillfälligt föremål.
	m_vertexCount = 300;
	m_indexCount = 300;
	for (int i = 0; i < 100; i++)
	{
		vertices[i].position = D3DXVECTOR3(cosf(i)/2, sinf(i)/2, 0.0f);
		vertices[i].texture = D3DXVECTOR2(0.0f, 0.0f);
		vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		indices[i] = i;
	}
	for (int i = 100; i < 200; i++)
	{
		vertices[i].position = D3DXVECTOR3(0.0f, sinf(i)/2, cosf(i)/2);
		vertices[i].texture = D3DXVECTOR2(0.0f, 0.0f);
		vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		indices[i] = i;
	}
	for (int i = 200; i < 300; i++)
	{
		vertices[i].position = D3DXVECTOR3(cosf(i)/2, 0.0f, sinf(i)/2);
		vertices[i].texture = D3DXVECTOR2(0.0f, 0.0f);
		vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		indices[i] = i;
	}


	/*
	//En triangel för enkelt trianglelist testning. Bollen behövs fortfarande göras.
	// Fyller vertex arrayen med data
	vertices[0].position = D3DXVECTOR3(-0.5f, -1.0f, 0.0f);  // Botten till vänster
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(-0.5f, 1.0f, 0.0f);  // Toppen till vänster
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(0.5f, -1.0f, 0.0f);  // Botten till höger
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Fyll index array med data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	*/
	

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Ball::LoadSphere(VertexType* vertices, unsigned long* indices)
{
	/*Det är tänkt att bollen ska laddas in från en fil, som .obj, men det har inte gjorts än*/
}

void Ball::ShutdownBuffers()
{
	// Release indexbuffern.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release vertexbuffern.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Ball::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	return;
}

bool Ball::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Skapa texturobjektet.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialisera texturobjektet.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Ball::ReleaseTexture()
{
	// Release texturobjektet.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
// Filename: Level.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Level.h"

Level::Level()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_scale = 5.0f;

	m_MaximumRotationValue = 20.0f; //Degrees

	D3DXMatrixIdentity(&worldMatrix);
}

Level::Level(const Level& other)
{

}

Level::~Level()
{

}

bool Level::Initialize(ID3D11Device* device, WCHAR* textureFileName)
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

void Level::Shutdown()
{
	// Release texturen
	ReleaseTexture();
	// Release vertex och index arrayen
	ShutdownBuffers();

	return;
}

void Level::Update(float deltaTime)
{
	//Uppdatera banan

	//Kolla så banan inte lutar för mycket åt båda riktningar i x-led.
	if (m_rotationX > m_MaximumRotationValue)
	{
		m_rotationX = m_MaximumRotationValue;
	}
	else if (m_rotationX < -m_MaximumRotationValue)
	{
		m_rotationX = -m_MaximumRotationValue;
	}

	//Kolla så banan inte lutar för mycket åt båda riktningar i z-led.
	if (m_rotationZ > m_MaximumRotationValue)
	{
		m_rotationZ = m_MaximumRotationValue;
	}
	else if (m_rotationZ < -m_MaximumRotationValue)
	{
		m_rotationZ = -m_MaximumRotationValue;
	}

}

void Level::Render(ID3D11DeviceContext* deviceContext)
{
	// Lägg in vertex och index buffrarna på grafikpipelinen för att förbereda dem för rendering.
	RenderBuffers(deviceContext);

	return;
}

int Level::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Level::GetTexture()
{
	return m_Texture->GetTexture();
}

void Level::SetPosition(float positionX, float positionY, float positionZ)
{
	//Sätt positionen
	m_positionX = positionX;
	m_positionY = positionY;
	m_positionZ = positionZ;
}

void Level::GetPosition(D3DXVECTOR3& pos)
{
	//Få positionen
	pos = D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

void Level::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	//Sätt rotationen
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Level::GetRotation(D3DXVECTOR3& rot)
{
	//Få rotationen
	rot = D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

float Level::GetRotationX()
{
	return m_rotationX;
}

void Level::SetRotationX(float rotX)
{
	m_rotationX = rotX;
}

float Level::GetRotationZ()
{
	return m_rotationZ;
}

void Level::SetRotationZ(float rotZ)
{
	m_rotationZ = rotZ;
}

void Level::SetScale(float scale)
{
	//Sätt skalan
	m_scale = scale;
}

float Level::GetScale()
{
	//Få skalan
	return m_scale;
}

void Level::GetNormal(D3DXVECTOR3& normal)
{
	//Ändrar och hämtar normalvektorn
	m_normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX inverseRotation;
	D3DXMATRIX transpose;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	
	D3DXMatrixInverse(&inverseRotation, NULL, &rotationMatrix);
	
	D3DXMatrixTranspose(&transpose, &inverseRotation);

	D3DXVec3TransformNormal(&normal, &m_normal, &transpose);
}

void Level::UpdateWorldMatrix()
{
	//Ändra worldmatrisen baserat på postion, rotation och skala.
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scaleMatrix;
	D3DXMATRIX localSpace;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	D3DXMatrixScaling(&scaleMatrix, m_scale, m_scale, m_scale);
	D3DXMatrixTranslation(&localSpace, m_positionX, m_positionY, m_positionZ);

	D3DXMatrixMultiply(&worldMatrix, &rotationMatrix, &localSpace);
	D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);
}

void Level::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	//Få worldmatrisen
	worldMatrix = this->worldMatrix;
}

bool Level::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Bestäm antalet vertexer i vertex arrayen
	m_vertexCount = 6;

	// Bestäm antalet index i index arrayen
	m_indexCount = 6;

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

	//En quad för enkelt trianglelist testning. En placeholder tills vi faktiskt kunnat arbeta mer på banor
	// Fyller vertex arrayen med data
	vertices[0].position = D3DXVECTOR3(-0.5f, 0.0f, -0.5f);  // Botten till vänster
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);  // Toppen till vänster
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(0.5f, 0.0f, -0.5f);  // Botten till höger
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);  // Toppen till vänster
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(0.5f, 0.0f, 0.5f);  // Toppen till höger
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[4].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(0.5f, 0.0f, -0.5f);  // Botten till höger
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[5].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Fyll index array med data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;
	


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

void Level::ShutdownBuffers()
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

void Level::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Level::LoadTexture(ID3D11Device* device, WCHAR* filename)
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

void Level::ReleaseTexture()
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

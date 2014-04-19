#include "Ball.h"

Ball::Ball()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 5.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_scale = 1.0f;
	m_radius = m_scale/2.0f;

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


	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void Ball::Shutdown()
{

	ReleaseTexture();
	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void Ball::Update(float deltaTime)
{
	this->m_positionZ = this->m_positionZ + 1.0f * deltaTime;
}

void Ball::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
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
	m_positionX = positionX;
	m_positionY = positionY;
	m_positionZ = positionZ;
}

void Ball::GetPosition(D3DXVECTOR3& pos)
{
	pos = D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

void Ball::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Ball::GetRotation(D3DXVECTOR3& rot)
{
	rot = D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Ball::SetScale(float scale)
{
	m_scale = scale;
	m_radius = m_scale / 2;
}

float Ball::GetScale()
{
	return m_scale;
}

void Ball::SetRadius(float radius)
{
	m_radius = radius;
	m_scale = radius * 2.0f;
}

float Ball::GetRadius()
{
	return m_radius;
}

void Ball::UpdateWorldMatrix()
{
	//Change the worldMatrix based on the position, rotatio and scale.
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * 0.0174532925f, m_rotationY * 0.0174532925f, m_rotationZ * 0.0174532925f);
	D3DXMATRIX scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix, m_scale, m_scale, m_scale);
	D3DXMATRIX localSpace;
	D3DXMatrixTranslation(&localSpace, m_positionX, m_positionY, m_positionZ);

	D3DXMatrixMultiply(&worldMatrix, &rotationMatrix, &localSpace);

	D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);
}

void Ball::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = this->worldMatrix;
}

bool Ball::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	LoadSphere(vertices, indices);
	
	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(0.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top Left.
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.*/
	

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

}

void Ball::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Ball::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Ball::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

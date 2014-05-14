#include "ImageClass.h"

ImageClass::ImageClass()
{
	this->mTexture = 0;
	this->mShader = 0;
}

ImageClass::~ImageClass()
{

}

bool ImageClass::Initialize(D3D* d3d, WCHAR* textureFileName, int screenW, int screenH, int imageW, int imageH)
{
	bool result;

	mPosX = 0;
	mPosY = 0;
	mScreenWidth = screenW;
	mScreenHeight = screenH;
	mImageWidth = imageW;
	mImageHeight = imageH;

	// Initiera vertex och index buffrar
	result = InitializeBuffers(d3d);
	if (!result)
	{
		return false;
	}

	// Ladda in texturen
	mTexture = d3d->LoadTextureFromFile(textureFileName);
	if (!mTexture)
	{
		return false;
	}

	// Skapa och initiera imageShaderClass objekt
	D3D11_INPUT_ELEMENT_DESC imageShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mShader = d3d->LoadVertexShader(ShaderInfo("shader/image.vs", "ImageVertexShader", "vs_4_0"),
		imageShaderElem,
		2);

	if (!mShader)
	{
		return false;
	}
	if (!d3d->LoadShaderStageIntoShader(ShaderInfo("shader/image.ps", "ImagePixelShader", "ps_4_0"),
		mShader,
		SVF_PIXELSHADER))
	{
		return false;
	}

	// Uppdatera den dynamiska vertex buffern
	result = UpdateBuffers(d3d, 0, 0);
	if (!result)
	{
		return false;
	}

	return true;
}

void ImageClass::Shutdown()
{
	// Släpp indexbuffer
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}

	// Släpp vertexbuffer
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}

	// Släpp textur
	if (mTexture)
	{
		mTexture->Shutdown();
		mTexture = 0;
	}
}

bool ImageClass::Render(D3D* d3d)
{
	bool result;
	m4 worldMatrix, orthoMatrix;

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	// Aktivera vertex och index buffrarna i input assemblern så de kan renderas
	d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	d3d->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Sätt shader variabler
	d3d->GetOrthoMatrix(orthoMatrix);
	v3 orthoViewTranslation = v3(mPosX, -mPosY, 2);
	worldMatrix = m4::CreateTranslation(orthoViewTranslation);

	mShader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));
	//mShader->SetVariable("viewMatrix", &orthoViewMatrix, sizeof(m4));
	mShader->SetVariable("orthoMatrix", &orthoMatrix, sizeof(m4));

	d3d->SetShader(mShader);
	d3d->ApplyConstantBuffers();
	d3d->ApplyTexture(mTexture, 0);

	d3d->GetDeviceContext()->DrawIndexed(6, 0, 0);

	return true;
}

Texture* ImageClass::GetTexture()
{
	return this->mTexture;
}

void ImageClass::SetPosition(int posX, int posY)
{
	this->mPosX = posX;
	this->mPosY = posY;
}

bool ImageClass::InitializeBuffers(D3D* d3d)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	mVertexCount = 6;
	mIndexCount = mVertexCount;

	// Skapa och initialisera vertex array
	vertices = new Vertex[mVertexCount];
	memset(vertices, 0, (sizeof(Vertex)* mVertexCount));

	// Skapa och initialisera index array
	indices = new unsigned long[mIndexCount];
	for (int i = 0; i < mIndexCount; i++)
	{
		indices[i] = i;
	}

	// Vertex buffer description (dynamisk)
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Skapa vertex buffern
	result = d3d->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Skapa index buffern
	result = d3d->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool ImageClass::UpdateBuffers(D3D* d3d, int posX, int posY)
{
	Vertex* vertices;
	float left, right, top, bottom;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* verticesPtr;
	HRESULT result;

	// Räkna ut koordinaterna för alla sidor av bilden
	left = (float)((mScreenWidth / 2) * -1) + (float)posX;
	right = left + (float)mImageWidth;
	top = (float)(mScreenHeight / 2) - (float)posY;
	bottom = top - (float)mImageHeight;

	// Skapa vertex array för rektangeln och fyll den med data
	vertices = new Vertex[mVertexCount];
	if (!vertices)
	{
		return false;
	}

	// Triangel 1
	vertices[0].pos = v3(left, top, 0.0f);
	vertices[0].tex = v2(0.0f, 0.0f);

	vertices[1].pos = v3(right, bottom, 0.0f);
	vertices[1].tex = v2(1.0f, 1.0f);

	vertices[2].pos = v3(left, bottom, 0.0f);
	vertices[2].tex = v2(0.0f, 1.0f);

	// Triangel 2
	vertices[3].pos = v3(left, top, 0.0f);
	vertices[3].tex = v2(0.0f, 0.0f);

	vertices[4].pos = v3(right, top, 0.0f);
	vertices[4].tex = v2(1.0f, 0.0f);

	vertices[5].pos = v3(right, bottom, 0.0f);
	vertices[5].tex = v2(1.0f, 1.0f);

	// Lås vertex buffern så den kan skrivas till
	result = d3d->GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Spara en pekare till datan i vertex buffern
	verticesPtr = (Vertex*)mappedResource.pData;

	// Kopiera datan till vertex buffern
	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex)* mVertexCount));

	// Lås upp vertex buffern
	d3d->GetDeviceContext()->Unmap(mVertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}
#include "SentenceClass.h"

#define			WBOX(x) MessageBox(NULL, x, L"Application Error!!", MB_OK | MB_ICONASTERISK);

SentenceClass::SentenceClass()
{
	mFont = 0;
	mFontShader = 0;
	mSentence = 0;
}

SentenceClass::~SentenceClass()
{

}


bool SentenceClass::Initialize(D3D* d3d, const char* alignment, float letterScale, int sentenceMaxLength, int screenWidth, int screenHeight)
{
	bool result;

	mPosX = 0;
	mPosY = 0;
	mLetterScale = letterScale;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mBaseViewMatrix = m4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 1, 1);

	if (alignment == "center")
	{
		mAlignment = ALIGNMENT_CENTER;
	}
	else if (alignment == "right")
	{
		mAlignment = ALIGNMENT_RIGHT;
	}
	else if (alignment == "vertical")
	{
		mAlignment = ALIGNMENT_VERTICAL;
	}
	else
	{
		mAlignment = ALIGNMENT_LEFT;	// Left som default
	}

	// Skapa font objektet
	mFont = new FontClass;
	if (!mFont)
	{
		return false;
	}

	// Initialisera font objektet
	result = mFont->Initialize(d3d, "data/fontdata_picross.txt", L"data/font_picross.png", letterScale, mAlignment);
	if (!result)
	{
		return false;
	}

	// Skapa och initiera fontShaderClass objekt
	D3D11_INPUT_ELEMENT_DESC fontShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mFontShader = d3d->LoadVertexShader(ShaderInfo("shader/font.vs", "FontVertexShader", "vs_4_0"),
		fontShaderElem,
		2);

	if (!mFontShader)
	{
		return false;
	}
	if (!d3d->LoadShaderStageIntoShader(ShaderInfo("shader/font.ps", "FontPixelShader", "ps_4_0"),
		mFontShader,
		SVF_PIXELSHADER))
	{
		return false;
	}

	// Initialisera meningen
	result = InitializeSentence(sentenceMaxLength, d3d);
	if (!result)
	{
		return false;
	}

	// Sätt default text
	result = SetText("0", d3d);
	if (!result)
	{
		return false;
	}

	return true;
}

void SentenceClass::Shutdown()
{
	// Släpp meningen
	if (mSentence)
	{
		if (mSentence->vertexBuffer)
		{
			mSentence->vertexBuffer->Release();
			mSentence->vertexBuffer = 0;
		}
		if (mSentence->indexBuffer)
		{
			mSentence->indexBuffer->Release();
			mSentence->indexBuffer = 0;
		}

		delete mSentence;
		mSentence = 0;
	}

	// Släpp font objektet
	if (mFont)
	{
		mFont->Shutdown();
		mFont = 0;
	}

	// Släpp shader objektet
	if (mFontShader)
	{
		mFontShader->Flush();
		mFontShader = 0;
	}
}

void SentenceClass::Render(D3D* d3d)
{
	m4 worldMatrix, orthoMatrix;
	q4 pixelColor;

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, &mSentence->vertexBuffer, &stride, &offset);
	d3d->GetDeviceContext()->IASetIndexBuffer(mSentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Hämta matriser
	d3d->GetWorldMatrix(worldMatrix);
	d3d->GetOrthoMatrix(orthoMatrix);

	// Sätt färgvärden för textfärgen
	pixelColor = q4(1.0f, mSentence->red, mSentence->green, mSentence->blue);	// ARGB i q4 ????

	// Translatera texten
	if (mAlignment == ALIGNMENT_CENTER)
	{
		worldMatrix = worldMatrix * m4::CreateTranslation(v3(mPosX - (20 * mLetterScale * mSentenceLength / 2), -mPosY, 0));
	}
	else if (mAlignment == ALIGNMENT_RIGHT)
	{
		worldMatrix = worldMatrix * m4::CreateTranslation(v3(mPosX - (20 * mLetterScale * mSentenceLength), -mPosY, 0));
	}
	else
	{
		worldMatrix = worldMatrix * m4::CreateTranslation(v3(mPosX, -mPosY, 0));
	}

	// Sätt shader variabler
	mFontShader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));
	mFontShader->SetVariable("viewMatrix", &mBaseViewMatrix, sizeof(m4));
	mFontShader->SetVariable("projectionMatrix", &orthoMatrix, sizeof(m4));
	mFontShader->SetVariable("pixelColor", &pixelColor, sizeof(q4));

	d3d->SetShader(mFontShader);
	d3d->ApplyConstantBuffers();
	d3d->ApplyTexture(mFont->GetTexture(), 0);

	d3d->GetDeviceContext()->DrawIndexed(mSentence->indexCount, 0, 0);
}

bool SentenceClass::SetText(char* text, D3D* d3d)
{
	int numLetters;
	Vertex* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* verticesPtr;

	// Kolla så textlängden inte överskrider bufferstorleken
	numLetters = (int)strlen(text);
	if (numLetters > mSentence->maxLength)
	{
		WBOX(L"Buffer overflow! Sentence too long.");
		return false;
	}
	else
	{
		// Spara längden på meningen
		mSentenceLength = numLetters;
	}

	// Skapa och initialisera vertex arrayen
	vertices = new Vertex[mSentence->vertexCount];
	if (!vertices)
	{
		return false;
	}
	memset(vertices, 0, (sizeof(Vertex)* mSentence->vertexCount));

	// Räkna ut pixel positionen att börja rita till (övre vänstra hörnet)
	drawX = (float)(((mScreenWidth / 2) * -1) + 0);
	drawY = (float)((mScreenHeight / 2) - 0);

	// Bygg upp meningen
	mFont->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lås vertex buffern så den kan skrivas till
	result = d3d->GetDeviceContext()->Map(mSentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Få en pekare till datan i vertex buffern
	verticesPtr = (Vertex*)mappedResource.pData;

	// Kopiera datan till vertex buffern
	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex)* mSentence->vertexCount));

	// Lås upp vertex buffern
	d3d->GetDeviceContext()->Unmap(mSentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	// Sätt den nya positionen
	//SetPosition(mPosX, mPosY);

	return true;
}

void SentenceClass::SetPosition(int posX, int posY)
{
	this->mPosX = posX;
	this->mPosY = posY;
}

void SentenceClass::SetColor(float r, float g, float b)
{
	this->mSentence->red = r;
	this->mSentence->green = g;
	this->mSentence->blue = b;
}

float SentenceClass::GetLetterScale() const
{
	return this->mLetterScale;
}

void SentenceClass::OffsetPosition(int xOffset, int yOffset)
{
	this->mPosX += xOffset;
	this->mPosY += yOffset;
}

bool SentenceClass::InitializeSentence(int maxLength, D3D* d3d)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Skapa Sentence objektet
	mSentence = new Sentence;
	if (!mSentence)
	{
		return false;
	}

	// Sätt default färg
	this->mSentence->red = 1.0f;
	this->mSentence->green = 1.0f;
	this->mSentence->blue = 1.0f;

	// Initialisera Sentence variablerna
	mSentence->vertexBuffer = 0;
	mSentence->indexBuffer = 0;
	mSentence->maxLength = maxLength;
	mSentence->vertexCount = 6 * maxLength;
	mSentence->indexCount = mSentence->vertexCount;

	// Skapa vertex arrayen
	vertices = new Vertex[mSentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Skapa index arrayen
	indices = new unsigned long[mSentence->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialisera vertex arrayen
	memset(vertices, 0, (sizeof(Vertex)* mSentence->vertexCount));

	// Initialisera index arrayen
	for (int i = 0; i < mSentence->indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* mSentence->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Skapa vertex buffern
	result = d3d->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mSentence->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* mSentence->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Skapa index buffern
	result = d3d->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &mSentence->indexBuffer);
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
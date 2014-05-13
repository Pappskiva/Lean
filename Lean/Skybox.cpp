#include "Skybox.h"

Skybox::Skybox()
{
	mMesh = 0;
	mShader = 0;
	mTextureCube = 0;

	mYaw = 0.0f;
	mPitch = 0.0f;
	mRoll = 0.0f;
}

Skybox::~Skybox()
{

}

bool Skybox::Initialize(D3D* d3d, const HString &textureFilePath)
{
	// Initialisera Skybox meshen
	mMesh = new Mesh();

	// Skapa vertex och index data för skybox
	struct Vertex
	{
		float position[3];
		float uv[2];
	};

	Vertex vertices[] =
	{
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },		// sida 1
		{ 1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },	// sida 2
		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },

		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f },		// sida 3
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },	// sida 4
		{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },

		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f },		// sida 5
		{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },	// sida 6
		{ -1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	};

	uint indices[] =
	{
		0, 2, 1,		// sida 1
		2, 3, 1,
		4, 6, 5,		// sida 2
		6, 7, 5,
		8, 10, 9,		// sida 3
		10, 11, 9,
		12, 14, 13,		// sida 4
		14, 15, 13,
		16, 18, 17,		// sida 5
		18, 19, 17,
		20, 22, 21,		// sida 6
		22, 23, 21,
	};

	// Skapa meshen för skybox
	mMesh = d3d->CreateMeshFromRam(vertices, sizeof(Vertex), 24, indices, 36);
	if (!mMesh)
	{
		return false;
	}

	// Ladda in texturen för skyboxen
	mTextureCube = d3d->LoadTextureFromFile(textureFilePath);
	if (!mTextureCube)
	{
		return false;
	}

	// Skapa och initialisera skyboxShaderClass objekt
	D3D11_INPUT_ELEMENT_DESC skyboxShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mShader = d3d->LoadVertexShader(ShaderInfo("shader/skybox.vs", "SkyboxVertexShader", "vs_4_0"),
		skyboxShaderElem,
		2);

	if (!mShader)
	{
		return false;
	}
	if (!d3d->LoadShaderStageIntoShader(ShaderInfo("shader/skybox.ps", "SkyboxPixelShader", "ps_4_0"),
		mShader,
		SVF_PIXELSHADER))
	{
		return false;
	}

	return true;
}

void Skybox::Shutdown()
{
	if (mMesh)
	{
		mMesh->Flush();
		mMesh = 0;
	}

	if (mShader)
	{
		mShader->Flush();
		mShader = 0;
	}

	if (mTextureCube)
	{
		mTextureCube->Shutdown();
		mTextureCube = 0;
	}
}

void Skybox::Render(D3D* d3d)
{
	d3d->SetShader(mShader);

	d3d->ApplyConstantBuffers();
	d3d->ApplyTexture(mTextureCube, 0);

	d3d->RenderMesh(mMesh);
}

void Skybox::UpdateShaderVariables(v3 cameraPos, m4 viewMatrix, m4 projectionMatrix)
{
	mShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	mShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));

	// Translatera skyboxen till kamerans position, sätt skala, och rotera om nödvändigt
	m4 worldMatrix = m4::CreateScale(v3(50, 50, 50)) *
		m4::CreateYawPitchRoll(mYaw, mPitch, mRoll) *
		m4::CreateTranslation(cameraPos);
	mShader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));
}

void Skybox::SetYawPitchRoll(float yaw, float pitch, float roll)
{
	this->mYaw = yaw;
	this->mPitch = pitch;
	this->mRoll = roll;
}

Texture* Skybox::GetTextureCube()
{
	return this->mTextureCube;
}

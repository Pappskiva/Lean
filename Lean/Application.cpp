////////////////////////////////////////////////////////////////////////////////
// Filename: Application.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"

#define			WBOX(x) MessageBox(NULL, x, L"Application Error!!", MB_OK | MB_ICONASTERISK);

Application::Application()
{
	m_Direct3D = nullptr;
	m_Input = nullptr;
	m_Camera = nullptr;
	m_Ball = nullptr;
	m_Level = nullptr;
	m_WaterObstacle = nullptr;
	m_Goal = nullptr;

	defaultShader = nullptr;
	levelShader = nullptr;
	skyboxShader = nullptr;
	obstacleShader = nullptr;
}


Application::Application(const Application& other)
{
}


Application::~Application()
{
}


bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	bool result;

	// Create the Direct3D object.
	m_Direct3D = new D3D();

	//Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		WBOX(L"Could not initialize DirectX 11.");
		return false;
	}


	//Create the input object
	m_Input = new Input();

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		WBOX(L"Could not initialize the input object.");
		return false;
	}

	//Create the camera object.
	m_Camera = new Camera;

	//Create the ball object
	m_Ball = new Ball;

	//Create the level object
	m_Level = new Level;

	//Create the obstacle object
	m_WaterObstacle = new WaterObstacle;

	//Create the goal object
	m_Goal = new Goal;

	//Initialize the level object
	result = m_Level->Initialize(m_Direct3D, L"testSpringGround.png", L"testDeepGround.png");
	if (!result)
	{
		return false;
	}

	//Initialize the ball object
	result = m_Ball->Initialize(m_Direct3D, L"ball.png");
	if (!result)
	{
		return false;
	}

	//Initialize the obstacle object
	result = m_WaterObstacle->Initialize(m_Direct3D);
	if (!result)
	{
		return false;
	}

	//Initialize the goal object
	result = m_Goal->Initialize(m_Direct3D, L"testTexture.png");
	if (!result)
	{
		return false;
	}

	// Initiera Skybox meshen
	m_skybox = new Mesh();

	// Skapa vertex och index data för skybox
	struct Vertex
	{
		float position[3];
		float uv[2];
	};

	Vertex vertices[] =
	{
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },    // sida 1
		{ 1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },    // sida 2
		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },

		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f },    // sida 3
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },    // sida 4
		{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },

		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f },    // sida 5
		{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },    // sida 6
		{ -1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	};

	uint indices[] =
	{
		0, 2, 1,    // sida 1
		2, 3, 1,
		4, 6, 5,    // sida 2
		6, 7, 5,
		8, 10, 9,    // sida 3
		10, 11, 9,
		12, 14, 13,    // sida 4
		14, 15, 13,
		16, 18, 17,    // sida 5
		18, 19, 17,
		20, 22, 21,    // sida 6
		22, 23, 21,
	};

	// Skapa meshen för skybox
	m_skybox = m_Direct3D->CreateMeshFromRam(vertices, sizeof(Vertex), 24, indices, 36);
	if (!m_skybox)
	{
		return false;
	}

	// Ladda in texturen för skyboxen
	m_SkyboxTexture = m_Direct3D->LoadTextureFromFile("BackgroundCube.dds");
	if (!m_SkyboxTexture)
	{
		return false;
	}

	// Skapa och initiera skyboxShaderClass objekt
	D3D11_INPUT_ELEMENT_DESC skyboxShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	skyboxShader = m_Direct3D->LoadVertexShader(ShaderInfo("skybox.vs", "SkyboxVertexShader", "vs_4_0"),
		skyboxShaderElem,
		2);

	if (!skyboxShader)
	{
		return false;
	}
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("skybox.ps", "SkyboxPixelShader", "ps_4_0"),
		skyboxShader,
		SVF_PIXELSHADER))
	{
		return false;
	}

	//Loads the PointLight mesh into vram
	lightSphereMesh = m_Direct3D->LoadMeshFromOBJ("LightSphere.obj");

	if (!lightSphereMesh)
	{
		WBOX(L"Couldn't Load LightSphere.obj");
		return false;
	}

	//Loads the light shader from file
	D3D11_INPUT_ELEMENT_DESC lightShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	pointLightShader = m_Direct3D->LoadVertexShader(ShaderInfo("pointLight.vs", "PointLightVertexShader", "vs_4_0"),
		lightShaderElem,
		1);
	if (!pointLightShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("pointLight.ps", "PointLightPixelShader", "ps_4_0"),
		pointLightShader,
		SVF_PIXELSHADER))
		return false;

	//Create the ballshaderclass object
	//Initialize  the ballshaderclass object
	D3D11_INPUT_ELEMENT_DESC ballShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	defaultShader = m_Direct3D->LoadVertexShader(ShaderInfo("ball.vs", "BallVertexShader", "vs_4_0"),
		ballShaderElem,
		3);
	if (!defaultShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("ball.ps", "BallPixelShader", "ps_4_0"),
		defaultShader,
		SVF_PIXELSHADER))
		return false;

	//Create the levelshaderclass object
	//Initialize  the levelshaderclass object
	D3D11_INPUT_ELEMENT_DESC levelShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	levelShader = m_Direct3D->LoadVertexShader(ShaderInfo("level.vs", "LevelVertexShader", "vs_4_0"),
		levelShaderElem,
		3);
	if (!levelShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("level.ps", "LevelPixelShader", "ps_4_0"),
		levelShader,
		SVF_PIXELSHADER))
		return false;


	//Create the obstacleshaderclass object
	//Initialize  the obstacleshaderclass object
	D3D11_INPUT_ELEMENT_DESC obstacleShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	obstacleShader = m_Direct3D->LoadVertexShader(ShaderInfo("obstacle.vs", "ObstacleVertexShader", "vs_4_0"),
		obstacleShaderElem,
		3);
	if (!obstacleShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("obstacle.ps", "ObstaclePixelShader", "ps_4_0"),
		obstacleShader,
		SVF_PIXELSHADER))
		return false;


	ID3D11SamplerState* m_sampleState;
	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = m_Direct3D->GetDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	levelShader->AddTextureSampler(m_sampleState, 0);
	
	//Initialize the fpsclass object
	m_fps.Initialize();

	m_Ball->SetPosition(1, 5, 0);
	m_Ball->SetShader(defaultShader);
	m_Level->SetShader(levelShader);
	m_Level->LoadLevel(1, m_Direct3D);
	m_WaterObstacle->SetShader(obstacleShader);
	m_Goal->SetShader(defaultShader);

	AddPointLight(v3(-1.0f, 0.5f, -1.0f), 2.0f, v3(0, 0, 1), 1.0f);
	AddPointLight(v3(-1.0f, 0.5f, 1.0f), 2.0f, v3(0, 1, 0), 1.0f);
	AddPointLight(v3(1.0f, 0.5f, -1.0f), 2.0f, v3(1, 0, 0), 1.0f);
	AddPointLight(v3(1.0f, 0.5f, 1.0f), 2.0f, v3(1, 1, 0), 1.0f);
		

	return true;
}

bool Application::Frame(float deltaTime)
{
	//m_fps.Frame();	

	//Sleep(3000);

	bool result = true;

	//Update the input object
	result = m_Input->Frame();

	//Check if the user has pressed Escape to close the program
	if (m_Input->IsEscapePressed())
	{
		return false;
	}


	//Check if the user is pressing buttons to rotate the level in the x-axis
	if (m_Input->IsUpPressed() || m_Input->IsWPressed())
	{
		float rotationAroundX = m_Level->GetRotationX();
		m_Level->SetRotationX(rotationAroundX + 28.0f * deltaTime);
	}
	else if (m_Input->IsDownPressed() || m_Input->IsSPressed())
	{
		float rotationAroundX = m_Level->GetRotationX();
		m_Level->SetRotationX(rotationAroundX - 28.0f * deltaTime);
	}

	//Check if the user is pressing buttons to rotate the level in the z-axis
	if (m_Input->IsLeftPressed() || m_Input->IsAPressed())
	{
		float rotationAroundZ = m_Level->GetRotationZ();
		m_Level->SetRotationZ(rotationAroundZ + 28.0f * deltaTime);
	}
	else if (m_Input->IsRightPressed() || m_Input->IsDPressed())
	{
		float rotationAroundZ = m_Level->GetRotationZ();
		m_Level->SetRotationZ(rotationAroundZ - 28.0f * deltaTime);
	}

	if (m_Input->IsSpacePressed())
	{

	}

	v3 testNormal;
	m_Level->GetNormal(testNormal);
	v3 ballPosition;	

	m_Ball->GetPosition(ballPosition);
	float value1 = (testNormal.x * ballPosition.x + testNormal.y * ballPosition.y + testNormal.z * ballPosition.z);
	float value2 = sqrtf(testNormal.x * testNormal.x + testNormal.y * testNormal.y + testNormal.z * testNormal.z);
	float distance = value1 / value2;

	if (distance >= m_Ball->GetRadius())
	{
		m_Ball->SetPosition(ballPosition.x, ballPosition.y - 1.0f * deltaTime, ballPosition.z);
	}
	if (distance <= m_Ball->GetRadius())
	{
		float testRollSpeed = 8.0f;
		m_Ball->SetPosition(ballPosition.x + testNormal.x * testRollSpeed * deltaTime, ballPosition.y + testNormal.y * deltaTime, ballPosition.z + testNormal.z * testRollSpeed * deltaTime);
	}

	m_Level->Update(deltaTime);

	m_Level->Update(deltaTime);
	m_Ball->Update(deltaTime);

	m_Camera->SetPosition(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f);
	m_Camera->SetTargetToLookAt(ballPosition.x, ballPosition.y, ballPosition.z);
	
	
	m_WaterObstacle->Update(deltaTime, ballPosition.x - 2.5f, ballPosition.z - 7.0f);

	m_Goal->Update(deltaTime);

	// Render the graphics.
	RenderGraphics();

	return result;
}

void Application::RenderGraphics()
{
	static float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//Clear the scene

	_UpdateShaderVariables();

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Render the ball buffers.
	
		m_Direct3D->BeginDeferredRenderingScene(clearColor);
		
			m_Ball->Render(m_Direct3D);

			m_Level->Render(m_Direct3D);

			m_Goal->Render(m_Direct3D);

			m_Direct3D->TurnOnAlphaBlending();
			m_WaterObstacle->Render(m_Direct3D);
			m_Direct3D->TurnOnAlphaBlending();

			m_Direct3D->BeginLightStage();

				_RenderLights();

			m_Direct3D->EndLightStage();

		m_Direct3D->EndDeferredRenderingScene();

		_RenderSkybox();

	m_Direct3D->EndScene();

}

void Application::Shutdown()
{
	//Release the level object
	if (m_Level)
	{
		m_Level->Shutdown();
		delete m_Level;
		m_Level = 0;
	}

	// Release the input object
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	//Release the camera object
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//Release the ball object
	if (m_Ball)
	{
		m_Ball->Shutdown();
		m_Ball = 0;
	}

	//Release the skybox object
	if (m_skybox)
	{
		m_skybox->Flush();
		m_skybox = 0;
	}

	//Release the obstacle object
	if (m_WaterObstacle)
	{
		m_WaterObstacle->Shutdown();
		m_WaterObstacle = 0;
	}

	//Release the goal object
	if (m_Goal)
	{
		m_Goal->Shutdown();
		m_Goal = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

void Application::_UpdateShaderVariables()
{
	m4 worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	/*v3 ballPosition;
	m_Ball->GetPosition(ballPosition);
	viewMatrix.ViewAtLH(v3(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f),
		ballPosition);*/

	//Get the world, view, projectio nand orthographic matrices from the camera and direct3d objects
	m_Camera->Render();

	//Get the world, view, projectio nand orthographic matrices from the camera and direct3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	defaultShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	defaultShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));
	levelShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	levelShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));
	skyboxShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	skyboxShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));
	obstacleShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	obstacleShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));

	// Translatera skyboxen till kamerans position
	v3 cameraPos;
	m_Camera->GetPosition(cameraPos);
	//static float r = 0;
	//r += 0.001f;
	m4 skyboxWorldMatrix = m4::CreateScale(v3(100, 100, 100)) * /*m4::CreateYawPitchRoll(r * 0.5f, r * 0.75, r) **/ m4::CreateTranslation(cameraPos);
	skyboxShader->SetVariable("worldMatrix", &skyboxWorldMatrix, sizeof(m4));

	m4 viewProj = viewMatrix * projectionMatrix;
	m4 viewProjInverted = viewProj.Inverse();

	//pointLightShader->UpdateConstantBuffer(0, &test, sizeof(stuff));
	pointLightShader->SetVariable("viewProj", &viewProj, sizeof(m4));
	pointLightShader->SetVariable("viewProjInverted", &viewProjInverted, sizeof(m4));
}

void Application::_RenderSkybox()
{
	// Rendera skybox
	
	//Shader* skyBoxShader = m_Direct3D->GetCurrentShader();
	m_Direct3D->SetShader(skyboxShader);
	
	m_Direct3D->ApplyConstantBuffers();
	m_Direct3D->ApplyTexture(m_SkyboxTexture, 0);

	m_Direct3D->RenderMesh(m_skybox);
}

void Application::_RenderLights()
{
	for (uint i = 0; i < lights.Length(); ++i)
	{
		//assert(lights[i].shader);
		m_Direct3D->SetShader(lights[i].shader);

		switch (lights[i].lightType)
		{
		case LT_Point:
			lights[i].shader->UpdateConstantBuffer(1, &lights[i], sizeof(LightPack));
			m_Direct3D->ApplyConstantBuffers();
			break;
		}

		m_Direct3D->RenderMesh(lights[i].mesh);
	}
}

void Application::AddPointLight(const v3 &center, const float radius, const v3 &color, const float intensity)
{
	LightPack &light = lights.Append();
	light.pos = center;
	light.range = radius;
	light.color = color;
	light.intensity = intensity;
	light.mesh = lightSphereMesh;
	light.shader = pointLightShader;
	light.lightType = LT_Point;
}
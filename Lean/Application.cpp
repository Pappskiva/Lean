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
	m_ObstacleHandler = nullptr;
	m_Goal = nullptr;

	defaultShader = nullptr;
	levelShader = nullptr;
	obstacleShader = nullptr;

	switchLevel = false;
	finishedSwitch = true;
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

	//Create the goal object
	m_Goal = new Goal;

	//Initialize the level object
	result = m_Level->Initialize(m_Direct3D, L"data//testSpringGround.png", L"data//testDeepGround.png");
	if (!result)
	{
		return false;
	}

	//Initialize the ball object
	result = m_Ball->Initialize(m_Direct3D, L"data//ball.png");
	if (!result)
	{
		return false;
	}

	//Initialize the goal object
	result = m_Goal->Initialize(m_Direct3D, L"data//blackGoal.png");
	if (!result)
	{
		return false;
	}

	// Skapa Skybox objektet
	m_Skybox = new Skybox;
	if (!m_Skybox)
	{
		return false;
	}

	// Initialisera Skybox objektet
	result = m_Skybox->Initialize(m_Direct3D, "data/BackgroundCube.dds");
	if (!result)
	{
		return false;
	}

	//Loads the PointLight mesh into vram
	lightSphereMesh = m_Direct3D->LoadMeshFromOBJ("data//LightSphere.obj");

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

	pointLightShader = m_Direct3D->LoadVertexShader(ShaderInfo("shader//pointLight.vs", "PointLightVertexShader", "vs_4_0"),
		lightShaderElem,
		1);
	if (!pointLightShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("shader//pointLight.ps", "PointLightPixelShader", "ps_4_0"),
		pointLightShader,
		SVF_PIXELSHADER))
		return false;

	D3D11_INPUT_ELEMENT_DESC dirLightElemDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	directionalLightShader = m_Direct3D->LoadVertexShader(ShaderInfo("shader//fullScreenQuad.vs", "VS", "vs_4_0"),
		dirLightElemDesc,
		2);
	if (!directionalLightShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("shader//DirectionalLightingWithShadows.ps", "LightPassPS", "ps_4_0"),
		directionalLightShader,
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

	defaultShader = m_Direct3D->LoadVertexShader(ShaderInfo("shader//ball.vs", "BallVertexShader", "vs_4_0"),
		ballShaderElem,
		3);
	if (!defaultShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("shader//ball.ps", "BallPixelShader", "ps_4_0"),
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

	levelShader = m_Direct3D->LoadVertexShader(ShaderInfo("shader//level.vs", "LevelVertexShader", "vs_4_0"),
		levelShaderElem,
		3);
	if (!levelShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("shader//level.ps", "LevelPixelShader", "ps_4_0"),
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

	obstacleShader = m_Direct3D->LoadVertexShader(ShaderInfo("shader//obstacle.vs", "ObstacleVertexShader", "vs_4_0"),
		obstacleShaderElem,
		3);
	if (!obstacleShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("shader//obstacle.ps", "ObstaclePixelShader", "ps_4_0"),
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

	m_Ball->SetShader(defaultShader);
	m_Level->SetShader(levelShader);
	m_ObstacleHandler = new ObstacleHandler();

	/*LevelLoaderClass::ObstacleType *obstacles;
	v3 startPos;
	v3 goalPos;
	int nrOfObst;
	m_Level->LoadLevel(1, m_Direct3D, obstacles, startPos, goalPos, nrOfObst);

	for (int i = 0; i < nrOfObst; i++)
	{
	m_ObstacleHandler->AddObstacle(obstacles[i].type, obstacles[i].pos);
	}

	delete [] obstacles;

	m_ObstacleHandler->Initialize(m_Direct3D);
	m_ObstacleHandler->SetShader(obstacleShader);

	m_Ball->SetPosition(startPos.x, 5, startPos.z);
	m_Goal->SetPosition(goalPos.x, goalPos.y, goalPos.z);
	m_Goal->SetNextLevelNumber(1);*/

	ChangeLevel(1);

	m_Goal->SetShader(defaultShader);

	AddPointLight(v3(-1.0f, 0.5f, -1.0f), 2.0f, v3(0, 0, 1), 1.0f);
	AddPointLight(v3(-1.0f, 0.5f, 1.0f), 2.0f, v3(0, 1, 0), 1.0f);
	AddPointLight(v3(1.0f, 0.5f, -1.0f), 2.0f, v3(1, 0, 0), 1.0f);
	AddPointLight(v3(1.0f, 0.5f, 1.0f), 2.0f, v3(1, 1, 0), 1.0f);
	

	m_Direct3D->CreateShadowmap(1024, 1024);

	v3 lightDir = v3(5, 10, -3).GetNormalized();
	v3 camPos = lightDir * 40.0f;
	shadowCamera.SetPosition(camPos);
	shadowCamera.LookAt(v3(0.0f));
	shadowCamera.SetScreenSize(1024, 1024);
	shadowCamera.SetFarPlane(100.0f);
	shadowCamera.SetNearPlane(10.0f);
	shadowCamera.SetFieldOfView((float)PI * 0.125f);
	shadowCamera.GeneratePerspectiveProjectionMatrix();
	shadowCamera.Generate3DViewMatrix();

	firstLightPassData.directionalLightDirection = lightDir;
	firstLightPassData.directionalLightColor = v3(1.0f, 1.0f, 1.0f);
	firstLightPassData.ambientColor = v3(0.3f, 0.3f, 0.3f);
	directionalLightShader->UpdateConstantBuffer(0, &firstLightPassData, sizeof(firstLightPassData));

	m_PhysicsBridge.Initialize(m_Level, m_Ball);
	m_PhysicsBridge.GenerateDebug(m_Direct3D);

	gameTimer = 0;

	//Particle test;
	struct ParticleVertex
	{
		float uv[2];
	} vertices[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 1.0f },

			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f },
	};

	Mesh *particleMesh = m_Direct3D->CreateMeshFromRam(vertices, sizeof(ParticleVertex), 6);
	
	particleRenderer.Initialize(512, 36, particleMesh);
	m_Direct3D->PrepareInstanceRenderer(particleRenderer);

	Texture *leaf = m_Direct3D->LoadTextureFromFile("data//64x64LeafParticleColored.png");

	D3D11_INPUT_ELEMENT_DESC particleElem[] =
	{
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 1, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 1, 24, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	particleBillboard = m_Direct3D->LoadVertexShader(ShaderInfo("shader//particleBillboard.vs", "BillboardVertexShader", "vs_4_0"),
		particleElem,
		5);
	if (!particleBillboard)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("shader//particleDefault.ps", "ParticlePixelShader", "ps_4_0"),
		particleBillboard,
		SVF_PIXELSHADER))
		return false;

	struct ParticleRenderData
	{
		v3 pos;
		float scale[2];
		float rot;
		float alpha;
	};

	ParticleRenderLayoutMember layout[] = {
			{ 0, Particle_Position },
			{ 0, Particle_XScale },
			{ 0, Particle_YScale },
			{ 0, Particle_Rotation },
			{ 0, Particle_Alpha },
	};
	ParticleBase particleBase;
	particleBase.fadeInTime = 0;
	particleBase.fadeOutTime = 100;
	particleBase.rotSpeed = 3.9f;
	particleBase.scaleXInTime = 1000;
	particleBase.scaleXOutTime = 100;
	particleBase.scaleYInTime = 1000;
	particleBase.scaleYOutTime = 100;
	particleBase.speed = 1.0f;
	particleBase.timeToLive = 2000;
	particleBase.weight = 0;
	particleBase.xSize = 1.0f;
	particleBase.ySize = 1.0f;

	ParticleEmitterBase emitterBase;
	emitterBase.SetTimeToLive(0);
	emitterBase.SetSpawnFrequency(25);
	emitterBase.SetMaximumParticles(100);
	emitterBase.SetParticleMove(v3(0, 1, 0));
	emitterBase.SetRandomMove(true);
	emitterBase.SetShader(particleBillboard);
	emitterBase.SetTexture(leaf);
	emitterBase.SetRenderLayout(layout, 5);
	emitterBase.SetParticleBase(particleBase);
	emitterBase.SetParticleRenderer(&particleRenderer);

	testEmitter.SetParticleEmitterBase(emitterBase);

	return true;
}

bool Application::Frame(float deltaTime)
{
	if (deltaTime < 0.32f)
		gameTimer += (uint) (deltaTime * 1000);
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
		if (!switchLevel)
		{
			float rotationAroundX = m_Level->GetRotationX();
			m_Level->SetRotationX(rotationAroundX + 28.0f * deltaTime);
		}
	}
	else if (m_Input->IsDownPressed() || m_Input->IsSPressed())
	{
		if (!switchLevel)
		{
			float rotationAroundX = m_Level->GetRotationX();
			m_Level->SetRotationX(rotationAroundX - 28.0f * deltaTime);
		}
	}

	//Check if the user is pressing buttons to rotate the level in the z-axis
	if (m_Input->IsLeftPressed() || m_Input->IsAPressed())
	{
		if (!switchLevel)
		{
			float rotationAroundZ = m_Level->GetRotationZ();
			m_Level->SetRotationZ(rotationAroundZ + 28.0f * deltaTime);
		}
	}
	else if (m_Input->IsRightPressed() || m_Input->IsDPressed())
	{
		if (!switchLevel)
		{
			float rotationAroundZ = m_Level->GetRotationZ();
			m_Level->SetRotationZ(rotationAroundZ - 28.0f * deltaTime);
		}
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
	float distance = value1 / value2;////???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????

	//if (distance >= m_Ball->GetRadius())
	//{
	//	m_Ball->SetPosition(ballPosition.x, ballPosition.y - 1.0f * deltaTime, ballPosition.z);
	//}
	//if (distance <= m_Ball->GetRadius())
	//{
	//	float testRollSpeed = 8.0f;
	//	m_Ball->SetPosition(ballPosition.x + testNormal.x * testRollSpeed * deltaTime, ballPosition.y + testNormal.y * deltaTime, ballPosition.z + testNormal.z * testRollSpeed * deltaTime);
	//}

	//m_Level->Update(deltaTime);

	m_Level->Update(deltaTime);
	//m_Ball->Update(deltaTime); //OBS: Saknar värde! Bollen uppdateras i PhysicsBridge::StepSimulation

	m_PhysicsBridge.StepSimulation(deltaTime, m_Level->GetRotationX()*0.0174532925f, 0, m_Level->GetRotationZ()*0.0174532925f, m_Ball, m_Level);


	//m_Camera->SetPosition(v3(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f));
	//m_Camera->LookAt(ballPosition);

	//Hämta bollens world, sätt kamerans position efter bollens position.
	m4 ballWorldM;
	m_Ball->GetWorldMatrix(ballWorldM);
	v3 camPos = ballWorldM.GetPos();
	m_Camera->LookAt(camPos);
	//m_Camera->SetPosition(v3(camPos.x - 2.5f, camPos.y + 3.65f, camPos.z - 7.0f));
	m_Camera->SetPosition(v3(camPos.x - 2.5f * 2, camPos.y + 3.65f * 2, camPos.z - 7.0f * 2));

	//shadowCamera.SetPosition(firstLightPassData.directionalLightDirection * 60.0f + v3(ballPosition.x, 0, ballPosition.z));
	//shadowCamera.Generate3DViewMatrix();
	
	float planeRotX = m_Level->GetRotationX();
	float planeRotZ = m_Level->GetRotationZ();

	m_ObstacleHandler->Update(deltaTime, ballPosition.x - 2.5f, ballPosition.z - 7.0f, planeRotX, planeRotZ, m_Ball);

	m_Goal->Update(deltaTime, planeRotX, planeRotZ);

	//Håller man på med att byta banan? Om man inte gör det ska man kolla med kollisionen, se om bollet är tillräckligt nära målet
	if (!switchLevel)
	{
		//Titta om målet och bollen är nära nog för att kollidera
		v3 goalPosition;
		m_Ball->GetPosition(ballPosition);
		m_Goal->GetPosition(goalPosition);
		v3 relPos = ballPosition - goalPosition;
		distance = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
		float minDist = m_Ball->GetRadius() + 2.0f;
		if (distance < minDist * minDist)
		{
			//Bollen är nära målet, så nu ska vi göra så att banan byts ut.
			switchLevel = true; 
			finishedSwitch = false;
		}
	}


	if (switchLevel)
	{
		if (!finishedSwitch)
		{
			finishedSwitch = true;
			//ChangeLevel(m_Goal->GetNextLevelNumber());
		}
		else
		{
			switchLevel = false;
		}
	}

	testEmitter.Update(gameTimer, deltaTime);
	particleRenderer.Update();

	// Render the graphics.
	RenderGraphics();

	return result;
}

void Application::RenderGraphics()
{
	static float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//Clear the scene

	_UpdateShaderVariables();

	m_Direct3D->BeginShadowPass();

		//m_Ball->Render(m_Direct3D);
		

		/*static m4 worldMatrix = m4::CreateTranslation(firstLightPassData.directionalLightDirection * 2.0f);
		Shader *currentShader = m_Direct3D->GetCurrentShader();

		currentShader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));

		m_Direct3D->RenderMesh(lightSphereMesh);*/

		//m_Level->Render(m_Direct3D);

	m_Direct3D->EndShadowPass();


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		m_Direct3D->BeginDeferredRenderingScene(clearColor);
		
			m_Ball->Render(m_Direct3D);

			m_Level->Render(m_Direct3D);

			if (!switchLevel)
			{
				m_Goal->Render(m_Direct3D);				
			}

			m_Direct3D->BeginLightStage();

				_RenderLights();

			m_Direct3D->EndLightStage();

		m_Direct3D->EndDeferredRenderingScene();
		
		if (!switchLevel)
		{
			m_Direct3D->TurnOnAlphaBlending();
			m_ObstacleHandler->Render(m_Direct3D);
			m_Direct3D->TurnOffAlphaBlending();
		}

		m_Skybox->Render(m_Direct3D);

		m_Direct3D->TurnZBufferReadOnWriteOff();
		m_Direct3D->TurnOnAlphaBlending();

		particleRenderer.Render();
		particleRenderer.ClearInstances();

		m_Direct3D->TurnOffAlphaBlending();
		m_Direct3D->TurnZBufferOn();

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
	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		m_Skybox = 0;
	}

	//Release the obstacleHandler object
	if (m_ObstacleHandler)
	{
		m_ObstacleHandler->Shutdown();
		m_ObstacleHandler = 0;
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
	m_Camera->Generate3DViewMatrix();

	//Get the world, view, projectio nand orthographic matrices from the camera and direct3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	defaultShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	defaultShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));
	levelShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	levelShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));
	obstacleShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	obstacleShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));

	// Updatera Skyboxens shader variabler
	v3 camPos = m_Camera->GetPosition();
	m_Skybox->UpdateShaderVariables(camPos, viewMatrix, projectionMatrix);

	m4 viewProj = viewMatrix * projectionMatrix;
	m4 viewProjInverted = viewProj.Inverse();

	//pointLightShader->UpdateConstantBuffer(0, &test, sizeof(stuff));
	pointLightShader->SetVariable("viewProj", &viewProj, sizeof(m4));
	pointLightShader->SetVariable("viewProjInverted", &viewProjInverted, sizeof(m4));

	//directional light
	m4 shadowView, shadowProjection;
	shadowCamera.GetViewMatrix(shadowView);
	shadowCamera.GetProjectionMatrix(shadowProjection);

	m4 shadowViewProj = shadowView * shadowProjection;
	directionalLightShader->SetVariable("viewProjectionInverse", &viewProjInverted, sizeof(m4));
	directionalLightShader->SetVariable("shadowViewProjection", &shadowViewProj, sizeof(m4));
	directionalLightShader->SetVariable("cameraPosition", &camPos, sizeof(v3));

	//shadowfill
	Shader *shadowFillShader = m_Direct3D->GetShadowFill();
	shadowFillShader->SetVariable("shadowView", &shadowView, sizeof(m4));
	shadowFillShader->SetVariable("shadowProjection", &shadowProjection, sizeof(m4));

	//ParticleShader
	particleBillboard->SetVariable("viewProjection", &viewProj, sizeof(m4));
	particleBillboard->SetVariable("cameraPosition", &camPos, sizeof(v3));

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


	m_Direct3D->SetShader(directionalLightShader);
	m_Direct3D->ApplyGBufferResource(0, 0);
	m_Direct3D->ApplyGBufferResource(1, 1);
	m_Direct3D->ApplyDepthResource(2);
	m_Direct3D->ApplyShadowmapResource(3);

	m_Direct3D->RenderFullScreenQuad();
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

void Application::ChangeLevel(int levelNumber)
{
	LevelLoaderClass::ObstacleType *obstacles;
	v3 startPos;
	v3 goalPos;
	int nrOfObst;
	m_Level->LoadLevel(levelNumber, m_Direct3D, obstacles, startPos, goalPos, nrOfObst);

	m_Level->SetRotationX(0.0f);
	m_Level->SetRotationZ(0.0f);

	//Ta bort alla tidigare hinder
	m_ObstacleHandler->RemoveAllObstacles();
	//Lägg in de nya hindren
	for (int i = 0; i < nrOfObst; i++)
	{
		m_ObstacleHandler->AddObstacle(obstacles[i].type, obstacles[i].pos);
	}
	//Reinitsiera de nya hindren
	m_ObstacleHandler->Initialize(m_Direct3D);
	m_ObstacleHandler->SetShader(obstacleShader);
	delete[] obstacles;

	m_Ball->SetPosition(startPos.x, 5, startPos.z);
	m_Goal->SetPosition(goalPos.x, goalPos.y, goalPos.z);
	if (m_Goal->GetNextLevelNumber() < MAX_LEVELS)
	{
		m_Goal->SetNextLevelNumber(m_Goal->GetNextLevelNumber() + 1);
	}
	else
	{
		m_Goal->SetNextLevelNumber(0);
	}
}
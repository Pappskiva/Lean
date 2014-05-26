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
	m_Sound = nullptr;
	m_Clock = nullptr;
	m_Text = nullptr;
	m_Image = nullptr;
	m_Logo = nullptr;
	m_GameOverSignImage = nullptr;
	m_WinSignImage = nullptr;
	m_StandardSignImage = nullptr;
	m_GameState = STATE_MAINMENU;
	m_GameMode = MODE_CLASSIC;
	m_Menu = nullptr;
	m_Highscore = nullptr;
	m_HSSentence = nullptr;
	m_NameInput = nullptr;

	defaultShader = nullptr;
	levelShader = nullptr;
	obstacleShader = nullptr;
	nrOfLives = MAX_NR_OF_LIVES;

	switchLevel = false;
	finishedSwitch = true;
	CompletedFirstPass = false;

}

Application::Application(const Application& other)
{
}

Application::~Application()
{
}

bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	m_hwnd = hwnd;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	bool result;

	m_Sound = new Sound;
	if (!m_Sound)
	{
		return false;
	}
	result = m_Sound->Initialize2DSound(hwnd, "../Lean/Data/Musik/LONELY_ROLLING_STAR.wav", 0, 0, 0);
	if (!result)
	{
		MessageBox(hwnd, L"Couldn't Initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}
	//m_Sound->PlayLoop(0.7f);

	m_Clock = new Clock();

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

	//Create the box object
	m_Box = new Box;

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
	result = m_Ball->Initialize(m_Direct3D, L"data//MetalBall.png");
	if (!result)
	{
		return false;
	}

	//Initialize the box object
	result = m_Box->Initialize(m_Direct3D, L"data//box.png");
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
	result = m_Skybox->Initialize(m_Direct3D, "data/skybox_abovesea.dds");
	if (!result)
	{
		return false;
	}

	// Skapa Text objekt
	m_Text = new SentenceClass;
	if (!m_Text)
	{
		return false;
	}

	// Skapa Text objekt
	m_StandardInfoText = new SentenceClass;
	if (!m_StandardInfoText)
	{
		return false;
	}

	// Initialisera Text objekt
	result = m_Text->Initialize(m_Direct3D, "center", 2.0f, 16, screenWidth, screenHeight);
	if (!result)
	{
		WBOX(L"Could not initialize the sentence object.");
		return false;
	}

	// Initialisera Text objekt
	result = m_StandardInfoText->Initialize(m_Direct3D, "center", 1.0f, 16, screenWidth, screenHeight);
	if (!result)
	{
		WBOX(L"Could not initialize the sentence object.");
		return false;
	}

	// Skapa meny objekt
	m_Menu = new Menu();
	if (!m_Menu)
	{
		return false;
	}

	// Initialisera meny objekt
	result = m_Menu->Initialize(m_Direct3D, m_Input, m_Camera, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	int x = 20 + ((float)(MAX_NR_OF_LIVES / 2) * 40);
	m_Image = new ImageClass*[MAX_NR_OF_LIVES];
	for (int i = 0; i < MAX_NR_OF_LIVES; i++)
	{
		// Skapa Image objekt
		m_Image[i] = new ImageClass;
		if (!m_Image[i])
		{
			return false;
		}

		// Initialisera Image objekt
		result = m_Image[i]->Initialize(m_Direct3D, L"data/Heart.png", x, screenHeight - 110, 20, 20);
		if (!result)
		{
			WBOX(L"Could not initialize the heart image object.");
			return false;
		}
		x -= 40;
	}
	

	// Skapa Image objekt
	m_StandardSignImage = new ImageClass;
	if (!m_StandardSignImage)
	{
		return false;
	}

	// Skapa Image objekt
	m_Logo = new ImageClass;
	if (!m_Logo)
	{
		return false;
	}

	// Initialisera Image objekt
	result = m_Logo->Initialize(m_Direct3D, L"data/LeanTitle.png", screenWidth, screenHeight, 400, 250);
	if (!result)
	{
		WBOX(L"Could not initialize the image object.");
		return false;
	}

	m_Logo->SetPosition(screenWidth / 2 - 200, screenHeight / 2 - 250);


	// Initialisera Image objekt
	result = m_StandardSignImage->Initialize(m_Direct3D, L"data/NormalSign.png", screenWidth, screenHeight, 640, 400);
	if (!result)
	{
		WBOX(L"Could not initialize the image object.");
		return false;
	}

	m_StandardSignImage->SetPosition(screenWidth / 2 - 320, screenHeight / 2 - 200);


	// Skapa Image objekt
	m_WinSignImage = new ImageClass;
	if (!m_WinSignImage)
	{
		return false;
	}

	// Initialisera Image objekt
	result = m_WinSignImage->Initialize(m_Direct3D, L"data/WinSign.png", screenWidth, screenHeight, 640, 400);
	if (!result)
	{
		WBOX(L"Could not initialize the image object.");
		return false;
	}

	m_WinSignImage->SetPosition(screenWidth / 2 - 320, screenHeight / 2 - 200);


	// Skapa Image objekt
	m_GameOverSignImage = new ImageClass;
	if (!m_GameOverSignImage)
	{
		return false;
	}

	// Initialisera Image objekt
	result = m_GameOverSignImage->Initialize(m_Direct3D, L"data/GameOverSign.png", screenWidth, screenHeight, 640, 400);
	if (!result)
	{
		WBOX(L"Could not initialize the image object.");
		return false;
	}

	m_GameOverSignImage->SetPosition(screenWidth / 2 - 320, screenHeight / 2 - 200);

	m_StandardInfoText->SetPosition(screenWidth / 2, screenHeight / 2 - 160);

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
	m_Box->SetShader(defaultShader);
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
	shadowCamera.SetFarPlane(40.0f);
	shadowCamera.SetNearPlane(10.0f);
	shadowCamera.SetFieldOfView((float)PI * 0.125f);
	shadowCamera.GeneratePerspectiveProjectionMatrix();
	shadowCamera.Generate3DViewMatrix();

	m_Camera->SetFarPlane(SCREEN_DEPTH);
	m_Camera->SetNearPlane(SCREEN_NEAR);
	m_Camera->SetFieldOfView(PI * 0.25f);
	m_Camera->SetScreenSize(screenWidth, screenHeight);
	m_Camera->GeneratePerspectiveProjectionMatrix();

	firstLightPassData.directionalLightDirection = lightDir;
	firstLightPassData.directionalLightColor = v3(1.0f, 1.0f, 1.0f);
	firstLightPassData.ambientColor = v3(0.3f, 0.3f, 0.3f);
	directionalLightShader->UpdateConstantBuffer(0, &firstLightPassData, sizeof(firstLightPassData));

	gameTimer = 0;
	

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

	m_Particles.Initialize(m_Direct3D, particleBillboard);

	ChangeLevel(0);
	//m_Goal->SetNextLevelNumber(2);

	m_Goal->SetShader(defaultShader);

	m_PhysicsBridge.Initialize(m_Level, m_Ball);
	m_PhysicsBridge.GenerateDebug(m_Direct3D);


	CompletedFirstPass = true;
	points = 0;
	m_Highscore = new Highscore();
	m_HSSentence = new SentenceClass;
	m_HSSentence->Initialize(m_Direct3D, "left", 1.0f, 16, screenWidth, screenHeight);

	m_PauseText.Initialize(m_Direct3D, "center", 1.0f, sizeof("Game Paused"), screenWidth, screenHeight);
	m_PauseText.SetColor(1, 1, 1);
	m_PauseText.SetPosition(screenWidth / 2, screenHeight / 2);
	m_PauseText.SetText("Game Paused", m_Direct3D);

	// Skapa NameInput objekt
	m_NameInput = new NameInput;
	if (!m_NameInput)
	{
		return false;
	}

	// Initialisera NameInput objekt
	result = m_NameInput->Initialize(m_Direct3D, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Application::Frame(float deltaTime)
{
	if (deltaTime < 0.32f)
		gameTimer += (uint) (deltaTime * 1000);
	//m_fps.Frame();
	//Sleep(3000);

	// Updatera input
	m_Input->Frame();

	bool result = true;

	// Pause
	if (m_GameState == STATE_PAUSE)
	{
		bool res = false;

		if (m_Input->IsEnterPressed())
		{
			res = true;
		}

		if (res)
		{
			m_Clock->UnPausClock();
			m_GameState = STATE_PLAYING;
			m_Clock->UnPausClock();
		}
	}
	// Main Menu
	else if (m_GameState == STATE_MAINMENU)
	{
		// Hämta menyval
		int decision;
		decision = m_Menu->Update(deltaTime);

		switch (decision)
		{
		// Highscore
		case 2:
			m_GameState = STATE_HIGHSCORE;
			break;

		// Quit
		case 3:
			return false;

		// Play Endless
		case 4:
			m_GameMode = MODE_ENDLESS;
			m_GameState = STATE_PLAYING;
			m_Clock->RestartClock();
			break;

		// Play Classic
		case 5:
			m_GameMode = MODE_CLASSIC;
			m_GameState = STATE_PLAYING;
			m_Clock->RestartClock();
			break;

		// Play Easy
		case 6:
			m_GameMode = MODE_EASY;
			m_GameState = STATE_PLAYING;
			m_Clock->RestartClock();
			break;
		}

	}
	else if (m_GameState == STATE_HIGHSCORE)
	{
		if (m_Input->IsEnterPressed())
		{
			m_GameState = STATE_MAINMENU;
		}
	}
	else if (m_GameState == STATE_GAMEOVER)
	{
		m_NameInput->Update(deltaTime);
		string name = m_NameInput->InputName();

		if (m_Input->IsEnterPressed())
		{
			m_Highscore->SaveScore(name, points); // Ingen poäng för tid här
			ChangeLevel(0);
			m_GameState = STATE_HIGHSCORE;
			nrOfLives = MAX_NR_OF_LIVES;
			points = 0;
		}
	}
	else if (m_GameState == STATE_WON)
	{
		m_NameInput->Update(deltaTime);
		string name = m_NameInput->InputName();

		if (m_Input->IsEnterPressed())
		{
			m_Highscore->SaveScore(name, points);
			ChangeLevel(0);
			m_GameState = STATE_MAINMENU;
			nrOfLives = MAX_NR_OF_LIVES;
		}
	}
	else if (m_GameState == STATE_SWITCHLEVEL)
	{
		if (m_Input->IsEnterPressed())
		{
			m_GameState = STATE_PLAYING;
			m_Clock->RestartClock();
		}
	}
	// Playing
	else if (m_GameState == STATE_PLAYING)
	{

		//Check if the user has pressed Escape to close the program
		if (m_Input->IsEscapePressed())
		{
			return false;
		}

		//Check if the user has pressed Enter to pause
		if (m_Input->IsEnterPressed())
		{
			m_GameState = STATE_PAUSE;
			m_Clock->PausClock();
		}

		// Vanlig "vinkelhastighet"
		m_angleVelocity = 14.0f;

		// Dubbla "vinkelhastigheten" om space är nedtryckt
		if (m_Input->IsSpacePressed())
		{
			m_angleVelocity*=2;
		}


		//Check if the user is pressing buttons to rotate the level in the x-axis
		if (m_Input->IsUpPressed() || m_Input->IsWPressed())
		{
			if (!switchLevel)
			{
				float rotationAroundX = m_Level->GetRotationX();
				m_Level->SetRotationX(rotationAroundX - m_angleVelocity * deltaTime);
				m_PhysicsBridge.ActivateBall();
			}
		}
		else if (m_Input->IsDownPressed() || m_Input->IsSPressed())
		{
			if (!switchLevel)
			{
				float rotationAroundX = m_Level->GetRotationX();
				m_Level->SetRotationX(rotationAroundX + m_angleVelocity * deltaTime);
				m_PhysicsBridge.ActivateBall();
			}
		}

		//Check if the user is pressing buttons to rotate the level in the z-axis
		if (m_Input->IsLeftPressed() || m_Input->IsAPressed())
		{
			if (!switchLevel)
			{
				float rotationAroundZ = m_Level->GetRotationZ();
				m_Level->SetRotationZ(rotationAroundZ - m_angleVelocity * deltaTime);
				m_PhysicsBridge.ActivateBall();
			}
		}
		else if (m_Input->IsRightPressed() || m_Input->IsDPressed())
		{
			if (!switchLevel)
			{
				float rotationAroundZ = m_Level->GetRotationZ();
				m_Level->SetRotationZ(rotationAroundZ + m_angleVelocity * deltaTime);
				m_PhysicsBridge.ActivateBall();
			}
		}

		m_Level->Update(deltaTime);
		m_Ball->Update(deltaTime); //OBS: Används bara för friktionen! Annat uppdateras i PhysicsBridge::StepSimulation

		m_PhysicsBridge.StepSimulation(deltaTime, m_Level->GetRotationX()*0.0174532925f, 0, m_Level->GetRotationZ()*0.0174532925f, m_Ball, m_Level);


		//m_Camera->SetPosition(v3(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f));
		//m_Camera->LookAt(ballPosition);

		//Hämta bollens world, sätt kamerans position efter bollens position.
		m4 ballWorldM;
		m_Ball->GetWorldMatrix(ballWorldM);
		v3 camPos = ballWorldM.GetPos();
		m_Camera->LookAt(camPos);
		//m_Camera->SetPosition(v3(camPos.x - 2.5f, camPos.y + 3.65f, camPos.z - 7.0f));
		camPos.x = camPos.x -20.0f;
		camPos.y = camPos.y + 24.0f;
		camPos.z = camPos.z - 50.0f;
		m_Camera->SetPosition(camPos);

		v3 ballPosition = ballWorldM.GetPos();

		float planeRotX = m_Level->GetRotationX();
		float planeRotZ = m_Level->GetRotationZ();

		m_ObstacleHandler->Update(deltaTime, camPos.x, camPos.z, planeRotX, planeRotZ, m_Ball, &m_Particles);

		m_Goal->Update(deltaTime, planeRotX, planeRotZ);
		m_Box->Update(deltaTime, planeRotX, planeRotZ);

		float distance;
		float radiusOfGoal = 1.8f;
		//Håller man på med att byta banan? Om man inte gör det ska man kolla med kollisionen, se om bollet är tillräckligt nära målet
		if (!switchLevel)
		{
			//Titta om målet och bollen är nära nog för att kollidera
			v3 goalPosition;
			m_Ball->GetPosition(ballPosition);
			m4 ballWorld;
			m_Goal->GetWorldMatrix(ballWorld);
			goalPosition = ballWorld.GetPos();
			v3 relPos = ballPosition - goalPosition;
			distance = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
			float minDist = m_Ball->GetRadius() + radiusOfGoal;
			if (distance < minDist * minDist)
			{
				//Bollen är nära målet, så nu ska vi göra så att banan byts ut.
				switchLevel = true;
				finishedSwitch = false;
				m_Particles.RemoveAllParticles();
			}
		}


		if (switchLevel)
		{
			if (!finishedSwitch)
			{
				/*m_Ball->GetPosition(ballPosition);
				if (ballPosition.y <= -20.0f)
				{*/
				finishedSwitch = true;
				points += (m_Clock->GetTime(true) + 100);

				if (m_Goal->GetNextLevelNumber() == MAX_LEVELS)
				{
					m_GameState = STATE_WON;
				}
				else
				{
					ChangeLevel(m_Goal->GetNextLevelNumber());
					m_GameState = STATE_SWITCHLEVEL;
				}

				switchLevel = false;
				

				/*}*/
			}
		}

		m_Particles.Update(gameTimer, deltaTime);

		// Har bollen fallit av banan?
		v3 ballPos;
		m_Ball->GetFlatPosition(ballPos);

		if (ballPos.y <= 1.0f)
		{

			/*v3 ballPos;
			m_Ball->GetPosition(ballPos);
			stringstream ss;
			ss << "Boll.x: " << ballPos.x << ", Boll.y: " << ballPos.y << ", Boll.z: " << ballPos.z;
			string str = ss.str();
			wstring wstr;
			WCHAR* wcharstr;

			for (int i = 0; i < str.length(); ++i)
			wstr += wchar_t(str[i]);

			wcharstr = (WCHAR*)wstr.c_str();

			WBOX(wcharstr);*/
			if (m_GameMode == MODE_CLASSIC || m_GameMode == MODE_EASY)
			{
				nrOfLives--;
			}			

			if (nrOfLives <= -1)
			{
				m_GameState = STATE_GAMEOVER;
			}

			// Resetta rotationen på banan
			m_Level->SetRotation(0, 0, 0);

			// Resetta boll i fysiken
			m_PhysicsBridge.ResetBall(m_Ball);

			// Resetta boll-friction
			m_Ball->SetFriction(1.0f);

			// Resetta klockan
			m_Clock->RestartClock();

			//Ta bort alla partiklar
			m_Particles.RemoveAllParticles();
		}
	}

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

		m_Ball->Render(m_Direct3D);
		
		/*static m4 worldMatrix = m4::CreateTranslation(firstLightPassData.directionalLightDirection * 2.0f);
		Shader *currentShader = m_Direct3D->GetCurrentShader();

		currentShader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));

		m_Direct3D->RenderMesh(lightSphereMesh);*/

		m_Level->Render(m_Direct3D);

	m_Direct3D->EndShadowPass();


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		m_Direct3D->BeginDeferredRenderingScene(clearColor);
		
		if (m_GameState == STATE_PLAYING || m_GameState == STATE_PAUSE)
		{
			m_Ball->Render(m_Direct3D);

			m_Box->Render(m_Direct3D);

			m_Level->Render(m_Direct3D);

			m_Goal->Render(m_Direct3D);

			m_Direct3D->BeginLightStage();

			_RenderLights();

			m_Direct3D->EndLightStage();

		}

		m_Direct3D->EndDeferredRenderingScene();

		

		m_Skybox->Render(m_Direct3D);

		m_Direct3D->TurnZBufferReadOnWriteOff();
		m_Direct3D->TurnOnAlphaBlending();

		if (m_GameState == STATE_PLAYING || m_GameState == STATE_PAUSE)
		{
			m_Particles.Render();

			if (m_GameMode == MODE_CLASSIC || m_GameMode == MODE_EASY)
			{
				for (int i = 0; i < nrOfLives; i++)
				{
					m_Image[i]->Render(m_Direct3D);
				}
			}
			
			// Render text object
			char timeText[16];
			_itoa_s(m_Clock->GetTime(m_GameState != STATE_PAUSE), timeText, 10);

			m_Text->SetText(timeText, m_Direct3D);
			m_Text->SetPosition(screenWidth / 2, 15);
			m_Text->SetColor(0.1f, 0.5f, 1.0f);
			m_Text->Render(m_Direct3D);

		}

		if (m_GameState == STATE_HIGHSCORE)
		{
			m_StandardSignImage->Render(m_Direct3D);
			m_StandardInfoText->SetText("Top Players", m_Direct3D);
			m_StandardInfoText->SetColor(0.1f, 0.5f, 1.0f);
			m_StandardInfoText->Render(m_Direct3D);
			m_HSSentence->SetColor(0.1f, 0.8f, 0.8f);
			m_Highscore->PrintHighscore(m_HSSentence, m_Direct3D, screenWidth, screenHeight);
			
		}
		if (m_GameState == STATE_MAINMENU)
		{
			m_Menu->Render(m_Direct3D);
			m_Logo->Render(m_Direct3D);
		}

		if (m_GameState == STATE_SWITCHLEVEL)
		{
			// Render image object
			m_StandardSignImage->Render(m_Direct3D);
			m_StandardInfoText->SetText("Level Cleared", m_Direct3D);
			m_StandardInfoText->SetColor(1.0f, 1.0f, 1.0f);
			m_StandardInfoText->Render(m_Direct3D);
		}
		if (m_GameState == STATE_WON)
		{
			m_WinSignImage->Render(m_Direct3D);
			m_StandardInfoText->SetText("Victory", m_Direct3D);
			m_StandardInfoText->SetColor(0.1f, 0.8f, 0.8f);
			m_StandardInfoText->Render(m_Direct3D);
			m_NameInput->Render(m_Direct3D);
		}

		if (m_GameState == STATE_GAMEOVER)
		{
			m_GameOverSignImage->Render(m_Direct3D);
			m_StandardInfoText->SetText("Game Over", m_Direct3D);
			m_StandardInfoText->SetColor(0.7f, 0.1f, 0.1f);
			m_StandardInfoText->Render(m_Direct3D);
			m_NameInput->Render(m_Direct3D);
		}
		if (m_GameState == STATE_PAUSE)
			m_PauseText.Render(m_Direct3D);

		m_Direct3D->TurnOffAlphaBlending();
		m_Direct3D->TurnZBufferOn();

		m_Direct3D->TurnOnAlphaBlending();
		if (m_GameState == STATE_PLAYING || m_GameState == STATE_PAUSE)
		{
			m_ObstacleHandler->Render(m_Direct3D);
		}
		m_Direct3D->TurnOffAlphaBlending();
	

	m_Direct3D->EndScene();

}

void Application::Shutdown()
{
	// Release the sentence object
	if (m_HSSentence)
	{
		m_HSSentence->Shutdown();
		delete m_HSSentence;
		m_HSSentence = 0;
	}

	if (m_Highscore)
	{
		m_Highscore->Shutdown();
		delete m_Highscore;
		m_Highscore = 0;
	}

	// Release the NameInput object
	if (m_NameInput)
	{
		m_NameInput->Shutdown();
		m_NameInput = 0;
	}

	// Release the sound object
	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}
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

	//Release the box object
	if (m_Box)
	{
		m_Box->Shutdown();
		m_Box = 0;
	}

	//Release the skybox object
	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		m_Skybox = 0;
	}

	// Release the sentence object
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the sentence object
	if (m_StandardInfoText)
	{
		m_StandardInfoText->Shutdown();
		delete m_StandardInfoText;
		m_StandardInfoText = 0;
	}

	for (int i = 0; i < MAX_NR_OF_LIVES; i++)
	{
		// Release the image object
		if (m_Image[i])
		{
			m_Image[i]->Shutdown();
			m_Image[i] = 0;
		}
	}
	delete [] m_Image;

	// Release the image object
	if (m_StandardSignImage)
	{
		m_StandardSignImage->Shutdown();
		m_StandardSignImage = 0;
	}

	// Release the image object
	if (m_WinSignImage)
	{
		m_WinSignImage->Shutdown();
		m_WinSignImage = 0;
	}

	// Release the image object
	if (m_GameOverSignImage)
	{
		m_GameOverSignImage->Shutdown();
		m_GameOverSignImage = 0;
	}

	// Release the image object
	if (m_Logo)
	{
		m_Logo->Shutdown();
		m_Logo = 0;
	}

	// Release the menu object
	if (m_Menu)
	{
		m_Menu->Shutdown();
		m_Menu = 0;
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
	const float frustumSizeHalf = 32.0f;
	static v3 viewFrustumCorners[8] {
			{ v3(-frustumSizeHalf, 0, frustumSizeHalf) },
			{ v3(frustumSizeHalf, 0, frustumSizeHalf) },
			{ v3(-frustumSizeHalf, 0, -frustumSizeHalf) },
			{ v3(frustumSizeHalf, 0, -frustumSizeHalf) },

			{ v3(-frustumSizeHalf, frustumSizeHalf, frustumSizeHalf) },
			{ v3(frustumSizeHalf, frustumSizeHalf, frustumSizeHalf) },
			{ v3(-frustumSizeHalf, frustumSizeHalf, -frustumSizeHalf) },
			{ v3(frustumSizeHalf, frustumSizeHalf, -frustumSizeHalf) },
	};


	shadowCamera.GenerateDirectionalLightCamera(-firstLightPassData.directionalLightDirection,
		viewFrustumCorners);
	//shadowCamera.SetPosition(m_Ball->GetPosition() + firstLightPassData.directionalLightDirection * 30.0f);
	//shadowCamera.Generate3DViewMatrix();

	//Get the world, view, projectio nand orthographic matrices from the camera and direct3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);
	

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
	if (!directionalLightShader->SetVariable("viewProjectionInverse", &viewProjInverted, sizeof(m4)))
		WBOX(L"LOL");
	if (!directionalLightShader->SetVariable("shadowViewProjection", &shadowViewProj, sizeof(m4)))
		WBOX(L"LOL");
	directionalLightShader->SetVariable("cameraPosition", &camPos, sizeof(v3));

	//shadowfill
	Shader *shadowFillShader = m_Direct3D->GetShadowFill();
	if (!shadowFillShader->SetVariable("shadowView", &shadowView, sizeof(m4)))
		WBOX(L"LOL");
	if (!shadowFillShader->SetVariable("shadowProjection", &shadowProjection, sizeof(m4)))
		WBOX(L"LOL");

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
	//levelNumber = 0;
	LevelLoaderClass::ObstacleType *obstacles;
	v3 startPos;
	v3 goalPos;
	int nrOfObst;
	m_Level->LoadLevel(levelNumber, m_Direct3D, obstacles, startPos, goalPos, nrOfObst);

	m_Level->SetRotationX(0.0f);
	m_Level->SetRotationZ(0.0f);
	
	m_Box->SetScale( (m_Level->GetWidth() / 3.2));

	//Ta bort alla tidigare hinder
	m_ObstacleHandler->RemoveAllObstacles();
	//Lägg in de nya hindren
	for (int i = 0; i < nrOfObst; i++)
	{
		m_ObstacleHandler->AddObstacle(obstacles[i].type, obstacles[i].pos);
	}
	//Reinitsiera de nya hindren
	m_ObstacleHandler->Initialize(m_Direct3D, m_hwnd);
	m_ObstacleHandler->SetShader(obstacleShader);
	delete[] obstacles;

	m_Ball->SetPosition(startPos.x, 10, startPos.z);
	m_Goal->SetPosition(goalPos.x, goalPos.y, goalPos.z);

	m_Goal->SetNextLevelNumber(levelNumber+1);

	if (CompletedFirstPass)
	{
		m_PhysicsBridge.ReInitialize(m_Level, m_Ball);
	}

	if (m_GameMode == MODE_EASY)
	{
		nrOfLives = MAX_NR_OF_LIVES;
	}

	m_Clock->RestartClock();
}
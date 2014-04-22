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

	m_BallShader = nullptr;
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
	if (!m_Direct3D)
	{
		return false;
	}

	//Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		WBOX(L"Could not initialize DirectX 11.");
		return false;
	}


	//Create the input object
	m_Input = new Input();
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		WBOX(L"Could not initialize the input object.");
		return false;
	}

	//Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	//Create the ball object
	m_Ball = new Ball;
	if (!m_Ball)
	{
		return false;
	}

	//Create the level object
	m_Level = new Level;

	//Initialize the level object
	result = m_Level->Initialize(m_Direct3D, L"testLevelTexture.png");
	if (!result)
	{
		return false;
	}

	//Initialize the ball object
	result = m_Ball->Initialize(m_Direct3D, L"testTexture.png");
	if (!result)
	{
		return false;
	}
	

	//Create the ballshaderclass object
	//Initialize  the ballshaderclass object
	D3D11_INPUT_ELEMENT_DESC ballShaderElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_BallShader = m_Direct3D->LoadVertexShader(ShaderInfo("ball.vs", "BallVertexShader", "vs_4_0"),
		ballShaderElem,
		3);
	if (!m_BallShader)
		return false;
	if (!m_Direct3D->LoadShaderStageIntoShader(ShaderInfo("ball.ps", "BallPixelShader", "ps_4_0"),
		m_BallShader,
		SVF_PIXELSHADER))
		return false;
	
	//Initialize the fpsclass object
	m_fps.Initialize();

	m_Ball->SetPosition(1, 5, 0);
		

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
	/*v3 ballVelocity = m_Ball->GetVelocity() + v3(0, -9.81f, 0) * deltaTime;

	float time = m_Level->SphereHeightmapIntersection(ballPosition, m_Ball->GetRadius(), ballVelocity);

	if (time > 0 && time < deltaTime)
	{
		ballVelocity = ballVelocity.Reflect(testNormal);
		ballVelocity.y = 0;// .1 * ballVelocity.y;

		v3 pos = ballPosition + testNormal * time;
		m_Ball->SetPosition(pos.x, pos.y, pos.z);
	}

	m_Ball->SetVelocity(ballVelocity);*/
	

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

	m_Camera->SetTargetToLookAt(ballPosition.x, ballPosition.y, ballPosition.z);
	m_Camera->SetPosition(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f);

	// Render the graphics.
	RenderGraphics();

	return result;
}

void Application::RenderGraphics()
{
	m4 worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	//bool result;

	//Clear the scene
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//m_Camera->Render();

	//Get the world, view, projectio nand orthographic matrices from the camera and direct3d objects
	//m_Camera->GetViewMatrix(viewMatrix);

	v3 ballPosition;
	m_Ball->GetPosition(ballPosition);
	viewMatrix.ViewAtLH(v3(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f), 
		ballPosition);

	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_BallShader->SetVariable("viewMatrix", &viewMatrix, sizeof(m4));
	m_BallShader->SetVariable("projectionMatrix", &projectionMatrix, sizeof(m4));
	m_Direct3D->SetShader(m_BallShader);
	//Render the ball buffers.
	m_Ball->Render(m_Direct3D);

	m_Level->Render(m_Direct3D);


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

	//Release the ballshader object
	if (m_BallShader)
	{
		m_BallShader->Flush();
		m_BallShader = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

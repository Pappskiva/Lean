////////////////////////////////////////////////////////////////////////////////
// Filename: Application.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"

Application::Application()
{
	m_Direct3D = 0;
	m_Input = 0;
	m_Camera = 0;
	m_Ball = 0;
	m_Level = 0;

	m_BallShader = 0;
	m_LevelShader = 0;
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
	if(!m_Direct3D)
	{
		return false;
	}

	//Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
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
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
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

	//Initialize the ball object
	result = m_Ball->Initialize(m_Direct3D->GetDevice(), L"testTexture.png");
	if (!result)
	{
		return false;
	}

	//Create the level object
	m_Level = new Level;
	if (!m_Level)
	{
		return false;
	}

	//Initialize the level object
	result = m_Level->Initialize(m_Direct3D->GetDevice(), L"testLevelTexture.png");
	if (!result)
	{
		return false;
	}

	//Create the ballshaderclass object
	m_BallShader = new BallShaderClass;
	if (!m_BallShader)
	{
		return false;
	}

	//Initialize  the ballshaderclass object
	result = m_BallShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	//Create the levelshaderclass object
	m_LevelShader = new LevelShaderClass;
	if (!m_LevelShader)
	{
		return false;
	}

	//Initialize the levelshaderclass object
	result = m_LevelShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}
	//Initialize the fpsclass object
	m_fps.Initialize();
		

	return true;
}


void Application::Shutdown()
{
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
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
		delete m_Ball;
		m_Ball = 0;
	}

	//Release the level object
	if (m_Level)
	{
		m_Level->Shutdown();
		delete m_Level;
		m_Level = 0;
	}

	//Release the ballshader object
	if (m_BallShader)
	{
		m_BallShader->Shutdown();
		delete m_BallShader;
		m_BallShader = 0;
	}

	//Release the levelshader object
	if (m_LevelShader)
	{
		m_LevelShader->Shutdown();
		delete m_LevelShader;
		m_LevelShader = 0;
	}

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

	D3DXVECTOR3 testNormal;
	m_Level->GetNormal(testNormal);

	D3DXVECTOR3 ballPosition;
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
	m_Ball->Update(deltaTime);

	//Set the position of the camera
	m_Camera->SetTargetToLookAt(ballPosition.x, ballPosition.y, ballPosition.z);
	m_Camera->SetPosition(ballPosition.x - 2.5f, ballPosition.y + 3.65f, ballPosition.z - 7.0f);

	// Render the graphics.
	RenderGraphics();

	return result;
}

void Application::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	//Clear the scene
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	//Get the world, view, projectio nand orthographic matrices from the camera and direct3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Ball->UpdateWorldMatrix();
	m_Ball->GetWorldMatrix(worldMatrix);

	//Render the ball buffers.
	m_Ball->Render(m_Direct3D->GetDeviceContext());

	//Render the ball using the ball shader.
	result = m_BallShader->Render(m_Direct3D->GetDeviceContext(), m_Ball->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Ball->GetTexture());

	if (!result)
	{
		
	}

	m_Level->UpdateWorldMatrix();
	m_Level->GetWorldMatrix(worldMatrix);

	//Render the level buffers.
	m_Level->Render(m_Direct3D->GetDeviceContext());

	//Render the level using the level shader.
	result = m_LevelShader->Render(m_Direct3D->GetDeviceContext(), m_Level->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Level->GetTexture());



	m_Direct3D->EndScene();

}

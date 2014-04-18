////////////////////////////////////////////////////////////////////////////////
// Filename: Application.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"

Application::Application()
{
	m_Direct3D = 0;
	m_Input = 0;
	m_Camera = 0;
	m_AppTimer = 0;
	m_Ball = 0;

	m_BallShader = 0;
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

	//Create the application timer object
	m_AppTimer = new ApplicationTimer;
	if (!m_AppTimer)
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

	//Release the applicationtimer object
	if(m_AppTimer) 
	{
		delete m_AppTimer;
		m_AppTimer = 0;
	}

	//Release the ballshader object
	if (m_BallShader)
	{
		m_BallShader->Shutdown();
		delete m_BallShader;
		m_BallShader = 0;
	}

}


bool Application::Frame()
{
	float deltaTime = 0.0f;
	m_AppTimer->reset();
	m_AppTimer->tick();
	deltaTime = m_AppTimer->getDeltaTime();

	m_fps.Frame();	

	//Sleep(3000);

	bool result = true;

	//Update the input object
	result = m_Input->Frame();

	//Check if the user has pressed Escape to close the program
	if (m_Input->IsEscapePressed())
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);

	m_Ball->Update(deltaTime);

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

	m_Direct3D->EndScene();

}

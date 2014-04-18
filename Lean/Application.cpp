////////////////////////////////////////////////////////////////////////////////
// Filename: Application.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"

Application::Application()
{
	m_Direct3D = 0;
	m_Input = 0;

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

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_BallShader)
	{
		m_BallShader->Shutdown();
		delete m_BallShader;
		m_BallShader = 0;
	}

}


bool Application::Frame()
{

	//Sleep(3000);

	bool result = true;

	//Update the input object
	result = m_Input->Frame();

	if (m_Input->IsEscapePressed())
	{
		return false;
	}

	// Render the graphics.
	RenderGraphics();

	return result;
}

void Application::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	//Clear the scene
	//m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
}

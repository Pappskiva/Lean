////////////////////////////////////////////////////////////////////////////////
// Filename: Application.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>

#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "Ball.h"
#include "Level.h"
#include "Obstacle.h"
//Hej och goddag

//FPS kollare för debug
#include "FPSClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: Application
////////////////////////////////////////////////////////////////////////////////
class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame(float);

private:
	void RenderGraphics();
	
private:
	int screenWidth;
	int screenHeight;

	D3D *m_Direct3D;
	Input *m_Input;
	Camera *m_Camera;
	Ball *m_Ball;
	Level *m_Level;
	Obstacle *m_Obstacle; //För testningsyften

	FPSClass m_fps;

	Shader *m_BallShader;

};
#endif


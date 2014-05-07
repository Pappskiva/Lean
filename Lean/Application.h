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
#include "WaterObstacle.h"
#include "Goal.h"
#include "Skybox.h"

//FPS kollare för debug
#include "FPSClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


enum LightType : char
{
	LT_Directional,
	LT_Point,
	LT_Spot,
	LT_UserDefined,
};

struct LightPack
{
	v3 pos;
	float padding;
	v3 color;
	float range;
	float intensity;

	Mesh *mesh;//for padding purpose
	Shader *shader;
	LightType lightType;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Application
////////////////////////////////////////////////////////////////////////////////
class Application
{
public:
					Application();
					Application(const Application&);
					~Application();

	bool			Initialize(HINSTANCE, HWND, int, int);
	void			Shutdown();
	bool			Frame(float);

private:
	void			RenderGraphics();
	void			_UpdateShaderVariables();
	void			_RenderLights();
	void			AddPointLight(const v3 &center, const float radius, const v3 &color, const float intensity);

	void			ChangeLevel(int);

	
private:
	int				screenWidth;
	int				screenHeight;

	D3D				*m_Direct3D;
	Input			*m_Input;
	Camera			*m_Camera;
	Ball			*m_Ball;
	Level			*m_Level;
	Skybox			*m_Skybox;
	Obstacle		*m_WaterObstacle; //För testningsyften
	Goal			*m_Goal; //För testningsyften

	FPSClass		m_fps;

	Shader			*defaultShader, *levelShader, *obstacleShader;


	//kanske borde vara i någon sceneManager	
	Array<LightPack>	lights;
	Mesh			*lightSphereMesh;
	Shader			*pointLightShader;

};
#endif


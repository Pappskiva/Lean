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
#include "Box.h"
#include "ObstacleHandler.h"
#include "Goal.h"
#include "Skybox.h"
#include "PhysicsBridge.h"
#include "ParticleHandler.h"
#include "Sound.h"
//FPS kollare f�r debug
#include "FPSClass.h"
#include "Clock.h"
#include "SentenceClass.h"
#include "ImageClass.h"
#include "Menu.h"
#include "Highscore.h"
#include "NameInput.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 250.0f;
const float SCREEN_NEAR = 0.1f;
const int MAX_LEVELS = 4;
const int MAX_NR_OF_LIVES = 7;


enum GameState
{
	STATE_MAINMENU,
	STATE_PLAYING,
	STATE_PAUSE,
	STATE_SWITCHLEVEL,
	STATE_GAMEOVER,
	STATE_WON,
	STATE_HIGHSCORE,
};


enum GameMode
{
	MODE_ENDLESS,
	MODE_CLASSIC,
	MODE_EASY,
};


enum LightType : char
{
	LT_Directional,
	LT_Point,
	LT_Spot,
	LT_UserDefined,
};

class LightPack
{
public:

	void SetPosition(const v3 &pos);
	void SetColor(const v3 &color);
	void SetRange(const float range);
	void SetIntensity(const float intensity);

private:
	v3 pos;
	float padding;
	v3 color;
	float range;
	float intensity;

protected:

	Mesh *mesh;//for padding purpose
	Shader *shader;
	LightType lightType;

	friend class Application;
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
	LightPack&		AddPointLight(const v3 &center, const float radius, const v3 &color, const float intensity);
	LightPack&		AddPointLight();


	void			ChangeLevel(int);

	
private:
	uint			screenWidth;
	uint			screenHeight;

	int				points;

	HWND			m_hwnd;

	D3D				*m_Direct3D;
	Input			*m_Input;
	Camera			*m_Camera;
	Ball			*m_Ball;
	Box				*m_Box;
	Level			*m_Level;
	Skybox			*m_Skybox;
	ObstacleHandler	*m_ObstacleHandler;
	Goal			*m_Goal; //F�r testningsyften
	Sound			*m_Sound;
	Clock			*m_Clock;
	SentenceClass	*m_Text;
	SentenceClass	*m_StandardInfoText;
	SentenceClass	m_PauseText;

	SentenceClass	*m_HSSentence;
	SentenceClass	*m_CLPoints;




	ImageClass		**m_Image;
	ImageClass		*m_Logo;
	ImageClass		*m_WinSignImage;
	ImageClass		*m_GameOverSignImage;
	ImageClass		*m_StandardSignImage;

	ParticleHandler	m_Particles;
	GameState		m_GameState;
	GameMode		m_GameMode;
	Menu			*m_Menu;
	Highscore		*m_Highscore;
	NameInput		*m_NameInput;

	FPSClass		m_fps;
	PhysicsBridge m_PhysicsBridge;

	Shader			*defaultShader, *levelShader, *obstacleShader, *particleBillboard;

	uint			gameTimer;
	int				nrOfLives;
	float			m_angleVelocity;



	//Variabler f�r att byta bana p� r�tt s�tt
	bool			switchLevel;
	bool			finishedSwitch;
	bool			CompletedFirstPass;


	//kanske borde vara i n�gon sceneManager	
	Array<LightPack>	lights;
	Mesh			*lightSphereMesh;
	Shader			*pointLightShader;
	Shader			*directionalLightShader;
	Camera			shadowCamera;

	struct FirstLightPassData 
	{
		m4 viewProjInverse;
		m4 shadowViewProj;

		v3 directionalLightDirection;
		float padding;
		v3 directionalLightColor;
		float padding2;
		v3 ambientColor;
		float padding3;
		v3 cameraPos;
		float padding4;
		v2 offsets1;
		float padding5[2];
		v2 offsets2;
		float padding6[2];
		v2 offsets3;
		float padding7[2];
		v2 offsets4;
		float padding8[2];
	}firstLightPassData;
};
#endif


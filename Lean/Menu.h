#pragma once

#ifndef MENU_H
#define MENU_H

#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "SentenceClass.h"
#include "ImageClass.h"

enum SubMenu
{
	SUB_MAIN,
	SUB_PLAY
};

class Menu
{
public:
	Menu();
	~Menu();

	bool Initialize(D3D* d3d, Input* inputPtr, Camera* cameraPtr, int screenWidth, int screenHeight);
	int Update(float deltaTime);
	void Render(D3D* d3d);
	void Shutdown();

private:

	struct Color
	{
		float r, g, b;
	};

	ImageClass*		mLogo;
	SentenceClass*	mOption1;
	SentenceClass*	mOption2;
	SentenceClass*	mOption3;
	Input*			mInputPtr;
	Camera*			mCameraPtr;
	SubMenu			mSubMenu;

	int mScreenWidth, mScreenHeight, decision, nrOfOptions;
	float inputCooldown;

	Color* colors;
	Color currentColor;
	int colorIndex;

	bool ChangeNumber(float &from, float to, float deltaTime);

};

#endif
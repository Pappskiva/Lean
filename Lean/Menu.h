#pragma once

#ifndef MENU_H
#define MENU_H

#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "SentenceClass.h"
#include "ImageClass.h"

class Menu
{
public:
	Menu();
	~Menu();

	bool Initialize(D3D* d3d, Input* inputPtr, Camera* cameraPtr, int screenWidth, int screenHeight, m4 baseViewMatrix);
	bool Update(float deltaTime);
	void Render(D3D* d3d);
	void Shutdown();

private:
	ImageClass*		mLogo;
	SentenceClass*	mOption1;
	SentenceClass*	mOption2;
	Input*			mInputPtr;
	Camera*			mCameraPtr;

	int mScreenWidth, mScreenHeight, decision, nrOfOptions;
	float inputCooldown;
};

#endif
////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

#define WIN32_LEAN_AND_MEAN

#include "Application.h"
#include "ApplicationTimer.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame(float);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	Application* m_Application;
	ApplicationTimer* m_AppTimer;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif

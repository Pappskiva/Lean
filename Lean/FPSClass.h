////////////////////////////////////////////////////////////////////////////////
// Filename: FPSClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef FPSCLASS_H
#define FPSCLASS_H

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class FPSClass
{
public:
	FPSClass();
	FPSClass(const FPSClass&);
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};
#endif
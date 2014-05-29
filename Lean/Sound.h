////////////////////////////////////////////////////////////////////////////////
// Filename: Sound.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SOUND_H_
#define _SOUND_H_


#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <cmath>
#include "DuckRenderer\DMath.h"
#include <fstream>



class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	Sound();
	~Sound();

	bool Initialize2DSound(HWND, char* filename, int xPosition, int yPosition, int zPosition);
	void Shutdown();
	void PlayOnce();
	void PlayLoop(double VolumePercentage);
	void Play3DSound();
	void UpdateListener(int xListPos, int yListPos, int zListPos, v3 lookAT);

private:
	bool InitializeDirectSound3D(HWND);
	bool InitializeDirectSound2D(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile2D(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);
	long CalculateDistance();
	long CalculateOrientation();
	float CalculateAngle(v3 v1, v3 v2);


	LPDWORD temp;
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_PrimaryBuffer;
	IDirectSoundBuffer8* m_SecondaryBuffer1;



	int xPos, yPos, zPos;
	int xListPos, yListPos, zListPos;
	v3 m_lookAt;

};

#endif
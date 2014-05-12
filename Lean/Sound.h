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

	bool Initialize3DSound(HWND hwnd, char* filename, int xPosition, int yPosition, int zPosition);
	bool Initialize2DSound(HWND, char* filename);
	void Shutdown();
	void PlayOnce();
	void PlayLoop();
	void Play3DSound();
	void UpdateListener(int xListPos, int yListPos, int zListPos);

private:
	bool InitializeDirectSound3D(HWND);
	bool InitializeDirectSound2D(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile3D(char*, IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
	bool LoadWaveFile2D(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
	long CalculateDistance();


	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_PrimaryBuffer;
	IDirectSound3DListener8* m_Listener;
	IDirectSoundBuffer8* m_SecondaryBuffer1;
	IDirectSound3DBuffer8* m_secondary3DBuffer1;

	int xPos, yPos, zPos;
	int xListPos, yListPos, zListPos;
};

#endif
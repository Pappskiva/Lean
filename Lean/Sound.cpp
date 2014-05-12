////////////////////////////////////////////////////////////////////////////////
// Filename: Sound.h
////////////////////////////////////////////////////////////////////////////////

#include "Sound.h"
#define MINSOUNDDIST 10

Sound::Sound()
{
	m_DirectSound = 0;
	m_PrimaryBuffer = 0;
	m_SecondaryBuffer1 = 0;
	m_Listener = 0;
	m_secondary3DBuffer1 = 0;
	xPos = 0;
	yPos = 0;
	zPos = 0;
	xListPos = 0;
	yListPos = 0;
	zListPos = 0;
}
Sound::~Sound(){}
bool Sound::Initialize3DSound(HWND hwnd, char* filename, int xPosition, int yPosition, int zPosition)
{
	bool result;

	result = InitializeDirectSound3D(hwnd);
	if (!result)
		return false;

	result = LoadWaveFile3D(filename, &m_SecondaryBuffer1, &m_secondary3DBuffer1);
	if (!result)
		return false;

	xPos = xPosition;
	yPos = yPosition;
	zPos = zPosition;

	return true;
}
bool Sound::Initialize2DSound(HWND hwnd, char* filename)
{
	bool result;

	result = InitializeDirectSound2D(hwnd);
	if (!result)
		return false;

	result = LoadWaveFile2D(filename, &m_SecondaryBuffer1);
	if (!result)
		return false;

	return true;
}
void Sound::Shutdown()
{
	ShutdownWaveFile(&m_SecondaryBuffer1, &m_secondary3DBuffer1);
	ShutdownDirectSound();
}
bool Sound::InitializeDirectSound3D(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
		return false;

	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_PrimaryBuffer, NULL);
	if (FAILED(result))
		return false;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = m_PrimaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
		return false;

	result = m_PrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_Listener);
	if (FAILED(result))
		return false;

	m_Listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return true;
}
bool Sound::InitializeDirectSound2D(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
		return false;

	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_PrimaryBuffer, NULL);
	if (FAILED(result))
		return false;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = m_PrimaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
		return false;


	return true;
}
void Sound::ShutdownDirectSound()
{
	if (m_Listener)
	{
		m_Listener->Release();
		m_Listener = 0;
	}
	if (m_PrimaryBuffer)
	{
		m_PrimaryBuffer->Release();
		m_PrimaryBuffer = 0;
	}
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}
bool Sound::LoadWaveFile3D(char* filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	int error;
	FILE* filePtr;
	unsigned long count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return false;

	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		return false;

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') || (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		return false;

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	if (waveFileHeader.numChannels != 1)
		return false;

	if (waveFileHeader.sampleRate != 44100)
		return false;

	if (waveFileHeader.bitsPerSample != 16)
		return false;

	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') || (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		return false;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)* waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
		return false;

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
		return false;

	delete[] waveData;
	waveData = 0;

	result = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer);
	if (FAILED(result))
		return false;

	return true;
}
bool Sound::LoadWaveFile2D(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;


	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return false;

	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		return false;

	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') || (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		return false;

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') || (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		return false;
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') || (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		return false;

	waveFormat.wFormatTag = waveFileHeader.audioFormat;
	waveFormat.nSamplesPerSec = waveFileHeader.sampleRate;
	waveFormat.wBitsPerSample = waveFileHeader.bitsPerSample;
	waveFormat.nChannels = waveFileHeader.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
		return false;

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
		return false;

	delete[] waveData;
	waveData = 0;

	return true;
}
void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	if (*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}
void Sound::PlayOnce()
{
	m_SecondaryBuffer1->SetCurrentPosition(0);

	m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX);

	m_SecondaryBuffer1->Play(0, 0, 0);
}
void Sound::PlayLoop()
{
	m_SecondaryBuffer1->SetCurrentPosition(0);

	m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX);


	m_SecondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
}
void Sound::Play3DSound()
{
	m_SecondaryBuffer1->SetCurrentPosition(0);

	long temp = CalculateDistance() / 10;

	m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX*temp);

	m_secondary3DBuffer1->SetPosition(xPos, yPos, zPos, DS3D_IMMEDIATE);

	m_SecondaryBuffer1->Play(0, 0, 0);
}
void Sound::UpdateListener(int xListenerPosition, int yListenerPosition, int zListenerPosition)
{
	xListPos = xListenerPosition;
	yListPos = yListenerPosition;
	zListPos = zListenerPosition;

}
long Sound::CalculateDistance()
{
	long ans;

	long ax = xPos;
	long ay = yPos;
	long az = zPos;
	long bx = xListPos;
	long by = yListPos;
	long bz = zListPos;
	long temp, temp1, temp2, temp3;
	temp1 = (bx - ax) * (bx - ax);
	temp2 = (by - ay) * (by - ay);
	temp3 = (bz - az) * (bz - az);
	temp = temp1 + temp2 + temp3;
	ans = sqrt(temp);

	if (ans > MINSOUNDDIST)
		ans = 0;

	return ans;
}
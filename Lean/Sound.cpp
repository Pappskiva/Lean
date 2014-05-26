////////////////////////////////////////////////////////////////////////////////
// Filename: Sound.h
////////////////////////////////////////////////////////////////////////////////

#include "Sound.h"
#define MINSOUNDDIST	10
#define DBPERANGLE		50
#define HALFPI			1.57079
#define PI				3.14159
#define THREEQUATPI		4.71239
#define TWOPI			6.28318
#define ONE80PI			57.29577951308


Sound::Sound()
{
	m_DirectSound = 0;
	m_PrimaryBuffer = 0;
	m_SecondaryBuffer1 = 0;
	xPos = 0;
	yPos = 0;
	zPos = 0;
	xListPos = 0;
	yListPos = 0;
	zListPos = 0;
}
Sound::~Sound(){}
bool Sound::Initialize2DSound(HWND hwnd, char* filename, int xPosition, int yPosition, int zPosition)
{
	bool result;

	result = InitializeDirectSound2D(hwnd);
	if (!result)
		return false;

	result = LoadWaveFile2D(filename, &m_SecondaryBuffer1);
	if (!result)
		return false;

	xPos = xPosition;
	yPos = yPosition;
	zPos = zPosition;

	return true;
}
void Sound::Shutdown()
{
	ShutdownWaveFile(&m_SecondaryBuffer1);
	ShutdownDirectSound();
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
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
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
void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}
void Sound::PlayOnce()
{
	m_SecondaryBuffer1->SetCurrentPosition(0);

	m_SecondaryBuffer1->SetVolume(-2500);

	m_SecondaryBuffer1->Play(0, 0, 0);
}
void Sound::PlayLoop(double VolumePercentage)
{
	double temp = 10000 * VolumePercentage;
	long volume = -10000 + (long)temp;
	m_SecondaryBuffer1->SetCurrentPosition(0);
	m_SecondaryBuffer1->SetVolume(volume);
	m_SecondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
}
void Sound::Play3DSound()
{
	m_SecondaryBuffer1->SetCurrentPosition(0);
	m_SecondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	m_SecondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
}
void Sound::UpdateListener(int xListenerPosition, int yListenerPosition, int zListenerPosition, v3 lookAT)
{
	xListPos = xListenerPosition;
	yListPos = yListenerPosition;
	zListPos = zListenerPosition;


	this->m_lookAt.x = lookAT.x;
	this->m_lookAt.y = lookAT.y;
	this->m_lookAt.z = lookAT.z;

	m_SecondaryBuffer1->SetVolume(0 - CalculateDistance());

	m_SecondaryBuffer1->SetPan(CalculateOrientation());

}
long Sound::CalculateDistance()
{
	long ans;

	long temp, temp1, temp2, temp3;
	temp1 = (xListPos - xPos) * (xListPos - xPos);
	temp2 = (yListPos - yPos) * (yListPos - yPos);
	temp3 = (zListPos - zPos) * (zListPos - zPos);
	temp = temp1 + temp2 + temp3;
	ans = sqrt(temp);

	return ans ;
}
long Sound::CalculateOrientation()
{
	float ans = 0, angle = 0;
	v3 vTarget, vAim;
	vTarget.x = xPos - xListPos;//Create vector from camera to object
	vTarget.y = yPos - yListPos;
	vTarget.z = zPos - zListPos;

	vAim = m_lookAt;

	angle = CalculateAngle(vTarget, vAim);
	if (angle <= 90 && angle >= -90)//Ljud källan finns i första eller andra kvadranten
	{
		ans = angle;
	}
	else if (angle <= -90)//Ljud källan finns i tredje kvadranten
	{
		ans = 170 + angle;
		ans = ans*-1;
	}
	else//Ljud källan finns i fjärde kvadranten
	{
		ans = 170 - angle;
	}

	ans = ans*DBPERANGLE;

	if (ans < -10000)//Fixar så att det inte överskrider -10 000 till 10 000 intervallet
		ans = -10000;
	if (ans > 10000)
		ans = 10000;

	return ans;
}
float Sound::CalculateAngle(v3 vTarg, v3 vA)
{
	float ans, dot, checkRight;
	v3 v1, v2, up, cross;
	bool IsRight = false;

	v1 = vTarg;
	v2 = vA;
	up = v3(0, 1, 0);
	

	dot = v1.Dot(v2);	
	float vAbs1 = v1.Length();
	float vAbs2 = v2.Length();
	float Abs = vAbs1 * vAbs2;
	ans = dot / Abs;
	ans = acos(ans);

	ans = ans* ONE80PI;

	cross = v2.Cross(up);


	float dot2 = cross.Dot(v1);
	float abs1 = cross.Length();
	float abs2 = v1.Length();
	float abs = abs1 * abs2;
	checkRight = dot2 / abs;
	checkRight = acos(checkRight)*ONE80PI;;


	if (checkRight <= 90)
		IsRight = true;

	if (!IsRight)//returns negative if speaker is to the right of lookAt vector
		return ans * -1;
	else
		return ans;
}

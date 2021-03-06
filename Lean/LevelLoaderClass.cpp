#include "LevelLoaderClass.h"


LevelLoaderClass::LevelLoaderClass()
{
	levelFilePath = "data\\Levels\\";
}


LevelLoaderClass::LevelLoaderClass(const LevelLoaderClass& other)
{
}


LevelLoaderClass::~LevelLoaderClass()
{
}


//Plockar ihop r�tt filnamn, �ppnar textfilen f�r banan, l�ser in heightmap, l�ser in viktiga positioner och sparar som goalPos/startPos/obstacles.
//Anropar sedan LoadHeightmap f�r att l�sa heightmapen den fick fr�n textfilen och till sist NormalizeHeightMap f�r att ge mindre h�jdskillnader.
bool LevelLoaderClass::LoadLevel(int level, float *&heightMap, int &height, int &width, v3 &goalPos, v3 &startPos, ObstacleType *&obstacles, int &nrOfObst)
{
	bool result;
	string heightMapFileName;
	stringstream ss;
	ss << levelFilePath;
	ss << level;
	ss << ".txt";
	string levelFileName = ss.str();
	ifstream in;
	in.open(levelFileName);
	if (!in.is_open())
	{
		return false;
	}
	getline(in, heightMapFileName);
	int obstPos = 0;
	in >> nrOfObst;
	obstacles = new ObstacleType[nrOfObst];
	string str;
	while (!in.eof())
	{
		in >> str;
		if (str == "g")
		{
			in >> goalPos.x;
			in >> goalPos.z;
			goalPos.y = 3.80;
		}
		else if (str == "s")
		{
			in >> startPos.x;
			in >> startPos.z;
			startPos.y = 3.80;
		}
		else
		{
			obstacles[obstPos].type = str;
			in >> obstacles[obstPos].pos.x;
			in >> obstacles[obstPos].pos.z;
			obstacles[obstPos].pos.y = 3.80;
			obstPos++;
		}
	}
	in.close();

	heightMapFileName = levelFilePath + heightMapFileName;
	result = LoadHeightMap((char*)heightMapFileName.c_str(), heightMap);
	if (!result)
	{
		return false;
	}

	NormalizeHeightMap(heightMap); //M�ste �ndras n�r resten av klasserna �ndras.

	height = m_terrainHeight;
	width = m_terrainWidth;

	////Eftersom positionerna �r r�knade fr�n �versta v�nstra h�rnet och projektets origo �r i mitten av banan placeras positionerna om.
	//for (int i = 0; i < nrOfObst; i++)
	//{
	//	obstacles[i].pos.x -= m_terrainWidth/4;
	//	obstacles[i].pos.z = m_terrainHeight/2 - obstacles[i].pos.z;
	//	obstacles[i].pos.z -= m_terrainHeight/4;
	//	obstacles[i].pos.x -= 0.5f; //Eftersom pixlarna p� kartan �r s� stora m�ste vi l�gga om dem lite f�r att hamna d�r vi vill.
	//	obstacles[i].pos.z -= 0.5f;
	//}
	//startPos.x -= m_terrainWidth/4;
	//startPos.z = m_terrainHeight/2 - startPos.z;
	//startPos.z -= m_terrainHeight/4;
	//goalPos.x -= m_terrainWidth/4;
	//goalPos.z = m_terrainHeight/2 - goalPos.z;
	//goalPos.z -= m_terrainHeight/4;

	//Eftersom positionerna �r r�knade fr�n �versta v�nstra h�rnet och projektets origo �r i mitten av banan placeras positionerna om.
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i].pos.x -= m_terrainWidth / 2;
		obstacles[i].pos.z = m_terrainHeight - obstacles[i].pos.z;
		obstacles[i].pos.z -= m_terrainHeight / 2;
	}
	startPos.x -= m_terrainWidth / 2;
	startPos.z = m_terrainHeight - startPos.z;
	startPos.z -= m_terrainHeight / 2;
	goalPos.x -= m_terrainWidth / 2;
	goalPos.z = m_terrainHeight - goalPos.z;
	goalPos.z -= m_terrainHeight / 2;

	return true;
}


bool LevelLoaderClass::LoadHeightMap(char* filename, float *&heightMap)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = m_terrainWidth * m_terrainHeight * 3;

	bitmapImage = new unsigned char[imageSize];

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	heightMap = new float[m_terrainHeight * m_terrainWidth];

	k = 0;

	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_terrainHeight * j) + i;

			heightMap[index] = (float)height;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}


void LevelLoaderClass::NormalizeHeightMap(float* heightMap)
{
	int i, j;

	const float heightFactor = 1.0f / 255.0f;

	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			heightMap[(m_terrainHeight * j) + i] *= heightFactor;
		}
	}

	return;
}
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


//Plockar ihop rätt filnamn, öppnar textfilen för banan, läser in heightmap, läser in viktiga positioner och sparar som goalPos/startPos/obstacles.
//Anropar sedan LoadHeightmap för att läsa heightmapen den fick från textfilen och till sist NormalizeHeightMap för att ge mindre höjdskillnader.
bool LevelLoaderClass::LoadLevel(int level, float *&heightMap, int &height, int &width, v3 &goalPos, v3 &startPos, ObstacleType *&obstacles)
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
	int nrOfObst;
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
			goalPos.y = -1;
		}
		else if (str == "s")
		{
			in >> startPos.x;
			in >> startPos.z;
		}
		else
		{
			obstacles[obstPos].type = str;
			in >> obstacles[obstPos].pos.x;
			in >> obstacles[obstPos].pos.z;
			obstacles[obstPos].pos.y = -1;
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

	NormalizeHeightMap(heightMap); //Måste ändras när resten av klasserna ändras.

	height = m_terrainHeight;
	width = m_terrainWidth;

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

	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			heightMap[(m_terrainHeight * j) + i] /= 1.0f; //Gör för tillfället inget eftersom resten av klasserna inte funkar med denna ännu.
		}
	}

	return;
}
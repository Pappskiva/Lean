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
			goalPos.y = 2.55;
		}
		else if (str == "s")
		{
			in >> startPos.x;
			in >> startPos.z;
			startPos.y = 2.55;
		}
		else
		{
			obstacles[obstPos].type = str;
			in >> obstacles[obstPos].pos.x;
			in >> obstacles[obstPos].pos.z;
			obstacles[obstPos].pos.y = 2.55;
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

	//Eftersom positionerna är räknade från översta vänstra hörnet och projektets origo är i mitten av banan placeras positionerna om.
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i].pos.x -= 16;
		obstacles[i].pos.z = 32 - obstacles[i].pos.z;
		obstacles[i].pos.z -= 16;
		obstacles[i].pos.x -= 0.5f; //Eftersom pixlarna på kartan är så stora måste vi lägga om dem lite för att hamna där vi vill.
		obstacles[i].pos.z -= 0.5f;
	}
	startPos.x -= 16;
	startPos.z = 32 - startPos.z;
	startPos.z -= 16;
	goalPos.x -= 16;
	goalPos.z = 32 - goalPos.z;
	goalPos.z -= 16;

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
			heightMap[(m_terrainHeight * j) + i] /= 100.0f;
		}
	}

	return;
}
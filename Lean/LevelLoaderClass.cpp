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


bool LevelLoaderClass::LoadLevel(int level, VertexType *&heightMap, int &height, int &width, D3DXVECTOR3 &goalPos)
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
	if(!in.is_open())
	{
		return false;
	}
	getline(in, heightMapFileName);
	in >> goalPos.x;
	in >> goalPos.z;
	in.close();
	goalPos.y = -1;
	heightMapFileName = levelFilePath + heightMapFileName;
	result = LoadHeightMap((char*)heightMapFileName.c_str(), heightMap);
	if(!result)
	{
		return false;
	}	

	NormalizeHeightMap(heightMap);

	height = m_terrainHeight;
	width = m_terrainWidth;

	return true;
}


bool LevelLoaderClass::LoadHeightMap(char* filename, VertexType *&heightMap)
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
	if(error != 0)
	{
		return false;
	}

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = m_terrainWidth * m_terrainHeight * 3;

	bitmapImage = new unsigned char[imageSize];

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	heightMap = (VertexType*) calloc(m_terrainWidth * m_terrainHeight, sizeof(VertexType));
	
	k=0;

	for(j=0; j<m_terrainHeight; j++)
	{
		for(i=0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];
			
			index = (m_terrainHeight * j) + i;

			heightMap[index].position.x = (float)i;
			heightMap[index].position.y = (float)height;
			heightMap[index].position.z = (float)j;

			k+=3;
		}
	}

	delete [] bitmapImage;
	bitmapImage = 0;

	return true;
}


void LevelLoaderClass::NormalizeHeightMap(VertexType* heightMap)
{
	int i, j;


	for(j=0; j<m_terrainHeight; j++)
	{
		for(i=0; i<m_terrainWidth; i++)
		{
			heightMap[(m_terrainHeight * j) + i].position.y /= 10.0f;
		}
	}

	return;
}
#ifndef _LEVELLOADERCLASS_H_
#define _LEVELLOADERCLASS_H_

#include <d3d11.h>
//#include "..\\Include\\d3dx10math.h"
#include "DuckRenderer\DMath.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

class LevelLoaderClass
{
public:
	struct VertexType
	{
		v3 position;
		v2 texture;
		v3 normal;
	};

public:
	LevelLoaderClass();
	LevelLoaderClass(const LevelLoaderClass&);
	~LevelLoaderClass();

	//LoadLevel tar in int f�r leveln, �ppnar upp textfilen f�r information och lagrar sedan all data om leveln i de andra parametrarna. GoalPos f�r enbart x och z v�rde. 
	bool LoadLevel(int level, VertexType *&heightMap, int &height, int &width, v3 &goalPos);
	void Shutdown();

private: 
	//LoadHeightMap laddar in sj�lva heightmapen och informationen fr�n den s� som storlek p� planen.
	bool LoadHeightMap(char* filename, VertexType *&heightMap);
	//NormalizeHeightMap minskar skillnaden p� h�jderna. Detta kan flyttas till heightmapen, men f�r tillf�llet �r detta mer praktiskt.
	void NormalizeHeightMap(VertexType* heightMap);
	string levelFilePath;
	int m_terrainWidth,	m_terrainHeight;

};

#endif
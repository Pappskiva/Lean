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

	//LoadLevel tar in int för leveln, öppnar upp textfilen för information och lagrar sedan all data om leveln i de andra parametrarna. GoalPos får enbart x och z värde. 
	bool LoadLevel(int level, VertexType *&heightMap, int &height, int &width, v3 &goalPos);
	void Shutdown();

private: 
	//LoadHeightMap laddar in själva heightmapen och informationen från den så som storlek på planen.
	bool LoadHeightMap(char* filename, VertexType *&heightMap);
	//NormalizeHeightMap minskar skillnaden på höjderna. Detta kan flyttas till heightmapen, men för tillfället är detta mer praktiskt.
	void NormalizeHeightMap(VertexType* heightMap);
	string levelFilePath;
	int m_terrainWidth,	m_terrainHeight;

};

#endif
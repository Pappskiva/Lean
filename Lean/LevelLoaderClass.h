#ifndef _LEVELLOADERCLASS_H_
#define _LEVELLOADERCLASS_H_

#include <d3d11.h>
#include "DuckRenderer\DMath.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

class LevelLoaderClass
{
public:
	struct ObstacleType
	{
		v3 pos;
		string type;
	};

	LevelLoaderClass();
	LevelLoaderClass(const LevelLoaderClass&);
	~LevelLoaderClass();

	//LoadLevel tar in int för leveln, öppnar upp textfilen för information och lagrar sedan all data om leveln i de andra parametrarna. Positionerna får enbart x och z värde. 
	bool LoadLevel(int level, float *&heightMap, int &height, int &width, v3 &goalPos, v3 &startPos, ObstacleType *&obstacles);
	void Shutdown();

private:
	//LoadHeightMap laddar in själva heightmapen och informationen från den så som storlek på planen.
	bool LoadHeightMap(char* filename, float *&heightMap);
	//NormalizeHeightMap minskar skillnaden på höjderna. Detta kan flyttas till heightmapen, men för tillfället är detta mer praktiskt.
	void NormalizeHeightMap(float* heightMap);

	string levelFilePath;
	int m_terrainWidth, m_terrainHeight;

};

#endif
#pragma once
#ifndef OBSTACLEHANDLER_H
#define OBSTACLEHANDLER_H

#include "D3D.h"
#include <string>
#include "DuckRenderer\DMath.h"
#include "LeafObstacle.h"
#include "WaterObstacle.h"
#include "TrampolineObstacle.h"
#include "MagnetObstacle.h"

using namespace std;

class ObstacleHandler
{
public:
	ObstacleHandler();
	~ObstacleHandler();
	void AddObstacle(string type, v3 pos);
	void Render(D3D *direct3D);
	void Update(float deltaTime, float cameraPosX, float cameraPosZ, float planeRotationX, float planeRotationZ, Ball *ball);
	void RemoveAllObstacles();
	void Shutdown();
	bool Initialize(D3D*);
	void SetShader(const Shader *shader);

private:
	Obstacle** obstacles;
	int nrOfObst;
	int maxNrOfObst;
};
#endif
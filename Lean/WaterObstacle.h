#pragma once
#ifndef WATEROBSTACLE_H
#define WATEROBSTACLE_H

#include "Obstacle.h"

class WaterObstacle : public Obstacle
{
public:

	WaterObstacle();
	~WaterObstacle();

	virtual bool Initialize(D3D*, WCHAR*);
	virtual void Shutdown();
	virtual void Update(float, float, float);

protected:
	virtual bool 	InitalizeBuffers(D3D*);


};
#endif
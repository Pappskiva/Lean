#pragma once
#ifndef TRAMPOLINEOBSTACLE_H
#define TRAMPOLINEOBSTACLE_H

#include "Obstacle.h"

class TrampolineObstacle : public Obstacle
{
public:

	TrampolineObstacle();
	~TrampolineObstacle();

	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	//virtual void Update(float, float, float);

protected:
	//virtual bool 	InitalizeBuffers(D3D*);


};
#endif
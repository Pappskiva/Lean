////////////////////////////////////////////////////////////////////////////////
// Filename: TrampolineObstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
TrampolineObstacle är en subklass till Obstacle
*/
#pragma once
#ifndef TRAMPOLINEOBSTACLE_H
#define TRAMPOLINEOBSTACLE_H

#include "Obstacle.h"

class TrampolineObstacle : public Obstacle
{
public:

	TrampolineObstacle();
	~TrampolineObstacle();
	void Update(float, float, float, float, float, Ball*, ParticleHandler*);

	/*Initialisering och nedstängning*/
	virtual bool Initialize(D3D*, HWND);
	virtual void Shutdown();
	//virtual void Update(float, float, float);

protected:
	//virtual bool 	InitalizeBuffers(D3D*);


};
#endif
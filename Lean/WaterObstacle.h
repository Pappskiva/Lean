////////////////////////////////////////////////////////////////////////////////
// Filename: WaterObstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
WaterObstacle �r en subklass till Obstacle.
*/

#pragma once
#ifndef WATEROBSTACLE_H
#define WATEROBSTACLE_H

#include "Obstacle.h"

class WaterObstacle : public Obstacle
{
public:

	WaterObstacle();
	~WaterObstacle();

	/*Initialisering och nedst�ngning*/
	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	void Update(float, float, float, float, float, Ball*, ParticleHandler*);
	//virtual void Update(float, float, float);

protected:
	//virtual bool 	InitalizeBuffers(D3D*);


};
#endif
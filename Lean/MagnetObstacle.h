////////////////////////////////////////////////////////////////////////////////
// Filename: MagnetObstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
MagnetObstacle �r en subklass till Obstacle
*/

#pragma once
#ifndef MAGNETOBSTACLE_H
#define MAGNETOBSTACLE_H

#include "Obstacle.h"

class MagnetObstacle : public Obstacle
{
public:

	MagnetObstacle();
	~MagnetObstacle();

	/*Initialisering och nedst�ngning*/
	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	//virtual void Update(float, float, float);

protected:
	//virtual bool 	InitalizeBuffers(D3D*);


};
#endif
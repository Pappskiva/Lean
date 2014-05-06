////////////////////////////////////////////////////////////////////////////////
// Filename: LeafObstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
LeafObstacle �r en subklass till Obstacle
*/
#pragma once
#ifndef LEAFOBSTACLE_H
#define LEAFOBSTACLE_H

#include "Obstacle.h"

class LeafObstacle : public Obstacle
{
public:

	LeafObstacle();
	~LeafObstacle();

	/*Initialisering och nedst�ngning*/
	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	//virtual void Update(float, float, float);

protected:
	//virtual bool 	InitalizeBuffers(D3D*);


};
#endif
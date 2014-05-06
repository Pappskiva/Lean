////////////////////////////////////////////////////////////////////////////////
// Filename: LeafObstacle.h
////////////////////////////////////////////////////////////////////////////////

/*
LeafObstacle är en subklass till Obstacle
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

	/*Initialisering och nedstängning*/
	virtual bool Initialize(D3D*);
	virtual void Shutdown();
	//virtual void Update(float, float, float);

protected:
	//virtual bool 	InitalizeBuffers(D3D*);


};
#endif
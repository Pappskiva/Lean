#include "Particle.h"

/*===============================================================
					Particle Base Class
===============================================================*/

ParticleBase::ParticleBase()
{
	weight = 0.0f;
	xSize = 1.0f;
	ySize = 1.0f;
	fadeOutTime = 0;
	fadeInTime = 0;
	scaleXOutTime = 0;
	scaleXInTime = 0;
	scaleYOutTime = 0;
	scaleYInTime = 0;
	timeToLive = 0;
}

ParticleBase& ParticleBase::operator=(const ParticleBase &in)
{	
	this->weight = in.weight;
	this->xSize = in.xSize;
	this->ySize = in.ySize;
	this->rotSpeed = in.rotSpeed;
	this->speed = in.speed;

	this->normal = in.normal;

	this->fadeOutTime = in.fadeOutTime;
	this->fadeInTime = in.fadeInTime;
	this->scaleXOutTime = in.scaleXOutTime;
	this->scaleXInTime = in.scaleXInTime;
	this->scaleYOutTime = in.scaleYOutTime;
	this->scaleYInTime = in.scaleYInTime;
	this->timeToLive = in.timeToLive;

	return *this;
}

/*===============================================================
						Particle Class
===============================================================*/

Particle::Particle(const Particle &in)
{
	*this = in;
}

Particle& Particle::operator = (const Particle &in)
{		
	this->pos = in.pos;
	this->dir = in.dir;
	this->moveDir = in.moveDir;
	
	this->xScale = in.xScale;
	this->yScale = in.yScale;
	this->alpha = in.alpha;
	this->rot = in.rot;
	this->timeToDie = in.timeToDie;
	this->alive = in.alive;	

	return *this;
}

void Particle::SetTimeToDieAfter(const uint ms)
{
	//if (ms < this->
	//timeToDie = frameTimeStamp + ms;//FIX
}
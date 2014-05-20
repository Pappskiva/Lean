#include "ParticleEmitter.h"
#include "InstanceRenderer.h"


void ParticleEmitter::SetParticleEmitterBase(ParticleEmitterBase &base)
{
	renderDataSize = 0;
	this->base = base;

	for (uint i = 0; i < base.layoutSize; ++i)
	{
		switch (base.layout[i].type)
		{
		case Particle_Position:
			renderDataSize += sizeof(v3);
			break;
		case Particle_YScale:
			renderDataSize += sizeof(float);
			break;
		case Particle_XScale:
			renderDataSize += sizeof(float);
			break;
		case Particle_Rotation:
			renderDataSize += sizeof(float);
			break;
		case Particle_Alpha:
			renderDataSize += sizeof(float);
			break;
		case Particle_Move:
			renderDataSize += sizeof(v3);
			break;
		case Particle_Direction:
			renderDataSize += sizeof(v3);
			break;
		};
	}

	renderData = new byte[renderDataSize];
	timeOfSpawn = 0;
	timeOfLastParticleSpawn = 0;
}

void ParticleEmitter::SpawnParticle()
{
	if (base.maxParticles == particles.Length())
		return;//particles.RemoveLast();

	Particle *newParticle = &particles.Append();
	newParticle->pos = base.startPos;
	newParticle->timeToDie = timeOfSpawn + base.particleBase.timeToLive;
	newParticle->xScale = 1.0f;
	newParticle->yScale = 1.0f;
	newParticle->alpha = 1.0f; // test
	newParticle->alive = true;
	newParticle->rot = (float)0;//D3DXToRadian((rand() % 360));
	newParticle->moveDir = base.particleMove;

	if (base.randomMove)
	{
		int x, y, z;
		x = rand() % 100 - 50;
		y = rand() % 100 - 50;
		z = rand() % 100 - 50;
		newParticle->moveDir += v3((float)x, (float)y, (float)z).GetNormalized();
	}

	if (base.randomStartPlacement)
	{
		int x, z, x1, z1;
		x1 = base.startVariantConstraints.x;
		z1 = base.startVariantConstraints.y;
		x = rand() % x1 - x1/2;
		z = rand() % z1 - z1/2;
		newParticle->pos.x += (float)x;
		newParticle->pos.z += (float)z;
	}

	timeOfLastParticleSpawn = timeOfSpawn;
};

bool ParticleEmitter::Update(const float deltaTime)
{
	timeOfSpawn += deltaTime * 1000;
	//bool lol = timeOfLastParticleSpawn + base.particleBase.timeToLive < base.timeToLive;
	if (timeOfLastParticleSpawn + base.particleBase.timeToLive < base.timeToLive &&
		base.spawnFrequence && timeOfSpawn - timeOfLastParticleSpawn > base.spawnFrequence)
		SpawnParticle();

	for (uint i = 0; i < particles.Length(); ++i)
	{
		UpdateParticle(particles[i], deltaTime);

		if (!particles[i].alive)
		{
			particles.Remove(i--);
			continue;
		}

		BuildRenderData(particles[i]);
		base.particleInstanceRenderer->AddInstance(renderData, renderDataSize, InstanceID(base.texture, base.shader));
	}

	return base.timeToLive ? timeOfSpawn < base.timeToLive : true;
}

void ParticleEmitter::UpdateParticle(Particle &particle, const float deltaTimeSeconds)
{
	if (!particle.alive)
		return;

	if (particle.timeToDie <= timeOfSpawn)
	{
		particle.alive = false;
		return;
	}

	uint delta = base.particleBase.timeToLive - (particle.timeToDie - timeOfSpawn);

	if (delta < base.particleBase.fadeInTime)
		particle.alpha = (float)delta / base.particleBase.fadeInTime;

	if (delta > base.particleBase.timeToLive - base.particleBase.fadeOutTime)
		particle.alpha = 1.0f - (float)(delta - (base.particleBase.timeToLive - base.particleBase.fadeOutTime)) / base.particleBase.fadeOutTime;

	if (delta < base.particleBase.scaleXInTime)
		particle.xScale = (float)delta * base.particleBase.xSize / base.particleBase.scaleXInTime;

	if (delta > base.particleBase.timeToLive - base.particleBase.scaleXOutTime)
		particle.xScale = (1.0f - (float)(delta - (base.particleBase.timeToLive - base.particleBase.scaleXOutTime)) / base.particleBase.scaleXOutTime) * base.particleBase.xSize;

	if (delta < base.particleBase.scaleYInTime)
		particle.yScale = (float)delta * base.particleBase.ySize / base.particleBase.scaleYInTime;

	if (delta > base.particleBase.timeToLive - base.particleBase.scaleYOutTime)
		particle.yScale = (1.0f - (float)(delta - (base.particleBase.timeToLive - base.particleBase.scaleYOutTime)) / base.particleBase.scaleYOutTime) * base.particleBase.ySize;

	particle.alive = delta < base.particleBase.timeToLive;
	particle.pos += particle.moveDir * base.particleBase.speed * deltaTimeSeconds;
	particle.rot += base.particleBase.rotSpeed * deltaTimeSeconds;
}

void ParticleEmitter::BuildRenderData(const Particle &particle)
{
	uint offset = 0;

	for (uint i = 0; i < base.layoutSize; ++i)
	{
		switch (base.layout[i].type)
		{
		case Particle_Position:
			memcpy(renderData + offset, particle.pos.v, sizeof(v3));
			offset += sizeof(v3);
			break;
		case Particle_YScale:
			memcpy(renderData + offset, &particle.yScale, sizeof(float));
			offset += sizeof(float);
			break;
		case Particle_XScale:
			memcpy(renderData + offset, &particle.xScale, sizeof(float));
			offset += sizeof(float);
			break;
		case Particle_Rotation:
			memcpy(renderData + offset, &particle.rot, sizeof(float));
			offset += sizeof(float);
			break;
		case Particle_Alpha:
			memcpy(renderData + offset, &particle.alpha, sizeof(float));
			offset += sizeof(float);
			break;
		case Particle_Move:
			memcpy(renderData + offset, particle.moveDir.v, sizeof(v3));
			offset += sizeof(v3);
			break;
		case Particle_Direction:
			memcpy(renderData + offset, particle.dir.v, sizeof(v3));
			offset += sizeof(v3);
			break;
		};
	}
}

void ParticleEmitter::Flush()
{
	particles.Clear();

	delete[] renderData;
	renderData = nullptr; 
}

void ParticleEmitter::Reset()
{
	particles.Clear();
}
 
void ParticleEmitterBase::Flush()
{
	delete[] layout;
	layoutSize = 0;
}
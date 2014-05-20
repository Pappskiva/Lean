
#ifndef PARTICLE_EMITTER_H

//#include 
#include "DuckRenderer//Array.h"
#include "Particle.h"
#include "InstanceRenderer.h"
#include "Texture.h"
#include "Shader.h"

typedef unsigned char byte;


class ParticleEmitterBase
{
	uint						timeToLive;
	uint						spawnFrequence;//in ms
	uint						maxParticles;

	v3							startPos;
	v3							particleMove;
	v2							startVariantConstraints;
	bool						randomMove;
	bool						randomStartPlacement;
	Texture						*texture;
	Shader						*shader;

	ParticleRenderLayoutMember	*layout;
	uint						layoutSize;
	ParticleBase				particleBase;
	InstanceRenderer			*particleInstanceRenderer;
public:

	void						SetTimeToLive(const uint ttl);
	void						SetSpawnFrequency(const uint milliSec);
	void						SetMaximumParticles(const uint max);

	void						SetStartPosition(const v3 &v);
	void						SetStartVariation(const v2 &v);
	void						SetRandomVariation(const bool variant);
	void						SetParticleMove(const v3 &v);
	void						SetRandomMove(const bool move);
	

	void						SetTexture(const Texture *texture);
	void						SetShader(const Shader *shader);

	void						SetRenderLayout(ParticleRenderLayoutMember *layout, const uint layoutSize);
	void						SetParticleBase(const ParticleBase & particleBase);
	void						SetParticleRenderer(InstanceRenderer *particleRenderer);


	
	void						Flush();

	//void Set

	friend class ParticleEmitter;
};

class ParticleEmitter
{
protected:
	ParticleEmitterBase			base;


	bool						active;
	uint						timeOfSpawn;
	uint						timeOfLastParticleSpawn;
	v3							pos;

	uint						renderDataSize;
	byte						*renderData;
	Array<Particle>				particles;

	void						SpawnParticle();
	void						UpdateParticle(Particle &particle, const float deltaTimeSeconds);
	void						BuildRenderData(const Particle &particle);

public:

	ParticleEmitter(){}
	void						SetParticleEmitterBase(ParticleEmitterBase &base);
	//virtual void				InitializeParticleEmitter(const ParticleEmitterInitialize &iniData);
	bool						Update(const float deltaTimeSeconds);
	void						Reset();
	void						Flush();

	const ParticleEmitterBase*	GetBase() const;

	bool						isActive() const;

	friend class Effect;
	friend class EffectBase;
};

inline bool ParticleEmitter::isActive() const
{
	return active;
}

inline void ParticleEmitterBase::SetTimeToLive(const uint ttl)
{
	timeToLive = ttl;
}

inline void ParticleEmitterBase::SetSpawnFrequency(const uint milliSec)
{
	spawnFrequence = milliSec;
}

inline void ParticleEmitterBase::SetMaximumParticles(const uint max)
{
	maxParticles = max;
}

inline void ParticleEmitterBase::SetStartPosition(const v3 &v)
{
	startPos = v;
}

inline void ParticleEmitterBase::SetStartVariation(const v2 &v)
{
	startVariantConstraints = v;
}

inline void ParticleEmitterBase::SetRandomVariation(const bool variant)
{
	randomStartPlacement = variant;
}

inline void ParticleEmitterBase::SetParticleMove(const v3 &v)
{
	particleMove = v;
}

inline void ParticleEmitterBase::SetRandomMove(const bool move)
{
	randomMove = move;
}

inline void ParticleEmitterBase::SetTexture(const Texture *texture)
{
	this->texture = (Texture *)texture;
}

inline void ParticleEmitterBase::SetShader(const Shader *shader)
{
	this->shader = (Shader *)shader;
}

inline void ParticleEmitterBase::SetRenderLayout(ParticleRenderLayoutMember *layout, const uint layoutSize)
{
	this->layout = new ParticleRenderLayoutMember[layoutSize];
	this->layoutSize = layoutSize;
	memcpy(this->layout, layout, sizeof(ParticleRenderLayoutMember)* layoutSize);
}

inline void ParticleEmitterBase::SetParticleBase(const ParticleBase &particleBase)
{
	this->particleBase = particleBase;
}

inline void ParticleEmitterBase::SetParticleRenderer(InstanceRenderer *particleRenderer)
{
	this->particleInstanceRenderer = particleRenderer;
}

#define PARTICLE_EMITTER_H
#endif
#ifndef PARTICLE_H

#include "DuckRenderer//DMath.h"

#ifndef uint
typedef unsigned int uint;
#endif


/*===============================================================
					Particle Base Class
===============================================================*/
/*
struct ParticleBase //advanced
{
	PARTICLE_TEXTURE_ID			particleType;
	
	bool					billboarded;

	template <typename _Type>
	struct ValueOverTime
	{
		uint timeStamp:
		_Type value;
	};

	Array<ValueOverTime<float>> weight;
	Array<ValueOverTime<float>> xSize;
	Array<ValueOverTime<float>> ySize;
	Array<ValueOverTime<float>> rotSpeed;
	Array<ValueOverTime<float>> speed;

							ParticleBase();
							ParticleBase(const ParticleBase& in);
}*/

enum ParticleMember : uint
{
	Particle_Position,
	Particle_YScale,
	Particle_XScale,
	Particle_Rotation,
	Particle_Alpha,
	Particle_Move,
	Particle_Direction,
	
};

struct ParticleRenderLayoutMember
{
	uint placeInBuffer;
	uint type;
};

struct ParticleBase
{
	//ParticleSystem			*particleSystem;
	
	float					weight;
	float					xSize;
	float					ySize;
	float					rotSpeed;
	float					speed;

	v3						normal;

	uint					fadeOutTime;//in ms
	uint					fadeInTime;//in ms
	uint					scaleXOutTime;//in ms
	uint					scaleXInTime;//in ms
	uint					scaleYOutTime;//in ms
	uint					scaleYInTime;//in ms
	uint					timeToLive;//in ms
							ParticleBase();
							ParticleBase& operator=(const ParticleBase &in);
};

/*===============================================================
						Particle Class
===============================================================*/

class Particle
{
	friend class ParticleEmitter;

protected:

	//ParticleBase			*particleBase;
	
	v3						pos;
	v3						dir;
	v3						moveDir;
	
	float					xScale;
	float					yScale;
	float					alpha;
	float					rot;
	uint					timeToDie;
	bool					alive;

public:

							Particle(const Particle &in);
							Particle(){}

	//virtual bool			Update();
	bool					isAlive() const;

	Particle&				operator = (const Particle &in);

	v3						GetPos() const;
	v3						GetDir() const;
	v3						GetMoveDir() const;
	float					GetScaleX() const;
	float					GetScaleY() const;
	float					GetAlpha() const;
	float					GetRotation() const;
	uint					GetTextureID() const;
	uint					GetShaderID() const;

	void					SetPos(v3 pos);
	void					SetDir(v3 dir);
	void					SetScaleY(const float scale);
	void					SetScaleX(const float scale);
	void					SetMoveDir(const v3 &moveDir);
	void					SetTimeToDieAfter(const uint ms);
	void					SetTimeToDie(const uint time);
};

inline v3 Particle::GetPos() const
{
	return pos;
}

inline v3 Particle::GetDir() const
{
	return dir;
}

inline v3 Particle::GetMoveDir() const
{
	return moveDir;
}

inline float Particle::GetAlpha() const
{
	return alpha;
}

inline float Particle::GetRotation() const
{
	return rot;
}

inline void Particle::SetPos(v3 pos)
{
	this->pos = pos;
}

inline void Particle::SetDir(v3 dir)
{
	this->dir = dir;
}

inline void Particle::SetScaleY(const float scale)
{
	yScale = scale;
}

inline void Particle::SetScaleX(const float scale)
{
	xScale = scale;
}

inline void Particle::SetMoveDir(const v3 &moveDir)
{
	this->moveDir = moveDir;
}

#define PARTICLE_H
#endif
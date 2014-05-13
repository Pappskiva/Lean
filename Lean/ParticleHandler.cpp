////////////////////////////////////////////////////////////////////////////////
// Filename: ParticleHandler.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ParticleHandler.h"

ParticleHandler::ParticleHandler()
{

}

ParticleHandler::~ParticleHandler()
{

}

void ParticleHandler::Initialize(D3D* direct3D, Shader* particleBillboard)
{
	//Partikel
	struct ParticleVertex
	{
		float uv[2];
	} vertices[] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f },

		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};

	Mesh *particleMesh = direct3D->CreateMeshFromRam(vertices, sizeof(ParticleVertex), 6);

	particleRenderer.Initialize(512, 36, particleMesh);
	direct3D->PrepareInstanceRenderer(particleRenderer);

	Texture *leaf = direct3D->LoadTextureFromFile("data//64x64LeafParticleColored.png");

	struct ParticleRenderData
	{
		v3 pos;
		float scale[2];
		float rot;
		float alpha;
	};

	ParticleRenderLayoutMember layout[] = {
		{ 0, Particle_Position },
		{ 0, Particle_XScale },
		{ 0, Particle_YScale },
		{ 0, Particle_Rotation },
		{ 0, Particle_Alpha },
	};
	ParticleBase particleBase;
	particleBase.fadeInTime = 0;
	particleBase.fadeOutTime = 100;
	particleBase.rotSpeed = 3.9f;
	particleBase.scaleXInTime = 1000;
	particleBase.scaleXOutTime = 100;
	particleBase.scaleYInTime = 1000;
	particleBase.scaleYOutTime = 100;
	particleBase.speed = 1.0f;
	particleBase.timeToLive = 2000;
	particleBase.weight = 0;
	particleBase.xSize = 1.0f;
	particleBase.ySize = 1.0f;

	ParticleEmitterBase emitterBase;
	emitterBase.SetTimeToLive(0);
	emitterBase.SetSpawnFrequency(25);
	emitterBase.SetMaximumParticles(100);
	emitterBase.SetParticleMove(v3(0, 1, 0));
	emitterBase.SetRandomMove(true);
	emitterBase.SetShader(particleBillboard);
	emitterBase.SetTexture(leaf);
	emitterBase.SetRenderLayout(layout, 5);
	emitterBase.SetParticleBase(particleBase);
	emitterBase.SetParticleRenderer(&particleRenderer);

	testEmitter.SetParticleEmitterBase(emitterBase);

}
void ParticleHandler::Shutdown()
{
	testEmitter.Flush();
	particleRenderer.Flush();
}
void ParticleHandler::Update(const uint frameTimeStamp, const float deltaTimeSeconds)
{
	testEmitter.Update(frameTimeStamp, deltaTimeSeconds);
	particleRenderer.Update();
}
void ParticleHandler::Render()
{
	particleRenderer.Render();
	particleRenderer.ClearInstances();
}

void ParticleHandler::AddParticles(int type)
{

}

void ParticleHandler::RemoveAllParticles()
{

}



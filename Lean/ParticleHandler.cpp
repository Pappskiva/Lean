////////////////////////////////////////////////////////////////////////////////
// Filename: ParticleHandler.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ParticleHandler.h"

ParticleHandler::ParticleHandler()
{
	particleBillboard = nullptr;
	leaf = nullptr;
}

ParticleHandler::~ParticleHandler()
{

}

void ParticleHandler::Initialize(D3D* direct3D, Shader* particleBillboard)
{
	this->particleBillboard = particleBillboard;
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

	leaf = direct3D->LoadTextureFromFile("data//64x64LeafParticleColored.png");

	/*struct ParticleRenderData
	{
		v3 pos;
		float scale[2];
		float rot;
		float alpha;
	};*/

	//AddParticles(1);


}
void ParticleHandler::Shutdown()
{
	for (int i = 0; i < emitters.Length(); i++)
	{
		emitters[i].Flush();
	}
	particleRenderer.Flush();
}
void ParticleHandler::Update(const uint frameTimeStamp, const float deltaTimeSeconds)
{
	for (int i = 0; i < emitters.Length(); i++)
	{
		if (!emitters[i].Update(deltaTimeSeconds))
		{
			emitters[i].Flush();
			emitters.Remove(i--);
		}
	}

	particleRenderer.Update();
}
void ParticleHandler::Render()
{
	particleRenderer.Render();
	particleRenderer.ClearInstances();
}

void ParticleHandler::AddParticles(int type)
{

	ParticleRenderLayoutMember layout[] = {
		{ 0, Particle_Position },
		{ 0, Particle_XScale },
		{ 0, Particle_YScale },
		{ 0, Particle_Rotation },
		{ 0, Particle_Alpha },
	};
	ParticleBase particleBase;
	ParticleEmitterBase emitterBase;

	switch (type)
	{
	case 1: //Standard lövhinder partikeleffekt
		particleBase.fadeInTime = 0;
		particleBase.fadeOutTime = 100;
		particleBase.rotSpeed = 3.9f;
		particleBase.scaleXInTime = 100;
		particleBase.scaleXOutTime = 200;
		particleBase.scaleYInTime = 100;
		particleBase.scaleYOutTime = 200;
		particleBase.speed = 4.0f;
		particleBase.timeToLive = 1700;
		particleBase.weight = 0;
		particleBase.xSize = 5.0f;
		particleBase.ySize = 5.0f;

		emitterBase.SetTimeToLive(30000);
		emitterBase.SetSpawnFrequency(35);
		emitterBase.SetMaximumParticles(200);
		emitterBase.SetStartPosition(v3(0, 12, 0));
		emitterBase.SetStartVariation(v2(80, 80)); //För stunden kan inget av värdena vara 0
		emitterBase.SetRandomVariation(true);
		emitterBase.SetParticleMove(v3(-1, -1, -1));
		emitterBase.SetRandomMove(false);
		emitterBase.SetShader(particleBillboard);
		emitterBase.SetTexture(leaf);
		emitterBase.SetRenderLayout(layout, 5);
		emitterBase.SetParticleBase(particleBase);
		emitterBase.SetParticleRenderer(&particleRenderer);

		emitters.Append().SetParticleEmitterBase(emitterBase);

		break;
	};

	
}

void ParticleHandler::RemoveAllParticles()
{
	for (int i = 0; i < emitters.Length(); i++)
	{
		emitters[i].Flush();
	}
}



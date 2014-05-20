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

	particleRenderer.Initialize(1024, 36, particleMesh);
	direct3D->PrepareInstanceRenderer(particleRenderer);

	leaf = direct3D->LoadTextureFromFile("data//64x64LeafParticleColored.png");
	waterSplash = direct3D->LoadTextureFromFile("data//128x128WaterSplash.png");
	waterDrop = direct3D->LoadTextureFromFile("data//64x64WaterParticle.png");

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

void ParticleHandler::AddParticles(int type, float x, float y, float z)
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

		emitterBase.SetTimeToLive(7000);
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
	case 2: //Lövhinder explosion!
		particleBase.fadeInTime = 0;
		particleBase.fadeOutTime = 100;
		particleBase.rotSpeed = 5.2f;
		particleBase.scaleXInTime = 800;
		particleBase.scaleXOutTime = 100;
		particleBase.scaleYInTime = 800;
		particleBase.scaleYOutTime = 100;
		particleBase.speed = 4.0f;
		particleBase.timeToLive = 1100;
		particleBase.weight = 1.0f;
		particleBase.xSize = 3.0f;
		particleBase.ySize = 3.0f;

		emitterBase.SetTimeToLive(2100);
		emitterBase.SetSpawnFrequency(10);
		emitterBase.SetMaximumParticles(100);
		emitterBase.SetStartPosition(v3(x, y, z));
		//emitterBase.SetStartVariation(v2(1, 1)); //För stunden kan inget av värdena vara 0
		//emitterBase.SetRandomVariation(false);
		emitterBase.SetParticleMove(v3(0, 2, 0));
		emitterBase.SetRandomMove(true);
		emitterBase.SetShader(particleBillboard);
		emitterBase.SetTexture(leaf);
		emitterBase.SetRenderLayout(layout, 5);
		emitterBase.SetParticleBase(particleBase);
		emitterBase.SetParticleRenderer(&particleRenderer);

		emitters.Append().SetParticleEmitterBase(emitterBase);

		break;
	case 3: //Water splash
		particleBase.fadeInTime = 100;
		particleBase.fadeOutTime = 100;
		particleBase.rotSpeed = 5.2f;
		particleBase.scaleXInTime = 400;
		particleBase.scaleXOutTime = 300;
		particleBase.scaleYInTime = 400;
		particleBase.scaleYOutTime = 300;
		particleBase.speed = 6.0f;
		particleBase.timeToLive = 800;
		particleBase.weight = 10.0f;
		particleBase.xSize = 1.6f;
		particleBase.ySize = 1.6f;

		emitterBase.SetTimeToLive(1400);
		emitterBase.SetSpawnFrequency(10);
		emitterBase.SetMaximumParticles(50);
		emitterBase.SetStartPosition(v3(x, y, z));
		//emitterBase.SetStartVariation(v2(1, 1)); //För stunden kan inget av värdena vara 0
		//emitterBase.SetRandomVariation(false);
		emitterBase.SetParticleMove(v3(0, 1, 0));
		emitterBase.SetRandomMove(true);
		emitterBase.SetShader(particleBillboard);
		emitterBase.SetTexture(waterSplash);
		emitterBase.SetRenderLayout(layout, 5);
		emitterBase.SetParticleBase(particleBase);
		emitterBase.SetParticleRenderer(&particleRenderer);

		emitters.Append().SetParticleEmitterBase(emitterBase);

		break;
	case 4: //Water dripping
			particleBase.fadeInTime = 0;
			particleBase.fadeOutTime = 100;
			particleBase.rotSpeed = 0.0f;
			particleBase.scaleXInTime = 50;
			particleBase.scaleXOutTime = 50;
			particleBase.scaleYInTime = 50;
			particleBase.scaleYOutTime = 50;
			particleBase.speed = 1.0f;
			particleBase.timeToLive = 400;
			particleBase.weight = 1.0f;
			particleBase.xSize = 0.8f;
			particleBase.ySize = 0.8f;

			emitterBase.SetTimeToLive(800);
			emitterBase.SetSpawnFrequency(50);
			emitterBase.SetMaximumParticles(20);
			emitterBase.SetStartPosition(v3(x, y, z));
			emitterBase.SetStartVariation(v2(2, 2)); //För stunden kan inget av värdena vara 0, och bör vara integers
			emitterBase.SetRandomVariation(true);
			emitterBase.SetParticleMove(v3(0, -0.5f, 0));
			emitterBase.SetRandomMove(true);
			emitterBase.SetShader(particleBillboard);
			emitterBase.SetTexture(waterDrop);
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



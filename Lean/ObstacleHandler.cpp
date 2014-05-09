#include "ObstacleHandler.h"

ObstacleHandler::ObstacleHandler()
{
	maxNrOfObst = 10;
	obstacles = new Obstacle*[maxNrOfObst];
	nrOfObst = 0;
}

ObstacleHandler::~ObstacleHandler()
{
	
}

void ObstacleHandler::AddObstacle(string type, v3 pos)
{
	if (nrOfObst == maxNrOfObst)
	{
		maxNrOfObst *= 2;
		Obstacle **temp = new Obstacle*[maxNrOfObst];
		for (int i = 0; i < nrOfObst; i++)
		{
			temp[i] = obstacles[i];
		}
		delete[] obstacles;
		obstacles = temp;
	}
	if (type == "l")
	{
		obstacles[nrOfObst] = new LeafObstacle();
		obstacles[nrOfObst]->SetPosition(pos.x, pos.y, pos.z);
	}
	else if (type == "i")
	{
		obstacles[nrOfObst] = new WaterObstacle();
		obstacles[nrOfObst]->SetPosition(pos.x, pos.y, pos.z);
	}
	else if (type == "m")
	{
		obstacles[nrOfObst] = new MagnetObstacle();
		obstacles[nrOfObst]->SetPosition(pos.x, pos.y, pos.z);
	}
	else if (type == "t")
	{
		obstacles[nrOfObst] = new TrampolineObstacle();
		obstacles[nrOfObst]->SetPosition(pos.x, pos.y, pos.z);
	}
	nrOfObst++;
}

void ObstacleHandler::Render(D3D *direct3D)
{
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i]->Render(direct3D);
	}
}

void ObstacleHandler::Update(float deltaTime, float cameraPosX, float cameraPosZ, float planeRotationX, float planeRotationZ, Ball *ball)
{
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i]->Update(deltaTime, cameraPosX, cameraPosZ, planeRotationX, planeRotationZ, ball);
	}
}

void ObstacleHandler::RemoveAllObstacles()
{
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i]->Shutdown();
	}

	for (int i = 0; i < nrOfObst; i++)
	{
		delete obstacles[i];
		obstacles[i] = 0;
	}

	nrOfObst = 0;
}

void ObstacleHandler::Shutdown()
{
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i]->Shutdown();
	}

	for (int i = 0; i < nrOfObst; i++)
	{
		delete obstacles[i];
	}
	delete[] obstacles;
}

bool ObstacleHandler::Initialize(D3D* device)
{
	bool result = true;
	for (int i = 0; i < nrOfObst; i++)
	{
		if (!obstacles[i]->Initialize(device))
		{
			result = false;
		}
	}
	return result;
}

void ObstacleHandler::SetShader(const Shader *shader)
{
	for (int i = 0; i < nrOfObst; i++)
	{
		obstacles[i]->SetShader(shader);
	}
}
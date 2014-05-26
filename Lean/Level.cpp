////////////////////////////////////////////////////////////////////////////////
// Filename: Level.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Level.h"

Level::Level()
{
	m_Texture1 = nullptr;
	m_Texture2 = nullptr;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_scale = 0.5f;

	heightmap = nullptr;

	m_MaximumRotationValue = 20.0f; //Degrees

	worldMatrix = m4::IDENTITY;
}

Level::Level(const Level& other)
{

}

Level::~Level()
{

}

bool Level::Initialize(D3D* direct3D, WCHAR* textureFileName1, WCHAR* textureFileName2)
{
	bool result;

	// Initialisera vertex och index buffrarna för att skapa föremålets geometri.
	result = InitializeBuffers(direct3D);
	if (!result)
		return false;


	//Ladda in texturerna till föremålet.
	m_Texture1 = direct3D->LoadTextureFromFile(textureFileName1);
	if (!m_Texture1)
		return false;

	m_Texture2 = direct3D->LoadTextureFromFile(textureFileName2);
	if (!m_Texture2)
		return false;

	m_mask = direct3D->LoadTextureFromFile("Data//randomHeightmap.png");
	if (!m_mask)
		return false;

	return true;
}

void Level::Shutdown()
{
	// Release vertex och index arrayen
	ShutdownBuffers();

	return;
}

void Level::Update(float deltaTime)
{
	//Uppdatera banan

	//Kolla så banan inte lutar för mycket åt båda riktningar i x-led.
	if (m_rotationX > m_MaximumRotationValue)
	{
		m_rotationX = m_MaximumRotationValue;
	}
	else if (m_rotationX < -m_MaximumRotationValue)
	{
		m_rotationX = -m_MaximumRotationValue;
	}

	//Kolla så banan inte lutar för mycket åt båda riktningar i z-led.
	if (m_rotationZ > m_MaximumRotationValue)
	{
		m_rotationZ = m_MaximumRotationValue;
	}
	else if (m_rotationZ < -m_MaximumRotationValue)
	{
		m_rotationZ = -m_MaximumRotationValue;
	}

}

void Level::Render(D3D* direct3D)
{
	// Lägg in vertex och index buffrarna på grafikpipelinen för att förbereda dem för rendering.
	//UpdateWorldMatrix();

	direct3D->SetShader(m_shader);
	Shader *currentShader = direct3D->GetCurrentShader();
	m4 newWorld = m4::CreateScale(v3(m_scale, 1.0f, m_scale)) * worldMatrix;
	currentShader->SetVariable("worldMatrix", &newWorld, sizeof(m4));
	direct3D->ApplyConstantBuffers();
	direct3D->ApplyTexture(m_Texture1, 0);
	direct3D->ApplyTexture(m_Texture2, 1);
	direct3D->ApplyTexture(m_mask, 2);

	direct3D->RenderMesh(m_mesh);

	return;
}


ID3D11ShaderResourceView* Level::GetTexture(int textureNumber)
{
	if (textureNumber == 0)
	{
		return m_Texture1->GetTexture();
	}
	else if (textureNumber == 1)
	{
		return m_Texture2->GetTexture();
	}

	return nullptr;
}

void Level::SetPosition(float positionX, float positionY, float positionZ)
{
	//Sätt positionen
	m_positionX = positionX;
	m_positionY = positionY;
	m_positionZ = positionZ;
}

void Level::GetPosition(v3& pos)
{
	//Få positionen
	pos = v3(m_positionX, m_positionY, m_positionZ);
}

void Level::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	//Sätt rotationen
	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;
}

void Level::GetRotation(v3& rot)
{
	//Få rotationen
	rot = v3(m_rotationX, m_rotationY, m_rotationZ);
}

float Level::GetRotationX()
{
	return m_rotationX;
}

void Level::SetRotationX(float rotX)
{
	m_rotationX = rotX;
}

float Level::GetRotationZ()
{
	return m_rotationZ;
}

void Level::SetRotationZ(float rotZ)
{
	m_rotationZ = rotZ;
}

void Level::SetScale(float scale)
{
	//Sätt skalan
	m_scale = scale;
}

float Level::GetScale()
{
	//Få skalan
	return m_scale;
}

void Level::GetNormal(v3& normal)
{
	//Ändrar och hämtar normalvektorn
	normal = v3(0.0f, 1.0f, 0.0f);
	
	m4 rotationMatrix;
	m4 inverseRotation;
	m4 transpose;
	rotationMatrix = m4::CreateYawPitchRoll(m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);

	inverseRotation = rotationMatrix.Inverse();

	normal = normal * inverseRotation;
}

void Level::UpdateWorldMatrix()
{
	//Ändra worldmatrisen baserat på postion, rotation och skala.
	m4 rotationMatrix;
	m4 scaleMatrix;
	m4 localSpace;

	rotationMatrix = m4::CreateYawPitchRoll(m_rotationY * 0.0174532925f, m_rotationX * 0.0174532925f, m_rotationZ * 0.0174532925f);
	scaleMatrix = m4::CreateScale(v3(m_scale, 1.0f, m_scale));
	localSpace = m4::CreateTranslation(v3(m_positionX, m_positionY, m_positionZ));

	worldMatrix = scaleMatrix * rotationMatrix * localSpace;
}

void Level::GetWorldMatrix(m4& worldMatrix)
{
	//Få worldmatrisen
	worldMatrix = this->worldMatrix;
}

float Level::SphereHeightmapIntersection(const v3 &center, const float radius, const v3 &velocity)
{
	v3 centerLocalSpace = center;
	m3 inverseRotation(worldMatrix);
	inverseRotation = inverseRotation.Transpose();

	centerLocalSpace = centerLocalSpace * inverseRotation;

	Sphere sphere;
	sphere.pos = center;
	sphere.radius = radius;

	float time;
	
	for (uint i = 0; i < triangleCount; ++i)
	if (TraceSphereTriangle(velocity, sphere, collisionTriangles[i], time))
	{
		return time;
	}

	return 0;
}

bool Level::InitializeBuffers(D3D* direct3D)
{
	uint vertexCount = 6;

	struct Vertex
	{
		v3 position;
		v3 normal;
		float u, v;
	} *vertices = new Vertex[vertexCount];


	//En quad för enkelt trianglelist testning. En placeholder tills vi faktiskt kunnat arbeta mer på banor
	// Fyller vertex arrayen med data

	const float planeSize = .5f;
	
	vertices[0].position = v3(planeSize, 0.0f, -planeSize);
	vertices[0].normal = v3(0, 1, 0);
	vertices[0].u = 1.0f;
	vertices[0].v = 0.0f;

	vertices[1].position = v3(-planeSize, 0.0f, -planeSize);
	vertices[1].normal = v3(0, 1, 0);
	vertices[1].u = 0.0f;
	vertices[1].v = 0.0f;

	vertices[2].position = v3(planeSize, 0.0f, planeSize);
	vertices[2].normal = v3(0, 1, 0);
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3].position = v3(planeSize, 0.0f, planeSize);
	vertices[3].normal = v3(0, 1, 0);
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;

	vertices[4].position = v3(-planeSize, 0.0f, -planeSize);
	vertices[4].normal = v3(0, 1, 0);
	vertices[4].u = 0.0f;
	vertices[4].v = 0.0f;

	vertices[5].position = v3(-planeSize, 0.0f, planeSize);
	vertices[5].normal = v3(0, 1, 0);
	vertices[5].u = 0.0f;
	vertices[5].v = 1.0f;

	triangleCount = 2;
	collisionTriangles = new Triangle[triangleCount];
	collisionTriangles[0].points[0] = vertices[0].position;
	collisionTriangles[0].points[1] = vertices[1].position;
	collisionTriangles[0].points[2] = vertices[2].position;

	collisionTriangles[1].points[0] = vertices[0].position;
	collisionTriangles[1].points[1] = vertices[1].position;
	collisionTriangles[1].points[2] = vertices[2].position;

	// Release the arrays now that the vertex and index buffers have been created and loaded.

	m_mesh = direct3D->CreateMeshFromRam(vertices, sizeof(Vertex), vertexCount);
	
	delete[] vertices;

	return true;
}

void Level::ShutdownBuffers()
{

	return;
}

void Level::LoadLevel(const uint levelIndex, D3D* direct3D, LevelLoaderClass::ObstacleType *&obstacles, v3 &startPos, v3 &goalPos, int &nrOfObst)
{
	if (heightmap)
	{
		delete [] heightmap;
		heightmap = 0;
	}

	LevelLoaderClass loader;
	int realHeight, realWidth;

	m_scale = 0.5f;	
	//Ohanterade variabler för att matcha ändring i parameterlistan i LevelLoaderClass::LoadLevel.

	loader.LoadLevel(levelIndex, heightmap, realHeight, realWidth, goalPos, startPos, obstacles, nrOfObst);	
	m_width = realWidth * 2;
	m_length = realWidth * 2;

	float *newHeightmap = new float[m_width * m_length];
	memset(newHeightmap, 0, sizeof(float) * m_width * m_length);

	//
	for (uint z = 1, nz = 1; z < realHeight - 1; ++z, nz += 2)
		for (uint x = 1, nx = 1; x < realWidth - 1; ++x, nx += 2)
		{
			newHeightmap[(nx - 1) + (nz - 1) * m_width] = heightmap[x + realWidth * z];
			newHeightmap[nx		  + (nz - 1) * m_width] = heightmap[x + realWidth * z];
			newHeightmap[(nx + 1) + (nz - 1) * m_width] = heightmap[x + realWidth * z];

			newHeightmap[(nx - 1) + nz * m_width] = heightmap[x + realWidth * z];
			newHeightmap[nx		  +	nz * m_width] = heightmap[x + realWidth * z];
			newHeightmap[(nx + 1) + nz * m_width] = heightmap[x + realWidth * z];

			newHeightmap[(nx - 1) + (nz + 1) * m_width] = heightmap[x + realWidth * z];
			newHeightmap[nx		  +	(nz + 1) * m_width] = heightmap[x + realWidth * z];
			newHeightmap[(nx + 1) + (nz + 1) * m_width] = heightmap[x + realWidth * z];
		}
	delete[] heightmap;
	heightmap = newHeightmap;

	_FilterHeightMap(heightmap, m_length, m_width);
	_FilterHeightMap(heightmap, m_length, m_width);

	/*for (uint z = 1; z < m_length - 1; ++z)
		for (uint x = 1; x < m_width - 1; ++x)
			newHeightmap[x + z * m_width] /= 9.0f;*/
	
	//m_scale = LEVEL_POINT_DISTANCE;

	


	//m_width = realWidth;
	//m_length = realWidth;


	struct Vertex
	{
		v3 pos;
		v3 normal;
		v2 uv;
	}*vertices = new Vertex[m_width * m_length + 8];

	for (uint z = 0; z < m_length; z++)
		for (uint x = 0; x < m_width; x++)
		{
			vertices[x + z * m_width].pos.x = (x - m_width * 0.5f);
			vertices[x + z * m_width].pos.y = heightmap[x + z * m_width] * LEVEL_MAX_HEIGHT;
			vertices[x + z * m_width].pos.z = (z - m_width * 0.5f);
			vertices[x + z * m_width].uv.v[0] = (float)x / m_width;
			vertices[x + z * m_width].uv.v[1] = (float)z / m_length;
		}

	const float thickness = 2.0f;
	vertices[m_width * m_length].pos = v3(m_width * -.5f, 0.0f, m_length * .5f - 1);
	vertices[m_width * m_length].normal = v3(-1, 0, 0);
	vertices[m_width * m_length + 1].pos = v3(m_width * -.5f, -thickness, m_length * .5f - 1);
	vertices[m_width * m_length + 1].normal = v3(-1, 0, 0);
	vertices[m_width * m_length + 2].pos = v3(m_width * -.5f, 0.0f, m_length * -.5f);
	vertices[m_width * m_length + 2].normal = v3(-1, 0, 0);
	vertices[m_width * m_length + 3].pos = v3(m_width * -.5f, -thickness, m_length * -.5f);
	vertices[m_width * m_length + 3].normal = v3(-1, 0, 0);

	vertices[m_width * m_length + 4].pos = v3(m_width * -.5f, 0.0f, m_length * -.5f);
	vertices[m_width * m_length + 4].normal = v3(0, 0, -1);
	vertices[m_width * m_length + 5].pos = v3(m_width * -.5f, -thickness, m_length * -.5f);
	vertices[m_width * m_length + 5].normal = v3(0, 0, -1);
	vertices[m_width * m_length + 6].pos = v3(m_width * .5f - 1, 0.0f, m_length * -.5f);
	vertices[m_width * m_length + 6].normal = v3(0, 0, -1);
	vertices[m_width * m_length + 7].pos = v3(m_width * .5f - 1, -thickness, m_length * -.5f);
	vertices[m_width * m_length + 7].normal = v3(0, 0, -1);

	//Calculates the normal for each triangle and adds it to the vertices 
	//that builds it up.
	for (uint x = 0; x < m_width - 1; ++x)	
		for (uint z = 0; z < m_length - 1; ++z)
		{
			v3 vec1 = vertices[(z + 1) * m_width + (x + 1)].pos
				-
				vertices[z * m_width + x].pos;

			v3 vec2 = vertices[z * m_width + (x + 1)].pos
				-
				vertices[z * m_width + x].pos;

			v3 normal = vec1.Cross(vec2);
			vertices[z * m_width + x].normal += normal;
			vertices[(x + 1) + z * m_width].normal += normal;
			vertices[(x + 1) + (z + 1) * m_width].normal += normal;

			vec1 = vertices[(z + 1) * m_width + x].pos
				-
				vertices[z * m_width + x].pos;

			vec2 = vertices[(z + 1) * m_width + (x + 1)].pos
				-
				vertices[z * m_width + x].pos;

			normal = vec1.Cross(vec2);
			vertices[(x + 1) + (z + 1) * m_width].normal += normal;
			vertices[x + z * m_width].normal += normal;
			vertices[x + (z + 1) * m_width].normal += normal;
		}

	uint *indices = new uint[(m_length - 1) * (m_width - 1) * 6 + 2 * 6];

	for (uint z = 0; z < m_length - 1; z++)
		for (uint x = 0; x < m_width - 1; x++)
		{
			//Calculating the normalized normal
			vertices[z * m_width + x].normal.Normalize();

			//Calculating the indices
			indices[(x + z * (m_width - 1)) * 6] = (x + 1) + (z + 1) * m_width;
			indices[(x + z * (m_width - 1)) * 6 + 1] = (x + 1) + z * m_width;
			indices[(x + z * (m_width - 1)) * 6 + 2] = x + z * m_width;

			indices[(x + z * (m_width - 1)) * 6 + 3] = (x + 1) + (z + 1) * m_width;
			indices[(x + z * (m_width - 1)) * 6 + 4] = x + z * m_width;
			indices[(x + z * (m_width - 1)) * 6 + 5] = x + (z + 1) * m_width;
		}

	indices[(m_length - 1) * (m_width - 1) * 6] = m_width * m_length;
	indices[(m_length - 1) * (m_width - 1) * 6 + 1] = m_width * m_length + 2;
	indices[(m_length - 1) * (m_width - 1) * 6 + 2] = m_width * m_length + 1;

	indices[(m_length - 1) * (m_width - 1) * 6 + 3] = m_width * m_length + 2;
	indices[(m_length - 1) * (m_width - 1) * 6 + 4] = m_width * m_length + 3;
	indices[(m_length - 1) * (m_width - 1) * 6 + 5] = m_width * m_length + 1; 


	indices[(m_length - 1) * (m_width - 1) * 6 + 6] = m_width * m_length + 4;
	indices[(m_length - 1) * (m_width - 1) * 6 + 7] = m_width * m_length + 6;
	indices[(m_length - 1) * (m_width - 1) * 6 + 8] = m_width * m_length + 5;

	indices[(m_length - 1) * (m_width - 1) * 6 + 9] = m_width * m_length + 6;
	indices[(m_length - 1) * (m_width - 1) * 6 + 10] = m_width * m_length + 7;
	indices[(m_length - 1) * (m_width - 1) * 6 + 11] = m_width * m_length + 5;

	m_mesh->Flush();
	m_mesh->Initialize(vertices, sizeof(Vertex), m_width * m_length + 8, indices, (m_length - 1) * (m_width - 1) * 6 + 2 * 6);
	direct3D->LoadMeshIntoDevice(m_mesh);
	heightmapToPhys = heightmap;

	delete[] vertices;	
	delete[] indices;

	SetTextureBasedOnNumber(levelIndex, direct3D);
}

void Level::SetTextureBasedOnNumber(int levelIndex, D3D* direct3D)
{
	//Ladda in texturen baserad på vilket nummer banan har

	if (levelIndex == 1)
	{
		m_Texture1 = direct3D->LoadTextureFromFile(L"data//testSpringGround.png");
	}
	else if (levelIndex == 2)
	{
		m_Texture1 = direct3D->LoadTextureFromFile(L"data//testSummerGround.png");
	}
	else if (levelIndex == 3)
	{
		m_Texture1 = direct3D->LoadTextureFromFile(L"data//testAutumnGround.png");
	}
	else if (levelIndex == 4)
	{
		m_Texture1 = direct3D->LoadTextureFromFile(L"data//testWinterGround.png");
	}
	else
	{
		m_Texture1 = direct3D->LoadTextureFromFile(L"data//testSpringGround.png");
	}
	
}

void Level::SetWorldMatrix(m4& world)
{
	worldMatrix = world;
}

float* Level::GetHeighMapData()
{
	return heightmapToPhys;
}

uint Level::GetWidth()
{
	return m_width;
}

uint Level::GetLength()
{
	return m_length;
}

float Level::GetMaxHeight() const
{
	return LEVEL_MAX_HEIGHT;
}

void Level::_FilterHeightMap(float *floatArray, const uint arrayHeight, const uint arrayWidth)
{
	float *temp = new float[arrayWidth * arrayHeight];
	memcpy(temp, floatArray, sizeof(float) * arrayHeight * arrayWidth);

	const float div = 1.0f / 9.0f;
	const float div2 = 1.0f / 6.0f;

	for (uint x = 1; x < arrayWidth - 1; ++x)
	{
		for (uint y = 1; y < arrayHeight - 1; ++y)
		{
			floatArray[x + y * arrayWidth] =
				temp[x - 1 + (y - 1) * arrayWidth] +
				temp[x + (y - 1) * arrayWidth] +
				temp[x + 1 + (y - 1) * arrayWidth] +

				temp[x - 1 + y * arrayWidth] +
				temp[x + y * arrayWidth] +
				temp[x + 1 + y * arrayWidth] +

				temp[x - 1 + (y + 1) * arrayWidth] +
				temp[x + (y + 1) * arrayWidth] +
				temp[x + 1 + (y + 1) * arrayWidth];

			floatArray[x + y * arrayWidth] *= div;
		}
	}

	delete[] temp;
}
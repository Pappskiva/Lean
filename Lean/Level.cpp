////////////////////////////////////////////////////////////////////////////////
// Filename: Level.cpp
////////////////////////////////////////////////////////////////////////////////

#include "Level.h"
#include "LevelLoaderClass.h"

Level::Level()
{
	m_Texture = nullptr;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_scale = 5.0f;

	m_MaximumRotationValue = 20.0f; //Degrees

	worldMatrix = m4::IDENTITY;
}

Level::Level(const Level& other)
{

}

Level::~Level()
{

}

bool Level::Initialize(D3D* direct3D, WCHAR* textureFileName)
{
	bool result;

	// Initialisera vertex och index buffrarna för att skapa föremålets geometri.
	result = InitializeBuffers(direct3D);
	if (!result)
		return false;


	//Ladda in texturen till föremålet.
	m_Texture = direct3D->LoadTextureFromFile(textureFileName);
	if (!m_Texture)
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
	UpdateWorldMatrix();

	direct3D->SetShader(m_shader);
	m_shader->SetVariable("worldMatrix", &worldMatrix, sizeof(m4));
	direct3D->ApplyConstantBuffers();
	direct3D->ApplyTexture(m_Texture, 0);

	direct3D->RenderMesh(m_mesh);

	return;
}


ID3D11ShaderResourceView* Level::GetTexture()
{
	return m_Texture->GetTexture();
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
	scaleMatrix = m4::CreateScale(v3(m_scale, m_scale, m_scale));
	localSpace = m4::CreateTranslation(v3(m_positionX, m_positionY, m_positionZ));

	worldMatrix = rotationMatrix * localSpace;
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

void Level::LoadLevel(const uint levelIndex, D3D* direct3D)
{
	LevelLoaderClass loader;
	int tempHeight, tempWidth;
	v3 pos;

	const float LEVEL_POINT_DISTANCE = 1.0f / 75.0f;
	const float LEVEL_MAX_HEIGHT = 1.0f / 255.0f;
	
	loader.LoadLevel(levelIndex, heightmap, tempHeight, tempWidth, pos);
	uint height = (uint)tempHeight, width = (uint)tempWidth;
	
	struct Vertex
	{
		v3 pos;
		v3 normal;
		v2 uv;
	}*vertices = new Vertex[height * width];

	for (uint z = 0; z < height; z++)
		for (uint x = 0; x < width; x++)
		{
			vertices[x + z * width].pos.x = (x - width * 0.5f) * LEVEL_POINT_DISTANCE;
			vertices[x + z * width].pos.y = heightmap[x + z * width] * LEVEL_MAX_HEIGHT;
			vertices[x + z * width].pos.z = (z - height * 0.5f) * LEVEL_POINT_DISTANCE;
			vertices[x + z * width].uv.v[0] = (float)x / width;
			vertices[x + z * width].uv.v[1] = (float)z / height;
		}

	//Calculates the normal for each triangle and adds it to the vertices 
	//that builds it up.
	for (uint x = 0; x < width - 1; ++x)	
		for (uint z = 0; z < height - 1; ++z)
		{
			v3 vec1 = vertices[(z + 1) * width + (x + 1)].pos
				-
				vertices[z * width + x].pos;

			v3 vec2 = vertices[z * width + (x + 1)].pos
				-
				vertices[z * width + x].pos;

			v3 normal = vec1.Cross(vec2);
			vertices[z * width + x].normal += normal;
			vertices[(x + 1) + z * width].normal += normal;
			vertices[(x + 1) + (z + 1) * width].normal += normal;

			vec1 = vertices[(z + 1) * width + x].pos
				-
				vertices[z * width + x].pos;

			vec2 = vertices[(z + 1) * width + (x + 1)].pos
				-
				vertices[z * width + x].pos;

			normal = vec1.Cross(vec2);
			vertices[(x + 1) + (z + 1) * width].normal += normal;
			vertices[x + z * width].normal += normal;
			vertices[x + (z + 1) * width].normal += normal;
		}

	uint *indices = new uint[(height - 1) * (width - 1) * 6];

	for (uint z = 0; z < height - 1; z++)
		for (uint x = 0; x < width - 1; x++)
		{
			//Calculating the normalized normal
			vertices[z * width + x].normal.Normalize();

			//Calculating the indices
			indices[(x + z * (width - 1)) * 6] = (x + 1) + (z + 1) * width;
			indices[(x + z * (width - 1)) * 6 + 1] = (x + 1) + z * width;
			indices[(x + z * (width - 1)) * 6 + 2] = x + z * width;

			indices[(x + z * (width - 1)) * 6 + 3] = (x + 1) + (z + 1) * width;
			indices[(x + z * (width - 1)) * 6 + 4] = x + z * width;
			indices[(x + z * (width - 1)) * 6 + 5] = x + (z + 1) * width;
		}

	m_mesh->Flush();
	m_mesh->Initialize(vertices, sizeof(Vertex), width * height, indices, (height - 1) * (width - 1) * 6);
	direct3D->LoadMeshIntoDevice(m_mesh);

	delete[] vertices;	
}
#ifndef INSTANCE_RENDERER_H

#include "DuckRenderer//Array.h"
#include "D3D Includes.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"


/*
struct ParticleEffectInfo
{
	uint					textureID;
	uint					shaderID;
	uint					materialID;
	uint					quantity;

							ParticleEffectInfo(){}

							ParticleEffectInfo(const uint materialID, const uint textureID, const uint shaderID, const uint quantity)
							{
								this->textureID = textureID;
								this->shaderID = shaderID;
								this->materialID = materialID;
								this->quantity = quantity;
							}
	
	bool					operator <	(const ParticleEffectInfo &in);
	bool					operator >	(const ParticleEffectInfo &in);
	ParticleEffectInfo &	operator =	(const ParticleEffectInfo &in);
};
*/

class D3D;

struct InstanceID
{
	Texture					*texture;
	Shader					*shader;

	bool					operator <	(const InstanceID &in);
	bool					operator >	(const InstanceID &in);
	bool					operator == (const InstanceID &in);
	InstanceID &			operator =	(const InstanceID &in);

							InstanceID(const Texture *texture, const Shader *shader)
							{
								this->texture = (Texture *) texture;
								this->shader = (Shader *) shader;
							}
							InstanceID(){}
};

class InstanceRenderer
{
	uint									numberOfInstances;
	uint									maxInstances;
	Array<InstanceID>						instanceIdentifications;
	byte*									instanceData;
	uint									instanceMaxSize;
	uint									type;
	uint									rtIndex;

	struct BatchInfo
	{
		InstanceID instanceType;
		uint quantity;

		BatchInfo()
		{

		}

		BatchInfo(const InstanceID &instanceType)
		{
			this->instanceType = instanceType;
			quantity = 1;
		}
		
		bool operator < (const BatchInfo &in)
		{
			return instanceType < in.instanceType;
		}
	};
	Array<BatchInfo>						batchInfo;

	Mesh									*mesh;
	ID3D11Buffer							*instanceBuffer;
	D3D										*direct3D;

	//void _SaveSortToFile(HString &path);

public:
							InstanceRenderer();
							InstanceRenderer(const uint maxInstances, const uint instanceMaxSize, const Mesh *mesh = nullptr);

	void					SetMesh(const Mesh* mesh);
	void					Initialize(const uint maxInstances, const uint instanceMaxSize, const Mesh *mesh = nullptr);

	void					AddInstance(void* data, const uint dataSize, const InstanceID &instanceID);
	void					QuickSort(Array<InstanceID> &arr, uint left, uint right);
	void					Update();
	void					Render();
	void					ClearInstances();

	void					Flush();

	friend class D3D;
};


#define INSTANCE_RENDERER_H
#endif